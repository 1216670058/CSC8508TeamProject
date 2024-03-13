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

namespace NCL::CSC8503 {
	class AnimalObject :public GameObject {
	public:
		AnimalObject(string filePath, Vector3 startingPos);
		~AnimalObject() {};

		void Update(float dt) override;

		void MoveToPosition(Vector3 targetPos);
		bool Pathfind(Vector3 targetPos);

		StateMachine* stateMachine;
		NavigationGrid* grid;

		Vector3 currentPos;
		std::vector<Vector3> wanderPathNodes = {};
		int currentNodeIndex = 0;

		int gridSize;
	};
}