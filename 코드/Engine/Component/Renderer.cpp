#include "Renderer.h"

Renderer::Renderer(const shared_ptr<GameObject> gameObject)
	: ComponentBase(gameObject),
	_materials(nullptr)
{
	_materials = make_unique<MaterialVec>();
}

Renderer::~Renderer()
{
}

void Renderer::Create(void* pData)
{
}

void Renderer::Destroy()
{

}