#include "TrainCarriage.h"
#include "TrainObject.h"

using namespace NCL::CSC8503;

TrainCarriage::TrainCarriage() {

}

TrainCarriage::~TrainCarriage() {

}

void TrainCarriage::OnCollisionBegin(GameObject *otherObject) {

}

void TrainCarriage::OnCollisionEnd(GameObject *otherObject) {

}

void TrainCarriage::UpdateOrientation() {
    Quaternion rotation;
    if (direction.x > 0) rotation = Quaternion::EulerAnglesToQuaternion(0, -90, 0);
    else if (direction.x < 0) rotation = Quaternion::EulerAnglesToQuaternion(0, 90, 0);
    else if (direction.z > 0) rotation = Quaternion::EulerAnglesToQuaternion(0, 180, 0);
    else if (direction.z < 0) rotation = Quaternion::EulerAnglesToQuaternion(0, 0, 0);
    transform.SetOrientation(rotation);
}

int TrainCarriage::GetDirection() {
    if (direction.x > 0) return 1;
    else if (direction.x < 0) return 2;
    else if (direction.z > 0) return 3;
    else if (direction.z < 0) return 4;
}

void TrainCarriage::Update(float dt) {
    if (path.size() == 0) return;
    auto it = path.begin();
    auto itt = it->first;
    int flag = it->second;

    Vector3 target = itt;
    direction = (target - this->GetTransform().GetPosition());
    direction = Vector3(direction.x, 0, direction.z);
    GetPhysicsObject()->SetLinearVelocity(direction.Normalised() * 100.0f * dt);
    UpdateOrientation();

    float mm = (this->GetTransform().GetPosition() - target).Length();
    if (mm < 0.5) {
        path.erase(it);
    }
}

void MaterialCarriage::OnCollisionBegin(GameObject* otherObject) {

}

//void MaterialCarriage::Update(float dt) {
//    std::cout << "plank: " << plank << std::endl;
//    std::cout << "stone: " << stones.size() << std::endl;
//}