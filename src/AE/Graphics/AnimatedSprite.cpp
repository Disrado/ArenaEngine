#include <AE/Graphics/AnimatedSprite.hpp>

namespace ae
{

AnimatedSprite::AnimatedSprite(float _fps)
    : fps(_fps)
{ }
    
void AnimatedSprite::addAnimation(const std::string& name,
                                  const std::vector<std::shared_ptr<ae::Texture>>& frames)
{
    animations.insert(std::make_pair(name, frames));
}
    
void AnimatedSprite::animate(float dt)
{

}
  
}
