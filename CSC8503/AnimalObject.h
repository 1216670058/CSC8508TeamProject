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
		AnimalObject(float xMin, float xMax, float zMin, float zMax);
		~AnimalObject() {};

		void Update(float dt) override;

		StateMachine* stateMachine;

		Vector3 currentPos;

		float xMin;
		float xMax;
		float zMin;
		float zMax;

		std::vector<Vector3> wanderPoints = {};
		int currentWanderIndex = 0;
	};
}