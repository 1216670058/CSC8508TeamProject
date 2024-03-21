#include "RobotObject.h"
#include "StateMachine.h"
#include "State.h"
#include "StateTransition.h"
#include "PhysicsObject.h"
#include "TutorialGame.h"
//part of this codes are from www.github.com

RobotObject::RobotObject(NavigationGrid* navGrid, PlayerObject* player, Vector3 initialPosition) : GameObject()
{
	grid = navGrid;
	_player = player;
	playerPosition = _player->GetTransform().GetPosition();
	typeID = 11;
	timer = 0;

	stateMachine = new StateMachine();

	State* IdleState = new State([&](float dt) -> void
		{
			this->Idle(dt);
		}
	);
	State* FollowPlayerState = new State([&](float dt) -> void
		{
			this->FollowPlayer(dt);
		}
	);
	State* MoveToTreeState = new State([&](float dt) -> void
		{
			this->MoveToTree(dt);
		}
	);
	State* MoveToRockState = new State([&](float dt) -> void
		{
			this->MoveToRock(dt);
		}
	);
	State* CuttingState = new State([&](float dt) -> void
		{
			this->CutTree(dt);
		}
	);
	State* DiggingState = new State([&](float dt) -> void
		{
			this->DigRock(dt);
		}
	);
	State* MoveToPlayerState = new State([&](float dt) -> void
		{
			this->MoveToPlayer(dt);
		}
	);

	stateMachine->AddState(IdleState);
	stateMachine->AddState(FollowPlayerState);
	stateMachine->AddState(MoveToTreeState);
	stateMachine->AddState(MoveToRockState);
	stateMachine->AddState(CuttingState);
	stateMachine->AddState(DiggingState);
	stateMachine->AddState(MoveToPlayerState);

	stateMachine->AddTransition(new StateTransition(IdleState, FollowPlayerState,
		[&]() -> bool
		{
			if (_player->RobotCut() || _player->RobotDig()) {
				if (_player->RobotCut()) _player->SetRobotCut(false);
				if (_player->RobotDig()) _player->SetRobotDig(false);
				stateID = 1;
				return true;
			}
			else {
				return false;
			}
		}
	));

	stateMachine->AddTransition(new StateTransition(FollowPlayerState, MoveToTreeState,
		[&]() -> bool
		{
			if (cutting && Window::GetKeyboard()->KeyPressed(KeyCodes::T)) {
				treeCounter = 5.0f;
				nextTree = true;
				stateID = 2;
				return true;
			}
			else {
				return false;
			}
		}
	));

	stateMachine->AddTransition(new StateTransition(FollowPlayerState, MoveToRockState,
		[&]() -> bool
		{
			if (digging && Window::GetKeyboard()->KeyPressed(KeyCodes::T)) {
				rockCounter = 5.0f;
				nextRock = true;
				stateID = 3;
				return true;
			}
			else {
				return false;
			}
		}
	));

	stateMachine->AddTransition(new StateTransition(MoveToTreeState, CuttingState,
		[&]() -> bool
		{
			bool flag = treeFound;
			treeFound = false;
			stateID = 4;
			return flag;
		}
	));

	stateMachine->AddTransition(new StateTransition(CuttingState, MoveToTreeState,
		[&]() -> bool
		{
			bool flag = treeCut;
			treeCut = false;
			if (flag) { 
				stateID = 2;
				treeCounter = 5.0f;
				nextTree = true;
			}
			return flag;
		}
	));

	stateMachine->AddTransition(new StateTransition(MoveToRockState, DiggingState,
		[&]() -> bool
		{
			bool flag = rockFound;
			rockFound = false;
			stateID = 5;
			return flag;
		}
	));

	stateMachine->AddTransition(new StateTransition(DiggingState, MoveToRockState,
		[&]() -> bool
		{
			bool flag = rockDug;
			rockDug = false;
			if (flag) {
				stateID = 3;
				rockCounter = 5.0f;
				nextRock = true;
			}
			return flag;
		}
	));

	stateMachine->AddTransition(new StateTransition(MoveToTreeState, MoveToPlayerState,
		[&]() -> bool
		{
			bool flag = moveToPlayer || Window::GetKeyboard()->KeyPressed(KeyCodes::T);
			moveToPlayer = false;
			if (flag) {
				stateID = 6;
			}
			return flag;
		}
	));

	stateMachine->AddTransition(new StateTransition(CuttingState, MoveToPlayerState,
		[&]() -> bool
		{
			bool flag = Window::GetKeyboard()->KeyPressed(KeyCodes::T);
			if (flag) {
				stateID = 6;
			}
			return flag;
		}
	));

	stateMachine->AddTransition(new StateTransition(MoveToRockState, MoveToPlayerState,
		[&]() -> bool
		{
			bool flag = moveToPlayer || Window::GetKeyboard()->KeyPressed(KeyCodes::T);
			moveToPlayer = false;
			if (flag) {
				stateID = 6;
			}
			return flag;
		}
	));

	stateMachine->AddTransition(new StateTransition(DiggingState, MoveToPlayerState,
		[&]() -> bool
		{
			bool flag = Window::GetKeyboard()->KeyPressed(KeyCodes::T);
			if (flag) {
				stateID = 6;
			}
			return flag;
		}
	));

	stateMachine->AddTransition(new StateTransition(MoveToPlayerState, IdleState,
		[&]() -> bool
		{
			bool flag = foundPlayer;
			foundPlayer = false;
			if (flag) {
				cutting = false;
				digging = false;
				stateID = 0;
			}
			return flag;
		}
	));
}

