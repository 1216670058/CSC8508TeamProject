#include "RailObject.h"
#include "Window.h"

using namespace NCL::CSC8503;

void RailObject::OnCollisionBegin(GameObject* otherObject) {
    if (!inCarriage) {
        if (putDown && otherObject->GetTypeID() == 1 && otherObject->GetSlot() == 0) {
            putDown = false;
            player->SetSlot(this->GetTypeID());
            player->SetSlotNum(player->GetSlotNum() + 1);
        }
        else if (putDown && otherObject->GetTypeID() == 1 && otherObject->GetSlot() == 7 && otherObject->GetSlotNum() < 3) {
            putDown = false;
            num = player->GetSlotNum() + 1;
            player->SetSlotNum(player->GetSlotNum() + 1);
        }
    }
}

void RailObject::Update(float dt) {
    if (inCarriage) {
        Vector3 carriagePosition = carriage->GetTransform().GetPosition();
        switch (carriage->GetDirection()) {
        case 1:
            transform.SetPosition(Vector3(carriagePosition.x - 1, height, carriagePosition.z));
            break;
        case 2:
            transform.SetPosition(Vector3(carriagePosition.x + 1, height, carriagePosition.z));
            break;
        case 3:
            transform.SetPosition(Vector3(carriagePosition.x, height, carriagePosition.z - 1));
            break;
        case 4:
            transform.SetPosition(Vector3(carriagePosition.x, height, carriagePosition.z + 1));
            break;
        default:
            break;
        }
    }
    else if (!putDown) {
        Vector3 playerPosition = player->GetTransform().GetPosition();
        if (num == 1) transform.SetPosition(Vector3(playerPosition.x, playerPosition.y + 10, playerPosition.z));
        if (num == 2) transform.SetPosition(Vector3(playerPosition.x, playerPosition.y + 11, playerPosition.z));
        if (num == 3) transform.SetPosition(Vector3(playerPosition.x, playerPosition.y + 12, playerPosition.z));
        transform.SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, 0));
        physicsObject->ClearForces();
    }
    else {
        physicsObject->SetAngularVelocity(Vector3(0, 5, 0));
    }

    if (Window::GetKeyboard()->KeyPressed(KeyCodes::R)) {
        if (!inCarriage) {
            putDown = true;
            num = 1;
            Vector3 position = transform.GetPosition();
            transform.SetPosition(FindNearestGridCenter(Vector3(position.x, 5, position.z) - player->GetFace() * 5.0f));
        }
    }
}