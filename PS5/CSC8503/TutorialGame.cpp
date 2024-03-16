#include "TutorialGame.h"
#include "GameWorld.h"
#include "PhysicsObject.h"
#include "RenderObject.h"
#include "TextureLoader.h"

#include "PositionConstraint.h"
#include "OrientationConstraint.h"
#include "StateGameObject.h"

#include "../PS5Core/PS5Window.h"


using namespace NCL;
using namespace CSC8503;

TutorialGame* TutorialGame::instance = nullptr;

TutorialGame::TutorialGame(GameWorld& inWorld, GameTechRendererInterface& inRenderer, PhysicsSystem& inPhysics) 
	: world(inWorld), 
	  renderer(inRenderer),
	  physics(inPhysics)
{
	level = 1;
	forceMagnitude	= 10.0f;
	useGravity		= false;
	inSelectionMode = false;

	InitialiseAssets();

	NCL::PS5::PS5Window* w = (NCL::PS5::PS5Window*)Window::GetWindow();
	
	world.GetMainCamera().SetController(*w->GetController());

	instance = this;
}

/*

Each of the little demo scenarios used in the game uses the same 2 meshes, 
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it if you like!

*/
void TutorialGame::InitialiseAssets() {
	InitMeshes();
	InitTextures();
	InitShaders();
	InitAnimations();
	InitGameWorld();
}

TutorialGame::~TutorialGame()	{
	delete cubeMesh;
	delete sphereMesh;
	delete charMesh;
	delete enemyMesh;
	delete bonusMesh;

	delete basicTex;
	delete basicShader;
}

void TutorialGame::UpdateGame(float dt) {
	if (!inSelectionMode) {
		world.GetMainCamera().UpdateCamera(dt);
	}
	if (lockedObject != nullptr) {
		Vector3 objPos = lockedObject->GetTransform().GetPosition();
		Vector3 camPos = objPos + lockedOffset;

		Matrix4 temp = Matrix::View(camPos, objPos, Vector3(0,1,0));

		Matrix4 modelMat = Matrix::Inverse(temp);

		Quaternion q(modelMat);
		Vector3 angles = q.ToEuler(); //nearly there now!

		world.GetMainCamera().SetPosition(camPos);
		world.GetMainCamera().SetPitch(angles.x);
		world.GetMainCamera().SetYaw(angles.y);
	}

	UpdateKeys();

	RayCollision closestCollision;
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::K) && selectionObject) {
		Vector3 rayPos;
		Vector3 rayDir;

		rayDir = selectionObject->GetTransform().GetOrientation() * Vector3(0, 0, -1);

		rayPos = selectionObject->GetTransform().GetPosition();

		Ray r = Ray(rayPos, rayDir);

		if (world.Raycast(r, closestCollision, true, selectionObject)) {
			if (objClosest) {
				objClosest->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
			}
			objClosest = (GameObject*)closestCollision.node;

			objClosest->GetRenderObject()->SetColour(Vector4(1, 0, 1, 1));
		}
	}

	//bucket->GetPhysicsObject()->AddForce(Vector3(0, 20, 0));

	SelectObject();
	MoveSelectedObject();

	world.UpdateWorld(dt);
	physics.Update(dt);
}

void TutorialGame::UpdateKeys() {
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::F1)) {
		InitWorld(); //We can reset the simulation at any time with F1
		selectionObject = nullptr;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyCodes::F2)) {
		InitCamera(); //F2 will reset the camera to a specific default place
	}

	if (Window::GetKeyboard()->KeyPressed(KeyCodes::G)) {
		useGravity = !useGravity; //Toggle gravity!
		physics.UseGravity(useGravity);
	}
	//Running certain physics updates in a consistent order might cause some
	//bias in the calculations - the same objects might keep 'winning' the constraint
	//allowing the other one to stretch too much etc. Shuffling the order so that it
	//is random every frame can help reduce such bias.
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::F9)) {
		world.ShuffleConstraints(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::F10)) {
		world.ShuffleConstraints(false);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyCodes::F7)) {
		world.ShuffleObjects(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::F8)) {
		world.ShuffleObjects(false);
	}

	if (lockedObject) {
		LockedObjectMovement();
	}
	else {
		DebugObjectMovement();
	}
}

void TutorialGame::LockedObjectMovement() {
	Matrix4 view		= world.GetMainCamera().BuildViewMatrix();
	Matrix4 camWorld	= Matrix::Inverse(view);

	Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!

	//forward is more tricky -  camera forward is 'into' the screen...
	//so we can take a guess, and use the cross of straight up, and
	//the right axis, to hopefully get a vector that's good enough!

	Vector3 fwdAxis = Vector::Cross(Vector3(0, 1, 0), rightAxis);
	fwdAxis.y = 0.0f;
	fwdAxis = Vector::Normalise(fwdAxis);

	if (Window::GetKeyboard()->KeyDown(KeyCodes::UP)) {
		selectionObject->GetPhysicsObject()->AddForce(fwdAxis);
	}

	if (Window::GetKeyboard()->KeyDown(KeyCodes::DOWN)) {
		selectionObject->GetPhysicsObject()->AddForce(-fwdAxis);
	}

	if (Window::GetKeyboard()->KeyDown(KeyCodes::NEXT)) {
		selectionObject->GetPhysicsObject()->AddForce(Vector3(0,-10,0));
	}
}

