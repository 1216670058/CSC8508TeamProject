#include "TutorialGame.h"
#include "GameWorld.h"
#include "PhysicsObject.h"
#include "RenderObject.h"
#include "AnimationObject.h"
#include "TextureLoader.h"

#include "PositionConstraint.h"
#include "OrientationConstraint.h"
#include "StateGameObject.h"

#include "Assets.h"
#include <fstream>
using namespace NCL;
using namespace CSC8503;

TutorialGame* TutorialGame::instance = nullptr;

TutorialGame::TutorialGame() : controller(*Window::GetWindow()->GetKeyboard(), *Window::GetWindow()->GetMouse()) {
    std::cout << std::endl << "--------Initialising Game--------" << std::endl;

    world = new GameWorld();
    audio = new Audio(world);
#ifdef USEVULKAN
    renderer = new GameTechVulkanRenderer(*world);
    renderer->Init();
    renderer->InitStructures();
#else
    renderer = new GameTechRenderer(*world);
#endif
    physics = new PhysicsSystem(*world);

    forceMagnitude = 10.0f;
    useGravity = false;
    inSelectionMode = false;

    world->GetMainCamera().SetController(controller);

    controller.MapAxis(0, "Sidestep");
    controller.MapAxis(1, "UpDown");
    controller.MapAxis(2, "Forward");

    controller.MapAxis(3, "XLook");
    controller.MapAxis(4, "YLook");

    world->SetGameState(GameState::LOADING);

    instance = this;
}

/*

Each of the little demo scenarios used in the game uses the same 2 meshes,
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it if you like!

*/

TutorialGame::~TutorialGame() {
    delete cubeMesh;
    delete sphereMesh;
    delete charMesh;
    delete enemyMesh;
    delete bonusMesh;

    delete basicTex;
    delete basicShader;

    delete physics;
    delete renderer;
    delete world;
    delete audio;
}

void TutorialGame::UpdateGame(float dt) {
    switch (world->GetGameState())
    {
    case LOADING:
        UpdateLoading(dt);
        break;
    case PLAYING:
        UpdatePlaying(dt);
        break;
    case PAUSED:
        UpdatePaused(dt);
        break;
    case MENU:
        UpdateMenu(dt);
        break;
    case EXIT:
        isExit = true;
        break;
    default:
        break;
    }

}

void TutorialGame::UpdateLoading(float dt)
{
    audio->Update();
    renderer->Update(dt);
    renderer->GetUI()->Update(dt); //UI
    renderer->GetUI()->SetLoadingStep(assetsLoadedStep);
    renderer->Render();
    if (assetsLoadedStep == 5) world->SetGameState(CSC8503::GameState::MENU);
    AssetsLoading();
}

