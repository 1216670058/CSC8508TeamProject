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
		RobotObject(PlayerObject* player, Vector3 position);
		RobotObject() {
			typeID = 11;
			name = "Robot";
		}
		~RobotObject() { delete grid; delete stateMachine; }

		void Update(float dt) override;
		void SetOrigin(Vector3 position) { origin = position; }

		void SetGrid(NavigationGrid* g) {
			grid = g;
		}

		bool IsCutting() const {
			return cutting;
		}
		void SetCutting(bool c) {
			cutting = c;
		}
		bool IsDigging() const {
			return digging;
		}
		void SetDigging(bool d) {
			digging = d;
		}

		virtual void OnCollisionBegin(PlayerObject* otherObject);
		void drawPath();

	protected:
		void Idle(float dt);
		void CutTree(float dt);
		void DigRock(float dt);

		void UpdateOrientation(Vector3 direction);

		PlayerObject* _player = nullptr;
		Vector3 playerPosition;
		StateMachine* stateMachine;
		float counter;
		NavigationGrid* grid;
		vector<Vector3> nodes;

		float speed = 20.0f;

		Vector3 origin;

		Vector3 targetPosition;

		NavigationPath path;

		Vector3 currentPosition;

		std::string filename;

		Vector3 lastKnownPosition;

		bool stateSwitch;

		Vector3 destPos;

		bool pathNotFound = true;

		bool cutting = false;
		bool digging = false;

		int j = 0;
	};
}