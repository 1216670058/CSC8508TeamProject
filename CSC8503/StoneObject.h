#pragma once

#include "CollectableObject.h"

namespace NCL::CSC8503 {
	class StoneObject :public CollectableObject {
	public:
		StoneObject(GameWorld* world) {
			this->world = world;
			triggerDelete = true;
			typeID = 6;
		}
		~StoneObject() {};
	};
}