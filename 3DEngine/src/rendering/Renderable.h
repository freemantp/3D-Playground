#pragma once;

class Scene;

class Renderable
{
public:
	virtual void Render(const Scene& scene) const = 0;
};