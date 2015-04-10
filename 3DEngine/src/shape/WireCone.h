#pragma once
#include "RenderMesh.h"

SHARED_PTR_CLASS_DECL(WireCone);

class WireCone : public RenderMesh
{
public:
	static WireCone_ptr Create(float opening_angle, float height);

	virtual void Init() override;

protected:
	WireCone(float opening_angle, float height);

	~WireCone() {};

	float openingAngle;
	float height;
};

