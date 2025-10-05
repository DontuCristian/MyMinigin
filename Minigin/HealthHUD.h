#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
    class TextRenderer;
    class HealthHUD final : public BComponent, public Observer
    {
    public:

        HealthHUD(GameObject& obj);
        virtual ~HealthHUD() = default;

        HealthHUD(const HealthHUD& other) = delete;
        HealthHUD& operator=(const HealthHUD& other) = delete;
        HealthHUD(HealthHUD&& other) noexcept = delete;
        HealthHUD& operator=(HealthHUD&& other) noexcept = delete;

        void Update() override;
        void Render() const override;

        void OnNotify(Event event, Subject*) override;

        void SetText(const std::string& text);
        void SetFont(const std::string& fontPath, int size);

    private:
        
        TextRenderer* m_pTextRenderer;
    };
}