void TutorialGame::DebugObjectMovement() {
//If we've selected an object, we can manipulate it with some key presses
	if (inSelectionMode && selectionObject) {
		//Twist the selected object!
		if (Window::GetKeyboard()->KeyDown(KeyCodes::LEFT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(-10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::NUM7)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, 10, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::NUM8)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, -10, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::UP)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, -10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::DOWN)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, 10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::NUM5)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, -10, 0));
		}
	}
}

void TutorialGame::InitGameWorld() {
	InitCamera();
	InitWorld();
}

void TutorialGame::InitCamera() {
	world.GetMainCamera().SetNearPlane(0.1f);
	world.GetMainCamera().SetFarPlane(500.0f);
	world.GetMainCamera().SetPitch(-45.0f);
	world.GetMainCamera().SetYaw(0.0f);
	world.GetMainCamera().SetPosition(Vector3(80, 100, 220));
	lockedObject = nullptr;
}

void TutorialGame::InitWorld() {
	std::cout << std::endl << "--------Initialising Game Objects--------" << std::endl;
	world.ClearAndErase();
	physics.Clear();

	InitPositions();
	InitDefaultFloor();
	InitGameExamples();	
}

void TutorialGame::InitDefaultFloor() {
	AddFloorToWorld(Vector3(150, 2, 95));
}

void TutorialGame::InitPositions() {
	switch (level) {
	case 1:
		playerPosition = Vector3(5, 4, 110);
		trainPosition = Vector3(30, 3.5f, 100);
		pickaxePosition = Vector3(25, 6.5f, 120);
		axePosition = Vector3(15, 8, 120);
		bucketPosition = Vector3(35, 6.5f, 120);
		break;
	case 2:
		playerPosition = Vector3(5, 4, 65);
		trainPosition = Vector3(30, 3.5f, 50);
		pickaxePosition = Vector3(20, 6.5f, 75);
		axePosition = Vector3(10, 8, 75);
		bucketPosition = Vector3(30, 6.5f, 75);
		break;
	case 3:
		playerPosition = Vector3(5, 4, 35);
		trainPosition = Vector3(30, 3.5f, 20);
		pickaxePosition = Vector3(20, 6.5f, 45);
		axePosition = Vector3(10, 8, 45);
		bucketPosition = Vector3(30, 6.5f, 45);
		break;
		//case 4:
		//    player1Position = Vector3(5, 4, 65);
		//    trainPosition = Vector3(30, 4.5f, 50);
		//    pickaxePosition = Vector3(20, 6.5f, 40);
		//    axePosition = Vector3(10, 8, 40);
		//    bucketPosition = Vector3(30, 6.5f, 40);
		//    break;
	default:
		break;
	}
}

void TutorialGame::InitGameExamples() {
	axe = AddAxeToWorld(axePosition);
	pickaxe = AddPickaxeToWorld(pickaxePosition);
	bucket = AddBucketToWorld(bucketPosition);
	player = AddPlayerToWorld(playerPosition);
	train = AddTrainToWorld(trainPosition);
	train->InitPaths(1);
	carriage1 = (MaterialCarriage*)(train->AddCarriage(21));
	carriage2 = (ProduceCarriage*)(train->AddCarriage(22));
	carriage3 = (WaterCarriage*)(train->AddCarriage(23));
	carriage1->SetProduceCarriage(carriage2);
	carriage2->SetMaterialCarriage(carriage1);
	pad = AddPadToWorld();
	AddSceneToWorld(1);
}

/*
Every frame, this code will let you perform a raycast, to see if there's an object
underneath the cursor, and if so 'select it' into a pointer, so that it can be 
manipulated later. Pressing Q will let you toggle between this behaviour and instead
letting you move the camera around. 

*/
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
		//Debug::Print("Press Q to change to camera mode!", Vector2(5, 85));

		if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::Left)) {
			if (selectionObject) {	//set colour to deselected;
				selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
				selectionObject = nullptr;
			}

			Ray ray = CollisionDetection::BuildRayFromMouse(world.GetMainCamera());

			RayCollision closestCollision;
			if (world.Raycast(ray, closestCollision, true)) {
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
		//Debug::Print("Press Q to change to select mode!", Vector2(5, 85));
	}
	return false;
}

/*
If an object has been clicked, it can be pushed with the right mouse button, by an amount
determined by the scroll wheel. In the first tutorial this won't do anything, as we haven't
added linear motion into our physics system. After the second tutorial, objects will move in a straight
line - after the third, they'll be able to twist under torque aswell.
*/

void TutorialGame::MoveSelectedObject() {
	//Debug::Print("Click Force:" + std::to_string(forceMagnitude), Vector2(5, 90));
	forceMagnitude += Window::GetMouse()->GetWheelMovement() * 100.0f;

	if (!selectionObject) {
		return;//we haven't selected anything!
	}
	//Push the selected object!
	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::Right)) {
		Ray ray = CollisionDetection::BuildRayFromMouse(world.GetMainCamera());

		RayCollision closestCollision;
		if (world.Raycast(ray, closestCollision, true)) {
			if (closestCollision.node == selectionObject) {
				selectionObject->GetPhysicsObject()->AddForceAtPosition(ray.GetDirection() * forceMagnitude, closestCollision.collidedAt);
			}
		}
	}
}