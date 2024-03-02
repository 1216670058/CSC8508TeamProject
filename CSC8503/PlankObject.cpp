#include "PlankObject.h"
#include "Window.h"

using namespace NCL::CSC8503;

void PlankObject::OnCollisionBegin(GameObject* otherObject) {
    if (putDown && otherObject->GetTypeID() == 1 && otherObject->GetSlot() == 0) {
        player = (PlayerObject*)otherObject;
        putDown = false;
        player->SetSlot(this->GetTypeID());
        player->SetSlotNum(player->GetSlotNum() + 1);
        AABBVolume* volume = new AABBVolume(Vector3(1, 1, 1));
        SetBoundingVolume((CollisionVolume*)volume);
        transform.SetScale(Vector3(2, 2, 2));
    }
    else if (putDown && otherObject->GetTypeID() == 1 && otherObject->GetSlot() == 5 && otherObject->GetSlotNum() < 3) {
        player = (PlayerObject*)otherObject;
        putDown = false;
        num = player->GetSlotNum() + 1;
        player->SetSlotNum(player->GetSlotNum() + 1);
        AABBVolume* volume = new AABBVolume(Vector3(1, 1, 1));
        SetBoundingVolume((CollisionVolume*)volume);
        transform.SetScale(Vector3(2, 2, 2));
    }
}

void PlankObject::Update(float dt) {
    if (putDown) {
        physicsObject->SetAngularVelocity(Vector3(0, 5, 0));
    }
    else {
        if (!inCarriage) {
            if (!loading) {
                Vector3 playerPosition = player->GetTransform().GetPosition();
                if (num == 1) transform.SetPosition(Vector3(playerPosition.x, playerPosition.y + 10, playerPosition.z));
                if (num == 2) transform.SetPosition(Vector3(playerPosition.x, playerPosition.y + 11, playerPosition.z));
                if (num == 3) transform.SetPosition(Vector3(playerPosition.x, playerPosition.y + 12, playerPosition.z));
                transform.SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, 0));
                physicsObject->ClearForces();
                if (player->GetCarriage()) loading = true;
            }
            else {
                if (player->GetSlotNum() > 0 && player->GetSlot() == 5) {
                    carriage = player->GetCarriage();
                    if (carriage->GetPlankVector().size() < 10) {
                        carriage->AddPlank(this);
                        loading = false;
                        inCarriage = true;
                        height = carriage->GetTransform().GetPosition().y + 4 + carriage->GetPlankVector().size();
                    }
                    player->SetSlotNum(player->GetSlotNum() - 1);
                    if (player->GetSlotNum() == 0) {
                        player->SetSlot(0);
                        player->SetCarriage(nullptr);
                        player = nullptr;
                    }
                }
            }
        }
        else {
            Vector3 carriagePosition = carriage->GetTransform().GetPosition();
            switch (carriage->GetDirection()) {
            case 1:
                transform.SetPosition(Vector3(carriagePosition.x + 1, height, carriagePosition.z));
                break;
            case 2:
                transform.SetPosition(Vector3(carriagePosition.x - 1, height, carriagePosition.z));
                break;
            case 3:
                transform.SetPosition(Vector3(carriagePosition.x, height, carriagePosition.z + 1));
                break;
            case 4:
                transform.SetPosition(Vector3(carriagePosition.x, height, carriagePosition.z - 1));
                break;
            default:
                break;
            }
        }

        if (!loading && !inCarriage) {
            bool RPressed = false;
            if (player->GetNetworkObject()->GetNetworkID() == 1)
                RPressed = Window::GetKeyboard()->KeyPressed(KeyCodes::R);
            else
                RPressed = player->GetButton(4);

            if (RPressed) {
                if (!loading && !inCarriage) {
                    putDown = true;
                    num = 1;
                    Vector3 position = transform.GetPosition();
                    transform.SetPosition(FindNearestGridCenter(Vector3(position.x, 5, position.z) - player->GetFace() * 5.0f));
                    AABBVolume* volume = new AABBVolume(Vector3(2, 2, 2));
                    SetBoundingVolume((CollisionVolume*)volume);
                    transform.SetScale(Vector3(4, 4, 4));
                    player = nullptr;
                }
            }
        }
    }
}