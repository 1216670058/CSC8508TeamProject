#pragma once

#include"PhysicsSystem.h"
#include"PhysicsObject.h"
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
#include "TrainCarriage.h"

namespace NCL::CSC8503 {
    class TrainObject : public GameObject {

    public:
        TrainObject();
        TrainObject(GameWorld* w);
        ~TrainObject();

        void OnCollisionBegin(GameObject* otherObject) override;

        void OnCollisionEnd(GameObject* otherObject) override;

        void Update(float dt);

        void UpdatePath(std::pair<Vector3, int> p);

        TrainCarriage* AddCarriage(int id, bool spawn = true);

        void AddConstraint(GameObject* a, GameObject* b);

        void UpdateOrientation(Vector3 direction);

        void UploadAssets(Mesh* mesh, Texture* texture, ShaderGroup* shader);

        float GetForce() { return force; }
        float GetDistance() { return distance; }
        float GetSpeed() { return speed; }
        void SetInitPos(Vector3 pos1) { lastpos = curpos = pos1; }

    protected:
        //0  1  2 3 up down left right
        std::vector< std::pair<Vector3, int> > path;
        TrainCarriage* trainCarriage;
        int trainMaxIndex = 20;
        int trainIndex = 0;
        GameWorld* world;
        Mesh* carriageMesh = nullptr;
        Texture* carriageTex = nullptr;
        ShaderGroup* basicShader = nullptr;

        //UI
        float force = 10.0f;
        Vector3 lastpos = Vector3(0.0f, 0.0f, 0.0f);
        Vector3 curpos = Vector3(0.0f, 0.0f, 0.0f);
        float distance = 0.0f;
        float speed = 0.0f;
        float dist_s = 0.0f;
        float time_s = 0.0f;

    };
}


