#include "CollectableObject.h"
#include "Window.h"

using namespace NCL::CSC8503;

void CollectableObject::Update(float dt) {
    if (putDown) {
        physicsObject->SetAngularVelocity(Vector3(0, 5, 0));
    }
    else {
        Vector3 playerPosition = player->GetTransform().GetPosition();
        if (num == 1) transform.SetPosition(Vector3(playerPosition.x, playerPosition.y + 5, playerPosition.z));
        if (num == 2) transform.SetPosition(Vector3(playerPosition.x, playerPosition.y + 6, playerPosition.z));
        if (num == 3) transform.SetPosition(Vector3(playerPosition.x, playerPosition.y + 7, playerPosition.z));
        transform.SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, 0));
    }
    if (Window::GetKeyboard()->KeyPressed(KeyCodes::R)) {
        if (!putDown) {
            putDown = true;
            num = 1;
            Vector3 position = transform.GetPosition();
            transform.SetPosition(FindNearestGridCenter(Vector3(position.x, 5, position.z) + player->GetFace()*5.0f));
        }
    }
}

void CollectableObject::OnCollisionBegin(GameObject* otherObject)  {
    if (putDown && otherObject->GetTypeID() == 1 && otherObject->GetSlot() == 0) {
            putDown = false;
            player->SetSlot(this->GetTypeID());
    }
}

float CollectableObject::distance(const Vector3& v1, const Vector3& v2) {
    float dx = v1.x - v2.x;
    float dy = v1.y - v2.y;
    float dz = v1.z - v2.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

Vector3 CollectableObject::PlayerFront()
{
    Vector3 playerPos = player->GetTransform().GetPosition();
    Quaternion facingDir = player->GetTransform().GetOrientation();
    Vector3 ObjectOffset(0, 2, -5);
    Vector3 ObjectPos = facingDir * ObjectOffset;
    ObjectOffset = facingDir * ObjectOffset;
    Vector3 finalObjectPos = ObjectPos + playerPos;
    return finalObjectPos;
}