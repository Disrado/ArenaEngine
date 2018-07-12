#include <iostream>

#include <AE/System/Vector2.hpp>

int main()
{
    ae::Vector2<float> vec2(1.0, 2.0);

    std::cout << vec2.x << " " << vec2.y;
}
