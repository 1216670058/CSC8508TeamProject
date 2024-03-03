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

	spawn1 = false;

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
	thisClient->RegisterPacketHandler(Add_Remove, this);

	SpawnPlayer();
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
	if (thisServer->IsConnected()) {
		if (!spawn1) {
			SpawnPlayer();
			spawn1 = true;
		}
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

	thisClient->SendPacket(newPacket);

	thisClient->UpdateClient();

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
		std::cout << "RailReceived: " << railNetworkID << std::endl;
		AddRailToWorld(Vector3(0, -3000, 0), true, railNetworkID);
		railProducedTag = 0;
	}

	//player2->Update(dt);

	//std::cout << "Player2: " << player2->GetTransform().GetPosition().x << " " <<
	//	player2->GetTransform().GetPosition().y << " " <<
	//	player2->GetTransform().GetPosition().z << " " << std::endl;
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
		AddPacket addPacket;

		addPacket.networkID1 = plankNetworkID;
		addPacket.worldID = treeWorldID;
		addPacket.tag = treeCutTag;
		thisServer->SendGlobalPacket(addPacket);
		treeCut = false;
	}
	if (rockDug) {
		AddPacket addPacket;

		addPacket.networkID1 = stoneNetworkID;
		addPacket.worldID = rockWorldID;
		addPacket.tag = rockDugTag;
		thisServer->SendGlobalPacket(addPacket);
		rockDug = false;
	}
	if (materialUpdating) {
		AddPacket addPacket;

		addPacket.networkID1 = removedPlankNetworkID;
		addPacket.networkID2 = removedStoneNetworkID;
		addPacket.tag = materialUpdatingTag;
		thisServer->SendGlobalPacket(addPacket);
		materialUpdating = false;
	}
	if (railProduced) {
		AddPacket addPacket;

		addPacket.networkID1 = railNetworkID;
		addPacket.tag = railProducedTag;
		thisServer->SendGlobalPacket(addPacket);
		railProduced = false;
		std::cout << "RailSend: " << addPacket.networkID1 << std::endl;
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
	player2->SetSpawned(true);
	player2->GetTransform().SetPosition(Vector3(20, 5, 110));
}

void NetworkedGame::SpawnCarriage() {
	train->SetSpawned(true);
	carriage1 = (MaterialCarriage*)(train->AddCarriage(21, true));
	carriage2 = (ProduceCarriage*)(train->AddCarriage(22, true));
	carriage3 = (WaterCarriage*)(train->AddCarriage(23, true));
	carriage1->SetProduceCarriage(carriage2);
	carriage2->SetMaterialCarriage(carriage1);
}

void NetworkedGame::StartLevel() {
	train->SetSpawned(true);
	train->GetTransform().SetPosition(Vector3(70, 5, 100));
	carriage1 = (MaterialCarriage*)(train->AddCarriage(21, true));
	carriage2 = (ProduceCarriage*)(train->AddCarriage(22, true));
	carriage3 = (WaterCarriage*)(train->AddCarriage(23, true));
	carriage1->SetProduceCarriage(carriage2);
	carriage2->SetMaterialCarriage(carriage1);
	player->SetSpawned(true);
	player->GetTransform().SetPosition(Vector3(20, 5, 100));
	pickaxe->SetSpawned(true);
	pickaxe->GetTransform().SetPosition(Vector3(40, 5, 90));
	axe->SetSpawned(true);
	axe->GetTransform().SetPosition(Vector3(40, 5, 100));
	bucket->SetSpawned(true);
	bucket->GetTransform().SetPosition(Vector3(40, 5, 110));
}

void NetworkedGame::ReceivePacket(int type, GamePacket* payload, int source) {
	if (thisClient) {
		if (payload->type != Add_Remove) {
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
		else {
			AddPacket* addPacket = (AddPacket*)payload;

			if (addPacket->tag == 1) {
				treeCutTag = addPacket->tag;
				plankNetworkID = addPacket->networkID1;
				treeWorldID = addPacket->worldID;
			}
			else if (addPacket->tag == 2) {
				rockDugTag = addPacket->tag;
				stoneNetworkID = addPacket->networkID1;
				rockWorldID = addPacket->worldID;
			}
			else if (addPacket->tag == 3) {
				materialUpdatingTag = addPacket->tag;
				removedPlankNetworkID = addPacket->networkID1;
				removedStoneNetworkID = addPacket->networkID2;
			}
			else if (addPacket->tag == 4) {
				railProducedTag = addPacket->tag;
				railNetworkID = addPacket->networkID1;
			}
		}
	}
	else if (thisServer) {
		ClientPacket* packet = (ClientPacket*)payload;
		player2->GetNetworkObject()->ReadClientPacket(*packet);
		//std::cout << "Receiving" << std::endl;
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