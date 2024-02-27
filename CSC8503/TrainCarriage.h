#pragma once

#include"PhysicsSystem.h"
#include"PhysicsObject.h"
#include "StateMachine.h"
#include "NavigationGrid.h"
#include "StateTransition.h"
#include "State.h"
#include "StateMachine.h"
#include "NavigationGrid.h"
#include "GameObject.h"
#include <vector>
#include "RenderObject.h"

namespace NCL::CSC8503 {
    class StoneObject;
    class PlankObject;
    class TrainCarriage : public GameObject {
    public:
        TrainCarriage();

        ~TrainCarriage();

        void OnCollisionBegin(GameObject* otherObject) override;

        void OnCollisionEnd(GameObject* otherObject) override;

        void Update(float dt);

        void UpdateOrientation();
        int GetDirection();

        std::vector< std::pair<Vector3, int> > path;
    protected:
        Vector3 direction;
    };

    class MaterialCarriage : public TrainCarriage {
    public:
        MaterialCarriage() {};

        ~MaterialCarriage() {};

        void OnCollisionBegin(GameObject* otherObject) override;

        //void Update(float dt) override;

        vector<PlankObject*> GetPlankVector() const {
            return planks;
        }
        vector<StoneObject*> GetStoneVector() const {
            return stones;
        }

        void AddStone(StoneObject* stone) {
            stones.push_back(stone);
        }
        void AddPlank(PlankObject* plank) {
            planks.push_back(plank);
        }

    protected:
        vector<PlankObject*> planks;
        vector<StoneObject*> stones;
    };
}