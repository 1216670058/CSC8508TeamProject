#pragma once

#include "CollectableObject.h"

namespace NCL::CSC8503 {
	class PlankObject :public CollectableObject {
	public:
		PlankObject(GameWorld* world) {
			this->world = world;
			triggerDelete = true;
			typeID = 5;
		}
		~PlankObject() {};
	};
}