#pragma once

#include "PhysicsSystem.h"
#include "PhysicsObject.h"
#include "StateMachine.h"
#include "NavigationGrid.h"
#include "StateTransition.h"
#include "State.h"
#include "StateMachine.h"
#include "PositionConstraint.h"
#include "NavigationGrid.h"
#include "GameObject.h"
#include <vector>
#include "RenderObject.h"
#include "AnimationObject.h"
#include "TrainCarriage.h"
#include "TrainCarriage.h"
#include "PlayerObject.h"
namespace NCL::CSC8503 {
	class StateMachine;
	class RobotObject :public GameObject {
	public:
		RobotObject(std::string filePath, PlayerObject* player, Vector3 position);
		RobotObject() {
			typeID = 11;
			name = "Robot";
		}
		~RobotObject() { delete grid; delete stateMachine; }

		void Update(float dt) override;
		virtual void UpdatePosition(PlayerObject* player, float dt);
		void SetOrigin(Vector3 position) { origin = position; }

		virtual void OnCollisionBegin(PlayerObject* otherObject);
		void drawPath();

	protected:

		void Chase(float dt);
		void Idle(float dt);

		PlayerObject* _player = nullptr;
		Vector3 playerPosition;
		StateMachine* stateMachine;
		float counter;
		NavigationGrid* grid;
		vector<Vector3> nodes;

		float speed = 300.0f;

		Vector3 origin;

		Vector3 targetPosition;

		NavigationPath path;

		Vector3 currentPosition;

		std::string filename;

		Vector3 lastKnownPosition;

		bool stateSwitch;

		Vector3 destPos;

		bool pathNotFound = true;

		int j = 0;
	};
}