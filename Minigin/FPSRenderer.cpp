#include "FPSRenderer.h"
#include "TextRenderer.h"
#include "Timer.h"
#include "GameObject.h"


dae::FPSRenderer::FPSRenderer(GameObject& owner):
	BComponent(owner)
{
	//The FPSComp is dependendent on having a text renderer, so if there isn't one 
	//on the object it should add it
	if (owner.HasComponent<TextRenderer>())
	{
		m_TextRenderer = owner.GetComponent<TextRenderer>();
	}
	else
	{
		owner.AddComponent<TextRenderer>();
		m_TextRenderer = owner.GetComponent<TextRenderer>();
	}
}

void dae::FPSRenderer::Update()
{
	std::stringstream fpsStream{};

	fpsStream << std::format("FPS: {:.1f}",Timer::GetInstance().GetFPS());
	m_TextRenderer->SetText(fpsStream.str());
}

void dae::FPSRenderer::Render() const
{
	m_TextRenderer->Render();
}
