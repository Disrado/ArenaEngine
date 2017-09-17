#ifndef MOVEANIMATION_HPP
#define MOVEANIMATION_HPP

#include <AE/Graphics/Animation.hpp>
#include <AE/Graphics/Sprite.hpp>
#include <vector>
#include <unordered_map>

namespace ae
{
    
class MoveAnimation : public Sprite,
                      public Animation
{
private:
    float fps;
    bool looped;

    //movingFunction()
    
public:
    MoveAnimation(float _fps);

    void bindMovingFunction();
    
    void animate(float dt) override;
};    
}

#endif //MOVEANIMATION_HPP
