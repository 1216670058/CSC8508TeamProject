#include "TutorialGame.h"
#include "PlayerObject.h"
#include "NetworkedGame.h"
#include "Window.h"

using namespace NCL::CSC8503;

void PlayerObject::Update(float dt) {
    if (NetworkedGame::GetNetworkedGame()->IsServer() || !TutorialGame::GetGame()->IsNetworked()) {
        bool RPressed = false;
        if (networkObject->GetNetworkID() == 1)
            RPressed = Window::GetKeyboard()->KeyPressed(KeyCodes::R);
        else
            RPressed = buttonStates[4];

        if (renderObject->GetAnimationObject())
            UpdateAnimation(dt);
        PlayerMovement(dt);
        //SwitchSkin();


        if (RPressed) {
            slot = 0;
            slotNum = 0;
        }

        doing = false;
        cutting = false;
        digging = false;

        CutTree();
        DigRock();
        ScoopWater();
        UseWater();
        LoadMaterial();
    }
    //else if (NetworkedGame::GetNetworkedGame()->IsClient()) {
    //    UpdateFace();
    //}
}

void PlayerObject::OnCollisionBegin(GameObject* otherObject) {

}
static float a = 0;

void PlayerObject::UpdateAnimation(float dt) {
    bool WHeld = false;
    bool DHeld = false;
    bool AHeld = false;
    bool SHeld = false;

    if (networkObject->GetNetworkID() == 1) {
        WHeld = Window::GetKeyboard()->KeyHeld(KeyCodes::W);
        DHeld = Window::GetKeyboard()->KeyHeld(KeyCodes::D);
        AHeld = Window::GetKeyboard()->KeyHeld(KeyCodes::A);
        SHeld = Window::GetKeyboard()->KeyHeld(KeyCodes::S);
    }
    else {
        WHeld = buttonStates[0];
        SHeld = buttonStates[1];
        AHeld = buttonStates[2];
        DHeld = buttonStates[3];
    }

    if (!doing) {
        if (WHeld && renderObject->GetAnimationObject()->GetAnim2()) {
            if (renderObject->GetAnimationObject()->GetFlag2()) {
                renderObject->GetAnimationObject()->SetActiveAnim(renderObject->GetAnimationObject()->GetAnim2());
                renderObject->GetAnimationObject()->SetCurrentFrame(0);
                renderObject->GetAnimationObject()->SetFrameTime(0.0f);
                renderObject->GetAnimationObject()->UpdateFlags(2);
            }
        }
        else if (DHeld && renderObject->GetAnimationObject()->GetAnim3()) {
            if (renderObject->GetAnimationObject()->GetFlag3()) {
                renderObject->GetAnimationObject()->SetActiveAnim(renderObject->GetAnimationObject()->GetAnim3());
                renderObject->GetAnimationObject()->SetCurrentFrame(0);
                renderObject->GetAnimationObject()->SetFrameTime(0.0f);
                renderObject->GetAnimationObject()->UpdateFlags(3);
            }
        }
        else if (AHeld && renderObject->GetAnimationObject()->GetAnim4()) {
            if (renderObject->GetAnimationObject()->GetFlag4()) {
                renderObject->GetAnimationObject()->SetActiveAnim(renderObject->GetAnimationObject()->GetAnim4());
                renderObject->GetAnimationObject()->SetCurrentFrame(0);
                renderObject->GetAnimationObject()->SetFrameTime(0.0f);
                renderObject->GetAnimationObject()->UpdateFlags(4);
            }
        }
        else if (SHeld && renderObject->GetAnimationObject()->GetAnim5()) {
            if (renderObject->GetAnimationObject()->GetFlag5()) {
                renderObject->GetAnimationObject()->SetActiveAnim(renderObject->GetAnimationObject()->GetAnim5());
                renderObject->GetAnimationObject()->SetCurrentFrame(0);
                renderObject->GetAnimationObject()->SetFrameTime(0.0f);
                renderObject->GetAnimationObject()->UpdateFlags(5);
            }
        }
        else if (Window::GetKeyboard()->KeyHeld(KeyCodes::NUM3) && renderObject->GetAnimationObject()->GetAnim8()) {
            if (renderObject->GetAnimationObject()->GetFlag8()) {
                renderObject->GetAnimationObject()->SetActiveAnim(renderObject->GetAnimationObject()->GetAnim8());
                renderObject->GetAnimationObject()->SetCurrentFrame(0);
                renderObject->GetAnimationObject()->SetFrameTime(0.0f);
                renderObject->GetAnimationObject()->UpdateFlags(8);
            }
        }
        else {
            if (renderObject->GetAnimationObject()->GetFlag1()) {
                renderObject->GetAnimationObject()->SetActiveAnim(renderObject->GetAnimationObject()->GetAnim1());
                renderObject->GetAnimationObject()->SetCurrentFrame(0);
                renderObject->GetAnimationObject()->SetFrameTime(0.0f);
                renderObject->GetAnimationObject()->UpdateFlags(1);
            }
        }
    }

    else {
        if (cutting && renderObject->GetAnimationObject()->GetAnim6()) {
            if (renderObject->GetAnimationObject()->GetFlag6()) {
                renderObject->GetAnimationObject()->SetActiveAnim(renderObject->GetAnimationObject()->GetAnim6());
                renderObject->GetAnimationObject()->SetCurrentFrame(0);
                renderObject->GetAnimationObject()->SetFrameTime(0.0f);
                renderObject->GetAnimationObject()->UpdateFlags(6);
            }
        }
        else if (digging && renderObject->GetAnimationObject()->GetAnim7()) {
            if (renderObject->GetAnimationObject()->GetFlag7()) {
                renderObject->GetAnimationObject()->SetActiveAnim(renderObject->GetAnimationObject()->GetAnim7());
                renderObject->GetAnimationObject()->SetCurrentFrame(0);
                renderObject->GetAnimationObject()->SetFrameTime(0.0f);
                renderObject->GetAnimationObject()->UpdateFlags(7);
            }
        }
        else {
            if (renderObject->GetAnimationObject()->GetFlag1()) {
                renderObject->GetAnimationObject()->SetActiveAnim(renderObject->GetAnimationObject()->GetAnim1());
                renderObject->GetAnimationObject()->SetCurrentFrame(0);
                renderObject->GetAnimationObject()->SetFrameTime(0.0f);
                renderObject->GetAnimationObject()->UpdateFlags(1);
            }
        }
    }

    if (renderObject->GetAnimationObject()->GetActiveAnim() != renderObject->GetAnimationObject()->GetAnim1() ||
        renderObject->GetAnimationObject()->HasIdle()) {
        if (!TutorialGame::GetGame()->IsNetworked())
            renderObject->GetAnimationObject()->SetFrameTime(renderObject->GetAnimationObject()->GetFrameTime() - dt);
        else
            renderObject->GetAnimationObject()->SetFrameTime(renderObject->GetAnimationObject()->GetFrameTime() - (dt * 4));
        while (renderObject->GetAnimationObject()->GetFrameTime() < 0.0f) {
            renderObject->GetAnimationObject()->SetCurrentFrame((renderObject->GetAnimationObject()->GetCurrentFrame() + 1) %
                renderObject->GetAnimationObject()->GetActiveAnim()->GetFrameCount());
            renderObject->GetAnimationObject()->SetFrameTime(renderObject->GetAnimationObject()->GetFrameTime() + 1.0f /
                renderObject->GetAnimationObject()->GetActiveAnim()->GetFrameRate());
        }
    }
}