void RobotObject::OnCollisionBegin(GameObject* otherObject){
	if (cutting && otherObject->GetTypeID() == 10086) {
		treeFound = true;
		face = (otherObject->GetTransform().GetPosition() - currentPosition);
	}
	else if (digging && otherObject->GetTypeID() == 10010) {
		rockFound = true;
		face = (otherObject->GetTransform().GetPosition() - currentPosition);
	}
}

void RobotObject::Idle(float dt)
{
	//do nothing
}

void RobotObject::FollowPlayer(float dt) {
	if (_player->GetSlot() == 3)cutting = true;
	if (_player->GetSlot() == 2)digging = true;
	if (GetGrid(currentPosition).position != GetGrid(playerPosition).position) {
		nodes.clear();
		path.Clear();
		grid->FindPath2(transform.GetPosition(), playerPosition, path);
		while (path.PopWaypoint(destPos))
		{
			nodes.push_back(destPos);
		}
		if (nodes.size() > 1) {
			Vector3 direction = nodes[1] - nodes[0];
			direction.Normalise();
			direction.y = 0;
			UpdateOrientation(direction);
			GetPhysicsObject()->AddForce(direction * speed);
			Debug::DrawLine(nodes[1], nodes[0], Vector4(0, 1, 0, 1));
			nodes.clear();
		}
	}
}

void RobotObject::MoveToTree(float dt) {
	nodes.clear();
	path.Clear();
	if (nextTree) {
		treePosition = grid->FindNearestTree(transform.GetPosition());
		if (treePosition != transform.GetPosition())
			nextTree = false;
	}
	//std::cout << "TreePosition: " << treePosition.x << "," << treePosition.y << "," << treePosition.z << "," << std::endl;
	grid->FindPath2(transform.GetPosition(), treePosition, path);
	while (path.PopWaypoint(destPos))
	{
		nodes.push_back(destPos);
	}
	if (nodes.size() > 1) {
		Vector3 direction = nodes[1] - nodes[0];
		direction.Normalise();
		direction.y = 0;
		UpdateOrientation(direction);
		GetPhysicsObject()->AddForce(direction * speed);
		Debug::DrawLine(nodes[1], nodes[0], Vector4(0, 1, 0, 1));
		nodes.clear();
	}
	treeCounter -= dt;
	if (treeCounter <= 0) {
		moveToPlayer = true;
	}
}

