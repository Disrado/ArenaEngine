#include <AE/Graphics.hpp>
#include <memory>
#include <gtest/gtest.h>
/*

// For this test need change private section to public in Layer

//------------------------------------------------------------------------------
TEST(LayerTest, buildDrawableQueue)
{
    auto layer = std::make_shared<ae::Layer>();
    std::shared_ptr<ae::Sprite> sprites[100];

    for(int i(0); i < 100; ++i) {
        sprites[i] = std::make_shared<ae::Sprite>(std::to_string(i));
    }

    // creating nodes and attaching sprite to it
    int t = 0;
    for(int i(0); i < 10; ++i) {
        auto node = layer->getRootSceneNode()->createChildSceneNode(std::to_string(i));

        for(int j(0); j < 10; ++j) {
            auto node2 = node->createChildSceneNode(std::to_string(j));
            node2->attachObject(sprites[t++]);
        }
    }
    
    // pool creating in update 
    layer->update();

    // 100 -  number of attached sprites
    ASSERT_EQ(layer->drawableQueue.size(), 100);
}
//------------------------------------------------------------------------------
TEST(LayerTest, buildNodesCommonPool)
{
    auto layer = std::make_shared<ae::Layer>();

    // creating 100 nodes
    for(int i(0); i < 10; ++i) {
        auto node = layer->getRootSceneNode()->createChildSceneNode(std::to_string(i));

        for(int j(0); j < 10; ++j) {
             node->createChildSceneNode(std::to_string(j));
        }
    }

    // pool creating in update 
    layer->update();

    // 1 rootSceneNode + 10 rootSceneNode children + 100 children of children  = 111
    ASSERT_EQ(layer->nodesCommonPool.size(), 111);
}
//------------------------------------------------------------------------------
TEST(LayerTest, getSceneNode)
{
    auto layer = std::make_shared<ae::Layer>();

    auto rootNode = layer->getRootSceneNode();

    auto firstChild = rootNode->createChildSceneNode("firstNode");
    rootNode->createChildSceneNode("secondNode");

    firstChild->createChildSceneNode("thirdNode");

    layer->update();

    ASSERT_NE(layer->getSceneNode("firstNode"), nullptr)
              << "First level node was not added to common pool";

    ASSERT_NE(layer->getSceneNode("firstNode"), nullptr)
              << "Second level node was not added to common pool";
}
//------------------------------------------------------------------------------
*/