void TutorialGame::UpdatePlaying(float dt)
{
    if (!inSelectionMode) {
        world->GetMainCamera().UpdateCamera(dt);
    }
    if (lockedObject != nullptr) {
        Vector3 objPos = lockedObject->GetTransform().GetPosition();
        Vector3 camPos = objPos + lockedOffset;

        Matrix4 temp = Matrix4::BuildViewMatrix(camPos, objPos, Vector3(0, 1, 0));

        Matrix4 modelMat = temp.Inverse();

        Quaternion q(modelMat);
        Vector3 angles = q.ToEuler(); //nearly there now!

        world->GetMainCamera().SetPosition(camPos);
        world->GetMainCamera().SetPitch(angles.x);
        world->GetMainCamera().SetYaw(angles.y);
        world->GetMainCamera().SetPitch(-90);
        world->GetMainCamera().SetYaw(0);
    }

    UpdateKeys();
    audio->Update();

    if (useGravity) {
        Debug::Print("(G)ravity on", Vector2(5, 95), Debug::RED);
    }
    else {
        Debug::Print("(G)ravity off", Vector2(5, 95), Debug::RED);
    }
    Debug::DrawLine(Vector3(0, 0, 0), Vector3(100, 0, 0), Debug::RED);
    Debug::DrawLine(Vector3(0, 0, 0), Vector3(0, 100, 0), Debug::GREEN);
    Debug::DrawLine(Vector3(0, 0, 0), Vector3(0, 0, 100), Debug::BLUE);
    RayCollision closestCollision;
    if (Window::GetKeyboard()->KeyPressed(KeyCodes::K) && selectionObject) {
        Vector3 rayPos;
        Vector3 rayDir;

        rayDir = selectionObject->GetTransform().GetOrientation() * Vector3(0, 0, -1);

        rayPos = selectionObject->GetTransform().GetPosition();

        Ray r = Ray(rayPos, rayDir);

        if (world->Raycast(r, closestCollision, true, selectionObject)) {
            if (objClosest) {
                objClosest->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
            }
            objClosest = (GameObject*)closestCollision.node;

            objClosest->GetRenderObject()->SetColour(Vector4(1, 0, 1, 1));
        }
    }

    Debug::DrawLine(Vector3(), Vector3(0, 100, 0), Vector4(1, 0, 0, 1));

    SelectObject();
    MoveSelectedObject();

    world->UpdateWorld(dt);
    renderer->Update(dt);
    renderer->GetUI()->Update(dt); //UI
    physics->Update(dt);

    if (testStateObject) {
        //std::cout<<"debug"<<std::endl;
        testStateObject->Update(dt);
    }
    if (trainObject) {
        trainObject->Update(dt);
    }

    renderer->Render();
    Debug::UpdateRenderables(dt);
}

void TutorialGame::UpdatePaused(float dt)
{
    audio->Update();
    renderer->Update(dt);
    renderer->GetUI()->Update(dt); //UI
    renderer->Render();
}

void TutorialGame::UpdateMenu(float dt)
{
    audio->Update();
    renderer->Update(dt);
    renderer->GetUI()->Update(dt); //UI
    renderer->Render();

}

void TutorialGame::UpdateKeys() {
    if (Window::GetKeyboard()->KeyPressed(KeyCodes::F1)) {
        InitWorld(); //We can reset the simulation at any time with F1
        selectionObject = nullptr;
    }

    if (Window::GetKeyboard()->KeyPressed(KeyCodes::F2)) {
        InitCamera(); //F2 will reset the camera to a specific default place
    }

    if (Window::GetKeyboard()->KeyPressed(KeyCodes::F3)) {
        renderer->ToggleNight();
    }

    if (Window::GetKeyboard()->KeyPressed(KeyCodes::ESCAPE)) {
        world->SetGameState(GameState::PAUSED);
    }

    if (Window::GetKeyboard()->KeyPressed(KeyCodes::G)) {
        useGravity = !useGravity; //Toggle gravity!
        physics->UseGravity(useGravity);
    }
    //Running certain physics updates in a consistent order might cause some
    //bias in the calculations - the same objects might keep 'winning' the constraint
    //allowing the other one to stretch too much etc. Shuffling the order so that it
    //is random every frame can help reduce such bias.
    if (Window::GetKeyboard()->KeyPressed(KeyCodes::F9)) {
        world->ShuffleConstraints(true);
    }
    if (Window::GetKeyboard()->KeyPressed(KeyCodes::F10)) {
        world->ShuffleConstraints(false);
    }

    if (Window::GetKeyboard()->KeyPressed(KeyCodes::F7)) {
        world->ShuffleObjects(true);
    }
    if (Window::GetKeyboard()->KeyPressed(KeyCodes::F8)) {
        world->ShuffleObjects(false);
    }

    if (lockedObject) {
        LockedObjectMovement();
    }
}

void TutorialGame::LockedObjectMovement() {
    Matrix4 view = world->GetMainCamera().BuildViewMatrix();
    Matrix4 camWorld = view.Inverse();

    Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!

    //forward is more tricky -  camera forward is 'into' the screen...
    //so we can take a guess, and use the cross of straight up, and
    //the right axis, to hopefully get a vector that's good enough!

    Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);
    fwdAxis.y = 0.0f;
    fwdAxis.Normalise();


    if (Window::GetKeyboard()->KeyDown(KeyCodes::UP)) {
        selectionObject->GetPhysicsObject()->AddForce(fwdAxis);
    }

    if (Window::GetKeyboard()->KeyDown(KeyCodes::DOWN)) {
        selectionObject->GetPhysicsObject()->AddForce(-fwdAxis);
    }

    if (Window::GetKeyboard()->KeyDown(KeyCodes::NEXT)) {
        selectionObject->GetPhysicsObject()->AddForce(Vector3(0, -10, 0));
    }
}

