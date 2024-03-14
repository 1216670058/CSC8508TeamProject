#include "TrainObject.h"
#include "TutorialGame.h"

using namespace NCL::CSC8503;

TrainObject::TrainObject() {

}

TrainObject::~TrainObject() {

}
 
TrainObject::TrainObject(GameWorld* w) {
    world = w;
    typeID = 20;
    trainCarriage = new TrainCarriage[trainMaxIndex];
    trainIndex = 0;
    name = "Train";
    onFire = false;
    fire = 100.0f;

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
    if (path.size() == 0) {
        if (transform.GetPosition() != finishPath) {
            TutorialGame::GetGame()->SetFailure(true);
            return;
        }
        else {
            TutorialGame::GetGame()->SetSuccess(true);
            return;
        }
    }
    else {
        if (path[path.size() - 1] == finalPath)
            path.push_back(finishPath);
        Vector3 target = path[0];
        direction = (target - this->GetTransform().GetPosition());
        direction = Vector3(direction.x, 0, direction.z);
        force = TutorialGame::GetGame()->IsNetworked() ? 10.0f : 10.0f;
        if (path[path.size() - 1] == finishPath) force = 800.0f;
        GetPhysicsObject()->SetLinearVelocity(direction.Normalised() * force * dt);

        float dtdist = (lastpos - curpos).Length();
        distance += dtdist; //run dist
        lastpos = curpos; curpos = this->GetTransform().GetPosition();
        time_s += dt;
        dist_s += dtdist;
        if (time_s >= 1.0f) {
            speed = dist_s / time_s;
            dist_s = time_s = 0.0f;
        }
        float2 = distance;
        float3 = speed;

        float mm = (this->GetTransform().GetPosition() - target).Length();
        if (mm < 0.5f) {
            if (GetDirection() < 3) transform.SetPosition(Vector3(target.x, transform.GetPosition().y, transform.GetPosition().z));
            else transform.SetPosition(Vector3(transform.GetPosition().x, transform.GetPosition().y, target.z));
            physicsObject->SetLinearVelocity(Vector3());
            path.erase(path.begin());
        }
    }
    for (int i = 1; i <= trainIndex; i++)
        trainCarriage[i].Update(dt);
    //std::cout << "Position: " << transform.GetPosition().x << " " << transform.GetPosition().y << " " << transform.GetPosition().z << std::endl;
    //std::cout << "Target: " << target.x << " " << target.y << " " << target.z << std::endl;
    UpdateOrientation(direction);

    flag1 = onFire;
    if (onFire) {
        float1 = fire;
        float speed = 1.0f;
        if (fire > 0.0f)
            fire -= dt * speed;
        else
            TutorialGame::GetGame()->SetFailure(true);
    }
}

void TrainObject::InitPaths(int level) {
    switch (level) {
    case 1:
        firstPath = Vector3(50, 4.5f, 100);
        finalPath = Vector3(70, 4.5f, 100);
        finishPath = Vector3(80, 4.5f, 100);
        path.push_back(firstPath);
        break;
    case 2:
        firstPath = Vector3(50, 4.5f, 50);
        finalPath = Vector3(70, 4.5f, 50);
        finishPath = Vector3(80, 4.5f, 50);
        path.push_back(firstPath);
        break;
    case 3:
        firstPath = Vector3(50, 4.5f, 20);
        finalPath = Vector3(70, 4.5f, 20);
        finishPath = Vector3(80, 4.5f, 20);
        path.push_back(firstPath);
        break;
    //case 4:
    //    firstPath = Vector3(50, 4.5f, 50);
    //    finalPath = Vector3(270, 4.5f, 150);
    //    finishPath = Vector3(290, 4.5f, 150);
    //    path.push_back(firstPath);
    //    break;
    default:
        break;
    }
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
        carriage->SetTrain(this);
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
        carriage->SetTrain(this);
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
        carriage->SetTrain(this);
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