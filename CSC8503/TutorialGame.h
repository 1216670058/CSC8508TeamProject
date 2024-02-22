#include "../NCLCoreClasses/KeyboardMouseController.h"

#pragma once
#include "GameTechRenderer.h"
#ifdef USEVULKAN
#include "GameTechVulkanRenderer.h"
#endif
#include "PhysicsSystem.h"
#include "NavigationGrid.h"
#include "MeshMaterial.h"
#include "Audio.h"
#include "StateGameObject.h"
#include "TrainObject.h"
#include "PlayerObject.h"
#include <cmath>
#include <limits>
class CollectableObject : public GameObject {
public:
    int objectId;
    CollectableObject(GameWorld* world, std::string name = "") : GameObject(name) {
        this->world = world;
        triggerDelete = true;
    };
    void OnCollisionBegin(GameObject* otherObject) override {
        if (otherObject->GetName() == "Player")
        {
            world->RemoveGameObject(this, true);
        }
    }

    GameWorld* world;
};
namespace NCL {
    namespace CSC8503 {
        class TutorialGame {
        public:
            TutorialGame();
            ~TutorialGame();

            virtual void UpdateGame(float dt);

            static TutorialGame* GetGame() {
                return instance;
            };

        protected:
            void InitialiseAssets();

            void InitMaterials();
            void InitAnimations();
            void InitCamera();
            void UpdateKeys();

            void InitWorld();

            /*
            These are some of the world/object creation functions I created when testing the functionality
            in the module. Feel free to mess around with them to see different objects being created in different
            test scenarios (constraints, collision types, and so on).
            */
            void InitGameExamples();

            void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
            void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
            void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims);

            void InitDefaultFloor();
            int SelectRailDir(Vector3 lastRailPosition, Vector3 RailPosition);
            bool SelectObject();
            void MoveSelectedObject();
            void DebugObjectMovement();
            void LockedObjectMovement();

            void AddSceneToWorld();
            void HoldObject();

            GameObject* AddFloorToWorld(const Vector3& position);
            GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f);
            GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f);

            GameObject* AddPlayer0ToWorld(const Vector3& position);
            GameObject* AddEnemyToWorld(const Vector3& position);
            TrainObject* AddTrainToWorld(const Vector3& position);
            GameObject* AddTestingLightToWorld(const Vector3& position, const Vector4& colour);
            PlayerObject* AddPlayerToWorld(const Vector3& position);

            CollectableObject* CreateObject(int ID);
            CollectableObject* AddCollectableObjectToGround(int ID);
            CollectableObject* AddRailToWorld(int direction, Vector3 position, Vector3 lastRailPosition);
            StateGameObject* AddStateObjectToWorld(const Vector3& position);
            StateGameObject* testStateObject = nullptr;


            void BridgeConstraintTest();

#ifdef USEVULKAN
            GameTechVulkanRenderer* renderer;
#else
            GameTechRenderer* renderer;
