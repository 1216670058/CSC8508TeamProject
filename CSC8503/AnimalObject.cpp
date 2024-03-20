#include "AnimalObject.h"

using namespace NCL::CSC8503;

AnimalObject::AnimalObject(NavigationGrid* navGrid, Vector3 startingPos, GameWorld* world) {
    this->typeID = 10;
    this->stateMachine = new StateMachine();
    this->currentPos = startingPos;
    this->grid = navGrid;
    this->gridSize = grid->GetGridWidth() * grid->GetGridHeight();
    this->world = world;

    GridNode n = grid->GetGridNode(rand() % this->gridSize);
    while (n.type != 0 || !Pathfind(n.position) || (n.position - currentPos).LengthSquared() > 6000.0f) {
        n = grid->GetGridNode(rand() % this->gridSize);
    }

    timer = 0;

    State* wander = new State([&](float dt) -> void {
        //std::cout << "enter wander state" << std::endl;
        if (wanderPathNodes.empty()) {
            n = grid->GetGridNode(rand() % this->gridSize);
            while (n.type != 0 || !Pathfind(n.position) || (n.position - currentPos).LengthSquared() > 6000.0f) {
                n = grid->GetGridNode(rand() % this->gridSize);
            }
        }

        Vector3 nextPathPos = wanderPathNodes[currentNodeIndex];

        if ((nextPathPos - currentPos).LengthSquared() < 100.0f) { // if close to current node
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

        MoveToPosition(nextPathPos, 10.0f);  // otherwise move towards next node

        });

    stateMachine->AddState(wander);


    State* scared = new State([&](float dt) -> void {
        //std::cout << "enter scared state p2" << std::endl;
        /*float speed = 20.0f;

        Vector3 prevPathPos = wanderPathNodes[currentNodeIndex - 1];

        Vector3 direction = (prevPathPos - scaredPos).Normalised();
        GetPhysicsObject()->SetLinearVelocity(Vector3(0, GetPhysicsObject()->GetLinearVelocity().y, 0) + direction * speed);

        float angle = atan2(-direction.x, -direction.z);
        float angleDegrees = Maths::RadiansToDegrees(angle);
        GetTransform().SetOrientation(Quaternion::AxisAngleToQuaterion(Vector3(0, 1, 0), angleDegrees));*/

        /*if (wanderPathNodes.empty()) {
            n = grid->GetGridNode(rand() % this->gridSize);
            while (n.type != 0 || !Pathfind(n.position) || (n.position - currentPos).LengthSquared() > 6000.0f) {
                n = grid->GetGridNode(rand() % this->gridSize);
            }
        }

        Vector3 nextPathPos = wanderPathNodes[currentNodeIndex];

        if ((nextPathPos - currentPos).LengthSquared() < 1.0f) { // if close to current node
            if (currentNodeIndex > 0) {    // if node isnt the final node, target next node
                currentNodeIndex--;
                nextPathPos = wanderPathNodes[currentNodeIndex];
                timer = 0;
            }
            else { 
                std::cout << "exit scared state" << std::endl;
                //currentNodeIndex = wanderPathNodes.size() - 1;
                wanderPathNodes.clear();
                noLongerScared = true;
            }
        }

        MoveToPosition(nextPathPos, 20.0f);  // otherwise move towards next node*/

        
        /*if (wanderPathNodes.empty()) {
            n = grid->GetGridNode(rand() % this->gridSize);
            while (n.type != 0 || !Pathfind(n.position) || (n.position - currentPos).LengthSquared() > 6000.0f) {
                n = grid->GetGridNode(rand() % this->gridSize);
            }
        }

        Vector3 nextPathPos = wanderPathNodes[currentNodeIndex];

        if ((nextPathPos - currentPos).LengthSquared() < 1.0f) { 
            if (currentNodeIndex < (wanderPathNodes.size() - 1)) {    
                currentNodeIndex++;
                nextPathPos = wanderPathNodes[currentNodeIndex];
                timer = 0;
            }
            else { 

                //std::cout << "exit scared state p1" << std::endl;
                noLongerScared = true;
            }
        }

        MoveToPosition(nextPathPos, 20.0f);  */

        float speed = 20.0f;
        Vector3 direction = (currentPos - threat->GetTransform().GetPosition()).Normalised();
        direction = Vector3(direction.x, 0, direction.z);
        GetPhysicsObject()->SetLinearVelocity(direction * speed);
        //GetPhysicsObject()->AddForce(direction * speed);

        float angle = atan2(-direction.x, -direction.z);
        float angleDegrees = Maths::RadiansToDegrees(angle);
        GetTransform().SetOrientation(Quaternion::AxisAngleToQuaterion(Vector3(0, 1, 0), angleDegrees));

        
        
        
        });

    stateMachine->AddState(scared);




    stateMachine->AddTransition(new StateTransition(wander, scared, [&]() -> bool { // transition to scared state if runs into player/enemy/etc

        if (threatDetected && stateCooldown > 1.0) {
            std::cout << "entering scared state\n";
            stateCooldown = 0;
            return true;
        }
        return false;

        /*Vector3 rayDir = GetTransform().GetOrientation() * Vector3(0, 0, -1);
        Vector3 rayPos = GetTransform().GetPosition();

        RayCollision closestCollision;  
        Ray r = Ray(rayPos, rayDir);

        if (this->world->Raycast(r, closestCollision, true, this, 10.0f) && stateCooldown > 0.5) {
            //std::cout << ((GameObject*)closestCollision.node)->GetTypeID() << std::endl;
            //scaredPos = currentPos;
            //currentNodeIndex--;
            stateCooldown = 0;

            //for (std::vector<Vector3>::iterator it = wanderPathNodes.begin(); it != wanderPathNodes.end(); ++it)
                //std::cout << ' ' << *it;
            //std::cout << '\n';

            std::reverse(wanderPathNodes.begin(), wanderPathNodes.end());

            for (std::vector<Vector3>::iterator it = wanderPathNodes.begin(); it != wanderPathNodes.end(); ++it)
                //std::cout << ' ' << *it;
            //std::cout << '\n';

            //std::cout << "enter scared state p1" << std::endl;
            currentNodeIndex = 0;
            return true;
        }

        return false;*/

        }));


    stateMachine->AddTransition(new StateTransition(scared, wander, [&]() -> bool {
        /*if (noLongerScared && stateCooldown > 0.5) {
            noLongerScared = false;
            //std::cout << "exit scared state p2" << std::endl;
            stateCooldown = 0;
            return true;
        }

        return false;*/

        if (!threatDetected && stateCooldown > 1.0) {
            std::cout << "exiting scared state\n";
            stateCooldown = 0;
            threat = nullptr;

            GridNode n = grid->GetGridNode(rand() % this->gridSize);
            while (n.type != 0 || !Pathfind(n.position) || (n.position - currentPos).LengthSquared() > 6000.0f) {
                n = grid->GetGridNode(rand() % this->gridSize);
            }

            return true;
        }
        return false;

        }));
}

void AnimalObject::Update(float dt) {
    timer += dt;
    stateCooldown += dt;
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

    if (wanderPathNodes.empty()) return;
    for (int i = 0; i < wanderPathNodes.size() - 1; i++)
    {
        Debug::DrawLine(wanderPathNodes[i] + Vector3(0, 1, 0), wanderPathNodes[i + 1] + Vector3(0, 1, 0), Vector4(1, 0, 0, 1));
    }
}

void AnimalObject::MoveToPosition(Vector3 targetPos, float speed) {
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

void AnimalObject::DetectThreat(GameObject* object) {
    //std::cout << "detected " << object->GetName() << std::endl;           

    if (object->GetTypeID() == 1) { // player
        threatDetected = true;
        threat = object;
    }
}

void AnimalObject::StopDetectThreat(GameObject* object) {
    //std::cout << "stopped detecting " << object->GetName() << std::endl;
    if (threat == object) {
        threatDetected = false;
    }
}





DetectionSphereObject::DetectionSphereObject(AnimalObject* animal) {
    this->animal = animal;
}