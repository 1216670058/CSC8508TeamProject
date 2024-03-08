#include "CollectableObject.h"
#include "Window.h"

using namespace NCL::CSC8503;

void CollectableObject::Update(float dt) {
    if (putDown) {
        physicsObject->SetAngularVelocity(Vector3(0, 5, 0));
    }
    else {
        Vector3 playerPosition = player->GetTransform().GetPosition();
        transform.SetPosition(Vector3(playerPosition.x, playerPosition.y + 8, playerPosition.z));
        transform.SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, 0));

        bool RPressed = false;
        if (player->GetNetworkObject()->GetNetworkID() == 1)
            RPressed = Window::GetKeyboard()->KeyPressed(KeyCodes::R);
        else
            RPressed = player->GetButton(4);
        
        if (RPressed) {
            putDown = true;
            num = 1;
            Vector3 position = transform.GetPosition();
            transform.SetPosition(FindGrid(Vector3(position.x, 5, position.z) - player->GetFace() * 5.0f));
            player = nullptr;
        }       
    }
}

void CollectableObject::OnCollisionBegin(GameObject* otherObject) {
    if (putDown && otherObject->GetTypeID() == 1 && otherObject->GetSlot() == 0) {
        player = (PlayerObject*)otherObject;
        putDown = false;
        player->SetSlot(this->GetTypeID());
    }
}

float CollectableObject::Distance(const Vector3& v1, const Vector3& v2) {
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