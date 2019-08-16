#pragma once
#include "RenderMesh.h"

SHARED_PTR_CLASS_DECL(WireCube);

class WireCube : public RenderMesh
{
public:
	SHARED_PTR_FACTORY(WireCube);

	virtual void Init() override;

	WireCube();
};