#endif
            PhysicsSystem* physics;
            GameWorld* world;
            Audio* audio;
            std::vector<GameObject*> scene;
            KeyboardMouseController controller;
            PlayerObject* player = nullptr;
            //collectObject
            CollectableObject* object = nullptr;
            CollectableObject* groundObject = nullptr;

            Vector3 ObjectPos;
            Vector3 finalObjectPos;
            Vector3 lastRailPositin;
            Vector3 RailPositin;

            bool useGravity;
            bool inSelectionMode;

            float		forceMagnitude;

            GameObject* selectionObject = nullptr;

            Mesh* capsuleMesh = nullptr;
            Mesh* cubeMesh = nullptr;
            Mesh* sphereMesh = nullptr;

            Texture* basicTex = nullptr;
            Texture* floorTex = nullptr;
            Texture* trainTex = nullptr;
            Texture* lightTex = nullptr;

            Texture* lightBumpTex = nullptr;

            Texture* lightSpecTex = nullptr;

            Shader* basicDayShader = nullptr;
            Shader* bumpDayShader = nullptr;
            Shader* specDayShader = nullptr;
            Shader* skinningPerPixelDayShader = nullptr;
            Shader* skinningBumpDayShader = nullptr;

            Shader* basicNightShader = nullptr;
            Shader* bumpNightShader = nullptr;
            Shader* specNightShader = nullptr;
            Shader* skinningPerPixelNightShader = nullptr;
            Shader* skinningBumpNightShader = nullptr;

            ShaderGroup* basicShader = nullptr;
            ShaderGroup* bumpShader = nullptr;
            ShaderGroup* specShader = nullptr;
            ShaderGroup* skinningPerPixelShader = nullptr;
            ShaderGroup* skinningBumpShader = nullptr;

            //Coursework Meshes
            Mesh* charMesh = nullptr;
            Mesh* enemyMesh = nullptr;
            Mesh* bonusMesh = nullptr;

            Mesh* maleMesh = nullptr;
            Mesh* femaleMesh = nullptr;
            Mesh* assassinMesh = nullptr;
            Mesh* girlMesh = nullptr;
            Mesh* smurfMesh = nullptr;

            OBJMesh* trainMesh = nullptr;
            OBJMesh* creeperMesh = nullptr;

            MeshMaterial* maleMaterial = nullptr;
            MeshMaterial* femaleMaterial = nullptr;
            MeshMaterial* assassinMaterial = nullptr;
            MeshMaterial* girlMaterial = nullptr;
            MeshMaterial* smurfMaterial = nullptr;

            vector<GLuint> maleTextures;
            vector<GLuint> femaleTextures;
            vector<GLuint> assassinTextures;
            vector<GLuint> girlTextures;
            vector<GLuint> smurfTextures;

            vector<GLuint> maleBumpTextures;
            vector<GLuint> femaleBumpTextures;
            vector<GLuint> girlBumpTextures;
            vector<GLuint> smurfBumpTextures;

            AnimationObject* maleAnimation = nullptr;
            AnimationObject* femaleAnimation = nullptr;
            AnimationObject* assassinAnimation = nullptr;
            AnimationObject* girlAnimation = nullptr;
            AnimationObject* smurfAnimation = nullptr;

            vector<Mesh*> meshes;
            vector<vector<GLuint>> textures;
            vector<vector<GLuint>> bumpTextures;
            vector<AnimationObject*> animations;
            vector<ShaderGroup*> shaders;

            //Coursework Additional functionality	
            GameObject* lockedObject = nullptr;
            Vector3 lockedOffset = Vector3(0, 88, 0);
            void LockCameraToObject(GameObject* o) {
                lockedObject = o;
            }

            TrainObject* trainObject = nullptr;

            GameObject* character = nullptr;
            GameObject* objClosest = nullptr;

            vector<Vector3> mazeNodes;
            NavigationGrid* navGrid;
            const int GRID_SIZE_X = 10;
            const int GRID_SIZE_Y = 10;
            const float GRID_CENTER_X = 5.0f;
            const float  GRID_CENTER_Y = 5.0f;

            float distance(const Vector3& v1, const Vector3& v2) {
                float dx = v1.x - v2.x;
                float dy = v1.y - v2.y;
                float dz = v1.z - v2.z;
                return std::sqrt(dx * dx + dy * dy + dz * dz);
            }
            double euclideanDistance(float x1, float y1, float x2, float y2) {
                return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
            }

            struct GridCenter {
                int x;
                int y;
            };

            Vector3 findNearestGridCenter(const Vector3& position) {
                float inputX = position.x;
                float inputY = position.y;
                GridCenter nearestGridCenter;
                Vector3 _nearestGridCenter;
                double minDistance = 1000;
                for (int x = 0; x < GRID_SIZE_X; ++x) {
                    for (int y = 0; y < GRID_SIZE_Y; ++y) {
                        double distance = euclideanDistance(inputX, inputY, x * 10 + GRID_CENTER_X, y * 10 + GRID_CENTER_Y);
                        if (distance < minDistance) {
                            minDistance = distance;
                            nearestGridCenter.x = x * 10 + GRID_CENTER_X;
                            nearestGridCenter.y = y * 10 + GRID_CENTER_Y;
                        }
                    }
                }
                _nearestGridCenter.x = nearestGridCenter.x;
                _nearestGridCenter.y = nearestGridCenter.y;
                _nearestGridCenter.z = 5;
                return _nearestGridCenter;
            }

            Vector3 PlayerFront()
            {
                Vector3 playerPos = player->GetTransform().GetPosition();
                Quaternion facingDir = player->GetTransform().GetOrientation();
                Vector3 ObjectOffset(0, 2, -5);
                ObjectPos = facingDir * ObjectOffset;
                ObjectOffset = facingDir * ObjectOffset;
                finalObjectPos = ObjectPos + playerPos;
                return finalObjectPos;
            }
            static TutorialGame* instance;
        };
    }
}

