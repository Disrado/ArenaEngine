#include <AE/Graphics/SceneManager.hpp>

namespace ae
{

SceneManager::SceneManager() :
    currentScene(nullptr)
{ }    
    
void SceneManager::addScene(std::shared_ptr<Scene> newScene)
{
    if(newScene)
        scenes.insert(newScene);
    if(!currentScene)
        currentScene = newScene;
}

void SceneManager::deleteScene(std::shared_ptr<Scene> sceneToDelete)
{
    auto itr = scenes.find(sceneToDelete);

    if(itr != scenes.end())
        scenes.erase(itr);
}

void SceneManager::setCurrentScene(std::shared_ptr<Scene> newScene)
{
    if(newScene) {
        if(scenes.find(newScene) == scenes.end())
            scenes.insert(newScene);
    
        currentScene = newScene;
    }
}

void SceneManager::replaceCurrentScene(std::shared_ptr<Scene> newScene)
{
    currentScene = newScene;
}

void SceneManager::drawCurrentScene(std::shared_ptr<ae::RenderWindow> renderWindow)
{
    if(currentScene)
        renderWindow->draw(*currentScene);
}

} //namespace ae