void PlayerObject::PlayerMovement(float dt) {
    bool WHeld = false;
    bool DHeld = false;
    bool AHeld = false;
    bool SHeld = false;

    if (networkObject->GetNetworkID() == 1) {
        WHeld = Window::GetKeyboard()->KeyHeld(KeyCodes::W);
        DHeld = Window::GetKeyboard()->KeyHeld(KeyCodes::D);
        AHeld = Window::GetKeyboard()->KeyHeld(KeyCodes::A);
        SHeld = Window::GetKeyboard()->KeyHeld(KeyCodes::S);
    }
    else {
        WHeld = buttonStates[0];
        SHeld = buttonStates[1];
        AHeld = buttonStates[2];
        DHeld = buttonStates[3];
    }

    Quaternion* qq;
    speed = TutorialGame::GetGame()->IsNetworked() ? 100 : 50;
    //float yaw = Maths::RadiansToDegrees(atan2(-np.x, -np.z));
    //start->GetTransform().SetOrientation(qq->EulerAnglesToQuaternion(0, yaw, 0));

    if (WHeld) {
        face = Vector3(0, 0, -1);
        physicsObject->SetRealDamping(0.962f);
        physicsObject->AddForceAtPosition(face * speed, transform.GetPosition());
        transform.SetOrientation(qq->EulerAnglesToQuaternion(0, 0, 0));
    }
    else if (AHeld) {
        face = Vector3(-1, 0, 0);
        physicsObject->SetRealDamping(0.962f);
        physicsObject->AddForceAtPosition(face * speed, transform.GetPosition());
        transform.SetOrientation(qq->EulerAnglesToQuaternion(0, 90, 0));
    }
    else if (SHeld) {
        face = Vector3(0, 0, 1);
        physicsObject->SetRealDamping(0.962f);
        physicsObject->AddForceAtPosition(face * speed, transform.GetPosition());
        transform.SetOrientation(qq->EulerAnglesToQuaternion(0, 180, 0));
    }
    else if (DHeld) {
        face = Vector3(1, 0, 0);
        physicsObject->SetRealDamping(0.962f);
        physicsObject->AddForceAtPosition(face * speed, transform.GetPosition());
        transform.SetOrientation(qq->EulerAnglesToQuaternion(0, -90, 0));
    }
    else {
        physicsObject->SetRealDamping(0.858f);
    }
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

void PlayerObject::UpdateFace() {
    if (transform.GetOrientation().ToEuler() == Vector3(0, 0, 0))
        face = Vector3(0, 0, -1);
    else if (transform.GetOrientation().ToEuler() == Vector3(0, 90, 0))
        face = Vector3(-1, 0, 0);
    else if (transform.GetOrientation().ToEuler() == Vector3(0, 180, 0))
        face = Vector3(0, 0, 1);
    else if (transform.GetOrientation().ToEuler() == Vector3(0, -90, 0))
        face = Vector3(1, 0, 0);
}

void PlayerObject::SwitchSkin() {
    if (Window::GetKeyboard()->KeyPressed(KeyCodes::E)) {
        renderObject->SetMesh(meshes[(index + 1) % 5]);
        renderObject->SetAnimationObject(animations[(index + 1) % 5]);
        renderObject->SetTextures(textures[(index + 1) % 5]);
        renderObject->SetBumpTextures(bumpTextures[(index + 1) % 5]);
        renderObject->SetShaderGroup(shaders[(index + 1) % 5]);
        index++;
    }
}

void PlayerObject::CutTree() {
    bool spaceHeld = false;
    if (networkObject->GetNetworkID() == 1)
        spaceHeld = Window::GetKeyboard()->KeyHeld(NCL::KeyCodes::SPACE);
    else
        spaceHeld = buttonStates[5];

    if (spaceHeld && slot == 3) {
        int worldID1;
        doing = true;
        cutting = true;        
        Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(1, 1, 0, 1));
        Ray r = Ray(transform.GetPosition(), face);
        RayCollision closestCollision;
        if (TutorialGame::GetGame()->GetWorld()->Raycast(r, closestCollision, true, this)) {
            GameObject* closest = (GameObject*)closestCollision.node;
            if (closest->GetTypeID() == 10086 && closestCollision.rayDistance < 5.0f) {
                closest->SetFlag1(true);
                closest->GetTransform().SetScale(closest->GetTransform().GetScale() - Vector3(0.05, 0.05, 0.05));
                if (closest->GetTransform().GetScale().x < 0.1f) {
                    closest->SetFlag1(false);
                    if (TutorialGame::GetGame()->IsNetworked()) {                       
                        worldID1 = closest->GetWorldID();
                    }
                    PlankObject* plank1 = TutorialGame::GetGame()->AddPlankToWorld(Vector3(closest->GetTransform().GetPosition().x, 5, closest->GetTransform().GetPosition().z));                   
                    TutorialGame::GetGame()->GetWorld()->RemoveGameObject(closest, false);
                    if (TutorialGame::GetGame()->IsNetworked()) {
                        NetworkedGame::GetNetworkedGame()->SetCutTreeFlag(true);
                        NetworkedGame::GetNetworkedGame()->SetPlankNetworkID(plank1->GetNetworkObject()->GetNetworkID());
                        NetworkedGame::GetNetworkedGame()->SetTreeWorldID(worldID1);
                        NetworkedGame::GetNetworkedGame()->SetTreeCutTag(1);
                    }
                }
            }
        }
    }
}

