#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <AE/Graphics/Transformable.hpp>
#include <AE/Graphics/Drawable.hpp>

namespace ae
{

class Object : public Drawable, public Transformable
{    
protected:
    virtual ~Object() {}
    virtual void draw(RenderTarget& target, RenderStates states) const = 0;
};

} // namespace ae

#endif //OBJECT_HPP
