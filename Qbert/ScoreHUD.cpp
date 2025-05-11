#include "ScoreHUD.h"  
#include "TextRenderer.h"  
#include "GameObject.h"

dae::ScoreHUD::ScoreHUD(GameObject& obj) :
    BComponent(obj)
{
    if (!obj.HasComponent<TextRenderer>())
        obj.AddComponent<TextRenderer>();

    m_pTextRenderer = obj.GetComponent<TextRenderer>();
}

void dae::ScoreHUD::Update()
{
    //No need for update  
}

void dae::ScoreHUD::Render() const
{
    m_pTextRenderer->Render();
}

void dae::ScoreHUD::OnNotify(Event event, Subject*)
{
    if (event.id == make_sdbm_hash("ScoreChanged"))
    {
        int healthValue = std::any_cast<int>(event.args[0]);
        m_pTextRenderer->SetText(std::to_string(healthValue));
    }
}

void dae::ScoreHUD::SetText(const std::string& text)
{
    m_pTextRenderer->SetText(text);
}

void dae::ScoreHUD::SetFont(const std::string& fontPath, int size)
{
    m_pTextRenderer->SetFont(fontPath, size);
}
