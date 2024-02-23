#pragma once

#include "CollectableObject.h"

namespace NCL::CSC8503 {
	class BucketObject :public CollectableObject {
	public:
		BucketObject(GameWorld* world, std::string name = "") {
			this->name = name;
			this->world = world;
			triggerDelete = true;
			typeID = 4;
		}
		~BucketObject() {};
	};
}