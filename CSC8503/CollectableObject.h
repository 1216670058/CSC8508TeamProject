#pragma once

#include "GameObject.h"
#include "GameWorld.h"
#include "PlayerObject.h"
#include <vector>

namespace NCL::CSC8503 {
    class CollectableObject : public GameObject {
    public:
        CollectableObject(GameWorld* world, std::string name = "") : GameObject(name) {
            this->name = name;
            this->world = world;
            putDown = true;
            player = nullptr;
            triggerDelete = true;
        };
        CollectableObject() {
            putDown = true;
            player = nullptr;
            triggerDelete = true;
        };
        ~CollectableObject() {};

        void Update(float dt) override;

        void OnCollisionBegin(GameObject* otherObject) override;

        void SetPlayer(PlayerObject* player) {
            this->player = player;
        }

        void SetPutDown(bool p) {
            putDown = p;
        }

        Vector3 PlayerFront();

    protected:
        bool putDown;
        int num = 1;
        PlayerObject* player;
        GameWorld* world;

        float distance(const Vector3& v1, const Vector3& v2);
    };
}