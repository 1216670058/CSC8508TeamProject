#pragma once

#include "CollectableObject.h"

namespace NCL::CSC8503 {
	class RailObject :public CollectableObject {
	public:
		RailObject(GameWorld* world) {
			this->world = world;
			triggerDelete = true;
			typeID = 7;
			placed = false;
			name = "Rail";
		}
		~RailObject() {};

		void SetCarriage(ProduceCarriage* c) {
			carriage = c;
		}

		void SetInCarriage(bool i) {
			inCarriage = i;
		}

		void SetHeight(float h) {
			height = h;
		}

		int GetNum() const {
			return num;
		}
		void SetNum(int n) {
			num = n;
		}

		void OnCollisionBegin(GameObject* otherObject) override;

		void Update(float dt) override;

		//int GetDirection(Vector3 point1, Vector3 point2);

		int GetRailDirection(const Vector3& position);
	protected:
		void PlaceRail();

		bool inCarriage = true;
		float height;
		int num = 1;
		bool placed;

		ProduceCarriage* carriage = nullptr;
	};
}