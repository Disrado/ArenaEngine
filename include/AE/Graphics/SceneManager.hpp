#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include <AE/Graphics/Scene.hpp>
#include <unordered_map>
#include <memory>

namespace ae
{
    
class SFML_GRAPHICS_API SceneManager
{
private:
    std::unordered_map<std::string, Scene::ScenePtr> scenes;
    Scene::ScenePtr currentScene;
    
public:
    SceneManager();

    static std::shared_ptr<SceneManager> create();
    
    void addScene(Scene::ScenePtr newScene);
    void deleteScene(Scene::ScenePtr scene);
    void setCurrentScene(Scene::ScenePtr newScene);
    void replaceCurrentScene(Scene::ScenePtr newScene);
    void updateCurrentScene();
    void drawCurrentScene(std::shared_ptr<ae::RenderWindow> renderWindow);
};
    
}

#endif //SCENEMANAGER_HPP
