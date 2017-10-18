#include <AE/Graphics/SpriteAnimation.hpp>

namespace ae
{

SpriteAnimation::SpriteAnimation(const std::string& _name,
                                 const std::vector<TexturePtr>& _frames,
                                 //index of frames
                                 int _currentFrame,
                                 int _fps)
    : name(_name),
      frames(_frames),
      fps(_fps),
      currentFrame(_frames[_currentFrame])
{}

void SpriteAnimation::animate(float dt)
{
    if(launched) {

    }

    if(currentFrame == frames[frames.size() - 1] && !looped)
        launched = false;
}

}