void PlayerObject::DigRock() {
    bool spaceHeld = false;
    if (networkObject->GetNetworkID() == 1)
        spaceHeld = Window::GetKeyboard()->KeyHeld(NCL::KeyCodes::SPACE);
    else
        spaceHeld = buttonStates[5];

    if (spaceHeld && slot == 2) {
        int worldID1;
        doing = true;
        digging = true;
        Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(8, 5, 0, 8));
        Ray r = Ray(transform.GetPosition(), face);
        RayCollision closestCollision;
        if (TutorialGame::GetGame()->GetWorld()->Raycast(r, closestCollision, true, this)) {
            GameObject* closest = (GameObject*)closestCollision.node;
            if (closest->GetTypeID() == 10010 && closestCollision.rayDistance < 5.0f) {
                closest->SetFlag1(true);
                closest->GetTransform().SetScale(closest->GetTransform().GetScale() - Vector3(0.05, 0.05, 0.05));
                if (closest->GetTransform().GetScale().x < 0.1f) {
                    closest->SetFlag1(false);
                    if (TutorialGame::GetGame()->IsNetworked()) {                       
                        worldID1 = closest->GetWorldID();
                    }
                    StoneObject* stone1 = TutorialGame::GetGame()->AddStoneToWorld(Vector3(closest->GetTransform().GetPosition().x, 5, closest->GetTransform().GetPosition().z));
                    TutorialGame::GetGame()->GetWorld()->RemoveGameObject(closest, false);
                    if (TutorialGame::GetGame()->IsNetworked()) {
                        NetworkedGame::GetNetworkedGame()->SetDigRockFlag(true);
                        NetworkedGame::GetNetworkedGame()->SetStoneNetworkID(stone1->GetNetworkObject()->GetNetworkID());
                        NetworkedGame::GetNetworkedGame()->SetRockWorldID(worldID1);
                        NetworkedGame::GetNetworkedGame()->SetRockDugTag(2);
                    }
                }
            }
        }
    }
}

