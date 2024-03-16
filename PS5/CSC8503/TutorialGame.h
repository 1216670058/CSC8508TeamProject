#pragma once
#include "PhysicsSystem.h"

#include "StateGameObject.h"

#include "MeshAnimation.h"

#include "GameTechRendererInterface.h"

#include "NavigationGrid.h"

#include "AxeObject.h"
#include "PickaxeObject.h"
#include "BucketObject.h"
#include "PlayerObject.h"
#include "TrainObject.h"
#include "TrainCarriage.h"
#include "TreeObject.h"
#include "RockObject.h"
#include "RailObject.h"

namespace NCL {
	namespace CSC8503 {
		class TutorialGame		{
		public:
			TutorialGame(GameWorld& gameWorld, GameTechRendererInterface& renderer, PhysicsSystem& physics);
			~TutorialGame();

			virtual void UpdateGame(float dt);

			static TutorialGame* GetGame() {
				return instance;
			};

			RailObject* AddRailToWorld(const Vector3& position, bool placed = false);

			Controller* GetController() const {
				return controller;
			}

		protected:
			void InitialiseAssets();
			void InitMeshes();
			void InitTextures();
			void InitShaders();
			void InitAnimations();

			void InitCamera();
			void UpdateKeys();

			void InitGameWorld();
			void InitWorld();
			void InitPositions();
			void InitGameExamples();
			void InitDefaultFloor();

			void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);

			/*
			These are some of the world/object creation functions I created when testing the functionality
			in the module. Feel free to mess around with them to see different objects being created in different
			test scenarios (constraints, collision types, and so on). 
			*/
			bool SelectObject();
			void MoveSelectedObject();
			void DebugObjectMovement();
			void LockedObjectMovement();

			GameObject* AddFloorToWorld(const Vector3& position);
			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f);
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f);

			GameObject* AddPlayer0ToWorld(const Vector3& position);
			GameObject* AddEnemyToWorld(const Vector3& position);
			GameObject* AddBonusToWorld(const Vector3& position);

			AxeObject* AddAxeToWorld(const Vector3& position);
			PickaxeObject* AddPickaxeToWorld(const Vector3& position);
			BucketObject* AddBucketToWorld(const Vector3& position);
			PlayerObject* AddPlayerToWorld(const Vector3& position);
			TrainObject* AddTrainToWorld(const Vector3& position);
			TreeObject* AddTreeToWorld(const Vector3& position);
			RockObject* AddRockToWorld(const Vector3& position);
			GameObject* AddDesertRockToWorld(const Vector3& position);
			GameObject* AddWaterToWorld(const Vector3& position);
			GameObject* AddStationToWorld(const Vector3& position);
			GameObject* AddPadToWorld();

			void AddSceneToWorld(int level);

			GameWorld&					world;
			GameTechRendererInterface&	renderer;
			PhysicsSystem&				physics;
			NavigationGrid*             navGrid;

			Controller* controller;

			bool useGravity;
			bool inSelectionMode;

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;
			
			AxeObject* axe = nullptr;
			PickaxeObject* pickaxe = nullptr;
			BucketObject* bucket = nullptr;
			PlayerObject* player = nullptr;
			TrainObject* train = nullptr;
			MaterialCarriage* carriage1 = nullptr;
			ProduceCarriage* carriage2 = nullptr;
			WaterCarriage* carriage3 = nullptr;
			GameObject* pad = nullptr;

			Mesh*	capsuleMesh = nullptr;
			Mesh*	cubeMesh	= nullptr;
			Mesh*	sphereMesh	= nullptr;
			Mesh*   charMesh = nullptr;
			Mesh*   enemyMesh = nullptr;
			Mesh*   bonusMesh = nullptr;
			Mesh*   axeMesh = nullptr;
			Mesh*   pickaxeMesh = nullptr;
			Mesh*   bucketMesh = nullptr;
			Mesh*   trainMesh = nullptr;
			Mesh*   carriageMesh = nullptr;
			Mesh*   treeMesh = nullptr;
			Mesh*   rockMesh = nullptr;
			Mesh*   desertRockMesh = nullptr;
			Mesh*   railMesh = nullptr;
			Mesh*   railTurnMesh = nullptr;
			Mesh*   stationMesh = nullptr;

			Shader*		basicShader = nullptr;

			Texture* basicTex = nullptr;
			Texture* floorTex = nullptr;
			Texture* axeTexture = nullptr;
			Texture* pickaxeTexture = nullptr;
			Texture* bucketTexture = nullptr;
			Texture* trainTexture = nullptr;
			Texture* carriageTexture = nullptr;
			Texture* treeTexture = nullptr;
			Texture* rockTexture = nullptr;
			Texture* desertRockTexture = nullptr;
			Texture* waterTexture = nullptr;
			Texture* railTexture = nullptr;
			Texture* railTurnTexture = nullptr;
			Texture* stationTexture = nullptr;

			MeshAnimation* enemyAnim = nullptr;

			//Coursework Additional functionality	
			GameObject* lockedObject	= nullptr;
			Vector3 lockedOffset		= Vector3(0, 14, 20);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}

			GameObject* objClosest = nullptr;

			int level = 1;

			Vector3 playerPosition;
			Vector3 axePosition;
			Vector3 pickaxePosition;
			Vector3 bucketPosition;
			Vector3 trainPosition;

			static TutorialGame* instance;
		};
	}
}

