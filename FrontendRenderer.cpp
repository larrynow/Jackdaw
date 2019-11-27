#include "FrontendRenderer.h"
#include"BackendRenderer.h"

RenderData jkFrontendRenderer::DrawMesh(jkMesh* mesh)
{
	RenderData renderData;
	renderData.pVertexBuffer = mesh->m_pVertexBuffer;
	renderData.pIndexBuffer = mesh->m_pIndexBuffer;
	renderData.vertexCount = renderData.pIndexBuffer->size() - renderData.offset;

	renderData.modelMatrix = mesh->GetWorldMatrx();
	
	return renderData;
}

void jkFrontendRenderer::PushToBackend(BackendRenderer* backend)
{
}
