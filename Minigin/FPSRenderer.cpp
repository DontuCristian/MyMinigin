#include "FPSRenderer.h"
#include "TextRenderer.h"
#include "Timer.h"
#include "GameObject.h"


dae::FPSRenderer::FPSRenderer(GameObject& obj):
	BComponent(obj)
{
	//The FPSComp is dependendent on having a text renderer, so if there isn't one 
	//on the object it should add it
	if (obj.HasComponent<TextRenderer>())
	{
		m_TextRenderer = obj.GetComponent<TextRenderer>();
	}
	else
	{
		obj.AddComponent<TextRenderer>();
		m_TextRenderer = obj.GetComponent<TextRenderer>();
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
