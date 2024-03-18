#include "TutorialGame.h"
#include "PlayerObject.h"
#include "NavigationGrid.h"
#include "Window.h"

#include <stdlib.h>

using namespace NCL::CSC8503;

void PlayerObject::Update(float dt) {
    //if (NetworkedGame::GetNetworkedGame()->IsServer() || !TutorialGame::GetGame()->IsNetworked()) {

        //if (renderObject->GetAnimationObject())
        //    UpdateAnimation(dt);

        PlayerMovement(dt);

        doing = false;
        cutting = false;
        digging = false;

        //CutTree();
        //DigRock();
        //ScoopWater();
        //UseWater();
        //BuildBridge();
        //LoadMaterial();

        //Vector3 position = transform.GetPosition();
        //Vector3 p = FindGrid(Vector3(position.x, 2, position.z));
        //int index = p.x / 10 + (p.z / 10) * TutorialGame::GetGame()->GetNavigationGrid()->GetGridWidth();
        //std::cout << "Index: " << index << std::endl;
        //std::cout << "Type: " << TutorialGame::GetGame()->GetNavigationGrid()->GetGridNode(index).type << std::endl;
    //}
}

//void PlayerObject::OnCollisionBegin(GameObject* otherObject) {
//
//}
//static float a = 0;

void PlayerObject::UpdateAnimation(float dt) {
    float LeftX = TutorialGame::GetGame()->GetController()->GetNamedAxis("LeftX");
    float LeftY = TutorialGame::GetGame()->GetController()->GetNamedAxis("LeftY");

    if (LeftX != 0 && LeftY != 0)
        renderObject->UpdateAnimation(dt);
}

void PlayerObject::PlayerMovement(float dt) {
    Quaternion* qq = new Quaternion();
    speed = 50;
    //float yaw = Maths::RadiansToDegrees(atan2(-np.x, -np.z));
    //start->GetTransform().SetOrientation(qq->EulerAnglesToQuaternion(0, yaw, 0));
    float LeftX = TutorialGame::GetGame()->GetController()->GetNamedAxis("LeftX");
    float LeftY = TutorialGame::GetGame()->GetController()->GetNamedAxis("LeftY");
    //Debug::Print("LeftX: " + std::to_string(LeftX), Vector2(20, 20), Debug::BLUE);
    //Debug::Print("LeftY: " + std::to_string(LeftY), Vector2(20, 30), Debug::BLUE);

    Vector3 dir = Vector3(LeftX, 0, LeftY);
    physicsObject->SetRealDamping(0.962f);
    physicsObject->AddForce(dir * speed);
    if (LeftX != 0 && LeftY != 0) {
        if (abs(LeftX) > abs(LeftY)) {
            if (LeftX > 0) {
                face = Vector3(1, 0, 0);
                transform.SetOrientation(qq->EulerAnglesToQuaternion(0, 90, 0));
            }
            else {
                face = Vector3(-1, 0, 0);
                transform.SetOrientation(qq->EulerAnglesToQuaternion(0, -90, 0));
            }
        }
        else {
            if (LeftY > 0) {
                face = Vector3(0, 0, 1);
                transform.SetOrientation(qq->EulerAnglesToQuaternion(0, 0, 0));
            }
            else {
                face = Vector3(0, 0, -1);
                transform.SetOrientation(qq->EulerAnglesToQuaternion(0, 180, 0));
            }
        }
    }

    //else if (TutorialGame::GetGame()->GetController()->GetAxis(2)) {
    //    face = Vector3(-1, 0, 0);
    //    physicsObject->SetRealDamping(0.962f);
    //    physicsObject->AddForceAtPosition(face * speed, transform.GetPosition());
    //    transform.SetOrientation(qq->EulerAnglesToQuaternion(0, 90, 0));
    //}
    //else if (TutorialGame::GetGame()->GetController()->GetAxis(3)) {
    //    face = Vector3(0, 0, 1);
    //    physicsObject->SetRealDamping(0.962f);
    //    physicsObject->AddForceAtPosition(face * speed, transform.GetPosition());
    //    transform.SetOrientation(qq->EulerAnglesToQuaternion(0, 180, 0));
    //}
    //else if (TutorialGame::GetGame()->GetController()->GetAxis(4)) {
    //    face = Vector3(1, 0, 0);
    //    physicsObject->SetRealDamping(0.962f);
    //    physicsObject->AddForceAtPosition(face * speed, transform.GetPosition());
    //    transform.SetOrientation(qq->EulerAnglesToQuaternion(0, -90, 0));
    //}
    //else {
    //    physicsObject->SetRealDamping(0.858f);
    //}
    //std::cout << "Player: " << transform.GetPosition().x << " " << transform.GetPosition().y << " " << transform.GetPosition().z << std::endl;

//else {
//   // physicsObject->SetLinearVelocity(Vector3(0, 0, 0));
//}
//a += dt;
//if (a >= 1.0f) {
//    std::cout << "The force is: " << physicsObject->GetLinearVelocity().x << " " << physicsObject->GetLinearVelocity().y << " " << physicsObject->GetLinearVelocity().z << std::endl;
//    a = 0;
//}
//std::cout << "Player: " << transform.GetPosition().x << " " << transform.GetPosition().y << " " << transform.GetPosition().z << std::endl;
}

