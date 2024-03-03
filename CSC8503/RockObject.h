#pragma once

#include "GameObject.h"
#include <vector>
using namespace NCL;
using namespace CSC8503;

class RockObject : public GameObject {

public:
    RockObject() {
        typeID = 10010;
        name = "Rock";
    };
    ~RockObject() {};

    void OnCollisionBegin(GameObject* otherObject) override {};

    void OnCollisionEnd(GameObject* otherObject) override {};

    void Setscale(float a) {
        scale = a;
    }
    float Getscale() {
        return scale;
    }
    // void Update(float dt) override;

protected:
    float scale;
};