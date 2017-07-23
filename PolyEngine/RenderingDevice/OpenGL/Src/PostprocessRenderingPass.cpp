#include "PostprocessRenderingPass.hpp"

#include "GLMeshDeviceProxy.hpp"
#include "GLTextureDeviceProxy.hpp"

#include <World.hpp>
#include <CameraComponent.hpp>
#include <TransformComponent.hpp>
#include <MeshRenderingComponent.hpp>

using namespace Poly;

struct PostprocessQuad : public BaseObject<>
{
public:
	PostprocessQuad()
	{
		// quad with uv mapping
		static const float vertices[] = {
			// x,y,z, u,v
			-1.f, 1.f, 0.f, 0.f, 1.f,
			1.f, -1.f, 0.f, 1.f, 0.f,
			1.f, 1.f, 0.f, 1.f, 1.f,

			-1.f, 1.f, 0.f, 0.f, 1.f,
			-1.f, -1.f, 0.f, 0.f, 0.f,
			1.f, -1.f, 0.f, 1.f, 0.f
		};

		glGenVertexArrays(1, &VAO);
		ASSERTE(VAO > 0, "PostprocessQuad VAO creation failed!");
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		ASSERTE(VBO > 0, "PostprocessQuad VBO creation failed!");
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(float), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	~PostprocessQuad()
	{
		if (VBO)
			glDeleteBuffers(1, &VBO);
		if (VAO)
			glDeleteVertexArrays(1, &VAO);
	}

	GLuint VAO = 0;
	GLuint VBO = 0;
};



PostprocessRenderingPass::PostprocessRenderingPass(const String& fragment)
	: RenderingPassBase("Shaders/postprocessCommonVert.shader", fragment)
{
}

PostprocessRenderingPass::PostprocessRenderingPass(const String& geometry, const String& fragment)
	: RenderingPassBase("Shaders/postprocessCommonVert.shader", geometry, fragment)
{
}


void PostprocessRenderingPass::OnRun(World* world, const CameraComponent* camera, const AABox& rect)
{
	static const PostprocessQuad QUAD;

	GetProgram().BindProgram();
	glBindVertexArray(QUAD.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
