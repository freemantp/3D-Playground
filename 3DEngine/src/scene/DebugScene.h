#pragma once

#include "Scene.h"


SHARED_PTR_CLASS_DECL(DebugScene);
SHARED_PTR_CLASS_DECL(Camera);

class DebugScene : public Scene
{
public:

	static DebugScene_ptr Create(Scene_ptr scene);

protected:

	DebugScene(Scene_ptr scene);
	~DebugScene();
};

