#include "EnginePCH.hpp"

#include "ShaderProgram.hpp"

#include <GL/glew.h>

using namespace Poly;

void RenderingSystem::RenderingPhase(World* world)
{
	// TODO get rid of this "once" code
	static ShaderProgram testProgram("test.vsh", "test.fsh");
	static bool Once = true;
	if (Once)
	{
		Once = false;
		//testProgram.RegisterUniform("uMVP");
		testProgram.RegisterUniform("uTransform");
	}
	
	glDepthMask(GL_TRUE);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	ScreenSize screen = world->GetEngine()->GetRenderingContext()->GetScreenSize();
	for (auto& kv : world->GetViewportWorldComponent().GetViewports())
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		const AARect& rect = kv.second.GetRect();
		glViewport((int)(rect.GetMin().X * screen.Width), (int)(rect.GetMin().Y * screen.Height),
			(int)(rect.GetSize().X * screen.Width), (int)(rect.GetSize().Y * screen.Height));

		testProgram.BindProgram();
		const Matrix& mvp = kv.second.GetCamera()->GetMVP();
		//testProgram.SetUniform("uMVP", mvp);
		
		// Render objects
		for (auto componentsTuple : world->IterateComponents<MeshRenderingComponent, TransformComponent>())
		{
			const MeshRenderingComponent* meshCmp = std::get<MeshRenderingComponent*>(componentsTuple);
			const TransformComponent* transCmp = std::get<TransformComponent*>(componentsTuple);

			const Matrix& objTransform = transCmp->GetGlobalTransformationMatrix();
			Matrix screenTransform = mvp * objTransform;
			testProgram.SetUniform("uTransform", screenTransform);
			for (const GLMeshResource::SubMesh* subMesh : meshCmp->GetMesh()->GetSubMeshes())
			{
				glBindVertexArray(subMesh->GetVAO());
				
				if (subMesh->GetDiffTexture())
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, subMesh->GetDiffTexture()->GetID());
				}
				
				glDrawElements(GL_TRIANGLES, subMesh->GetVertexCount(), GL_UNSIGNED_INT, NULL);
				glBindTexture(GL_TEXTURE_2D, 0);
				glBindVertexArray(0);
			}
		}

		CHECK_GL_ERR();
		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
	}

	world->GetEngine()->GetRenderingContext()->EndFrame();
}
