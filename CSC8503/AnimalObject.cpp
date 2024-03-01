#include "AnimalObject.h"

using namespace NCL::CSC8503;

AnimalObject::AnimalObject(float xMin, float xMax, float zMin, float zMax) {
    this->typeID = 10;
    this->stateMachine = new StateMachine();
    this->xMin = xMin;
    this->xMax = xMax;
    this->zMin = zMin;
    this->zMax = zMax;

    wanderPoints.push_back(Vector3(xMin, 2.5, zMin));
    wanderPoints.push_back(Vector3(xMax, 2.5, zMin));
    wanderPoints.push_back(Vector3(xMax, 2.5, zMax));
    wanderPoints.push_back(Vector3(xMin, 2.5, zMax));

    State* wander = new State([&](float dt) -> void {
        Vector3 nextWanderPointPos = wanderPoints[currentWanderIndex];
        float speed = 10.0f;

        if ((nextWanderPointPos - currentPos).LengthSquared() < 1.0f) {
            currentWanderIndex++;
            currentWanderIndex %= 4;
            nextWanderPointPos = wanderPoints[currentWanderIndex];
        }

        Vector3 direction = (nextWanderPointPos - currentPos).Normalised();
        GetPhysicsObject()->SetLinearVelocity(direction * speed);

        //float angle = atan2(-direction.x, -direction.z);
        //float angleDegrees = Maths::RadiansToDegrees(angle);
        //GetTransform().SetOrientation(Quaternion::AxisAngleToQuaterion(Vector3(0, 1, 0), angleDegrees));

        });

    stateMachine->AddState(wander);
}

void AnimalObject::Update(float dt) {
    renderObject->GetAnimationObject()->SetFrameTime(renderObject->GetAnimationObject()->GetFrameTime() - dt);
    while (renderObject->GetAnimationObject()->GetFrameTime() < 0.0f) {
        renderObject->GetAnimationObject()->SetCurrentFrame((renderObject->GetAnimationObject()->GetCurrentFrame() + 1) %
            renderObject->GetAnimationObject()->GetActiveAnim()->GetFrameCount());
        renderObject->GetAnimationObject()->SetFrameTime(renderObject->GetAnimationObject()->GetFrameTime() + 1.0f /
            renderObject->GetAnimationObject()->GetActiveAnim()->GetFrameRate());
    }

    currentPos = GetTransform().GetPosition();
    stateMachine->Update(dt);
}