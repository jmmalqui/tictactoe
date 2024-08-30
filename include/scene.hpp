#pragma once
#include "common.hpp"

class SceneManager;

class Scene {
public:
    explicit Scene(std::string sceneName, SceneManager* sceneManager = nullptr);

private:
    std::string m_SceneName;

public:
    SceneManager* m_SceneManager;
    void SetSceneName(std::string sceneName);
    std::string GetSceneName(void);
    virtual void Update() = 0;
    virtual void Render() = 0;
};