#include "RobotObject.h"
#include "StateMachine.h"
#include "State.h"
#include "StateTransition.h"
#include "PhysicsObject.h"
//part of this codes are from www.github.com

RobotObject::RobotObject(string filePath, PlayerObject* player, Vector3 initialPosition) : GameObject()
{
	_player = player;
	playerPosition = _player->GetTransform().GetPosition();
	filename = filePath;
	grid = new NavigationGrid(filename);
	grid->FindPath(initialPosition, _player->GetTransform().GetPosition(), path);
	path.PopWaypoint(destPos);

	stateMachine = new StateMachine();

	State* IdleState = new State([&](float dt) -> void
		{
			this->Idle(dt);
		}
	);
	State* ChaseState = new State([&](float dt) -> void
		{

			this->Chase(dt);
		}
	);
	stateMachine->AddState(IdleState);

	stateMachine->AddState(ChaseState);

	stateMachine->AddTransition(new StateTransition(IdleState, ChaseState,
		[&]() -> bool
		{
			if (true) {
				return true;
			}
			else {
				return false;
			}
		}
	));
	stateMachine->AddTransition(new StateTransition(ChaseState, IdleState,
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

void RobotObject::UpdatePosition(PlayerObject* player, float dt)
{
	_player = player;
	currentPosition = GetTransform().GetPosition();
	stateMachine->Update(dt);
}

void RobotObject::OnCollisionBegin(PlayerObject* otherObject)
{
	if (otherObject->GetName() == "Goat") {
		std::cout << "Caught";
	}
}

void RobotObject::Chase(float dt)
{
	nodes.clear();
	path.Clear();
	grid->FindPath(currentPosition, _player->GetTransform().GetPosition(), path);
	while (path.PopWaypoint(destPos))
	{
		nodes.push_back(destPos);
	}
	//drawPath();
	if (nodes.size() >= 2) {
		Vector3 direction = nodes[1] - nodes[0];
		direction.Normalise();
		direction.y = 0;
		/*GetPhysicsObject()->SetForce(direction * speed * dt);*/
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
    while (renderObject->GetAnimationObject()->GetFrameTime() < 0.0f) {
        renderObject->GetAnimationObject()->SetCurrentFrame((renderObject->GetAnimationObject()->GetCurrentFrame() + 1) %
            renderObject->GetAnimationObject()->GetActiveAnim()->GetFrameCount());
        renderObject->GetAnimationObject()->SetFrameTime(renderObject->GetAnimationObject()->GetFrameTime() + 1.0f /
            renderObject->GetAnimationObject()->GetActiveAnim()->GetFrameRate());
    }
}