//void PlayerObject::UpdateFace() {
//    if (transform.GetOrientation().ToEuler() == Vector3(0, 0, 0))
//        face = Vector3(0, 0, -1);
//    else if (transform.GetOrientation().ToEuler() == Vector3(0, 90, 0))
//        face = Vector3(-1, 0, 0);
//    else if (transform.GetOrientation().ToEuler() == Vector3(0, 180, 0))
//        face = Vector3(0, 0, 1);
//    else if (transform.GetOrientation().ToEuler() == Vector3(0, -90, 0))
//        face = Vector3(1, 0, 0);
//}
//
////void PlayerObject::SwitchSkin() {
////    if (Window::GetKeyboard()->KeyPressed(KeyCodes::E)) {
////        renderObject->SetMesh(meshes[(index + 1) % 5]);
////        renderObject->SetAnimationObject(animations[(index + 1) % 5]);
////        renderObject->SetTextures(textures[(index + 1) % 5]);
////        renderObject->SetBumpTextures(bumpTextures[(index + 1) % 5]);
////        renderObject->SetShaderGroup(shaders[(index + 1) % 5]);
////        index++;
////    }
////}
//
//void PlayerObject::CutTree() {
//    bool spaceHeld = false;
//    if (networkObject->GetNetworkID() == 1)
//        spaceHeld = Window::GetKeyboard()->KeyHeld(NCL::KeyCodes::SPACE);
//    else
//        spaceHeld = buttonStates[5];
//
//    if (spaceHeld && slot == 3) {
//        int worldID1;
//        doing = true;
//        cutting = true;        
//        Ray r = Ray(transform.GetPosition(), face);
//        RayCollision closestCollision;
//        if (TutorialGame::GetGame()->GetWorld()->Raycast(r, closestCollision, true, this)) {
//            GameObject* closest = (GameObject*)closestCollision.node;
//            if (closest->GetTypeID() == 10086 && closestCollision.rayDistance < 5.0f) {
//                Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(1, 1, 0, 1));
//                closest->SetFlag1(true);
//                closest->GetTransform().SetScale(closest->GetTransform().GetScale() - Vector3(0.05, 0.05, 0.05));
//                if (closest->GetTransform().GetScale().x < 0.1f) {
//                    Vector3 position = FindGrid(closest->GetTransform().GetPosition());
//                    int index = position.x / 10 + (position.z / 10) * TutorialGame::GetGame()->GetNavigationGrid()->GetGridWidth();
//                    GridNode& n = TutorialGame::GetGame()->GetNavigationGrid()->GetGridNode(index);
//                    n.SetType(0);
//                    closest->SetFlag1(false);
//                    if (TutorialGame::GetGame()->IsNetworked()) {                       
//                        worldID1 = closest->GetWorldID();
//                    }
//                    PlankObject* plank1 = TutorialGame::GetGame()->AddPlankToWorld(Vector3(closest->GetTransform().GetPosition().x, 5, closest->GetTransform().GetPosition().z));                   
//                    TutorialGame::GetGame()->GetWorld()->RemoveGameObject(closest, false);
//                    if (TutorialGame::GetGame()->IsNetworked()) {
//                        NetworkedGame::GetNetworkedGame()->SetCutTreeFlag(true);
//                        NetworkedGame::GetNetworkedGame()->SetPlankNetworkID(plank1->GetNetworkObject()->GetNetworkID());
//                        NetworkedGame::GetNetworkedGame()->SetTreeWorldID(worldID1);
//                        NetworkedGame::GetNetworkedGame()->SetTreeCutTag(1);
//                    }
//                }
//            }
//            else {
//                Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(1, 0, 0, 1));
//            }
//        }
//        else {
//            Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(1, 0, 0, 1));
//        }
//    }
//}
//
//void PlayerObject::DigRock() {
//    bool spaceHeld = false;
//    if (networkObject->GetNetworkID() == 1)
//        spaceHeld = Window::GetKeyboard()->KeyHeld(NCL::KeyCodes::SPACE);
//    else
//        spaceHeld = buttonStates[5];
//
//    if (spaceHeld && slot == 2) {
//        int worldID1;
//        doing = true;
//        digging = true;
//        Ray r = Ray(transform.GetPosition(), face);
//        RayCollision closestCollision;
//        if (TutorialGame::GetGame()->GetWorld()->Raycast(r, closestCollision, true, this)) {
//            GameObject* closest = (GameObject*)closestCollision.node;
//            if (closest->GetTypeID() == 10010 && closestCollision.rayDistance < 5.0f) {
//                Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(1, 1, 0, 1));
//                closest->SetFlag1(true);
//                closest->GetTransform().SetScale(closest->GetTransform().GetScale() - Vector3(0.05, 0.05, 0.05));
//                if (closest->GetTransform().GetScale().x < 0.1f) {
//                    Vector3 position = FindGrid(closest->GetTransform().GetPosition());
//                    int index = position.x / 10 + (position.z / 10) * TutorialGame::GetGame()->GetNavigationGrid()->GetGridWidth();
//                    GridNode& n = TutorialGame::GetGame()->GetNavigationGrid()->GetGridNode(index);
//                    n.SetType(0);
//                    closest->SetFlag1(false);
//                    if (TutorialGame::GetGame()->IsNetworked()) {                       
//                        worldID1 = closest->GetWorldID();
//                    }
//                    StoneObject* stone1 = TutorialGame::GetGame()->AddStoneToWorld(Vector3(closest->GetTransform().GetPosition().x, 5, closest->GetTransform().GetPosition().z));
//                    TutorialGame::GetGame()->GetWorld()->RemoveGameObject(closest, false);
//                    if (TutorialGame::GetGame()->IsNetworked()) {
//                        NetworkedGame::GetNetworkedGame()->SetDigRockFlag(true);
//                        NetworkedGame::GetNetworkedGame()->SetStoneNetworkID(stone1->GetNetworkObject()->GetNetworkID());
//                        NetworkedGame::GetNetworkedGame()->SetRockWorldID(worldID1);
//                        NetworkedGame::GetNetworkedGame()->SetRockDugTag(2);
//                    }
//                }
//            }
//            else {
//                Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(1, 0, 0, 1));
//            }
//        }
//        else {
//            Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(1, 0, 0, 1));
//        }
//    }
//}
//
//void PlayerObject::ScoopWater() {
//    bool spaceHeld = false;
//    if (networkObject->GetNetworkID() == 1)
//        spaceHeld = Window::GetKeyboard()->KeyHeld(NCL::KeyCodes::SPACE);
//    else
//        spaceHeld = buttonStates[5];
//
//    if (spaceHeld && slot == 4) {
//        doing = true;
//        Ray r = Ray(transform.GetPosition(), face);
//        RayCollision closestCollision;
//        if (TutorialGame::GetGame()->GetWorld()->Raycast(r, closestCollision, true, this)) {
//            GameObject* closest = (GameObject*)closestCollision.node;
//            if (closest->GetTypeID() == 10000 && closestCollision.rayDistance < 5.0f) {
//                Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(1, 1, 0, 1));
//                TutorialGame::GetGame()->GetBucket()->GetRenderObject()->SetColour(Vector4(0, 0, 1, 1));
//                TutorialGame::GetGame()->GetBucket()->SetWater(true);
//            }
//            else {
//                Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(1, 0, 0, 1));
//            }
//        }
//        else {
//            Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(1, 0, 0, 1));
//        }
//    }
//}
//
//void PlayerObject::UseWater() {
//    bool spaceHeld = false;
//    if (networkObject->GetNetworkID() == 1)
//        spaceHeld = Window::GetKeyboard()->KeyHeld(NCL::KeyCodes::SPACE);
//    else
//        spaceHeld = buttonStates[5];
//
//    if (spaceHeld && slot == 4) {
//        doing = true;        
//        Ray r = Ray(transform.GetPosition(), face);
//        RayCollision closestCollision;
//        if (TutorialGame::GetGame()->GetWorld()->Raycast(r, closestCollision, true, this)) {
//            GameObject* closest = (GameObject*)closestCollision.node;
//            if (closest->GetTypeID() == 23 && closestCollision.rayDistance < 5.0f && TutorialGame::GetGame()->GetBucket()->GetWater() == true) {
//                Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(1, 1, 0, 1));
//                WaterCarriage* waterCarriage = (WaterCarriage*)closest;
//                waterCarriage->SetCarriageWater(100.0f);
//                TutorialGame::GetGame()->GetTrain()->SetOnFire(false);
//                TutorialGame::GetGame()->GetTrain()->SetFire(100.0f);
//                TutorialGame::GetGame()->GetBucket()->SetWater(false);
//                TutorialGame::GetGame()->GetBucket()->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
//            }
//            else {
//                Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(1, 0, 0, 1));
//            }
//        }
//        else {
//            Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(1, 0, 0, 1));
//        }
//    }
//}
//
//void PlayerObject::BuildBridge() {
//    bool FPressed = false;
//    if (networkObject->GetNetworkID() == 1)
//        FPressed = Window::GetKeyboard()->KeyPressed(NCL::KeyCodes::F);
//    else
//        FPressed = buttonStates[7];
//
//    if (FPressed && slot == 5) {
//        int worldID1;
//        doing = true;
//        Ray r = Ray(transform.GetPosition(), face);
//        RayCollision closestCollision;
//        if (TutorialGame::GetGame()->GetWorld()->Raycast(r, closestCollision, true, this)) {
//            GameObject* closest = (GameObject*)closestCollision.node;
//            if (closest->GetTypeID() == 10000 && closestCollision.rayDistance < 5.0f) {
//                Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(1, 1, 0, 1));
//                bridgePosition = closest->GetTransform().GetPosition();
//                if (TutorialGame::GetGame()->IsNetworked()) {
//                    worldID1 = closest->GetWorldID();
//                }
//                TutorialGame::GetGame()->GetWorld()->RemoveGameObject(closest, false);
//                building = true;
//                if (TutorialGame::GetGame()->IsNetworked()) {
//                    NetworkedGame::GetNetworkedGame()->SetBuildBridgeFlag(true);
//                    NetworkedGame::GetNetworkedGame()->SetBridgeBuiltTag(8);
//                    NetworkedGame::GetNetworkedGame()->SetWaterWorldID(worldID1);
//                }
//            }
//            else {
//                Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(1, 0, 0, 1));
//            }
//        }
//        else {
//            Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(1, 0, 0, 1));
//        }
//    }
//}
//
//void PlayerObject::LoadMaterial() {
//    bool spacePressed = false;
//    if (networkObject->GetNetworkID() == 1)
//        spacePressed = Window::GetKeyboard()->KeyPressed(NCL::KeyCodes::SPACE);
//    else
//        spacePressed = buttonStates[6];
//
//    if (spacePressed) {
//        if (slot == 5 || slot == 6) {
//            Ray r = Ray(transform.GetPosition(), face);
//            RayCollision closestCollision;
//            if (TutorialGame::GetGame()->GetWorld()->Raycast(r, closestCollision, true, this)) {
//                GameObject* closest = (GameObject*)closestCollision.node;
//                if (closest->GetTypeID() == 21 && closestCollision.rayDistance < 5.0f) {
//                    Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(1, 1, 0, 1));
//                    carriage = (MaterialCarriage*)closest;
//                }
//                else {
//                    Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(1, 0, 0, 1));
//                }
//            }
//            else {
//                Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(1, 0, 0, 1));
//            }
//        }
//    }
//}
//
//bool PlayerObject::CanPlaceRail() {
//    bool canConnect = false;
//    bool isPath = false;
//    bool notRail = false;
//    int connectedIndex;
//    Vector3 position = FindGrid(Vector3(transform.GetPosition().x, 4.5f, transform.GetPosition().z));
//    int index = position.x / 10 + (position.z / 10) * TutorialGame::GetGame()->GetNavigationGrid()->GetGridWidth();
//    GridNode& n = TutorialGame::GetGame()->GetNavigationGrid()->GetGridNode(index);
//    notRail = n.type != 7 ? true : false;
//    for (int i = 0; i < 4; ++i) {
//        if (n.connected[i]) {
//            if (n.connected[i]->type == 7) {
//                canConnect = true;
//                connectedIndex = i;
//                break;
//            }
//        }
//    }
//    if ((TutorialGame::GetGame()->GetTrain()->GetLastPath() - position).Length() < 14)
//        isPath = true;
//
//    return canConnect && isPath && notRail;
//}