#include "RobotObject.h"
#include "StateMachine.h"
#include "State.h"
#include "StateTransition.h"
#include "PhysicsObject.h"
#include "TutorialGame.h"
//part of this codes are from www.github.com

RobotObject::RobotObject(PlayerObject* player, Vector3 initialPosition) : GameObject()
{
	_player = player;
	playerPosition = _player->GetTransform().GetPosition();
	typeID = 11;

	stateMachine = new StateMachine();

	State* IdleState = new State([&](float dt) -> void
		{
			this->Idle(dt);
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

	stateMachine->AddState(IdleState);
	stateMachine->AddState(CuttingState);
	stateMachine->AddState(DiggingState);

	stateMachine->AddTransition(new StateTransition(IdleState, CuttingState,
		[&]() -> bool
		{
			if (_player->RobotCut()) {
				return true;
			}
			else {
				return false;
			}
		}
	));
	stateMachine->AddTransition(new StateTransition(IdleState, DiggingState,
		[&]() -> bool
		{
			if (_player->RobotDig()) {
				return true;
			}
			else {
				return false;
			}
		}
	));
	stateMachine->AddTransition(new StateTransition(CuttingState, IdleState,
		[&]() -> bool
		{
			if (this->counter > 30.0f) {
				return true;
			}
			else {
				return false;
			}
		}
	));
}

void RobotObject::OnCollisionBegin(PlayerObject* otherObject)
{
	if (otherObject->GetName() == "Goat") {
		std::cout << "Caught";
	}
}

void RobotObject::CutTree(float dt) {
	cutting = true;
	nodes.clear();
	path.Clear();
	grid->FindPath(transform.GetPosition(), grid->FindNearestTree(transform.GetPosition()), path);
	while (path.PopWaypoint(destPos))
	{
		nodes.push_back(destPos);
	}
	if (nodes.size() > 1) {
		Vector3 direction = nodes[1] - nodes[0];
		physicsObject->AddForce(direction.Normalised() * speed);
		UpdateOrientation(direction);
		Debug::DrawLine(nodes[1], nodes[0], Vector4(0, 1, 0, 1));
		nodes.clear();
	}
	counter += dt;
}

void RobotObject::DigRock(float dt) {
	digging = true;
	nodes.clear();
	path.Clear();
	grid->FindPath(transform.GetPosition(), _player->GetTransform().GetPosition(), path);
	while (path.PopWaypoint(destPos))
	{
		nodes.push_back(destPos);
	}
	if (nodes.size() >= 2) {
		Vector3 direction = nodes[1] - nodes[0];
		direction.Normalise();
		direction.y = 0;
		GetPhysicsObject()->AddForce(direction * speed * dt);
	}
	counter += dt;
}

void RobotObject::Idle(float dt)
{
	//do nothing
}

void RobotObject::drawPath()
{
	for (int i = 0; i < nodes.size() - 1; i++)
	{
		Debug::DrawLine(nodes[i] + Vector3(0, 1, 0), nodes[i + 1] + Vector3(0, 1, 0), Vector4(1, 0, 0, 1));
	}
}
void RobotObject::Update(float dt) {
    renderObject->GetAnimationObject()->SetFrameTime(renderObject->GetAnimationObject()->GetFrameTime() - dt);
    while (renderObject->GetAnimationObject()->GetFrameTime() <= 0.0f) {
        renderObject->GetAnimationObject()->SetCurrentFrame((renderObject->GetAnimationObject()->GetCurrentFrame() + 1) %
            renderObject->GetAnimationObject()->GetActiveAnim()->GetFrameCount());
        renderObject->GetAnimationObject()->SetFrameTime(renderObject->GetAnimationObject()->GetFrameTime() + 1.0f /
            renderObject->GetAnimationObject()->GetActiveAnim()->GetFrameRate());
    }

	stateMachine->Update(dt);
}

void RobotObject::UpdateOrientation(Vector3 direction) {
	Quaternion rotation;
	if (direction.x > 0) rotation = Quaternion::EulerAnglesToQuaternion(0, -90, 0);
	else if (direction.x < 0) rotation = Quaternion::EulerAnglesToQuaternion(0, 90, 0);
	else if (direction.z > 0) rotation = Quaternion::EulerAnglesToQuaternion(0, 180, 0);
	else if (direction.z < 0) rotation = Quaternion::EulerAnglesToQuaternion(0, 0, 0);
	transform.SetOrientation(rotation);
}