#include "DroneObject.h"

using namespace NCL::CSC8503;

DroneObject::DroneObject(NavigationGrid* grid, GameWorld* world) {
    typeID = 12;
    name = "Drone";
    this->grid = grid;
    this->world = world;


    detectPlayer = new BehaviourAction("Detect Player", [&](float dt, BehaviourState state)->BehaviourState {
        // if player in collider then success, else fail
        return Success;
        }
    );

    moveAwayPlayer = new BehaviourAction("Move Away Player", [&](float dt, BehaviourState state)->BehaviourState {
        // always ongoing, success for error
        return Ongoing;
        }
    );


    detectItem = new BehaviourAction("Detect Item", [&](float dt, BehaviourState state)->BehaviourState {
        // if item in collider then success, else fail
        return Success;
        }
    );

    moveTowardItem = new BehaviourAction("Move Toward Item", [&](float dt, BehaviourState state)->BehaviourState {
        // ongoing until item reached, then success
        return Ongoing;
        }
    );

    stealItem = new BehaviourAction("Steal Item", [&](float dt, BehaviourState state)->BehaviourState {
        // fail after stealing (to continue root selector), success for error
        return Failure;
        }
    );


    moveOnPatrol = new BehaviourAction("Move On Patrol", [&](float dt, BehaviourState state)->BehaviourState {
        // ongoing, success if new path needed
        return Ongoing;
        }
    );

    pathfindForPatrol = new BehaviourAction("Pathfind For Patrol", [&](float dt, BehaviourState state)->BehaviourState {
        // always ongoing, success for error
        return Ongoing;
        }
    );


    playerSequence = new BehaviourSequence("Player Sequence");  // ongoing if player detected, fail if not, success=error
    playerSequence->AddChild(detectPlayer);
    playerSequence->AddChild(moveAwayPlayer);

    itemSequence = new BehaviourSequence("Item Sequence");  // ongoing if item detected, fail if no item anymore, success=error
    itemSequence->AddChild(detectItem);
    itemSequence->AddChild(moveTowardItem);
    itemSequence->AddChild(stealItem);

    patrolSequence = new BehaviourSequence("Patrol Sequence");  // always ongoing, success=error
    patrolSequence->AddChild(moveOnPatrol);
    patrolSequence->AddChild(pathfindForPatrol);

    rootSelector = new BehaviourSelector("Root Selector");  // always ongoing, success=error
    rootSelector->AddChild(playerSequence);
    rootSelector->AddChild(itemSequence);
    rootSelector->AddChild(patrolSequence);

}

void DroneObject::Update(float dt) {
    renderObject->GetAnimationObject()->SetFrameTime(renderObject->GetAnimationObject()->GetFrameTime() - dt);
    while (renderObject->GetAnimationObject()->GetFrameTime() < 0.0f) {
        renderObject->GetAnimationObject()->SetCurrentFrame((renderObject->GetAnimationObject()->GetCurrentFrame() + 1) %
            renderObject->GetAnimationObject()->GetActiveAnim()->GetFrameCount());
        renderObject->GetAnimationObject()->SetFrameTime(renderObject->GetAnimationObject()->GetFrameTime() + 1.0f /
            renderObject->GetAnimationObject()->GetActiveAnim()->GetFrameRate());
    }

    currentPos = GetTransform().GetPosition();

    if (currentState == Ongoing || currentState == Success) currentState = rootSelector->Execute(dt);   // root selector - do until succeed/ongoing
    if (currentState == Success) std::cout << "Behaviour Tree Error\n";    // error
}