#include <AE/Graphics/AnimationManager.hpp>


namespace ae
{
    
namespace
{
    typedef ae::AnimationManager::ASpritePtr ASpritePtr;
}
    
ASpritePtr AnimationManager::createAnimatedSprite(float fps)
{
    auto asprite = std::make_shared<ae::AnimatedSprite>(fps);
    animations.push_back(asprite);
    return asprite;
}
    
void AnimationManager::update(float dt)
{
    for(auto animation : animations) {
        animation->animate(dt);
    }
}

}
