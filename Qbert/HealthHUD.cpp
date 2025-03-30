#include "HealthHUD.h"  
#include "TextRenderer.h"  
#include "GameObject.h"

dae::HealthHUD::HealthHUD(GameObject& obj):  
   BComponent(obj)  
{  
   if(!obj.HasComponent<TextRenderer>())  
       obj.AddComponent<TextRenderer>();  

   m_pTextRenderer = obj.GetComponent<TextRenderer>();  
}  

void dae::HealthHUD::Update()  
{  
   //No need for update  
}  

void dae::HealthHUD::Render() const  
{  
   m_pTextRenderer->Render();  
}  

void dae::HealthHUD::OnNotify(Event event, Subject*)  
{  
   if (event.id == make_sdbm_hash("HealthChanged"))  
   {  
       int healthValue = std::any_cast<int>(event.args[0]);  
       m_pTextRenderer->SetText("HP: " + std::to_string(healthValue));
   }  
}

void dae::HealthHUD::SetText(const std::string& text)
{
    m_pTextRenderer->SetText(text);
}

void dae::HealthHUD::SetFont(const std::string& fontPath, int size)
{
    m_pTextRenderer->SetFont(fontPath, size);
}
