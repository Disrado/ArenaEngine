#include <AE/Graphics.hpp>
#include <memory>
#include <gtest/gtest.h>
#include <cstdlib>

//------------------------------------------------------------------------------
TEST(LayerTest, buildDrawableQueue)
{
    auto layer = std::make_shared<ae::Layer>();
    std::shared_ptr<ae::Sprite> sprites[100];

    for(int i(0); i < 100; ++i) {
        sprites[i] = std::make_shared<ae::Sprite>(std::to_string(i));
    }

    for(int i(0); i < 10; ++i) {
        auto node = layer->getRootSceneNode()->createChildSceneNode(std::to_string(i));
        node->attachObject(sprites[i]);

        for(int j = i + 1; j < i + 11; ++j) {
             auto node2 = node->createChildSceneNode(std::to_string(j));
            node2->attachObject(sprites[j]);
        }
    }

    layer->update();
    
    ASSERT_EQ(layer->drawableQueue.size(), layer->getNumberAllNodes());
}
//------------------------------------------------------------------------------
TEST(LayerTest, updateDrawableQueue)
{
    
}
//------------------------------------------------------------------------------
