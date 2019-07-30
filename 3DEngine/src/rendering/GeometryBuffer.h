#pragma once

#include "../util/SharedPointer.h"
#include <glm/fwd.hpp>
#include "../core/gl.h"


SHARED_PTR_CLASS_DECL(GeometryBuffer);
SHARED_PTR_CLASS_DECL(GBufferShader);
SHARED_PTR_CLASS_DECL(Scene);


class GeometryBuffer
{
public:

	static GeometryBuffer_ptr Create(int width, int height);
	
	virtual ~GeometryBuffer();

	/// 
	void StartGBufferComp(const Scene_ptr& scene);
	
	
	void EndGBufferComp();


protected:
	GeometryBuffer(int width, int height);

	enum TexTypes {Position, Normal};

	GLuint m_DeferredFboHandle;
	GLuint m_DepthBuffer;
	GLuint m_Textures[2];

	GBufferShader_ptr m_Shader;
};

