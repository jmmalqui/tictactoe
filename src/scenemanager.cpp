#include "scenemanager.hpp"

SceneManager::SceneManager() { }

void SceneManager::RegisterScene(Scene* scene)
{
    std::string sceneName = scene->GetSceneName();
    bool isSceneAlreadyREgistered = false;
    for (std::map<std::string, Scene*>::iterator iter = m_Scenes.begin(); iter != m_Scenes.end(); ++iter) {
        if (iter->first == sceneName) {
            isSceneAlreadyREgistered = true;
            break;
        }
    }
    if (!isSceneAlreadyREgistered) {
        m_Scenes.insert(std::map<std::string, Scene*>::value_type(sceneName, scene));
    }
}

void SceneManager::ChangeSceneTo(std::string sceneName)
{
    m_CurrentScene = sceneName;
}

void SceneManager::Run()
{
    std::map<std::string, Scene*>::iterator sceneIter = m_Scenes.find(m_CurrentScene);
    if (sceneIter != m_Scenes.end()) {
        sceneIter->second->Update();
        sceneIter->second->Render();
    }
}