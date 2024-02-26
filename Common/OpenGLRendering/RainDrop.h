#pragma once
#include "Vector3.h"
using namespace NCL::Maths;
#include "OGLRenderer.h"
#include "Camera.h"
#include <vector>

struct Raindrop {
    Vector3 position;
    Vector3 velocity;
    float lifetime;
    float maxLifetime;
};

class RainDrop
{
public:
    std::vector<Raindrop>& GetRaindrops() {
        return raindrops;
    }

    int GetRaindropCount() const {
        return raindrops.size();
    }
protected:
	std::vector<Raindrop> raindrops;
};

