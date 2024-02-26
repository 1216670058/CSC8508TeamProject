#include "TrainObject.h"
#include "TutorialGame.h"

using namespace NCL::CSC8503;

TrainObject::TrainObject() {
    path.push_back({Vector3(10, 5, 60), 4});

    // path.push_back({Vector3(60, 0, 60), 1});
}

TrainObject::~TrainObject() {

}

TrainObject::TrainObject(GameWorld *w) {

    path.push_back({Vector3(10, 3, 60), 4});
    path.push_back({Vector3(60, 3, 60), 1});
    world = w;
    trainCarriage = new TrainCarriage[20];
    trainIndex = 0;
}

void TrainObject::OnCollisionBegin(GameObject *otherObject) {

}

void TrainObject::OnCollisionEnd(GameObject *otherObject) {

}

Quaternion RotateBetweenVectors(const Vector3 &from, const Vector3 &to) {
    Vector3 source = from.Normalised();
    Vector3 target = to.Normalised();
    float dotProduct = Vector3::Dot(source, target);
    if (dotProduct < -0.999f) {
        Vector3 axis = Vector3::Cross(Vector3(1.0f, 0.0f, 0.0f), source);
        if (axis.LengthSquared() < 0.01f) {
            axis = Vector3::Cross(Vector3(0.0f, 1.0f, 0.0f), source);
        }
        axis.Normalise();
        return Quaternion(axis, 0.0f);
    }
    Vector3 rotationAxis = Vector3::Cross(source, target);
    rotationAxis.Normalise();
    float rotationAngle = std::acos(dotProduct);
    return Quaternion(rotationAxis, rotationAngle);
}

void TrainObject::UpdateOrientation(Vector3 direction) {
    Quaternion rotation;
    if (direction.x > 0) rotation = Quaternion::EulerAnglesToQuaternion(0, -90, 0);
    else if (direction.x < 0) rotation = Quaternion::EulerAnglesToQuaternion(0, 90, 0);
    else if (direction.z > 0) rotation = Quaternion::EulerAnglesToQuaternion(0, 180, 0);
    else if (direction.z < 0) rotation = Quaternion::EulerAnglesToQuaternion(0, 0, 0);
    transform.SetOrientation(rotation);
}

void TrainObject::Update(float dt) {
    if (path.size() == 0) return;
    auto it = path.begin();
    auto itt = it->first;
    int flag = it->second;

    Vector3 target = itt;
    Vector3 dir = (target - this->GetTransform().GetPosition());
    dir = Vector3(dir.x, 0, dir.z);
    GetPhysicsObject()->SetLinearVelocity(dir.Normalised() * 100.0f * dt);

    std::cout<< " this position :"<<this->GetTransform().GetPosition()<<std::endl;
    float mm = (this->GetTransform().GetPosition() - target).Length();
    std::cout<< " mm  :"<<mm<<std::endl;
    if (mm < 0.5f) {
        path.erase(it);
    }
    for (int i = 1; i <= trainIndex; i++)
        trainCarriage[i].Update(dt);
    UpdateOrientation(dir);
}

void TrainObject::UpdatePath(std::vector<std::pair<Vector3, int>> p) {
    path = p;
}

void TrainObject::UploadAssets(Mesh* mesh, Texture* texture, ShaderGroup* shader) {
    carriageMesh = mesh;
    carriageTex = texture;
    basicShader = shader;
}

void TrainObject::AddCarriage() {
    Vector3 nowPos;
    if (trainIndex == 0)
        nowPos = GetTransform().GetPosition();
    else
        nowPos = trainCarriage[trainIndex].GetTransform().GetPosition();

    Vector3 nextPos;


    if (path.front().second <= 1) { //车头方向为上下 ，添加的车厢竖值放置
        nextPos = nowPos;
        nextPos.x -= 3;
    } else {
        nextPos = nowPos;
        nextPos.z -= 3;

    }
//    std::cout<<nowPos<<std::endl;
//    std::cout<<nextPos<<std::endl;

    TrainCarriage* carriage = new TrainCarriage();
    carriage->path = path;
    AABBVolume *volume = new AABBVolume(Vector3(0.5f,0.5f,0.5f));
    carriage->SetBoundingVolume((CollisionVolume *) volume);
    carriage->GetTransform()
            .SetScale(Vector3(1, 1, 1))
            .SetPosition(nextPos);

    carriage->SetRenderObject(new RenderObject(&carriage->GetTransform(), carriageMesh, carriageTex, basicShader));
    carriage->SetPhysicsObject(new PhysicsObject(&carriage->GetTransform(), carriage->GetBoundingVolume()));

    carriage->GetPhysicsObject()->SetInverseMass(0);
    carriage->GetPhysicsObject()->InitSphereInertia();

    trainCarriage[++trainIndex] = *carriage;
    world->AddGameObject(carriage);

//    if(trainIndex==1){
//        AddConstraint(this,&trainCarriage[trainIndex]);
//    }
//    else{
//        AddConstraint(&trainCarriage[trainIndex-1],&trainCarriage[trainIndex]);
//    }

}

void TrainObject::AddConstraint(GameObject *a, GameObject *b) {
    float maxDistance = 10.0f;
    PositionConstraint *constraint = new PositionConstraint(a, b, maxDistance);
    world->AddConstraint(constraint);
}
//void TutorialGame::BridgeConstraintTest() {
//    Vector3 cubeSize = Vector3(8, 8, 8);
//    float invCubeMass = 5;  // how heavy the middle pieces are
//    int numLinks = 10;
//    float maxDistance = 30;  // constraint distance
//    float cubeDistance = 20;  // distance between links
//
//    Vector3 startPos = Vector3(5, 100, 5);
//
//    GameObject* start = AddCubeToWorld(startPos + Vector3(0, 0, 0), cubeSize, 0);
//    GameObject* end = AddCubeToWorld(startPos + Vector3((numLinks + 2) * cubeDistance, 0, 0), cubeSize, 0);
//
//    GameObject* previous = start;
//
//    for (int i = 0; i < numLinks; ++i) {
//        GameObject* block = AddCubeToWorld(startPos + Vector3((i + 1) * cubeDistance, 0, 0), cubeSize, invCubeMass);
//        PositionConstraint* constraint = new PositionConstraint(previous, block, maxDistance);
//        world->AddConstraint(constraint);
//        previous = block;
//    }
//    PositionConstraint* constraint = new PositionConstraint(previous, end, maxDistance);
//    world->AddConstraint(constraint);
//}

