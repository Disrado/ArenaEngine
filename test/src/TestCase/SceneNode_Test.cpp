#include <AE/Graphics.hpp>
#include <memory>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

TEST(SceneNodeTest, createChildSceneNode)
{
    
}
/*
void SceneNode_Test::createChildSceneNode_Test()
{
        auto SNode = ae::SceneNode::create("SNode");

        auto childNode = SNode->createChildSceneNode("childNode");
	
        if(!childNode)
            ERROR("Incorrect return value");

        if(childNode->getParent() != SNode)
            ERROR("Incorrect value of parent in child node");

        if(SNode->getChildrenCount() == 0)
            ERROR("Error while child edding as child to parent");
}
    
void SceneNode_Test::addChild_Test()
{
    BEGIN_TEST
    {
 	auto SNode = ae::SceneNode::create("SNode");
	auto childNode = ae::SceneNode::create("childNode");

        SNode->addChild(childNode);
	
 	if(childNode->getParent() != SNode)
 	    ERROR("Incorrect value of parent in child node");

        if(SNode->getChildrenCount() == 0)
 	    ERROR("Error while child edding as child to parent");
    }
    END_TEST
}

void SceneNode_Test::removeChild_Test()
{
    BEGIN_TEST
    {
        auto SNode = ae::SceneNode::create("SNode");
        auto childNode = SNode->createChildSceneNode("child_1");
	
        SNode->removeChild(childNode);
	
        if(childNode->getParent() == SNode)
            ERROR("Parent in child was not removed");

        if(SNode->getChildrenCount() != 0)
            ERROR("Error while children deleting from parent");
    }
    END_TEST
}

void SceneNode_Test::removeChildren_Test()
{
    BEGIN_TEST
    {	
        auto SNode = ae::SceneNode::create("SNode");
        auto child_1 = SNode->createChildSceneNode("child_1");
        auto child_2 = SNode->createChildSceneNode("child_2");

        SNode->removeChildren();
        
        if(child_1->getParent() == SNode)
            ERROR("Parent in child node was not removed");

        if(SNode->getChildrenCount() != 0)
            ERROR("Error while children deleting from parent");
    }	
    END_TEST
}

void SceneNode_Test::destroyChild_Test()
{
    BEGIN_TEST
    {
        auto SNode = ae::SceneNode::create("SNode");
        auto childNode = SNode->createChildSceneNode("child_1");
        auto childChildNode = childNode->createChildSceneNode("child_1");
        auto sprite = std::make_shared<ae::Sprite>("sprite");
	childNode->attachObject(sprite);

        SNode->destroyChild(childNode);
	
        if(childNode->getParent() == SNode)
            ERROR("Parent in child was not removed");

        if(childChildNode->getParent() == childNode)
            ERROR("Parent was not deleted from child of child");

        if(SNode->getChildrenCount() != 0)
            ERROR("Children was not removed from parent");

        if(sprite->isAttached())
            ERROR("Object was not been detached");
    }
    END_TEST
}

void SceneNode_Test::destroyChildren_Test()
{
    BEGIN_TEST
    {
        auto SNode = ae::SceneNode::create("SNode");
        auto childNode = SNode->createChildSceneNode("child_1");
        auto childChildNode = childNode->createChildSceneNode("child_1");
        auto sprite = std::make_shared<ae::Sprite>("sprite");
	childNode->attachObject(sprite);

        SNode->destroyChild(childNode);
	
        if(childNode->getParent() == SNode)
            ERROR("Parent in child was not removed");

        if(childChildNode->getParent() == childNode)
            ERROR("Parent was not deleted from child of child");

        if(SNode->getChildrenCount() != 0)
            ERROR("Children was not removed from parent");

        if(sprite->isAttached())
            ERROR("Object was not been detached from");
    }
    END_TEST    
}

void SceneNode_Test::rebaseToNewParent_Test()
{
    BEGIN_TEST
    {
        auto firstParent = ae::SceneNode::create("firstParent");
        auto secondParent = ae::SceneNode::create("secondParent");
	
        auto childNode = firstParent->createChildSceneNode("childNode");
	
        childNode->rebaseToNewParent(secondParent);
	
        if(childNode->getParent() == firstParent)
            ERROR("First parent was not been removed from childNode parent");

        if (childNode->getParent() != secondParent)
            ERROR("SecondParent was not been set as parent of childNode");
    
        if(firstParent->getChild("childNode"))
            ERROR("Child node was not been deleted from first parent");
    
        if (!secondParent->getChild("childNode"))
            ERROR("Child node has not been added to new parent");
    }
    END_TEST
}

void SceneNode_Test::rebaseChildrenToNewParent_Test()
{
    BEGIN_TEST
    {
        auto firstParent = ae::SceneNode::create("firstParent");
        auto secondParent = ae::SceneNode::create("secondParent");
	
        for(int i(0); i < 10; ++i)
            firstParent->createChildSceneNode(std::to_string(i));
	
        firstParent->rebaseChildrenToNewParent(secondParent);
	
        if(firstParent->getChildrenCount() != 0)
            ERROR("Children was not been removed from parent");

        if(secondParent->getChildrenCount() == 0)
            ERROR("Children was not been added to new parent");
    }
    END_TEST
}

void SceneNode_Test::getDescendantCount_Test()
{
    BEGIN_TEST
    {
        auto parent = ae::SceneNode::create("parent");
        auto child = parent->createChildSceneNode("child1");
        auto childChild = child->createChildSceneNode("child2");
        auto childChildChild = childChild->createChildSceneNode("child3");
	
        if(parent->getDescendantCount()  != 3)
            ERROR("Incorect descendant count");
    }
    END_TEST    
}
    
void SceneNode_Test::setVisibleRecursive_Test()
{
    BEGIN_TEST
    {
        auto parent = ae::SceneNode::create("parent");
        auto child = parent->createChildSceneNode("child1");
        auto childChild = child->createChildSceneNode("child2");
        auto childChildChild = childChild->createChildSceneNode("child3");
	
        parent->setVisibleRecursive(false);
        
        if(childChildChild->isVisible())
            ERROR("Visible was not been set recirsive");
    }
    END_TEST    
}

void SceneNode_Test::attachObject_Test()
{
    BEGIN_TEST
    {
        auto SNode = ae::SceneNode::create("SNode");

        auto sprite = std::make_shared<ae::Sprite>("sprite");

        SNode->attachObject(sprite);
        if (SNode->numAttachedObjects() == 0)
            ERROR("Object was not been attached");
    }
    END_TEST
}

void SceneNode_Test::detachObject_Test()
{
    BEGIN_TEST
    {
        auto SNode = ae::SceneNode::create("SNode");

        auto sprite = std::make_shared<ae::Sprite>("sprite");

        SNode->detachObject(sprite);

        if (SNode->numAttachedObjects() != 0)
            ERROR("Object was not been detached");
    }
    END_TEST
}
    
void SceneNode_Test::detachAllObjects_Test()
{
    BEGIN_TEST
    {
        auto SNode = ae::SceneNode::create("SNode");

        for(int i(0); i < 10; ++i) {
            auto sprite = std::make_shared<ae::Sprite>(std::to_string(i));
            SNode->attachObject(sprite);
	}

        SNode->detachAllObjects();
        
        if (SNode->numAttachedObjects() != 0)
            ERROR("Objects was not been detached");
    }
    END_TEST    
}

void SceneNode_Test::getAttachedObject_Test()
{
    BEGIN_TEST
    {
        auto SNode = ae::SceneNode::create("SNode");

        auto sprite = std::make_shared<ae::Sprite>("sprite");


        auto obj = SNode->getAttachedObject("sprite");

        if (obj != sprite)
            ERROR("Incorect value of returned object");
    }
    END_TEST    
}

void SceneNode_Test::getDrawableObjects_Test()
{
    BEGIN_TEST
    {
        auto SNode = ae::SceneNode::create("SNode");

        for(int i(0); i < 10; ++i) {
            auto sprite = std::make_shared<ae::Sprite>(std::to_string(i));
            SNode->attachObject(sprite);
            //            if(i < 5)
            //                sprite->setVisible(false);
	}
        
        if (SNode->getDrawableObjects().size() != 5)
            ERROR("Incorect size of returned vector");
    }
    END_TEST        
}

void SceneNode_Test::getDrawableChildren_Test()
{
    BEGIN_TEST
    {
        auto SNode = ae::SceneNode::create("SNode");

        for(int i(0); i < 10; ++i) {
            auto node = SNode->createChildSceneNode(std::to_string(i));
            if(i < 5)
                node->setVisible(false);
	}
        
        if (SNode->getDrawableChildren().size() != 5)
            ERROR("Incorect size of returned vector");
    }
    END_TEST        
}
void SceneNode_Test::setDrawOrder_Test()
{
    BEGIN_TEST
    {	
        auto SNode = ae::SceneNode::create("SNode");
        auto child1 = SNode->createChildSceneNode("child1", 5);
        auto child2 = SNode->createChildSceneNode("child2", 10);
            
        child2->setDrawOrder(2);
	
        if (child2->getDrawOrder() != 2)
            ERROR("New value of draw order was't been set");

        if(child1->getParent()->getDrawableChildren()[0]->getDrawOrder() != 2)
            ERROR("Parent does't sort children after they change his draw order");
    }
    END_TEST
}

void SceneNode_Test::changeObjectDrawOrder_Test()
{
    BEGIN_TEST
    {	
        auto SNode = ae::SceneNode::create("SNode");
        auto sprite1 = std::make_shared<ae::Sprite>("sprite1");
        auto sprite2 = std::make_shared<ae::Sprite>("sprite2");

        SNode->attachObject(sprite1, 1);
        SNode->attachObject(sprite2, 2);

        SNode->changeObjectDrawOrder(sprite2, 0);
        
        if (sprite2->getDrawOrder() != 0)
            ERROR("New value of draw order was't been set");

        if(SNode->getDrawableObjects()[0]->getDrawOrder() != 2)
            ERROR("Parent does't sort objects after they change his draw order");
    }
    END_TEST    
}

void SceneNode_Test::setOriginRecursive_Test()
{
    BEGIN_TEST
    {	
        auto SNode = ae::SceneNode::create("SNode");
        auto child_1 = SNode->createChildSceneNode("child_1");

        child_1->createChildSceneNode("1");
        child_1->createChildSceneNode("2");
        child_1->createChildSceneNode("3");

        SNode->setOriginRecursive(ae::Vector2f(100, 100));

        if(child_1->getOrigin() != ae::Vector2f(100, 100) ||
           child_1->getChild("1")->getOrigin() != ae::Vector2f(100, 100) ||
           child_1->getChild("2")->getOrigin() != ae::Vector2f(100, 100) ||
           child_1->getChild("3")->getOrigin() != ae::Vector2f(100, 100))
            ERROR("Error while set origins to children");
    }        
    END_TEST
}

void SceneNode_Test::setScaleRecursive_Test()
{
    BEGIN_TEST
    {	
        auto SNode = ae::SceneNode::create("SNode");
        auto child_1 = SNode->createChildSceneNode("child_1");

        child_1->createChildSceneNode("1");
        child_1->createChildSceneNode("2");
        child_1->createChildSceneNode("3");

        SNode->setScaleRecursive(ae::Vector2f(0.5, 0.5));

        if(child_1->getScale() != ae::Vector2f(0.5, 0.5) ||
           child_1->getChild("1")->getScale() != ae::Vector2f(0.5, 0.5) ||
           child_1->getChild("2")->getScale() != ae::Vector2f(0.5, 0.5) ||
           child_1->getChild("3")->getScale() != ae::Vector2f(0.5, 0.5))
            ERROR("Error while set scale to children");
    }
    END_TEST
}

void SceneNode_Test::setPositionRecursive_Test()
{
    BEGIN_TEST
    {	
        auto SNode = ae::SceneNode::create("SNode");
        auto child_1 = SNode->createChildSceneNode("child_1");

        child_1->createChildSceneNode("1");
        child_1->createChildSceneNode("2");
        child_1->createChildSceneNode("3");

        SNode->setPositionRecursive(ae::Vector2f(100, 100));

        if(child_1->getPosition() != ae::Vector2f(100, 100) ||
           child_1->getChild("1")->getPosition() != ae::Vector2f(100, 100) ||
           child_1->getChild("2")->getPosition() != ae::Vector2f(100, 100) ||
           child_1->getChild("3")->getPosition() != ae::Vector2f(100, 100))
            ERROR("Error while set positions to children");
        
    }
    END_TEST
}
 
void SceneNode_Test::setRotationRecursive_Test()
{
    BEGIN_TEST
    {	
        auto SNode = ae::SceneNode::create("SNode");
        auto child_1 = SNode->createChildSceneNode("child_1");

        child_1->createChildSceneNode("1");
        child_1->createChildSceneNode("2");
        child_1->createChildSceneNode("3");

        SNode->setRotationRecursive(100);

        if(child_1->getRotation() != 100 ||
           child_1->getChild("1")->getRotation() != 100 ||
           child_1->getChild("2")->getRotation() != 100 ||
           child_1->getChild("3")->getRotation() != 100)
            ERROR("Error while set rotation to children");
    }
    END_TEST
}

void SceneNode_Test::moveRecursive_Test()
{
    BEGIN_TEST
    {	
        auto SNode = ae::SceneNode::create("SNode");
        auto child_1 = SNode->createChildSceneNode("child_1");

        auto tempChild = child_1->createChildSceneNode("1");
        tempChild = child_1->createChildSceneNode("2");
        tempChild = child_1->createChildSceneNode("3");

        SNode->moveRecursive(ae::Vector2f(100, 100));

        if(child_1->getPosition() != ae::Vector2f(100, 100) ||
           child_1->getChild("1")->getPosition() != ae::Vector2f(100, 100) ||
           child_1->getChild("2")->getPosition() != ae::Vector2f(100, 100) ||
           child_1->getChild("3")->getPosition() != ae::Vector2f(100, 100))
            ERROR("Error while moving children");
    }
    END_TEST
}

void SceneNode_Test::rotateRecursive_Test()
{
    BEGIN_TEST
    {	
        auto SNode = ae::SceneNode::create("SNode");
        auto child_1 = SNode->createChildSceneNode("child_1");
    
        auto tempChild = child_1->createChildSceneNode("1");
        tempChild = child_1->createChildSceneNode("2");
        tempChild = child_1->createChildSceneNode("3");
    
        SNode->rotateRecursive(100);
    
        if(child_1->getRotation() != 100 ||
           child_1->getChild("1")->getRotation() != 100 ||
           child_1->getChild("2")->getRotation() != 100 ||
           child_1->getChild("3")->getRotation() != 100)
            ERROR("Error while rotating children");
    }
    END_TEST
}

void SceneNode_Test::scaleRecursive_Test()
{
    BEGIN_TEST
    {	
        auto SNode = ae::SceneNode::create("SNode");
        auto child_1 = SNode->createChildSceneNode("child_1");

        auto tempChild = child_1->createChildSceneNode("1");
        tempChild = child_1->createChildSceneNode("2");
        tempChild = child_1->createChildSceneNode("3");
        
        SNode->scaleRecursive(ae::Vector2f(0.5, 0.5));

        if(child_1->getScale() != ae::Vector2f(0.5, 0.5) ||
           child_1->getChild("1")->getScale() != ae::Vector2f(0.5, 0.5) ||
           child_1->getChild("2")->getScale() != ae::Vector2f(0.5, 0.5) ||
           child_1->getChild("3")->getScale() != ae::Vector2f(0.5, 0.5))
            ERROR("Error while scaling children");
    }
    END_TEST
}
 
void SceneNode_Test::beginTests()
{
    MESSAGE("SceneNode test block:");
    {
        createChildSceneNode_Test();
        addChild_Test();
        removeChild_Test();
        removeChildren_Test();
        destroyChild_Test();
        destroyChildren_Test();
        rebaseToNewParent_Test();
        rebaseChildrenToNewParent_Test();
        getDescendantCount_Test();
        setVisibleRecursive_Test();
        attachObject_Test();
        detachObject_Test();
        detachAllObjects_Test();
        getAttachedObject_Test();
        getDrawableObjects_Test();
        getDrawableChildren_Test();
        setDrawOrder_Test();
        changeObjectDrawOrder_Test();
        setOriginRecursive_Test();
        setScaleRecursive_Test();
        setPositionRecursive_Test();
        setRotationRecursive_Test();
        moveRecursive_Test();
        rotateRecursive_Test();
        scaleRecursive_Test();
    }
    MESSAGE("End SceneNode test block"); 
}
*/
