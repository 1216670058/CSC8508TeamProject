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
	};
}