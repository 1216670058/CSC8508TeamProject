#include "AnimalObject.h"

using namespace NCL::CSC8503;

AnimalObject::AnimalObject(string filePath, Vector3 startingPos, GameWorld* world) {
    this->typeID = 10;
    this->stateMachine = new StateMachine();
    this->currentPos = startingPos;
    this->grid = new NavigationGrid(filePath);
    this->gridSize = grid->GetGridWidth() * grid->GetGridHeight();
    this->world = world;

    GridNode n = grid->GetGridNode(rand() % this->gridSize);
    while (n.type != 0 || !Pathfind(n.position) || (n.position - currentPos).LengthSquared() > 6000.0f) {
        n = grid->GetGridNode(rand() % this->gridSize);
    }

    timer = 0;

    State* wander = new State([&](float dt) -> void {
        Vector3 nextPathPos = wanderPathNodes[currentNodeIndex];
        

        if (wanderPathNodes.empty() || (nextPathPos - currentPos).LengthSquared() < 1.0f) { // if close to current node
            if (currentNodeIndex < (wanderPathNodes.size() - 1)) {    // if node isnt the final node, target next node
                currentNodeIndex++;
                nextPathPos = wanderPathNodes[currentNodeIndex];
                timer = 0;
            }
            else { // if final node reached, find new path
              
                n = grid->GetGridNode(rand() % this->gridSize);
                while (n.type != 0 || !Pathfind(n.position) || (n.position - currentPos).LengthSquared() > 6000.0f) {
                    n = grid->GetGridNode(rand() % this->gridSize);
                }
            }
        }

        MoveToPosition(nextPathPos);  // otherwise move towards next node

        });

    stateMachine->AddState(wander);


    State* scared = new State([&](float dt) -> void {
        std::cout << "scared state" << std::endl;
        });

    stateMachine->AddState(scared);




    stateMachine->AddTransition(new StateTransition(wander, scared, [&]() -> bool { // transition to scared state if runs into player/enemy/etc
        Vector3 rayDir = GetTransform().GetOrientation() * Vector3(0, 0, -1);
        Vector3 rayPos = GetTransform().GetPosition();

        RayCollision closestCollision;  
        Ray r = Ray(rayPos, rayDir);

        if (this->world->Raycast(r, closestCollision, true, this, 5.0f)) {
            std::cout << ((GameObject*)closestCollision.node)->GetTypeID() << std::endl;
            return true;
        }

        }));
}

void AnimalObject::Update(float dt) {
    timer += dt;
    if (timer > 1.0) timer = 1.0;

    renderObject->GetAnimationObject()->SetFrameTime(renderObject->GetAnimationObject()->GetFrameTime() - dt);
    while (renderObject->GetAnimationObject()->GetFrameTime() < 0.0f) {
        renderObject->GetAnimationObject()->SetCurrentFrame((renderObject->GetAnimationObject()->GetCurrentFrame() + 1) %
            renderObject->GetAnimationObject()->GetActiveAnim()->GetFrameCount());
        renderObject->GetAnimationObject()->SetFrameTime(renderObject->GetAnimationObject()->GetFrameTime() + 1.0f /
            renderObject->GetAnimationObject()->GetActiveAnim()->GetFrameRate());
    }

    currentPos = GetTransform().GetPosition();

    stateMachine->Update(dt);

    for (int i = 0; i < wanderPathNodes.size() - 1; i++)
    {
        Debug::DrawLine(wanderPathNodes[i] + Vector3(0, 1, 0), wanderPathNodes[i + 1] + Vector3(0, 1, 0), Vector4(1, 0, 0, 1));
    }
}

void AnimalObject::MoveToPosition(Vector3 targetPos) {
    float speed = 10.0f;
    Vector3 direction = (targetPos - currentPos).Normalised();
    GetPhysicsObject()->SetLinearVelocity(Vector3(0, GetPhysicsObject()->GetLinearVelocity().y, 0) + direction * speed);

    float angle = atan2(-direction.x, -direction.z);
    float angleDegrees = Maths::RadiansToDegrees(angle);
    GetTransform().SetOrientation(Quaternion::Lerp(GetTransform().GetOrientation(), Quaternion::AxisAngleToQuaterion(Vector3(0, 1, 0), angleDegrees), timer));
}

bool AnimalObject::Pathfind(Vector3 targetPos) { // pathfinds to target position
    wanderPathNodes = {};
    currentNodeIndex = 0;

    NavigationPath outPath;
    bool foundPath = grid->FindPath(currentPos, targetPos, outPath);

    Vector3 pos;
    while (outPath.PopWaypoint(pos)) {  // converts path into Vector3 position nodes
        wanderPathNodes.push_back(pos);
    }

    return foundPath;
}