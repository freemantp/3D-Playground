#pragma once

#include "Scene.h"


SHARED_PTR_CLASS_DECL(SceneOverlay2D);
SHARED_PTR_CLASS_DECL(Camera);

class SceneOverlay2D : public Scene
{
public:

	static SceneOverlay2D_ptr Create(Scene_ptr scene);

	virtual void TimeUpdate(double time) override;

protected:

	SceneOverlay2D(Scene_ptr scene);
	~SceneOverlay2D();
};

