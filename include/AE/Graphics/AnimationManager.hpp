#ifndef ANIMATIONMANAGER_HPP
#define ANIMATIONMANAGER_HPP


#include <AE/Graphics/AnimatedSprite.hpp>
#include <vector>
#include <memory>

namespace ae
{
    
class AnimationManager
{
public:
    typedef std::shared_ptr<AnimatedSprite> ASpritePtr;
    typedef std::vector<std::shared_ptr<Animation>> AnimationList;
    
private:
    AnimationList animations;

    // TODO: How get any animation with his name?
    //std::vector<std::shared_ptr<AnimatedSprite>> animatedSprites;

    
public:
    ASpritePtr createAnimatedSprite(float fps);

    void stopAllAnimation();
    void playAllAnimation();
    
    void update(float dt);
};
    
}

#endif //ANIMATIONMANAGER_HPP
