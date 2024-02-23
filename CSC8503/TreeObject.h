#pragma once

#include "GameObject.h"
#include <vector>
using namespace NCL;
using namespace CSC8503;

class TreeObject : public GameObject {

public:
    TreeObject() {
        typeID = 10086;
    };
    ~TreeObject() {};

    void OnCollisionBegin(GameObject* otherObject) override {};

    void OnCollisionEnd(GameObject* otherObject) override {};

    void Setscale(float a) {
        scale = a;
    }
    float Getscale() {
        return scale;
    }
    // void Update(float dt) override;
    bool moving = false;
    float scale;
};