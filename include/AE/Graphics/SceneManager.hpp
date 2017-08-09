#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include <AE/Graphics/Scene.hpp>
#include <unordered_set>
#include <memory>

namespace ae
{
    
class SFML_GRAPHICS_API SceneManager
{
private:
    std::unordered_set<std::shared_ptr<Scene>> scenes;
    std::shared_ptr<Scene> currentScene;
    
public:
    SceneManager();
    void addScene(std::shared_ptr<Scene> newScene);
    void deleteScene(std::shared_ptr<Scene> scene);
    void setCurrentScene(std::shared_ptr<Scene> newScene);
    void replaceCurrentScene(std::shared_ptr<Scene> newScene);
    void updateCurrentScene();
    void drawCurrentScene(std::shared_ptr<ae::RenderWindow> renderWindow);
};
    
}

#endif //SCENEMANAGER_HPP
