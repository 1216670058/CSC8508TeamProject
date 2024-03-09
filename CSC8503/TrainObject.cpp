#include "TrainObject.h"
#include "TutorialGame.h"

using namespace NCL::CSC8503;

TrainObject::TrainObject() {

}

TrainObject::~TrainObject() {

}

TrainObject::TrainObject(GameWorld* w) {

    path.push_back(Vector3(90, 4.5f, 100));
    //path.push_back({ Vector3(300, 5, 0), 1 });
    world = w;
    trainCarriage = new TrainCarriage[trainMaxIndex];
    trainIndex = 0;
    name = "Train";

    distance = 0.0f;
}

void TrainObject::OnCollisionBegin(GameObject* otherObject) {

}

void TrainObject::OnCollisionEnd(GameObject* otherObject) {

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
    Vector3 target = path[0];
    direction = (target - this->GetTransform().GetPosition());
    direction = Vector3(direction.x, 0, direction.z);
    force = 20.0f;
    GetPhysicsObject()->SetLinearVelocity(direction.Normalised() * force * dt);

    //std::cout << GetPhysicsObject()->GetInverseMass() << std::endl;
    float dtdist = (lastpos - curpos).Length();
    distance += dtdist; //run dist
    lastpos = curpos; curpos = this->GetTransform().GetPosition();
    time_s += dt;
    dist_s += dtdist;
    if (time_s >= 1.0f) {
        speed = dist_s / time_s;
        dist_s = time_s = 0.0f;
    }

    float mm = (this->GetTransform().GetPosition() - target).Length();
    if (mm < 0.5f) {
        if (GetDirection() < 3) transform.SetPosition(Vector3(target.x, transform.GetPosition().y, transform.GetPosition().z));
        else transform.SetPosition(Vector3(transform.GetPosition().x, transform.GetPosition().y, target.z));
        physicsObject->SetLinearVelocity(Vector3());
        path.erase(path.begin());
    }
    for (int i = 1; i <= trainIndex; i++)
        trainCarriage[i].Update(dt);
    //std::cout << "Position: " << transform.GetPosition().x << " " << transform.GetPosition().y << " " << transform.GetPosition().z << std::endl;
    //std::cout << "Target: " << target.x << " " << target.y << " " << target.z << std::endl;
    UpdateOrientation(direction);
}

void TrainObject::AddPath(Vector3 p) {
    path.push_back(p);
}

void TrainObject::AddCarriagePath(Vector3 p) {
    TutorialGame::GetGame()->GetMaterialCarriage()->AddPath(p);
    TutorialGame::GetGame()->GetProduceCarriage()->AddPath(p);
    TutorialGame::GetGame()->GetWaterCarriage()->AddPath(p);
}

void TrainObject::UploadAssets(Mesh* mesh, Texture* texture, ShaderGroup* shader) {
    carriageMesh = mesh;
    carriageTex = texture;
    basicShader = shader;
}

TrainCarriage* TrainObject::AddCarriage(int id, bool spawn) {
    Vector3 nowPos;
    if (trainIndex == 0)
        nowPos = GetTransform().GetPosition();
    else
        nowPos = trainCarriage[trainIndex].GetTransform().GetPosition();

    Vector3 nextPos;

    nextPos = nowPos;
    nextPos.x -= 10;

    if (id == 21) {
        MaterialCarriage* carriage = new MaterialCarriage(world);
        carriage->SetPath(path);
        AABBVolume* volume = new AABBVolume(Vector3(2, 2, 2));
        carriage->SetBoundingVolume((CollisionVolume*)volume);

        carriage->SetSpawned(spawn);

        carriage->GetTransform()
            .SetScale(Vector3(4, 4, 4))
            .SetPosition(nextPos);

        carriage->SetRenderObject(new RenderObject(&carriage->GetTransform(), carriageMesh, carriageTex, basicShader));
        carriage->SetPhysicsObject(new PhysicsObject(&carriage->GetTransform(), carriage->GetBoundingVolume()));

        carriage->GetPhysicsObject()->SetInverseMass(0);
        carriage->GetPhysicsObject()->InitSphereInertia();
        carriage->GetPhysicsObject()->SetChannel(1);

        carriage->SetNetworkObject(new NetworkObject(*carriage, id));

        carriage->SetTypeID(id);

        trainCarriage[++trainIndex] = *carriage;
        world->AddGameObject(carriage);

        return carriage;
    }
    if (id == 22) {
        ProduceCarriage* carriage = new ProduceCarriage(world);
        carriage->SetPath(path);
        AABBVolume* volume = new AABBVolume(Vector3(2, 2, 2));
        carriage->SetBoundingVolume((CollisionVolume*)volume);

        carriage->SetSpawned(spawn);

        carriage->GetTransform()
            .SetScale(Vector3(4, 4, 4))
            .SetPosition(nextPos);

        carriage->SetRenderObject(new RenderObject(&carriage->GetTransform(), carriageMesh, carriageTex, basicShader));
        carriage->SetPhysicsObject(new PhysicsObject(&carriage->GetTransform(), carriage->GetBoundingVolume()));

        carriage->GetRenderObject()->SetColour(Vector4(1, 1, 0, 1));

        carriage->GetPhysicsObject()->SetInverseMass(0);
        carriage->GetPhysicsObject()->InitSphereInertia();
        carriage->GetPhysicsObject()->SetChannel(1);

        carriage->SetNetworkObject(new NetworkObject(*carriage, id));

        carriage->SetTypeID(id);

        trainCarriage[++trainIndex] = *carriage;
        world->AddGameObject(carriage);

        return carriage;
    }
    if (id == 23) {
        WaterCarriage* carriage = new WaterCarriage(world);
        carriage->SetPath(path);
        AABBVolume* volume = new AABBVolume(Vector3(2, 2, 2));
        carriage->SetBoundingVolume((CollisionVolume*)volume);

        carriage->SetSpawned(spawn);

        carriage->GetTransform()
            .SetScale(Vector3(4, 4, 4))
            .SetPosition(nextPos);

        carriage->SetRenderObject(new RenderObject(&carriage->GetTransform(), carriageMesh, carriageTex, basicShader));
        carriage->SetPhysicsObject(new PhysicsObject(&carriage->GetTransform(), carriage->GetBoundingVolume()));

        carriage->GetRenderObject()->SetColour(Vector4(0, 0, 1, 1));

        carriage->GetPhysicsObject()->SetInverseMass(0);
        carriage->GetPhysicsObject()->InitSphereInertia();
        carriage->GetPhysicsObject()->SetChannel(1);

        carriage->SetNetworkObject(new NetworkObject(*carriage, id));

        carriage->SetTypeID(id);

        trainCarriage[++trainIndex] = *carriage;
        world->AddGameObject(carriage);

        return carriage;
    }
}

void TrainObject::AddConstraint(GameObject* a, GameObject* b) {
    float maxDistance = 10.0f;
    PositionConstraint* constraint = new PositionConstraint(a, b, maxDistance);
    world->AddConstraint(constraint);
}

int TrainObject::GetDirection() {
    if (direction.x > 0) return 1;
    else if (direction.x < 0) return 2;
    else if (direction.z > 0) return 3;
    else if (direction.z < 0) return 4;
}