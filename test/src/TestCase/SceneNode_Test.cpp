#include <AE/Graphics.hpp>
#include <memory>
#include <gtest/gtest.h>

//------------------------------------------------------------------------------
TEST(SceneNodeTest, createChild)
{
    auto SNode = ae::SceneNode::create("SNode");
    auto childNode = SNode->createChildSceneNode("childNode");
	
    ASSERT_TRUE(childNode)
        << "Incorrect return value";

    ASSERT_EQ(childNode->getParent(), SNode)
        << "Incorrect value of parent in child node";

    ASSERT_NE(SNode->getChildrenCount(), 0)
        << "Error while child edding as child to parent";
}
//------------------------------------------------------------------------------
TEST(SceneNodeTest, addChild)
{
    auto SNode = ae::SceneNode::create("SNode");
    auto childNode = ae::SceneNode::create("childNode");

    SNode->addChild(childNode);
	
    ASSERT_EQ(childNode->getParent(), SNode)
        << "Incorrect value of parent in child node";

    ASSERT_NE(SNode->getChildrenCount(), 0)
        << "Error while child edding as child to parent";
}
//------------------------------------------------------------------------------
TEST(SceneNodeTest, removeChild)
{
    auto SNode = ae::SceneNode::create("SNode");
    auto childNode = SNode->createChildSceneNode("child_1");
	
    SNode->removeChild(childNode);
	
    ASSERT_NE(childNode->getParent(), SNode)
        << "Parent in child was not removed";

    ASSERT_EQ(SNode->getChildrenCount(), 0)
        << "Error while children deleting from parent";
}
//------------------------------------------------------------------------------
TEST(SceneNodeTest, removeChildren)
{
    auto SNode = ae::SceneNode::create("SNode");
    auto child_1 = SNode->createChildSceneNode("child_1");
    auto child_2 = SNode->createChildSceneNode("child_2");

    SNode->removeChildren();
        
    ASSERT_NE(child_1->getParent(), SNode)
        << "Parent in child node was not removed";

    ASSERT_EQ(SNode->getChildrenCount(), 0)
        << "Error while children deleting from parent";
}
//------------------------------------------------------------------------------
TEST(SceneNodeTest, destroyChild)
{
    auto SNode = ae::SceneNode::create("SNode");
    auto childNode = SNode->createChildSceneNode("child_1");
    auto childChildNode = childNode->createChildSceneNode("child_1");
    auto sprite = std::make_shared<ae::Sprite>("sprite");
    childNode->attachObject(sprite);

    SNode->destroyChild(childNode);
	
    ASSERT_NE(childNode->getParent(), SNode)
        << "Parent in child was not removed";

    ASSERT_NE(childChildNode->getParent(), childNode)
        << "Parent was not deleted from child of child";

    ASSERT_EQ(SNode->getChildrenCount(), 0)
        << "Children was not removed from parent";

    ASSERT_FALSE(sprite->isAttached())
        << "Object was not been detached";
}
//------------------------------------------------------------------------------
TEST(SceneNodeTest, destroyChildren)
{
    auto SNode = ae::SceneNode::create("SNode");
    auto childNode = SNode->createChildSceneNode("child_1");
    auto childChildNode = childNode->createChildSceneNode("child_1");
    auto sprite = std::make_shared<ae::Sprite>("sprite");
    childNode->attachObject(sprite);

    SNode->destroyChild(childNode);
	
    ASSERT_NE(childNode->getParent(), SNode)
        << "Parent in child was not removed";

    ASSERT_NE(childChildNode->getParent(), childNode)
        << "Parent was not deleted from child of child";

    ASSERT_EQ(SNode->getChildrenCount(), 0)
        << "Children was not removed from parent";

    ASSERT_FALSE(sprite->isAttached())
        << "Object was not been detached from parent";
}
//------------------------------------------------------------------------------
TEST(SceneNodeTest, rebaseToNewParent)
{
    auto firstParent = ae::SceneNode::create("firstParent");
    auto secondParent = ae::SceneNode::create("secondParent");
	
    auto childNode = firstParent->createChildSceneNode("childNode");
	
    childNode->rebaseToNewParent(secondParent);
	
    ASSERT_NE(childNode->getParent(), firstParent)
        << "First parent was not been removed from childNode parent";

    ASSERT_EQ(childNode->getParent(), secondParent)
        << "SecondParent was not been set as parent of childNode";
    
    ASSERT_FALSE(firstParent->getChild("childNode"))
        << "Child node was not been deleted from first parent";
    
    ASSERT_TRUE(secondParent->getChild("childNode"))
        << "Child node has not been added to new parent";
}
//------------------------------------------------------------------------------
TEST(SceneNodeTest, rebaseChildrenToNewParent)
{
    auto firstParent = ae::SceneNode::create("firstParent");
    auto secondParent = ae::SceneNode::create("secondParent");
	
    for(int i(0); i < 10; ++i)
        firstParent->createChildSceneNode(std::to_string(i));
	
    firstParent->rebaseChildrenToNewParent(secondParent);
	
    ASSERT_EQ(firstParent->getChildrenCount(), 0)
        << "Children was not been removed from parent";

    ASSERT_NE(secondParent->getChildrenCount(), 0)
        << "Children was not been added to new parent";
}
//------------------------------------------------------------------------------
TEST(SceneNodeTest, setVisibleRecursive)
{
    auto parent = ae::SceneNode::create("parent");
    auto child = parent->createChildSceneNode("child1");
    auto childChild = child->createChildSceneNode("child2");
    auto childChildChild = childChild->createChildSceneNode("child3");
	
    parent->setVisibleRecursive(false);
        
    ASSERT_FALSE(childChildChild->isVisible())
        << "Visible was not been set recursive";
}
//------------------------------------------------------------------------------
TEST(SceneNodeTest, attachObject)
{
    auto SNode = ae::SceneNode::create("SNode");

    auto sprite = std::make_shared<ae::Sprite>("sprite");

    SNode->attachObject(sprite);

    ASSERT_NE(SNode->numAttachedObjects(), 0)
        << "Object was not been attached";
}
//------------------------------------------------------------------------------
TEST(SceneNodeTest, detachObject)
{
    auto SNode = ae::SceneNode::create("SNode");

    auto sprite = std::make_shared<ae::Sprite>("sprite");

    SNode->attachObject(sprite);
    SNode->detachObject(sprite);

    ASSERT_FALSE(sprite->isAttached())
        << "Object was not been notified that he detached";
    
    ASSERT_EQ(SNode->numAttachedObjects(), 0)
        << "Object was not been detached";
}
//------------------------------------------------------------------------------    
TEST(SceneNodeTest, detachAllObjects)
{
    auto SNode = ae::SceneNode::create("SNode");

    for(int i(0); i < 10; ++i) {
        auto sprite = std::make_shared<ae::Sprite>(std::to_string(i));
        SNode->attachObject(sprite);
    }

    SNode->detachAllObjects();
        
    ASSERT_EQ(SNode->numAttachedObjects(), 0)
        << "Objects was not been detached";
}
//------------------------------------------------------------------------------
TEST(SceneNodeTest, getAttachedObject)
{
    auto SNode = ae::SceneNode::create("SNode");

    auto sprite = std::make_shared<ae::Sprite>("sprite");

    SNode->attachObject(sprite);
    
    ASSERT_EQ(SNode->getAttachedObject("sprite"), sprite)
        << "Incorect value of returned object";
}
//------------------------------------------------------------------------------
TEST(SceneNodeTest, getDrawableObjects)
{
    auto SNode = ae::SceneNode::create("SNode");

    for(int i(0); i < 10; ++i) {
        auto sprite = std::make_shared<ae::Sprite>(std::to_string(i));
        SNode->attachObject(sprite);
        if(i < 5) // Set visible 5/10 objects
            sprite->setVisible(false);
    }
        
    ASSERT_EQ(SNode->getDrawableObjects().size(), 5u)
        << "Incorect size of returned vector";
}
//------------------------------------------------------------------------------
TEST(SceneNodeTest, getDrawableChildren)
{
    auto SNode = ae::SceneNode::create("SNode");

    for(int i(0); i < 10; ++i) {
        auto node = SNode->createChildSceneNode(std::to_string(i));
        if(i < 5)
            node->setVisible(false);
    }
        
    ASSERT_EQ(SNode->getDrawableChildren().size(), 5u)
        << "Incorect size of returned vector";
}
//------------------------------------------------------------------------------
TEST(SceneNodeTest, setDrawOrder)
{
    auto SNode = ae::SceneNode::create("SNode");
    auto child1 = SNode->createChildSceneNode("child1", 5);
    auto child2 = SNode->createChildSceneNode("child2", 10);
            
    child2->setDrawOrder(2);
	
    ASSERT_EQ(child2->getDrawOrder(), 2)
        << "New value of draw order was't been set";

    ASSERT_EQ(child1->getParent()->getDrawableChildren()[0]->getDrawOrder(), 2)
        << "Parent does't sort children after they change his draw order";
}
//------------------------------------------------------------------------------
TEST(SceneNodeTest, changeObjectDrawOrder)
{
    auto SNode = ae::SceneNode::create("SNode");
    auto sprite1 = std::make_shared<ae::Sprite>("sprite1");
    auto sprite2 = std::make_shared<ae::Sprite>("sprite2");

    SNode->attachObject(sprite1, 1);
    SNode->attachObject(sprite2, 2);

    SNode->changeObjectDrawOrder(sprite2, 0);

    SNode->update();
    
    ASSERT_EQ(sprite2->getDrawOrder(), 0)
        << "New value of draw order was't been set";

    ASSERT_EQ(SNode->getDrawableObjects()[0]->getDrawOrder(), 0)
        << "Parent does't sort objects after they change his draw order";
}
//------------------------------------------------------------------------------
TEST(SceneNodeTest, setOriginRecursive)
{
    auto SNode = ae::SceneNode::create("SNode");
    auto child_1 = SNode->createChildSceneNode("child_1");

    child_1->createChildSceneNode("1");
    child_1->createChildSceneNode("2");
    child_1->createChildSceneNode("3");

    SNode->setOriginRecursive(ae::Vector2f(100, 100));

    ASSERT_EQ(child_1->getOrigin(), ae::Vector2f(100, 100));
    ASSERT_EQ(child_1->getChild("1")->getOrigin(), ae::Vector2f(100, 100));
    ASSERT_EQ(child_1->getChild("2")->getOrigin(), ae::Vector2f(100, 100));
    ASSERT_EQ(child_1->getChild("3")->getOrigin(), ae::Vector2f(100, 100));
}        
//------------------------------------------------------------------------------
TEST(SceneNodeTest, setScaleRecursive)
{
    auto SNode = ae::SceneNode::create("SNode");
    auto child_1 = SNode->createChildSceneNode("child_1");

    child_1->createChildSceneNode("1");
    child_1->createChildSceneNode("2");
    child_1->createChildSceneNode("3");

    SNode->setScaleRecursive(ae::Vector2f(0.5, 0.5));

    ASSERT_EQ(child_1->getScale(), ae::Vector2f(0.5, 0.5));
    ASSERT_EQ(child_1->getChild("1")->getScale(), ae::Vector2f(0.5, 0.5));
    ASSERT_EQ(child_1->getChild("2")->getScale(), ae::Vector2f(0.5, 0.5));
    ASSERT_EQ(child_1->getChild("3")->getScale(), ae::Vector2f(0.5, 0.5));
}
//------------------------------------------------------------------------------
TEST(SceneNodeTest, setPositionRecursive)
{
    auto SNode = ae::SceneNode::create("SNode");
    auto child_1 = SNode->createChildSceneNode("child_1");

    child_1->createChildSceneNode("1");
    child_1->createChildSceneNode("2");
    child_1->createChildSceneNode("3");

    SNode->setPositionRecursive(ae::Vector2f(100, 100));

    ASSERT_EQ(child_1->getPosition(), ae::Vector2f(100, 100));
    ASSERT_EQ(child_1->getChild("1")->getPosition(), ae::Vector2f(100, 100));
    ASSERT_EQ(child_1->getChild("2")->getPosition(), ae::Vector2f(100, 100));
    ASSERT_EQ(child_1->getChild("3")->getPosition(),ae::Vector2f(100, 100));
}
//------------------------------------------------------------------------------
TEST(SceneNodeTest, setRotationRecursive)
{
    auto SNode = ae::SceneNode::create("SNode");
    auto child_1 = SNode->createChildSceneNode("child_1");

    child_1->createChildSceneNode("1");
    child_1->createChildSceneNode("2");
    child_1->createChildSceneNode("3");

    SNode->setRotationRecursive(100);

    ASSERT_EQ(child_1->getRotation(), 100);
    ASSERT_EQ(child_1->getChild("1")->getRotation(), 100);
    ASSERT_EQ(child_1->getChild("2")->getRotation(), 100);
    ASSERT_EQ(child_1->getChild("3")->getRotation(), 100);
}
//------------------------------------------------------------------------------
TEST(SceneNodeTest, moveRecursive)
{
    auto SNode = ae::SceneNode::create("SNode");
    auto child_1 = SNode->createChildSceneNode("child_1");

    child_1->createChildSceneNode("1");
    child_1->createChildSceneNode("2");
    child_1->createChildSceneNode("3");

    SNode->moveRecursive(ae::Vector2f(100, 100));

    ASSERT_EQ(child_1->getPosition(), ae::Vector2f(100, 100));
    ASSERT_EQ(child_1->getChild("1")->getPosition(), ae::Vector2f(100, 100));
    ASSERT_EQ(child_1->getChild("2")->getPosition(), ae::Vector2f(100, 100));
    ASSERT_EQ(child_1->getChild("3")->getPosition(), ae::Vector2f(100, 100));
}
//------------------------------------------------------------------------------
TEST(SceneNodeTest, rotateRecursive)
{
    auto SNode = ae::SceneNode::create("SNode");
    auto child_1 = SNode->createChildSceneNode("child_1");
    
    auto tempChild = child_1->createChildSceneNode("1");
    tempChild = child_1->createChildSceneNode("2");
    tempChild = child_1->createChildSceneNode("3");
    
    SNode->rotateRecursive(100);
    
    ASSERT_EQ(child_1->getRotation(), 100);
    ASSERT_EQ(child_1->getChild("1")->getRotation(), 100);
    ASSERT_EQ(child_1->getChild("2")->getRotation(), 100);
    ASSERT_EQ(child_1->getChild("3")->getRotation(), 100);
}
//------------------------------------------------------------------------------
TEST(SceneNodeTest, scaleRecursive)
{
    auto SNode = ae::SceneNode::create("SNode");
    auto child_1 = SNode->createChildSceneNode("child_1");

    auto tempChild = child_1->createChildSceneNode("1");
    tempChild = child_1->createChildSceneNode("2");
    tempChild = child_1->createChildSceneNode("3");
        
    SNode->scaleRecursive(ae::Vector2f(0.5, 0.5));

    ASSERT_EQ(child_1->getScale(), ae::Vector2f(0.5, 0.5));
    ASSERT_EQ(child_1->getChild("1")->getScale(), ae::Vector2f(0.5, 0.5));
    ASSERT_EQ(child_1->getChild("2")->getScale(), ae::Vector2f(0.5, 0.5));
    ASSERT_EQ(child_1->getChild("3")->getScale(), ae::Vector2f(0.5, 0.5));
}
//------------------------------------------------------------------------------
