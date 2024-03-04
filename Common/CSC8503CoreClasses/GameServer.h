#pragma once
#include "NetworkBase.h"

namespace NCL {
	namespace CSC8503 {
		class GameWorld;
		class GameServer : public NetworkBase {
		public:
			GameServer(int onPort, int maxClients);
			~GameServer();

			bool Initialise();
			void Shutdown();

			void SetGameWorld(GameWorld &g);

			bool SendGlobalPacket(int msgID);
			bool SendGlobalPacket(GamePacket& packet);

			virtual void UpdateServer();

			bool GetSpawnFlag() const {
				return spawnFlag;
			}
			void SetSpawnFlag(bool f) {
				spawnFlag = f;
			}

			bool IsConnected() const {
				return connected;
			}

			int GetClientCount() const {
				return clientCount;
			}

		protected:
			int			port;
			int			clientMax;
			int			clientCount;
			GameWorld*	gameWorld;

			int incomingDataRate;
			int outgoingDataRate;

			bool spawnFlag = false;
			bool connected = false;
		};
	}
}
