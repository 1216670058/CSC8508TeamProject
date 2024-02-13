
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
using namespace NCL;
using namespace CSC8503;

class TrainObject : public GameObject {

public:
    TrainObject();
    TrainObject(GameWorld *w,OBJMesh* mesh,Shader* shader);
    ~TrainObject();

    void OnCollisionBegin(GameObject *otherObject) override;

    void OnCollisionEnd(GameObject *otherObject) override;

    void Update(float dt);

    void UpdatePath(  std::vector<std::pair<Vector3,int>> p);

    void AddCarriage();

    void AddConstraint(GameObject *a,GameObject *b);

public:
    //0  1  2 3 up down left right
    std::vector< std::pair<Vector3,int> > path;
    TrainCarriage *trainCarriage;
    int trainIndex =0 ;
    GameWorld *world;
    OBJMesh* trainMesh = nullptr;
    Shader* basicShader = nullptr;
    OBJMesh* carriageMesh = nullptr;
    Shader* carriageShader = nullptr;
};

