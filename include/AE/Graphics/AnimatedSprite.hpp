#ifndef ANIMATEDSPRITE_HPP
#define ANIMATEDSPRITE_HPP

#include <AE/Graphics/Animation.hpp>
#include <AE/Graphics/Sprite.hpp>
#include <AE/Graphics/Texture.hpp>
#include <vector>
#include <unordered_map>

using namespace std;

namespace ae
{
    
class SFML_GRAPHICS_API AnimatedSprite : public Sprite,
                                         public Animation
{
public:
    typedef std::unordered_map<std::string,
                               vector<std::shared_ptr<ae::Texture>>> AnimationsList;
    typedef std::shared_ptr<ae::Texture> TexturePtr;
    
private:
    AnimationsList animations;
    TexturePtr currentFrame;
    float fps;
    bool looped;
    
public:
    AnimatedSprite(float _fps);
    void addAnimation(const std::string& name,
                      const std::vector<std::shared_ptr<ae::Texture>>& frames);

    void setLoop();
    void stop();
    void play();
    
    void animate(float dt) override;
};    
}

#endif //ANIMATEDSPRITE_HPP
