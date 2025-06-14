#pragma once
#include <BaseComponent.h>
#include <Font.h>
#include <Texture2D.h>
#include <string>
#include <memory>
#include <array>
#include <vector>

namespace dae
{
    class TextInput final : public BComponent
    {
    public:

        TextInput(GameObject& obj, const std::string& fontPath, int characterSpacing, const std::string& pointerTexturePath);
        TextInput(const TextInput&) = delete;
        TextInput& operator=(const TextInput&) = delete;
        TextInput(TextInput&&) = delete;
        TextInput& operator=(TextInput&&) = delete;
        ~TextInput() = default;

        void Update() override;
        void Render() const override;

        //Navigation
        void Up();
        void Down();
        void Left();
        void Right();

        std::string GetResult() { return m_Result; }
        bool IsDone() { return m_Result.size() == SIZE; }
    private:
        static constexpr std::string_view CHARACTERS = "_ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        static constexpr int SIZE = 3;

        std::shared_ptr<Font> m_pFont;

        int m_Spacing;

        std::string m_Result{};
        std::array<int, SIZE> m_CharacterValues{};

        int m_Index{};
        std::vector<std::shared_ptr<Texture2D>> m_pCharacterTextures;
        std::shared_ptr<Texture2D> m_pPointerTexture;

        bool m_isDirty = true;
    };
}

