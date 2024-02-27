#pragma once

#include "CollectableObject.h"

namespace NCL::CSC8503 {
	class RailObject :public CollectableObject {
	public:
		RailObject(GameWorld* world) {
			this->world = world;
			triggerDelete = true;
			typeID = 7;
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

	protected:
		bool inCarriage = true;
		float height;
		int num = 1;

		ProduceCarriage* carriage = nullptr;
	};
}