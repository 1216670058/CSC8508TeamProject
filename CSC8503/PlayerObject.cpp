#include "TutorialGame.h"
#include "PlayerObject.h"
#include "Window.h"

using namespace NCL::CSC8503;

void PlayerObject::Update(float dt) {
    if (!doing) {
        if (Window::GetKeyboard()->KeyDown(KeyCodes::UP) && renderObject->GetAnimationObject()->GetAnim2()) {
            if (renderObject->GetAnimationObject()->GetFlag2()) {
                renderObject->GetAnimationObject()->SetActiveAnim(renderObject->GetAnimationObject()->GetAnim2());
                renderObject->GetAnimationObject()->SetCurrentFrame(0);
                renderObject->GetAnimationObject()->SetFrameTime(0.0f);
                renderObject->GetAnimationObject()->UpdateFlags(2);
            }
        }
        else if (Window::GetKeyboard()->KeyDown(KeyCodes::RIGHT) && renderObject->GetAnimationObject()->GetAnim3()) {
            if (renderObject->GetAnimationObject()->GetFlag3()) {
                renderObject->GetAnimationObject()->SetActiveAnim(renderObject->GetAnimationObject()->GetAnim3());
                renderObject->GetAnimationObject()->SetCurrentFrame(0);
                renderObject->GetAnimationObject()->SetFrameTime(0.0f);
                renderObject->GetAnimationObject()->UpdateFlags(3);
            }
        }
        else if (Window::GetKeyboard()->KeyDown(KeyCodes::LEFT) && renderObject->GetAnimationObject()->GetAnim4()) {
            if (renderObject->GetAnimationObject()->GetFlag4()) {
                renderObject->GetAnimationObject()->SetActiveAnim(renderObject->GetAnimationObject()->GetAnim4());
                renderObject->GetAnimationObject()->SetCurrentFrame(0);
                renderObject->GetAnimationObject()->SetFrameTime(0.0f);
                renderObject->GetAnimationObject()->UpdateFlags(4);
            }
        }
        else if (Window::GetKeyboard()->KeyDown(KeyCodes::DOWN) && renderObject->GetAnimationObject()->GetAnim5()) {
            if (renderObject->GetAnimationObject()->GetFlag5()) {
                renderObject->GetAnimationObject()->SetActiveAnim(renderObject->GetAnimationObject()->GetAnim5());
                renderObject->GetAnimationObject()->SetCurrentFrame(0);
                renderObject->GetAnimationObject()->SetFrameTime(0.0f);
                renderObject->GetAnimationObject()->UpdateFlags(5);
            }
        }
        else if (Window::GetKeyboard()->KeyDown(KeyCodes::NUM3) && renderObject->GetAnimationObject()->GetAnim8()) {
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
    if (doing) {
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
        renderObject->GetAnimationObject()->SetFrameTime(renderObject->GetAnimationObject()->GetFrameTime() - dt);
        while (renderObject->GetAnimationObject()->GetFrameTime() < 0.0f) {
            renderObject->GetAnimationObject()->SetCurrentFrame((renderObject->GetAnimationObject()->GetCurrentFrame() + 1) %
                renderObject->GetAnimationObject()->GetActiveAnim()->GetFrameCount());
            renderObject->GetAnimationObject()->SetFrameTime(renderObject->GetAnimationObject()->GetFrameTime() + 1.0f /
                renderObject->GetAnimationObject()->GetActiveAnim()->GetFrameRate());
        }
    }

    //if (Window::GetKeyboard()->KeyPressed(KeyCodes::E)) {
    //    renderObject->SetMesh(meshes[(index + 1) % 5]);
    //    renderObject->SetAnimationObject(animations[(index + 1) % 5]);
    //    renderObject->SetTextures(textures[(index + 1) % 5]);
    //    renderObject->SetBumpTextures(bumpTextures[(index + 1) % 5]);
    //    renderObject->SetShaderGroup(shaders[(index + 1) % 5]);
    //    index++;
    //}

    if (Window::GetKeyboard()->KeyPressed(KeyCodes::R))
        slot = 0;

    PlayerMovement();
    
    doing = false;
    cutting = false;
    digging = false;

    CutTree();
    DigRock();
}

void PlayerObject::OnCollisionBegin(GameObject* otherObject) {

}

void PlayerObject::PlayerMovement() {
    Quaternion* qq;
    //float yaw = Maths::RadiansToDegrees(atan2(-np.x, -np.z));
   // start->GetTransform().SetOrientation(qq->EulerAnglesToQuaternion(0, yaw, 0));
    if (Window::GetKeyboard()->KeyHeld(NCL::KeyCodes::UP)) {

        face = Vector3(1, 0, 0);
        physicsObject->AddForceAtPosition(face*speed, transform.GetPosition());
        transform.SetOrientation(qq->EulerAnglesToQuaternion(0, -90, 0));
    }
    else if (Window::GetKeyboard()->KeyHeld(NCL::KeyCodes::LEFT)) {
        face = Vector3(0, 0, -1);
        physicsObject->AddForceAtPosition(face * speed, transform.GetPosition());
        transform.SetOrientation(qq->EulerAnglesToQuaternion(0, 0, 0));
    }
    else if (Window::GetKeyboard()->KeyHeld(NCL::KeyCodes::DOWN)) {
        face = Vector3(-1, 0, 0);
        physicsObject->AddForceAtPosition(face * speed, transform.GetPosition());
        transform.SetOrientation(qq->EulerAnglesToQuaternion(0, 90, 0));
    }
    else if (Window::GetKeyboard()->KeyHeld(NCL::KeyCodes::RIGHT)) {
        face = Vector3(0, 0, 1);
        physicsObject->AddForceAtPosition(face * speed, transform.GetPosition());
        transform.SetOrientation(qq->EulerAnglesToQuaternion(0, 180, 0));
    }
    else {
        physicsObject->SetLinearVelocity(Vector3(0, 0, 0));
    }
}

void PlayerObject::CutTree() {
    if (Window::GetKeyboard()->KeyHeld(NCL::KeyCodes::F) && slot == 3) {
        doing = true;
        cutting = true;
        Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(8, 5, 0, 8));
        Ray r = Ray(transform.GetPosition(), face);
        RayCollision closestCollision;
        if (TutorialGame::GetGame()->GetWorld()->Raycast(r, closestCollision, true, this)) {
            GameObject* closest = (GameObject*)closestCollision.node;
            if (closest->GetTypeID() == 10086 && closestCollision.rayDistance < 5.0f) {
                std::cout << "yes" << "\n";
                closest->GetTransform().SetScale(closest->GetTransform().GetScale() - Vector3(0.01, 0.01, 0.01));
                if (closest->GetTransform().GetScale().x < 0.1f) {
                    TutorialGame::GetGame()->AddPlankToWorld(Vector3(closest->GetTransform().GetPosition().x, 5, closest->GetTransform().GetPosition().z));
                    TutorialGame::GetGame()->GetWorld()->RemoveGameObject(closest, false);
                }
            }
        }
    }
}

void PlayerObject::DigRock() {
    if (Window::GetKeyboard()->KeyHeld(NCL::KeyCodes::F) && slot == 2) {
        doing = true;
        digging = true;
        Debug::DrawLine(transform.GetPosition(), transform.GetPosition() + face * 5.0f, Vector4(8, 5, 0, 8));
        Ray r = Ray(transform.GetPosition(), face);
        RayCollision closestCollision;
        if (TutorialGame::GetGame()->GetWorld()->Raycast(r, closestCollision, true, this)) {
            GameObject* closest = (GameObject*)closestCollision.node;
            if (closest->GetTypeID() == 10010 && closestCollision.rayDistance < 5.0f) {
                std::cout << "yes" << "\n";
                closest->GetTransform().SetScale(closest->GetTransform().GetScale() - Vector3(0.01, 0.01, 0.01));
                if (closest->GetTransform().GetScale().x < 0.1f) {
                    TutorialGame::GetGame()->AddStoneToWorld(Vector3(closest->GetTransform().GetPosition().x, 5, closest->GetTransform().GetPosition().z));
                    TutorialGame::GetGame()->GetWorld()->RemoveGameObject(closest, false);
                }
            }
        }
    }
}
