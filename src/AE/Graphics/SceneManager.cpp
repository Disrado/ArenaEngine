#include <AE/Graphics/SceneManager.hpp>

namespace ae
{

SceneManager::SceneManager() :
    currentScene(nullptr)
{ }    

std::shared_ptr<SceneManager> SceneManager::create()
{
    return std::make_shared<SceneManager>();
}
    
void SceneManager::addScene(std::shared_ptr<Scene> newScene)
{
    if(newScene) {
        scenes.insert(std::make_pair(newScene->getName(), newScene));

	if(!currentScene)
	    currentScene = newScene;
    }
}
    
void SceneManager::deleteScene(std::shared_ptr<Scene> sceneToDelete)
{
    auto itr = scenes.find(sceneToDelete->getName());

    if(itr != scenes.end())
        scenes.erase(itr);
}

void SceneManager::setCurrentScene(std::shared_ptr<Scene> newScene)
{
    if(newScene) {
        if(scenes.find(newScene->getName()) == scenes.end())
            scenes.insert(std::make_pair(newScene->getName(), newScene));
    
        currentScene = newScene;
    }
}

void SceneManager::replaceCurrentScene(std::shared_ptr<Scene> newScene)
{
    currentScene = newScene;
}

void SceneManager::updateCurrentScene()
{
    if(currentScene)
        currentScene->update();
}
    
void SceneManager::drawCurrentScene(std::shared_ptr<ae::RenderWindow> renderWindow)
{
    if(currentScene)
        renderWindow->draw(*currentScene);
}

} //namespace ae
