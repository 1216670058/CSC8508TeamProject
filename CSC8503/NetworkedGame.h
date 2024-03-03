#pragma once
#include "TutorialGame.h"
#include "NetworkBase.h"

namespace NCL {
	namespace CSC8503 {
		class GameServer;
		class GameClient;
		class NetworkPlayer;

		class NetworkedGame : public TutorialGame, public PacketReceiver {
		public:
			NetworkedGame();
			~NetworkedGame();

			void StartAsServer();
			void StartAsClient(char a, char b, char c, char d);

			void UpdateGame(float dt) override;

			void UpdateNetworkedPlaying(float dt);

			void UpdateKeys();

			void SpawnPlayer();

			void SpawnCarriage();

			void StartLevel();

			void ReceivePacket(int type, GamePacket* payload, int source) override;

			void OnPlayerCollision(NetworkPlayer* a, NetworkPlayer* b);

			bool IsServer() {
				return thisServer != nullptr;
			}
			bool IsClient() {
				return thisClient != nullptr;
			}

			bool GetCutTreeFlag() const {
				return treeCut;
			}
			void SetCutTreeFlag(bool f) {
				treeCut = f;
			}
			bool GetDigRockFlag() const {
				return rockDug;
			}
			void SetDigRockFlag(bool f) {
				rockDug = f;
			}

			//bool IsCutting() const {
			//	return cutting;
			//}
			//void SetCutting(bool c) {
			//	cutting = c;
			//}
			//bool IsDigging() const {
			//	return digging;
			//}
			//void SetDigging(bool d) {
			//	digging = d;
			//}

			Vector3 GetTempPosition() const {
				return tempPosition;
			}
			void SetTempPosition(Vector3 t) {
				tempPosition = t;
			}
			int GetTempNetworkID() const {
				return tempNetworkID;
			}
			void SetTempNetworkID(int t) {
				tempNetworkID = t;
			}
			int GetTempWorldID() const {
				return tempWorldID;
			}
			void SetTempWorldID(int t) {
				tempWorldID = t;
			}
			int GetTempTag() const {
				return tempTag;
			}
			void SetTempTag(int t) {
				tempTag = t;
			}

			static NetworkedGame* GetNetworkedGame() {
				return networkInstance;
			};

		protected:
			void UpdateAsServer(float dt);
			void UpdateAsClient(float dt);

			void BroadcastSnapshot(bool deltaFrame);
			void ClientSend(bool deltaFrame);
			void UpdateMinimumState();

			void UpdateChooseServer(float dt);
			std::map<int, int> stateIDs;

			GameServer* thisServer;
			GameClient* thisClient;
			float timeToNextPacket;
			int packetsToSnapshot;

			std::vector<NetworkObject*> networkObjects;

			std::map<int, GameObject*> serverPlayers;
			GameObject* localPlayer;

			static NetworkedGame* networkInstance;

			bool spawn1;

			bool treeCut = false;
			bool rockDug = false;

			//bool cutting = false;
			//bool digging = false;

			Vector3 tempPosition;
			int     tempNetworkID;
			int     tempWorldID;
			int     tempTag = 0;
		};
	}
}

