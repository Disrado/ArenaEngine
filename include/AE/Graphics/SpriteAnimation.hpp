#ifndef SPRITEANIMATION_HPP
#define SPRITEANIMATION_HPP

#include <AE/Graphics/Animation.hpp>
#include <AE/Graphics/Texture.hpp>
#include <vector>
#include <memory>

namespace ae
{

class SFML_GRAPHICS_API SpriteAnimation : public Animation
{
public:
    typedef std::shared_ptr<ae::Texture> TexturePtr;

private:
    std::string name;
    std::vector<TexturePtr> frames;
    TexturePtr currentFrame;
    bool launched;
    bool looped;
    int fps;
        
public:
    SpriteAnimation(const std::string& _name,
                    const std::vector<TexturePtr>& _frames,
                    int _currentFrame,
                    int _fps);
               
    void play() { launched = true; }
    void pause() { launched = false; }
    void setLooped(bool _looped) { looped = _looped; }

    bool isLaunched() const { return launched; }
    bool isLooped() const { return looped; }

    const std::string& getName() const { return name; }
    const TexturePtr getCurrentFrame() const { return currentFrame; }
    
    void animate(float dt);
};

} // namespace ae


#endif //SPRITEANIMATION_HPP
