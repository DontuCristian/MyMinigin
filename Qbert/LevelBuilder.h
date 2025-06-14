#pragma once
#include <Singleton.h>
#include "Scene.h"
#include "json.hpp"
#include "glm.hpp"
#include <string>
#include <memory>

namespace dae
{
    using json = nlohmann::json;

	class LevelBuilder : public Singleton<LevelBuilder>
	{
    public:
        void BuildLevel(const std::string& filePath, Scene* scene);

    private:
        //Helpers
        void BuildPyramid(const json& pyramidData, Scene* scene, glm::vec2& outStartPos, float& outCubeSize, int& outTotalRows, float& outOffset);
        void AddKillZones(Scene* scene, const glm::vec2& startPos, float cubeSize, int totalRows, float offset);
        std::vector<std::shared_ptr<GameObject>> AddPlayers(const json& playersData, Scene* scene);
        void AddElevator(const json& elevatorData, Scene* scene);
        void AddUI(Scene* scene, const std::vector<std::shared_ptr<GameObject>>& players);
        void BindPlayerControls(GameObject& player, const json& controlMap, int controller);
	};
}