void RobotObject::MoveToRock(float dt) {
	nodes.clear();
	path.Clear();
	if (nextRock) {
		rockPosition = grid->FindNearestRock(transform.GetPosition());
		if (rockPosition != transform.GetPosition())
			nextRock = false;
	}
	grid->FindPath2(transform.GetPosition(), rockPosition, path);
	while (path.PopWaypoint(destPos))
	{
		nodes.push_back(destPos);
	}
	if (nodes.size() > 1) {
		Vector3 direction = nodes[1] - nodes[0];
		direction.Normalise();
		direction.y = 0;
		UpdateOrientation(direction);
		GetPhysicsObject()->AddForce(direction * speed);
		Debug::DrawLine(nodes[1], nodes[0], Vector4(0, 1, 0, 1));
		nodes.clear();
	}
	rockCounter -= dt;
	if (rockCounter <= 0) {
		moveToPlayer = true;
	}
}

void RobotObject::CutTree(float dt) {
	Ray r = Ray(transform.GetPosition(), face);
	RayCollision closestCollision;
	if (TutorialGame::GetGame()->GetWorld()->Raycast(r, closestCollision, true, this)) {
		GameObject* closest = (GameObject*)closestCollision.node;
		if (closest->GetTypeID() == 10086) {
			if (closestCollision.rayDistance < 5.0f) {
				TutorialGame::GetGame()->GetAudio()->PlayWood();
				closest->SetFlag1(true);
				closest->GetTransform().SetScale(closest->GetTransform().GetScale() - Vector3(0.025, 0.025, 0.025));
				if (closest->GetTransform().GetScale().x < 0.1f) {
					Vector3 position = FindGrid(closest->GetTransform().GetPosition());
					int index = position.x / 10 + (position.z / 10) * TutorialGame::GetGame()->GetNavigationGrid()->GetGridWidth();
					GridNode& n = TutorialGame::GetGame()->GetNavigationGrid()->GetGridNode(index);
					n.SetType(0);
					closest->SetFlag1(false);
					PlankObject* plank1 = TutorialGame::GetGame()->AddPlankToWorld(Vector3(closest->GetTransform().GetPosition().x, 5, closest->GetTransform().GetPosition().z));
					TutorialGame::GetGame()->GetWorld()->RemoveGameObject(closest, false);
					treeCut = true;
				}
			}
			else {
				//std::cout << "Moving" << std::endl;
				Vector3 direction = closest->GetTransform().GetPosition() - currentPosition;
				direction.Normalise();
				direction.y = 0;
				UpdateOrientation(direction);
				GetPhysicsObject()->AddForce(direction * speed);
				Debug::DrawLine(transform.GetPosition(), closest->GetTransform().GetPosition(), Vector4(1, 1, 0, 1));
			}
		}
		else {
			//std::cout << "Not Tree" << std::endl;
			treeCut = true;
		}
	}
	else {
		//std::cout << "Not Tree" << std::endl;
		treeCut = true;
	}
}

