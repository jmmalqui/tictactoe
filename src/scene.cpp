#include "scene.hpp"
#include "scenemanager.hpp"

Scene::Scene(std::string sceneName, SceneManager* sceneManager)
    : m_SceneName(sceneName)
    , m_SceneManager(sceneManager)
{
    if (m_SceneManager) {
        m_SceneManager->RegisterScene(this);
    }
}

void Scene::SetSceneName(std::string sceneName)
{
    m_SceneName = sceneName;
}

std::string Scene::GetSceneName()
{
    return m_SceneName;
}