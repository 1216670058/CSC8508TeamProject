#pragma once

#include "PhysicsSystem.h"
#include "PhysicsObject.h"
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
    class RailObject;
    class MaterialCarriage;
    class ProduceCarriage;
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
        GameWorld* world;
    };

    class MaterialCarriage : public TrainCarriage {
    public:
        MaterialCarriage(GameWorld* w) { world = w; };

        ~MaterialCarriage() {};

        void Update(float dt) override;

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

        bool IsReady() const {
            return ready;
        }
        void SetReady(bool r) {
            ready = r;
        }

        ProduceCarriage* GetProduceCarriage() const {
            return produceCarriage;
        }
        void SetProduceCarriage(ProduceCarriage* c) {
            produceCarriage = c;
        }

        void UpdateMaterial();

    protected:
        vector<PlankObject*> planks;
        vector<StoneObject*> stones;
        bool ready = false;

        ProduceCarriage* produceCarriage;
    };

    class ProduceCarriage :public TrainCarriage {
    public:
        ProduceCarriage(GameWorld* w) { world = w; };

        ~ProduceCarriage() {};

        void OnCollisionBegin(GameObject* otherObject) override;

        void Update(float dt) override;

        bool Finished() const {
            return finish;
        }
        void SetFinish(bool f) {
            finish = f;
        }

        MaterialCarriage* GetMaterialCarriage() const {
            return materialCarriage;
        }
        void SetMaterialCarriage(MaterialCarriage* c) {
            materialCarriage = c;
        }

    protected:
        vector<RailObject*> rails;
        bool finish = true;
        float counter = 3.0f;

        MaterialCarriage* materialCarriage;
    };
}