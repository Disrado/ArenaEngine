#include <AE/Graphics.hpp>
#include <memory>
#include <gtest/gtest.h>

//------------------------------------------------------------------------------
TEST(ObjectTest, notifyAttached)
{
    auto SNode = ae::SceneNode::create("SNode");
    auto sprite = std::make_shared<ae::Sprite>("sprite");

    sprite->notifyAttached(SNode);

    ASSERT_TRUE(sprite->isAttached())
        << "Object was't been attached";

    ASSERT_EQ(SNode, sprite->getParentSceneNode())
        << "Parent was't be setted";
}
//------------------------------------------------------------------------------
TEST(ObjectTest, notifyDetached)
{
    auto SNode = ae::SceneNode::create("SNode");
    auto sprite = std::make_shared<ae::Sprite>("sprite");

    sprite->notifyAttached(SNode);
    sprite->notifyDetached();
    
    ASSERT_FALSE(sprite->isAttached())
        << "Object was't been detached";

    ASSERT_NE(SNode, sprite->getParentSceneNode())
        << "Parent was't be deleted";
}
//------------------------------------------------------------------------------
TEST(ObjectTest, detachFromParent)
{
    auto SNode = ae::SceneNode::create("SNode");
    auto sprite = std::make_shared<ae::Sprite>("sprite");

    SNode->attachObject(sprite);

    sprite->detachFromParent();
    
    ASSERT_FALSE(sprite->isAttached())
        << "Object was't been detached";

    ASSERT_NE(SNode, sprite->getParentSceneNode())
        << "Parent was't be deleted";

    ASSERT_FALSE(SNode->getAttachedObject("sprite"))
        << "Objce was't removed in parent";
}
//------------------------------------------------------------------------------
