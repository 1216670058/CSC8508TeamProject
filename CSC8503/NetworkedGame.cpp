#include "NetworkedGame.h"
#include "NetworkPlayer.h"
#include "NetworkObject.h"
#include "GameServer.h"
#include "GameClient.h"

#define COLLISION_MSG 30

NetworkedGame* NetworkedGame::networkInstance = nullptr;

struct MessagePacket : public GamePacket {
	short playerID;
	short messageID;

	MessagePacket() {
		type = Message;
		size = sizeof(short) * 2;
	}
};

NetworkedGame::NetworkedGame()	{
	thisServer = nullptr;
	thisClient = nullptr;

	NetworkBase::Initialise();
	timeToNextPacket  = 0.0f;
	packetsToSnapshot = 0;

	playerNum = -1;

	networkInstance = this;
}

NetworkedGame::~NetworkedGame()	{ 
	delete thisServer;
	delete thisClient;
}

void NetworkedGame::StartAsServer() {
	thisServer = new GameServer(NetworkBase::GetDefaultPort(), 4);

	thisServer->RegisterPacketHandler(Delta_State, this);
	thisServer->RegisterPacketHandler(Full_State, this);
	thisServer->RegisterPacketHandler(Received_State, this);

	StartLevel();
}

void NetworkedGame::StartAsClient(char a, char b, char c, char d) {
	thisClient = new GameClient();
	thisClient->Connect(a, b, c, d, NetworkBase::GetDefaultPort());

	thisClient->RegisterPacketHandler(Delta_State, this);
	thisClient->RegisterPacketHandler(Full_State, this);
	thisClient->RegisterPacketHandler(Player_Connected, this);
	thisClient->RegisterPacketHandler(Player_Disconnected, this);
	thisClient->RegisterPacketHandler(Update_Objects, this);
	thisClient->RegisterPacketHandler(Client_Num, this);

	SpawnCarriage();
}

void NetworkedGame::UpdateGame(float dt) {
	switch (world->GetGameState())
	{
	case LOADING:
		UpdateLoading(dt);
		break;
	case PLAYING:
		UpdatePlaying(dt);
		break;
	case SERVERPLAYING:
		UpdateNetworkedPlaying(dt);
		break;
	case CLIENTPLAYING:
		UpdateNetworkedPlaying(dt);
		break;
	case PAUSED:
		UpdatePaused(dt);
		break;
	case MENU:
		UpdateMenu(dt);
		break;
	case CHOOSESERVER:
		UpdateChooseServer(dt);
		break;
	case EXIT:
		isExit = true;
		break;
	default:
		break;
	}
}

void NetworkedGame::UpdateNetworkedPlaying(float dt) {
	timeToNextPacket -= dt;
	if (timeToNextPacket < 0) {
		if (thisServer) {
			UpdateAsServer(dt);
		}
		else if (thisClient) {
			UpdateAsClient(dt);
		}
		timeToNextPacket += 1.0f / 20.0f; //20hz server/client update
	}

	if (!inSelectionMode) {
		if (cameraMode == 1) {
			CameraUpdate();
		}
		else {
			world->GetMainCamera().UpdateCamera(dt);
		}
	}

	UpdateKeys();
	audio->Update();

	renderer->Update(dt);
	renderer->GetUI()->Update(dt); //UI
	physics->Update(dt);
	renderer->Render();
	Debug::UpdateRenderables(dt);
}

void NetworkedGame::UpdateKeys() {
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::ESCAPE)) {
		if (world->GetGameState() == GameState::SERVERPLAYING ||
			world->GetGameState() == GameState::CLIENTPLAYING)
			world->SetGameState(GameState::PAUSED);
		else if (world->GetGameState() == GameState::PAUSED) {
			if (IsServer())
				world->SetGameState(GameState::SERVERPLAYING);
			else if (IsClient())
				world->SetGameState(GameState::CLIENTPLAYING);
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
		}
	}
}

