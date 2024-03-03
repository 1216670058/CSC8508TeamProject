#pragma once

#include "PhysicsSystem.h"
#include "PhysicsObject.h"
#include "StateMachine.h"
#include "NavigationGrid.h"
#include "StateTransition.h"
#include "State.h"
#include "StateMachine.h"
#include "PositionConstraint.h"
#include "NavigationGrid.h"
#include "GameObject.h"
#include <vector>
#include "RenderObject.h"
#include "AnimationObject.h"
#include "NetworkObject.h"
#include "TrainCarriage.h"



namespace NCL::CSC8503 {
    class BucketObject;
    class PlayerObject : public GameObject {

    public:
        PlayerObject(std::string name) {
            this->name = name;
            typeID = 1;
        };
        ~PlayerObject() {};

        void OnCollisionBegin(GameObject* otherObject) override;

        void OnCollisionEnd(GameObject* otherObject) override {};

        void Update(float dt) override;

        void SetPlayerMeshes(vector<Mesh*> m) {
            meshes = m;
        }
        void SetPlayerTextures(vector<vector<GLuint>> t) {
            textures = t;
        }
        void SetPlayerBumpTextures(vector<vector<GLuint>> t) {
            bumpTextures = t;
        }
        void SetPlayerAnimations(vector<AnimationObject*> a) {
            animations = a;
        }
        void SetPlayerShaders(vector<ShaderGroup*> s) {
            shaders = s;
        }

        void SetFace(Vector3 a) {
            face = a;
        }
        void SetSpeed(float a) {
            speed = a;
        }
        float GetSpeed() const {
            return speed;
        }
        Vector3 GetFace() const {
            return face;
        }

        MaterialCarriage* GetCarriage() const {
            return carriage;
        }
        void SetCarriage(MaterialCarriage* c) {
            carriage = c;
        }
        void SetBucket(BucketObject* b) {
            bucket = b;
        }

        BucketObject* GetBucket() {
            return bucket;
        }

        void UpdateAnimation(float dt);
        void PlayerMovement(float dt);
        void UpdateFace();
        void SwitchSkin();

    protected:
        void CutTree();
        void DigRock();
        void ScoopWater();
        void LoadMaterial();
        void UseWater();
        bool doing = false;
        bool cutting = false;
        bool digging = false;

        int index;
        Vector3 face;
        float speed = 50.0f;
        vector<Mesh*> meshes;
        vector<vector<GLuint>> textures;
        vector<vector<GLuint>> bumpTextures;
        vector<AnimationObject*> animations;
        vector<ShaderGroup*> shaders;
        MaterialCarriage* carriage;
        BucketObject* bucket;
    };
}