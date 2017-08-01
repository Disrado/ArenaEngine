#ifndef UPDATABLE_HPP
#define UPDATABLE_HPP

namespace ae
{

class Updatable
{
public:
    virtual ~Drawable() {}

protected:
    virtual void update() const = 0;
};

} // namespace ae


#endif // UPDATABLE_HPP