void NetworkedGame::UpdateAsServer(float dt) {
	if (thisServer->GetSpawnFlag()) {
		SpawnPlayer();
		SynchronizeGameObjects();
	}

	packetsToSnapshot--;
	if (packetsToSnapshot < 0) {
		BroadcastSnapshot(false);
		packetsToSnapshot = 5;
	}
	else {
		BroadcastSnapshot(true);
	}
	thisServer->UpdateServer();

	world->UpdateWorld(dt);

	//std::cout << "Player1: " << player->GetTransform().GetPosition().x << " " <<
	//	player->GetTransform().GetPosition().y << " " <<
	//	player->GetTransform().GetPosition().z << " " << std::endl;
}

void NetworkedGame::UpdateAsClient(float dt) {
	ClientPacket newPacket;

	newPacket.playerNum = playerNum;
	if (Window::GetKeyboard()->KeyHeld(KeyCodes::W))
		newPacket.buttonstates[0] = 1;
	else
		newPacket.buttonstates[0] = 0;
	if (Window::GetKeyboard()->KeyHeld(KeyCodes::S))
		newPacket.buttonstates[1] = 1;
	else
		newPacket.buttonstates[1] = 0;
	if (Window::GetKeyboard()->KeyHeld(KeyCodes::A))
		newPacket.buttonstates[2] = 1;
	else
		newPacket.buttonstates[2] = 0;
	if (Window::GetKeyboard()->KeyHeld(KeyCodes::D))
		newPacket.buttonstates[3] = 1;
	else
		newPacket.buttonstates[3] = 0;
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::R))
		newPacket.buttonstates[4] = 1;
	else
		newPacket.buttonstates[4] = 0;
	if (Window::GetKeyboard()->KeyHeld(KeyCodes::SPACE))
		newPacket.buttonstates[5] = 1;
	else
		newPacket.buttonstates[5] = 0;
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::SPACE))
		newPacket.buttonstates[6] = 1;
	else
		newPacket.buttonstates[6] = 0;
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::F))
		newPacket.buttonstates[7] = 1;
	else
		newPacket.buttonstates[7] = 0;

	thisClient->SendPacket(newPacket);

	thisClient->UpdateClient();

	if (spawn)
		SpawnPlayer();

	if (!objectSpawned)
		SpawnGameObjects();

	UpdateGameObjects();
}

void NetworkedGame::BroadcastSnapshot(bool deltaFrame) {
	std::vector<GameObject*>::const_iterator first;
	std::vector<GameObject*>::const_iterator last;

	world->GetObjectIterators(first, last);

	for (auto i = first; i != last; ++i) {
		NetworkObject* o = (*i)->GetNetworkObject();
		if (!o) {
			continue;
		}
		//TODO - you'll need some way of determining
		//when a player has sent the server an acknowledgement
		//and store the lastID somewhere. A map between player
		//and an int could work, or it could be part of a 
		//NetworkPlayer struct. 
		int playerState = 0;
		GamePacket* newPacket = nullptr;
		if (o->WritePacket(&newPacket, deltaFrame, playerState)) {
			thisServer->SendGlobalPacket(*newPacket);
			delete newPacket;
		}
	}
	if (treeCut) {
		UpdatePacket updatePacket;

		updatePacket.networkID1 = plankNetworkID;
		updatePacket.worldID = treeWorldID;
		updatePacket.tag = treeCutTag;
		thisServer->SendGlobalPacket(updatePacket);
		treeCut = false;
	}
	if (rockDug) {
		UpdatePacket updatePacket;

		updatePacket.networkID1 = stoneNetworkID;
		updatePacket.worldID = rockWorldID;
		updatePacket.tag = rockDugTag;
		thisServer->SendGlobalPacket(updatePacket);
		rockDug = false;
	}
	if (materialUpdating) {
		UpdatePacket updatePacket;

		updatePacket.networkID1 = removedPlankNetworkID;
		updatePacket.networkID2 = removedStoneNetworkID;
		updatePacket.tag = materialUpdatingTag;
		thisServer->SendGlobalPacket(updatePacket);
		materialUpdating = false;
	}
	if (railProduced) {
		UpdatePacket updatePacket;

		updatePacket.networkID1 = railNetworkID;
		updatePacket.tag = railProducedTag;
		thisServer->SendGlobalPacket(updatePacket);
		railProduced = false;
	}
	if (updateRail) {
		UpdatePacket updatePacket;

		updatePacket.networkID1 = updateRailNetworkID;
		updatePacket.tag = updateRailTag;
		thisServer->SendGlobalPacket(updatePacket);
		updateRail = false;
	}
}