void TutorialGame::InitCamera() {
    world->GetMainCamera().SetNearPlane(0.1f);
    world->GetMainCamera().SetFarPlane(500.0f);
    world->GetMainCamera().SetPitch(-45.0f);
    world->GetMainCamera().SetYaw(0.0f);
    world->GetMainCamera().SetPosition(Vector3(80, 100, 220));
    lockedObject = nullptr;
}

void TutorialGame::InitWorld() {
    std::cout << std::endl << "--------Initialising Game Objects--------" << std::endl;
    world->ClearAndErase();
    physics->Clear();

    //InitMixedGridWorld(15, 15, 3.5f, 3.5f);
    //AddSceneToWorld();
    InitGameExamples();
    InitDefaultFloor();

}

void TutorialGame::InitDefaultFloor() {
    AddFloorToWorld(Vector3(155, 0, 95));
}

void TutorialGame::InitGameExamples() {
    //AddPlayer0ToWorld(Vector3(0, 5, 0));
    //AddEnemyToWorld(Vector3(5, 5, 0));
    trainObject = AddTrainToWorld(Vector3(10, 3, 0));
    trainObject->AddCarriage(21);
    trainObject->AddCarriage(21);
    //AddTestingLightToWorld(Vector3(10, 20, 0), Vector4(1, 1, 1, 0.7));
    //AddTestingLightToWorld(Vector3(30, 20, 40), Vector4(1, 0, 0, 0.7));
    //AddTestingLightToWorld(Vector3(60, 20, 20), Vector4(0, 1, 0, 0.7));
    player = AddPlayerToWorld(Vector3(20, 0, 100));
    pickaxe = AddPickaxeToWorld(Vector3(40, 5, 90));
    axe = AddAxeToWorld(Vector3(40, 5, 100));
    bucket = AddBucketToWorld(Vector3(40, 5, 110));
    //AddPlankToWorld(Vector3(60, 5, 20));
    //AddStoneToWorld(Vector3(80, 5, 20));
    //AddRailToWorld(Vector3(90, 5, 20));
    //AddTreeToWorld(Vector3(20, 10, 50));
    //AddTreeToWorld(Vector3(30, 10, 50));
    //AddTreeToWorld(Vector3(40, 10, 50));
    //AddRockToWorld(Vector3(20, 5, 40));
    //AddRockToWorld(Vector3(30, 5, 40));
    //AddRockToWorld(Vector3(40, 5, 40));
    //AddMooseToWorld(Vector3(40, 3, 0));
    //AddRobotToWorld(Vector3(50, 3, 0));
    //AddDroneToWorld(Vector3(60, 3, 0));
    AddSceneToWorld();
}

