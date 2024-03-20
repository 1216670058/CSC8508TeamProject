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
		AnimalObject(string filePath, Vector3 startingPos, GameWorld* world);
		~AnimalObject() {};

		void Update(float dt) override;

		void MoveToPosition(Vector3 targetPos, float speed);
		bool Pathfind(Vector3 targetPos);

		void DetectThreat(GameObject* object);
		void StopDetectThreat(GameObject* object);

		StateMachine* stateMachine;
		NavigationGrid* grid;
		GameWorld* world;

		Vector3 currentPos;
		//Vector3 scaredPos;
		std::vector<Vector3> wanderPathNodes = {};
		int currentNodeIndex = 0;

		float timer = 0;
		float stateCooldown = 0;

		int gridSize;
		bool noLongerScared = false;
	};


	class DetectionSphereObject : public GameObject {
	public:
		DetectionSphereObject(AnimalObject* animal);
		~DetectionSphereObject() {};

		void OnCollisionBegin(GameObject* otherObject) override {
			//std::cout << "DETECTION COL BEGIN\n";
			animal->DetectThreat(otherObject);
		}

		/*void OnCollisionStay(GameObject* otherObject) override {
			std::cout << "DETECTION COL STAY\n";
		}*/

		void OnCollisionEnd(GameObject* otherObject) override {
			//std::cout << "DETECTION COL END\n";
			animal->StopDetectThreat(otherObject);
		}

	protected:
		AnimalObject* animal;
	};
}