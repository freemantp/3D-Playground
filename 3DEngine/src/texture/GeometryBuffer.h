#pragma once

#include "../util/SharedPointer.h"
#include <glm/core/type.hpp>

SHARED_PTR_CLASS_DECL(GeometryBuffer);
SHARED_PTR_CLASS_DECL(GBufferShader);

class Scene;


class GeometryBuffer
{
public:

	static GeometryBuffer_ptr Create(int width, int height);
	
	virtual ~GeometryBuffer();

	/// 
	void StartGBufferComp(const Scene& scene, const glm::mat4& modelTransform);
	
	
	void EndGBufferComp();


protected:
	GeometryBuffer(int width, int height);

	enum TexTypes {Position, Normal};

	GLuint m_DeferredFboHandle;
	GLuint m_DepthBuffer;
	GLuint m_Textures[2];

	GBufferShader_ptr m_Shader;
};

