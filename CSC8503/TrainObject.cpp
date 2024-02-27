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

    path.push_back({Vector3(300, 5, 100), 4});
    path.push_back({Vector3(300, 5, 0), 1});
    world = w;
    trainCarriage = new TrainCarriage[20];
    trainIndex = 0;
}

void TrainObject::OnCollisionBegin(GameObject *otherObject) {

}

void TrainObject::OnCollisionEnd(GameObject *otherObject) {

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
    GetPhysicsObject()->SetLinearVelocity(dir.Normalised() * 10.0f * dt);

    float mm = (this->GetTransform().GetPosition() - target).Length();
    if (mm < 0.5f) {
        if (flag > 2) transform.SetPosition(Vector3(itt.x, transform.GetPosition().y, transform.GetPosition().z));
        else transform.SetPosition(Vector3(transform.GetPosition().x, transform.GetPosition().y, itt.z));
        path.erase(it);
    }
    for (int i = 1; i <= trainIndex; i++)
        trainCarriage[i].Update(dt);
    //std::cout << "Dir: " << dir.x << " " << dir.y << " " << dir.z << std::endl;
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

TrainCarriage* TrainObject::AddCarriage(int id) {
    Vector3 nowPos;
    if (trainIndex == 0)
        nowPos = GetTransform().GetPosition();
    else
        nowPos = trainCarriage[trainIndex].GetTransform().GetPosition();

    Vector3 nextPos;


    if (path.front().second <= 1) { 
        nextPos = nowPos;
        nextPos.z -= 8;
    }
    else {
        nextPos = nowPos;
        nextPos.x -= 8;

    }

    if (id == 21) {
        MaterialCarriage* carriage = new MaterialCarriage(world);
        carriage->path = path;
        AABBVolume* volume = new AABBVolume(Vector3(2, 2, 2));
        carriage->SetBoundingVolume((CollisionVolume*)volume);
        carriage->GetTransform()
            .SetScale(Vector3(3, 3, 3))
            .SetPosition(nextPos);

        carriage->SetRenderObject(new RenderObject(&carriage->GetTransform(), carriageMesh, carriageTex, basicShader));
        carriage->SetPhysicsObject(new PhysicsObject(&carriage->GetTransform(), carriage->GetBoundingVolume()));

        carriage->GetPhysicsObject()->SetInverseMass(0);
        carriage->GetPhysicsObject()->InitSphereInertia();
        carriage->GetPhysicsObject()->SetChannel(1);

        carriage->SetTypeID(id);

        trainCarriage[++trainIndex] = *carriage;
        world->AddGameObject(carriage);

        return carriage;
    }
    if (id == 22) {
        ProduceCarriage* carriage = new ProduceCarriage(world);
        carriage->path = path;
        AABBVolume* volume = new AABBVolume(Vector3(2, 2, 2));
        carriage->SetBoundingVolume((CollisionVolume*)volume);
        carriage->GetTransform()
            .SetScale(Vector3(3, 3, 3))
            .SetPosition(nextPos);

        carriage->SetRenderObject(new RenderObject(&carriage->GetTransform(), carriageMesh, carriageTex, basicShader));
        carriage->SetPhysicsObject(new PhysicsObject(&carriage->GetTransform(), carriage->GetBoundingVolume()));

        carriage->GetRenderObject()->SetColour(Vector4(1, 1, 0, 1));

        carriage->GetPhysicsObject()->SetInverseMass(0);
        carriage->GetPhysicsObject()->InitSphereInertia();
        carriage->GetPhysicsObject()->SetChannel(1);

        carriage->SetTypeID(id);

        trainCarriage[++trainIndex] = *carriage;
        world->AddGameObject(carriage);

        return carriage;
    }
}

void TrainObject::AddConstraint(GameObject *a, GameObject *b) {
    float maxDistance = 10.0f;
    PositionConstraint *constraint = new PositionConstraint(a, b, maxDistance);
    world->AddConstraint(constraint);
}