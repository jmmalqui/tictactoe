#pragma once
#include "common.hpp"

class Scene;

class SceneManager {
public:
    // Constructors
    SceneManager();
    virtual ~SceneManager() = default;

private:
    std::map<std::string, Scene*> m_Scenes;
    std::string m_CurrentScene;

public:
    void RegisterScene(Scene* scene);
    void ChangeSceneTo(std::string sceneName);
    void Run();
};