#ifndef UPDATABLE_HPP
#define UPDATABLE_HPP

namespace ae
{

class Updatable
{
public:
    virtual ~Updatable() {}

protected:
    virtual void update() = 0;
};

} // namespace ae


#endif // UPDATABLE_HPP