void NetworkedGame::ClientSend(bool deltaFrame) {
	int playerState = 0;
	GamePacket* newPacket = nullptr;
	if (player2->GetNetworkObject()->WritePacket(&newPacket, deltaFrame, playerState)) {
		thisClient->SendPacket(*newPacket);
		delete newPacket;
	}
}

void NetworkedGame::UpdateMinimumState() {
	//Periodically remove old data from the server
	int minID = INT_MAX;
	int maxID = 0; //we could use this to see if a player is lagging behind?

	for (auto i : stateIDs) {
		minID = std::min(minID, i.second);
		maxID = std::max(maxID, i.second);
	}
	//every client has acknowledged reaching at least state minID
	//so we can get rid of any old states!
	std::vector<GameObject*>::const_iterator first;
	std::vector<GameObject*>::const_iterator last;
	world->GetObjectIterators(first, last);

	for (auto i = first; i != last; ++i) {
		NetworkObject* o = (*i)->GetNetworkObject();
		if (!o) {
			continue;
		}
		o->UpdateStateHistory(minID); //clear out old states so they arent taking up memory...
	}
}

void NetworkedGame::SpawnPlayer() {
	if (thisServer) {
		if (thisServer->GetClientCount() == 1) {
			player2->SetSpawned(true);
			player2->GetTransform().SetPosition(Vector3(20, 5, 110));
			thisServer->SetSpawnFlag(false);
		}
		else if (thisServer->GetClientCount() == 2) {
			player3->SetSpawned(true);
			player3->GetTransform().SetPosition(Vector3(20, 5, 120));
			thisServer->SetSpawnFlag(false);
		}
		else if (thisServer->GetClientCount() == 3) {
			player4->SetSpawned(true);
			player4->GetTransform().SetPosition(Vector3(20, 5, 130));
			thisServer->SetSpawnFlag(false);
		}

		NumPacket numPacket;

		numPacket.num = thisServer->GetClientCount() + 1;
		thisServer->SendGlobalPacket(numPacket);
	}
	else if (thisClient) {
		if (spawnNum == 2) {
			player2->SetSpawned(true);
			player2->GetTransform().SetPosition(Vector3(20, 5, 110));
			if (playerNum == -1) {
				playerNum = 2;
				localPlayer = player2;
			}
			spawn = false;
		}
		if (spawnNum == 3) {
			player3->SetSpawned(true);
			player3->GetTransform().SetPosition(Vector3(20, 5, 120));
			if (playerNum == -1) {
				playerNum = 3;
				localPlayer = player3;
			}
			spawn = false;
		}
		if (spawnNum == 4) {
			player4->SetSpawned(true);
			player4->GetTransform().SetPosition(Vector3(20, 5, 130));
			if (playerNum == -1) {
				playerNum = 4;
				localPlayer = player4;
			}
			spawn = false;
		}
	}
}

void NetworkedGame::SpawnCarriage() {
	train->SetSpawned(true);
	carriage1 = (MaterialCarriage*)(train->AddCarriage(21, true));
	carriage2 = (ProduceCarriage*)(train->AddCarriage(22, true));
	carriage3 = (WaterCarriage*)(train->AddCarriage(23, true));
	carriage1->SetProduceCarriage(carriage2);
	carriage2->SetMaterialCarriage(carriage1);
}