void RobotObject::DigRock(float dt) {
	Ray r = Ray(transform.GetPosition(), face);
	RayCollision closestCollision;
	if (TutorialGame::GetGame()->GetWorld()->Raycast(r, closestCollision, true, this)) {
		GameObject* closest = (GameObject*)closestCollision.node;
		if (closest->GetTypeID() == 10010) {
			if (closestCollision.rayDistance < 5.0f) {
				TutorialGame::GetGame()->GetAudio()->PlayWood();
				closest->SetFlag1(true);
				closest->GetTransform().SetScale(closest->GetTransform().GetScale() - Vector3(0.025, 0.025, 0.025));
				if (closest->GetTransform().GetScale().x < 0.1f) {
					Vector3 position = FindGrid(closest->GetTransform().GetPosition());
					int index = position.x / 10 + (position.z / 10) * TutorialGame::GetGame()->GetNavigationGrid()->GetGridWidth();
					GridNode& n = TutorialGame::GetGame()->GetNavigationGrid()->GetGridNode(index);
					n.SetType(0);
					closest->SetFlag1(false);
					StoneObject* stone1 = TutorialGame::GetGame()->AddStoneToWorld(Vector3(closest->GetTransform().GetPosition().x, 5, closest->GetTransform().GetPosition().z));
					TutorialGame::GetGame()->GetWorld()->RemoveGameObject(closest, false);
					rockDug = true;
				}
			}
			else {
				//std::cout << "Moving" << std::endl;
				Vector3 direction = closest->GetTransform().GetPosition() - currentPosition;
				direction.Normalise();
				direction.y = 0;
				UpdateOrientation(direction);
				GetPhysicsObject()->AddForce(direction * speed);
				Debug::DrawLine(transform.GetPosition(), closest->GetTransform().GetPosition(), Vector4(1, 1, 0, 1));
			}
		}
		else {
			//std::cout << "Not Tree" << std::endl;
			rockDug = true;
		}
	}
	else {
		//std::cout << "Not Tree" << std::endl;
		rockDug = true;
	}
}

void RobotObject::MoveToPlayer(float dt) {
	if (GetGrid(currentPosition).position != GetGrid(playerPosition).position) {
		nodes.clear();
		path.Clear();
		grid->FindPath2(transform.GetPosition(), playerPosition, path);
		while (path.PopWaypoint(destPos))
		{
			nodes.push_back(destPos);
		}
		if (nodes.size() > 1) {
			Vector3 direction = nodes[1] - nodes[0];
			direction.Normalise();
			direction.y = 0;
			UpdateOrientation(direction);
			GetPhysicsObject()->AddForce(direction * speed);
			Debug::DrawLine(nodes[1], nodes[0], Vector4(0, 1, 0, 1));
			nodes.clear();
		}
	}
	else {
		foundPlayer = true;
	}
}

void RobotObject::Update(float dt) {
	timer += dt;
	if (timer > 1.0) timer = 1.0;

    renderObject->GetAnimationObject()->SetFrameTime(renderObject->GetAnimationObject()->GetFrameTime() - dt);
    while (renderObject->GetAnimationObject()->GetFrameTime() <= 0.0f) {
        renderObject->GetAnimationObject()->SetCurrentFrame((renderObject->GetAnimationObject()->GetCurrentFrame() + 1) %
            renderObject->GetAnimationObject()->GetActiveAnim()->GetFrameCount());
        renderObject->GetAnimationObject()->SetFrameTime(renderObject->GetAnimationObject()->GetFrameTime() + 1.0f /
            renderObject->GetAnimationObject()->GetActiveAnim()->GetFrameRate());
    }

	stateMachine->Update(dt);

	currentPosition = transform.GetPosition();
	playerPosition = _player->GetTransform().GetPosition();
}

void RobotObject::UpdateOrientation(Vector3 direction) {
	Quaternion rotation;
	if (direction.x > 0) rotation = Quaternion::EulerAnglesToQuaternion(0, -90, 0);
	else if (direction.x < 0) rotation = Quaternion::EulerAnglesToQuaternion(0, 90, 0);
	else if (direction.z > 0) rotation = Quaternion::EulerAnglesToQuaternion(0, 180, 0);
	else if (direction.z < 0) rotation = Quaternion::EulerAnglesToQuaternion(0, 0, 0);
	transform.SetOrientation(rotation);
}

GridNode& RobotObject::GetGrid(const Vector3& position) {
	Vector3 p = FindGrid(Vector3(position.x, 4.5f, position.z));
	int index = p.x / 10 + (p.z / 10) * TutorialGame::GetGame()->GetNavigationGrid()->GetGridWidth();
	return TutorialGame::GetGame()->GetNavigationGrid()->GetGridNode(index);
}