#include "AnimalObject.h"

using namespace NCL::CSC8503;

void AnimalObject::Update(float dt) {
    renderObject->GetAnimationObject()->SetFrameTime(renderObject->GetAnimationObject()->GetFrameTime() - dt);
    while (renderObject->GetAnimationObject()->GetFrameTime() < 0.0f) {
        renderObject->GetAnimationObject()->SetCurrentFrame((renderObject->GetAnimationObject()->GetCurrentFrame() + 1) %
            renderObject->GetAnimationObject()->GetActiveAnim()->GetFrameCount());
        renderObject->GetAnimationObject()->SetFrameTime(renderObject->GetAnimationObject()->GetFrameTime() + 1.0f /
            renderObject->GetAnimationObject()->GetActiveAnim()->GetFrameRate());
    }
}