void NetworkedGame::SynchronizeGameObjects() {
	std::vector<GameObject*>::const_iterator first1;
	std::vector<GameObject*>::const_iterator last1;
	world->GetObjectIterators(first1, last1);

	for (auto i = first1; i != last1; ++i) {
		if ((*i)->GetTypeID() == 5 || (*i)->GetTypeID() == 6 ||
			((*i)->GetTypeID() == 7) && !(*i)->GetFlag1()) {
			UpdatePacket updatePacket;

			updatePacket.typeID = (*i)->GetTypeID();
			updatePacket.networkID1 = (*i)->GetNetworkObject()->GetNetworkID();
			updatePacket.tag = 5;
			thisServer->SendGlobalPacket(updatePacket);
			//std::cout << "Server: updatePacket" << std::endl;
		}
	}

	std::vector<int>::const_iterator first2;
	std::vector<int>::const_iterator last2;
	world->GetRemovedObjectIterators(first2, last2);

	for (auto i = first2; i != last2; ++i) {
		UpdatePacket updatePacket;

		updatePacket.networkID1 = *i;
		updatePacket.tag = 6;
		thisServer->SendGlobalPacket(updatePacket);
	}
}

void NetworkedGame::SpawnGameObjects() {
	if (newObjectTag == 5) {
		for (int i = 0; i < objectType.size(); ++i) {
			switch (objectType[i]) {
			case 5:
				AddPlankToWorld(Vector3(0, -2000, 0), true, objectNetworkID[i]);
				break;
			case 6:
				AddStoneToWorld(Vector3(0, -2000, 0), true, objectNetworkID[i]);
				break;
			case 7:
				AddRailToWorld(Vector3(0, -2000, 0), true, objectNetworkID[i]);
				break;
			}
		}
		newObjectTag = 0;
	}
	if (removeObjectTag == 6) {
		for (int i = 0; i < removedObjectNetworkID.size(); ++i) {
			world->RemoveGameObject(removedObjectNetworkID[i], true);
		}
		removeObjectTag = 0;
		objectSpawned = true;
	}
}

void NetworkedGame::UpdateGameObjects() {
	if (treeCutTag == 1) {
		AddPlankToWorld(Vector3(0, -2000, 0), true, plankNetworkID);
		world->RemoveGameObject(treeWorldID);
		treeCutTag = 0;
	}
	else if (rockDugTag == 2) {
		AddStoneToWorld(Vector3(0, -2000, 0), true, stoneNetworkID);
		world->RemoveGameObject(rockWorldID);
		rockDugTag = 0;
	}
	else if (materialUpdatingTag == 3) {
		world->RemoveGameObject(removedPlankNetworkID, true);
		world->RemoveGameObject(removedStoneNetworkID, true);
		materialUpdatingTag = 0;
	}
	else if (railProducedTag == 4) {
		AddRailToWorld(Vector3(0, -3000, 0), true, railNetworkID);
		railProducedTag = 0;
	}
	else if (updateRailTag == 7) {
		std::vector<GameObject*>::const_iterator first;
		std::vector<GameObject*>::const_iterator last;

		world->GetObjectIterators(first, last);

		for (auto i = first; i != last; ++i) {
			if ((*i)->GetTypeID() == 7) {
				if ((*i)->GetNetworkObject()->GetNetworkID() == updateRailNetworkID) {
					(*i)->GetRenderObject()->SetMesh(railTurnMesh);
					(*i)->GetRenderObject()->SetDefaultTexture(railTurnTex);
					(*i)->GetRenderObject()->SetBumpTexture(railTurnBumpTex);
				}
			}
		}
		updateRailTag = 0;
	}
}

