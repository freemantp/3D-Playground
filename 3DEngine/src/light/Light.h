#pragma once;

class Scene;

class Light
{
public:
	virtual void render(const Scene& scene) = 0;
};