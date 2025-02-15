#include "FPSRenderer.h"
#include "TextRenderer.h"
#include "Timer.h"
#include "GameObject.h"


dae::FPSRenderer::FPSRenderer(GameObject* obj):
	BComponent(obj)
{
	m_TextRenderer = obj->GetComponent<TextRenderer>();
}

void dae::FPSRenderer::Update()
{
	if (!m_FixedFramerate)
	{
		m_TextRenderer->SetText("FPS: " + std::to_string(Timer::GetInstance().GetFPS()));
	}
}

void dae::FPSRenderer::Render() const
{
	m_TextRenderer->Render();
}

void dae::FPSRenderer::FixedFPS(bool myBool)
{
	m_FixedFramerate = myBool;
}
