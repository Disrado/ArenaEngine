#ifndef ANIMATION_HPP
#define ANIMATION_HPP

namespace ae
{
    
class Animation
{    
public:
    virtual void animate(float dt) = 0;
};
    
}

#endif //ANIMATION_HPP