void PlayerObject::ScoopWater() {
    bool spaceHeld = false;
    if (networkObject->GetNetworkID() == 1)
        spaceHeld = Window::GetKeyboard()->KeyHeld(NCL::KeyCodes::SPACE);
    else
        spaceHeld = buttonStates[5];

    if (spaceHeld && slot == 4) {
        doing = true;
        Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(8, 5, 0, 8));
        Ray r = Ray(transform.GetPosition(), face);
        RayCollision closestCollision;
        if (TutorialGame::GetGame()->GetWorld()->Raycast(r, closestCollision, true, this)) {
            GameObject* closest = (GameObject*)closestCollision.node;
            if (closest->GetTypeID() == 10000 && closestCollision.rayDistance < 5.0f) {
                TutorialGame::GetGame()->GetBucket()->GetRenderObject()->SetColour(Vector4(0, 0, 1, 1));
                TutorialGame::GetGame()->GetBucket()->SetWater(true);
            }
        }
    }
}

void PlayerObject::UseWater() {
    bool spaceHeld = false;
    if (networkObject->GetNetworkID() == 1)
        spaceHeld = Window::GetKeyboard()->KeyHeld(NCL::KeyCodes::SPACE);
    else
        spaceHeld = buttonStates[5];

    if (spaceHeld && slot == 4) {
        doing = true;
        Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(8, 5, 0, 8));
        Ray r = Ray(transform.GetPosition(), face);
        RayCollision closestCollision;
        if (TutorialGame::GetGame()->GetWorld()->Raycast(r, closestCollision, true, this)) {
            GameObject* closest = (GameObject*)closestCollision.node;
            if (closest->GetTypeID() == 23 && closestCollision.rayDistance < 5.0f && TutorialGame::GetGame()->GetBucket()->GetWater() == true) {
                WaterCarriage* watercarriage = (WaterCarriage*)closest;
                watercarriage->SetCarriageWater(100.0f);
                TutorialGame::GetGame()->GetBucket()->SetWater(false);
                TutorialGame::GetGame()->GetBucket()->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
            }
        }
    }
}

void PlayerObject::LoadMaterial() {
    bool spacePressed = false;
    if (networkObject->GetNetworkID() == 1)
        spacePressed = Window::GetKeyboard()->KeyPressed(NCL::KeyCodes::SPACE);
    else
        spacePressed = buttonStates[6];

    if (spacePressed) {
        if (slot == 5 || slot == 6) {
            Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(8, 5, 0, 8));
            Ray r = Ray(transform.GetPosition(), face);
            RayCollision closestCollision;
            if (TutorialGame::GetGame()->GetWorld()->Raycast(r, closestCollision, true, this)) {
                GameObject* closest = (GameObject*)closestCollision.node;
                if (closest->GetTypeID() == 21 && closestCollision.rayDistance < 5.0f) {
                    //(std::cout << "yes" << "\n";
                    carriage = (MaterialCarriage*)closest;
                }
            }
        }
    }
}
