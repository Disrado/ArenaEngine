#ifndef ANIMATEDSPRITE_HPP
#define ANIMATEDSPRITE_HPP

#include <AE/Graphics/Animation.hpp>
#include <AE/Graphics/Sprite.hpp>
#include <AE/Graphics/Texture.hpp>
#include <AE/Graphics/SpriteAnimation.hpp>
#include <vector>
#include <unordered_map>

using namespace std;

namespace ae
{
    
class SFML_GRAPHICS_API AnimatedSprite : public Sprite
{
public:
    typedef std::shared_ptr<ae::SpriteAnimation> SAnimationPtr;
    typedef std::unordered_map<std::string name, SAnimationPtr> AnimationsList;

private:
    AnimationsList animations;
    SAnimationPtr currentAnimation;
    
public:
    AnimatedSprite();

    //new animation can be created only from this class
    //createAnimation();
    
    void addAnimation(const SpriteAnimation& animation);

    SAnimationPtr getAnimation(std::string name) const;
    
    void animate(float dt);
    void draw(RenderTarget& target, RenderStates states) const override;
};    
}

#endif //ANIMATEDSPRITE_HPP
