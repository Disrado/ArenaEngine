#ifndef TRANSFORMABLE_OBJECT_HPP
#define TRANSFORMABLE_OBJECT_HPP

#include <AE/Graphics/Transformable.hpp>
#include <AE/Graphics/Object.hpp>
#include <memory>

namespace ae
{

class TransformableObject : public Object, 
{
public:
    TransformableObject(); 
    TransformableObject(std::string _name, bool _visible = true);
    
    virtual ~TransformableObject() {}

    virtual void draw(RenderTarget& target, RenderStates states) const = 0;
};

} // namespace ae

#endif //TRANSFORMABLE_OBJECT_HPP
