#include <AE/Graphics/AnimatedSprite.hpp>

namespace ae
{

// AnimatedSprite::AnimatedSprite()
// { }
    
void AnimatedSprite::addAnimation(const SpriteAnimation& animation)
{
    animations.insert(std::make_pair(animation->getName(), animation));
}

SAnimationPtr AnimatedSprite::getAnimation(std::string name) const
{
    auto anim = animations.find(name);
    if(anim)
        return anim;
    else
        return nullptr;
}
    
void AnimatedSprite::animate(float dt)
{
    for(auto animation : animations)
        animation->animate(dt);

    Sprite::setTexture(currentAnimation->getCurrentFrame().get());
}
   
void AnimatedSprite::draw(RenderTarget& target, RenderStates states) const
{
    Sprite::draw(RenderTarget& target, RenderStates states);
}
    
}