bool TutorialGame::SelectObject() {
    if (Window::GetKeyboard()->KeyPressed(KeyCodes::Q)) {
        inSelectionMode = !inSelectionMode;
        if (inSelectionMode) {
            Window::GetWindow()->ShowOSPointer(true);
            Window::GetWindow()->LockMouseToWindow(false);
        }
        else {
            Window::GetWindow()->ShowOSPointer(false);
            Window::GetWindow()->LockMouseToWindow(true);
        }
    }
    if (inSelectionMode) {
        Debug::Print("Press Q to change to camera mode!", Vector2(5, 85));

        if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::Left)) {
            if (selectionObject) {	//set colour to deselected;
                selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
                selectionObject = nullptr;
            }

            Ray ray = CollisionDetection::BuildRayFromMouse(world->GetMainCamera());

            RayCollision closestCollision;
            if (world->Raycast(ray, closestCollision, true)) {
                selectionObject = (GameObject*)closestCollision.node;
                selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
                return true;
            }
            else {
                return false;
            }
        }
        if (Window::GetKeyboard()->KeyPressed(NCL::KeyCodes::L)) {
            if (selectionObject) {
                if (lockedObject == selectionObject) {
                    lockedObject = nullptr;
                }
                else {
                    lockedObject = selectionObject;
                }
            }
        }
    }
    else {
        Debug::Print("Press Q to change to select mode!", Vector2(5, 85));
        //Vector3 position = findNearestGridCenter(Vector3(3, 12.5, 0));
        //Debug::Print("x:" + std::to_string(position.x), Vector2(5, 85));
        //Debug::Print("y:" + std::to_string(position.y), Vector2(5, 100));
    }
    return false;
}
void TutorialGame::MoveSelectedObject() {
    // renderer -> DrawString ( " Click Force : " + std :: to_string ( forceMagnitude ) , Vector2 (10 , 20));
    Debug::Print("Click Force:" + std::to_string(forceMagnitude), Vector2(5, 90));
    forceMagnitude += Window::GetMouse()->GetWheelMovement() * 100.0f;

    if (!selectionObject) {
        return;//we haven't selected anything!
    }
    //Push the selected object!
    if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::Right)) {
        Ray ray = CollisionDetection::BuildRayFromMouse(world->GetMainCamera());

        RayCollision closestCollision;
        if (world->Raycast(ray, closestCollision, true)) {
            if (closestCollision.node == selectionObject) {
                selectionObject->GetPhysicsObject()->AddForceAtPosition(ray.GetDirection() * forceMagnitude, closestCollision.collidedAt);
            }
        }
    }

    if (Window::GetKeyboard()->KeyHeld(NCL::KeyCodes::W)) {
        selectionObject->GetPhysicsObject()->AddForceAtPosition(Vector3(100, 0, 0), selectionObject->GetTransform().GetPosition());
    }
    else if (Window::GetKeyboard()->KeyHeld(NCL::KeyCodes::A)) {
        selectionObject->GetPhysicsObject()->AddForceAtPosition(Vector3(0, 0, 100), selectionObject->GetTransform().GetPosition());
    }
    else if (Window::GetKeyboard()->KeyHeld(NCL::KeyCodes::S)) {
        selectionObject->GetPhysicsObject()->AddForceAtPosition(Vector3(-100, 0, 0), selectionObject->GetTransform().GetPosition());
    }
    else if (Window::GetKeyboard()->KeyHeld(NCL::KeyCodes::D)) {
        selectionObject->GetPhysicsObject()->AddForceAtPosition(Vector3(0, 0, -100), selectionObject->GetTransform().GetPosition());
    }
    else {
        selectionObject->GetPhysicsObject()->SetLinearVelocity(Vector3(0, 0, 0));
    }

}

void TutorialGame::BridgeConstraintTest() {
    Vector3 cubeSize = Vector3(8, 8, 8);
    float invCubeMass = 5;  // how heavy the middle pieces are
    int numLinks = 10;
    float maxDistance = 30;  // constraint distance
    float cubeDistance = 20;  // distance between links

    Vector3 startPos = Vector3(5, 100, 5);

    GameObject* start = AddCubeToWorld(startPos + Vector3(0, 0, 0), cubeSize, 0);
    GameObject* end = AddCubeToWorld(startPos + Vector3((numLinks + 2) * cubeDistance, 0, 0), cubeSize, 0);

    GameObject* previous = start;

    for (int i = 0; i < numLinks; ++i) {
        GameObject* block = AddCubeToWorld(startPos + Vector3((i + 1) * cubeDistance, 0, 0), cubeSize, invCubeMass);
        PositionConstraint* constraint = new PositionConstraint(previous, block, maxDistance);
        world->AddConstraint(constraint);
        previous = block;
    }
    PositionConstraint* constraint = new PositionConstraint(previous, end, maxDistance);
    world->AddConstraint(constraint);
}

void TutorialGame::HoldObject()
{
    object->GetTransform().SetPosition(object->PlayerFront());
}