void NetworkedGame::StartLevel() {
	train->SetSpawned(true);
	train->GetTransform().SetPosition(Vector3(70, 4.5f, 100));
	carriage1 = (MaterialCarriage*)(train->AddCarriage(21, true));
	carriage2 = (ProduceCarriage*)(train->AddCarriage(22, true));
	carriage3 = (WaterCarriage*)(train->AddCarriage(23, true));
	carriage1->SetProduceCarriage(carriage2);
	carriage2->SetMaterialCarriage(carriage1);
	player->SetSpawned(true);
	player->GetTransform().SetPosition(Vector3(20, 5, 100));
	localPlayer = player;
	playerNum = 1;
	pickaxe->SetSpawned(true);
	pickaxe->GetTransform().SetPosition(Vector3(40, 5, 90));
	axe->SetSpawned(true);
	axe->GetTransform().SetPosition(Vector3(40, 5, 100));
	bucket->SetSpawned(true);
	bucket->GetTransform().SetPosition(Vector3(40, 5, 110));
}

void NetworkedGame::ReceivePacket(int type, GamePacket* payload, int source) {
	if (thisClient) {
		if (payload->type == Full_State) {
			std::vector<GameObject*>::const_iterator first;
			std::vector<GameObject*>::const_iterator last;
			world->GetObjectIterators(first, last);
			for (auto i = first; i != last; ++i) {
				NetworkObject* o = (*i)->GetNetworkObject();
				if (!o) {
					continue;
				}
				o->ReadPacket(*payload);
			}
		}
		else if (payload->type == Update_Objects) {
			UpdatePacket* updatePacket = (UpdatePacket*)payload;

			if (updatePacket->tag == 1) {
				treeCutTag = updatePacket->tag;
				plankNetworkID = updatePacket->networkID1;
				treeWorldID = updatePacket->worldID;
			}
			else if (updatePacket->tag == 2) {
				rockDugTag = updatePacket->tag;
				stoneNetworkID = updatePacket->networkID1;
				rockWorldID = updatePacket->worldID;
			}
			else if (updatePacket->tag == 3) {
				materialUpdatingTag = updatePacket->tag;
				removedPlankNetworkID = updatePacket->networkID1;
				removedStoneNetworkID = updatePacket->networkID2;
			}
			else if (updatePacket->tag == 4) {
				railProducedTag = updatePacket->tag;
				railNetworkID = updatePacket->networkID1;
			}
			else if (updatePacket->tag == 5) {
				newObjectTag = updatePacket->tag;
				objectType.push_back(updatePacket->typeID);
				objectNetworkID.push_back(updatePacket->networkID1);
				//std::cout << "Client: updatePacket" << std::endl;
			}
			else if (updatePacket->tag == 6) {
				removeObjectTag = updatePacket->tag;
				removedObjectNetworkID.push_back(updatePacket->networkID1);
			}
			else if (updatePacket->tag == 7) {
				updateRailTag = updatePacket->tag;
				updateRailNetworkID = updatePacket->networkID1;
			}
		}
		else if (payload->type == Client_Num) {
			NumPacket* numPacket = (NumPacket*)payload;

			spawnNum = numPacket->num;
			spawn = true;
		}
	}
	else if (thisServer) {
		ClientPacket* packet = (ClientPacket*)payload;
		switch (packet->playerNum) {
		case 2:
			player2->GetNetworkObject()->ReadClientPacket(*packet);
			break;
		case 3:
			player3->GetNetworkObject()->ReadClientPacket(*packet);
			break;
		case 4:
			player4->GetNetworkObject()->ReadClientPacket(*packet);
			break;
		}
	}
}

void NetworkedGame::OnPlayerCollision(NetworkPlayer* a, NetworkPlayer* b) {
	if (thisServer) { //detected a collision between players!
		MessagePacket newPacket;
		newPacket.messageID = COLLISION_MSG;
		newPacket.playerID  = a->GetPlayerNum();

		thisClient->SendPacket(newPacket);

		newPacket.playerID = b->GetPlayerNum();
		thisClient->SendPacket(newPacket);
	}
}

void NetworkedGame::UpdateChooseServer(float dt) {
	audio->Update();
	renderer->Update(dt);
	renderer->GetUI()->Update(dt); //UI
	renderer->Render();
}