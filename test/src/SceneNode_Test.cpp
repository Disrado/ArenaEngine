#include <SceneNode_Test.hpp>
#include <AE/Graphics.hpp>
#include <memory>
#include <iostream>
#include <string>

void SceneNode_Test::SceneNodeConstructor_Test()
{
    BEGIN_TEST
    {    
	auto SNode = ae::SceneNode::create();

	if(SNode->getDrawOrder() != 0
	   || SNode->getTag() != ""
	   || SNode->getPosition() != ae::Vector2f(0, 0)
	   || SNode->getScale() != ae::Vector2f(1, 1)
	   || SNode->getOrigin() != SNode->getPosition()
	   || SNode->getRotation() != 0.0)
	{
	    ERROR("Incorrect values of default parameters");
	}
    }
    END_TEST
}

void SceneNodeDestructor_Test()
{
    BEGIN_TEST
    {
	ae::SceneNode SNode;

	{
	    auto child = std::make_shared<SceneNode>();
	    SNode->addChild();
	}
	
    }
    END_TEST
}

void SceneNode_Test::createChildSceneNode_Test()
{
    BEGIN_TEST
    {
	ae::SceneNode SNode;
	
	
    // 	auto SNode = ae::SceneNode::create();
	
    // 	auto childNode = SNode->createChildSceneNode();
	
    // 	std::string error;
	
    // 	if(!childNode)
    // 	{
    // 	    error = "Incorrect return value";
    // 	}
    // 	else if(childNode->getParent() != SNode)
    // 	{
    // 	    error = "Incorrect value of parent in child node";
    // 	}
    // 	else if(SNode->getChildrenCount() == 0)
    // 	{
    // 	    error = "Error while child edding as child to parent";
	    
    // 	    ERROR(error);
    // 	}
    }
    END_TEST
}

void SceneNode_Test::addChild_Test()
{
    BEGIN_TEST
    {
	auto SNode = ae::SceneNode::create();
	
	auto childNode = SNode->createChildSceneNode();
	
	SNode->addChild(childNode);
	
	std::string error;
	
	if(childNode->getParent() != SNode)
	{
	    error = "Incorrect value of parent in child node";
	}
	else if(SNode->getChildrenCount() == 0)
	{
	    error = "Error while child edding as child to parent";

	    ERROR(error);
	}
    }
    END_TEST
}

void SceneNode_Test::removeChild_Test()
{
    BEGIN_TEST
    {
	auto SNode = ae::SceneNode::create();
	auto childNode = SNode->createChildSceneNode();
	childNode->setTag("child_1");
	auto childNode2 = childNode->createChildSceneNode();
	childNode->setTag("child_1");
	
	SNode->removeChild(childNode);

	std::string error;
	
	if(childNode->getParent() == SNode)
	{
	    error =  "Incorrect value of parent in child node";
	}
	else if(SNode->getChildrenCount() != 0)
	{
	    error =  "Error while child deleting from parent";
	}
	else if(childNode->getChildrenCount() != 0)
	{
	    error =  "Error while child deleting recursive";
	    ERROR(error);
	}
    }
    END_TEST
}

void SceneNode_Test::removeChildren_Test()
{
    BEGIN_TEST
    {	
	auto SNode = ae::SceneNode::create();
	auto child_1_1 = SNode->createChildSceneNode();
	auto child_1_2 = SNode->createChildSceneNode();
	
	auto child_2_1 = child_1_1->createChildSceneNode();
	auto child_2_2 = child_1_2->createChildSceneNode();
	
	auto child_3_1 = child_2_1->createChildSceneNode();
	auto child_3_2 = child_2_2->createChildSceneNode();
	
	auto sprite = std::make_shared<ae::Sprite>();
	child_3_2->attachObject(sprite);
	
	SNode->removeChildren();
	
	if(SNode->getChildrenCount() != 0 &&
	   child_1_1->getChildrenCount() != 0 &&
	   child_1_2->getChildrenCount() != 0 &&
	   child_2_1->getChildrenCount() != 0 &&
	   child_2_2->getChildrenCount() != 0)
	{
	    ERROR("Error while deleting childredn from parent");
	}
	else if(child_3_2->detachObject() != nullptr)
	{
	    ERROR("Error while removing object from node");
	}
    }	
    END_TEST
}

void SceneNode_Test::rebaseToNewParent_Test()
{
    BEGIN_TEST
    {
	auto firstParent = ae::SceneNode::create();
	auto secondParent = ae::SceneNode::create();
	
	auto childNode = firstParent->createChildSceneNode();
	childNode->setTag("childNode");
	
	firstParent->addChild(childNode);
	
	childNode->rebaseToNewParent(secondParent);
	
	std::string error;
	
	if(childNode->getParent() == firstParent)
	{
	    error = "First parent was not been removed from childNode parent";
	}
	else if (childNode->getParent() != secondParent)
	{
	    error = "SecondParent was not been set as parent of childNode";
	}
	else if(firstParent->getChildByTag("childNode"))
	{
	    error = "Child node was not been deleted from first parent";
	}
	else if (!secondParent->getChildByTag("childNode")) {
	    error = "Child node has not been added to new parent";
	    
	    ERROR(error);
	}
    }
    END_TEST
}

void SceneNode_Test::rebaseChildrenToNewParent_Test()
{
    BEGIN_TEST
    {
	auto first_SNode = ae::SceneNode::create();
	auto second_SNode = ae::SceneNode::create();
	
	for(int i(0); i < 10; ++i)
	    first_SNode->createChildSceneNode();
	
	first_SNode->rebaseChildrenToNewParent(second_SNode);
	
	if(first_SNode->getChildrenCount() != 0)
	{
	    ERROR("Children was not been removed from parent");
	}
	else if (second_SNode->getChildrenCount() == 0)
	{
	    ERROR("Children was not been added to new parent");
	}
    }
    END_TEST
}

void SceneNode_Test::attachObject_Test()
{
    BEGIN_TEST
    {
	auto SNode = std::make_shared<ae::SceneNode>();
	
	auto sprite = std::make_shared<ae::Sprite>();
	
	SNode->attachObject(sprite);
	
	if (SNode->detachObject() != sprite)
	{
	    ERROR("Object was not been added");
	} 
    }
    END_TEST
}

void SceneNode_Test::detachObject_Test()
{
    BEGIN_TEST
    {
	auto SNode = ae::SceneNode::create();
	
	auto sprite = std::make_shared<ae::Sprite>();
	
	SNode->attachObject(sprite);
	
	if (SNode->detachObject() != sprite)
	{
	    ERROR("Object was not been added");
	} 
    }
    END_TEST
}

void SceneNode_Test::set_get_DrawOrder_Test()
{
    BEGIN_TEST
    {	
	auto SNode = ae::SceneNode::create();
	auto child_1 = SNode->createChildSceneNode();
	
	child_1->setDrawOrder(4);
	
	if (child_1->getDrawOrder() != 4)
	{
	    ERROR("Error while change drawOrder");
	}    
    }
    END_TEST
}

void SceneNode_Test::set_get_Tag_Test()
{
    BEGIN_TEST
    {	
	auto SNode = std::make_shared<ae::SceneNode>();
	auto child_1 = SNode->createChildSceneNode();

	child_1->setTag("child");
    
	if (child_1->getTag() != "child")
	{
	    ERROR("Error while change tag");
	}
    }
    END_TEST
}

void SceneNode_Test::setOriginRecursive_Test()
{
    BEGIN_TEST
    {	
	auto SNode = ae::SceneNode::create();
	auto child_1 = SNode->createChildSceneNode();

	auto tempChild = child_1->createChildSceneNode();
	tempChild->setTag("1");
	tempChild = child_1->createChildSceneNode();
	tempChild->setTag("2");
	tempChild = child_1->createChildSceneNode();
	tempChild->setTag("3");

	SNode->setOriginRecursive(ae::Vector2f(100, 100));

	if(child_1->getOrigin() != ae::Vector2f(100, 100) ||
	   child_1->getChildByTag("1")->getOrigin() != ae::Vector2f(100, 100) ||
	   child_1->getChildByTag("2")->getOrigin() != ae::Vector2f(100, 100) ||
	   child_1->getChildByTag("3")->getOrigin() != ae::Vector2f(100, 100))
	{
	    ERROR("Error while set origins to children");
	}
    }        
    END_TEST
}

void SceneNode_Test::setScaleRecursive_Test()
{
    BEGIN_TEST
    {	
	auto SNode = ae::SceneNode::create();
	auto child_1 = SNode->createChildSceneNode();

	auto tempChild = child_1->createChildSceneNode();
	tempChild->setTag("1");
	tempChild = child_1->createChildSceneNode();
	tempChild->setTag("2");
	tempChild = child_1->createChildSceneNode();
	tempChild->setTag("3");

	SNode->setScaleRecursive(ae::Vector2f(0.5, 0.5));

	if(child_1->getScale() != ae::Vector2f(0.5, 0.5) ||
	   child_1->getChildByTag("1")->getScale() != ae::Vector2f(0.5, 0.5) ||
	   child_1->getChildByTag("2")->getScale() != ae::Vector2f(0.5, 0.5) ||
	   child_1->getChildByTag("3")->getScale() != ae::Vector2f(0.5, 0.5))
	{
	    ERROR("Error while set scale to children");
	}
    }
    END_TEST
}

void SceneNode_Test::setPositionRecursive_Test()
{
    BEGIN_TEST
    {	
	auto SNode = ae::SceneNode::create();
	auto child_1 = SNode->createChildSceneNode();

	auto tempChild = child_1->createChildSceneNode();
	tempChild->setTag("1");
	tempChild = child_1->createChildSceneNode();
	tempChild->setTag("2");
	tempChild = child_1->createChildSceneNode();
	tempChild->setTag("3");

	SNode->setPositionRecursive(ae::Vector2f(100, 100));

	if(child_1->getPosition() != ae::Vector2f(100, 100) ||
	   child_1->getChildByTag("1")->getPosition() != ae::Vector2f(100, 100) ||
	   child_1->getChildByTag("2")->getPosition() != ae::Vector2f(100, 100) ||
	   child_1->getChildByTag("3")->getPosition() != ae::Vector2f(100, 100))
	{
	    ERROR("Error while set positions to children");
	}
    }
    END_TEST
}

void SceneNode_Test::setRotationRecursive_Test()
{
    BEGIN_TEST
    {	
	auto SNode = ae::SceneNode::create();
	auto child_1 = SNode->createChildSceneNode();

	auto tempChild = child_1->createChildSceneNode();
	tempChild->setTag("1");
	tempChild = child_1->createChildSceneNode();
	tempChild->setTag("2");
	tempChild = child_1->createChildSceneNode();
	tempChild->setTag("3");

	SNode->setRotationRecursive(100);

	if(child_1->getRotation() != 100 ||
	   child_1->getChildByTag("1")->getRotation() != 100 ||
	   child_1->getChildByTag("2")->getRotation() != 100 ||
	   child_1->getChildByTag("3")->getRotation() != 100)
	{
	    ERROR("Error while set rotation to children");
	}
    }
    END_TEST
}

void SceneNode_Test::moveRecursive_Test()
{
    BEGIN_TEST
    {	
	auto SNode = ae::SceneNode::create();
	auto child_1 = SNode->createChildSceneNode();

	auto tempChild = child_1->createChildSceneNode();
	tempChild->setTag("1");
	tempChild = child_1->createChildSceneNode();
	tempChild->setTag("2");
	tempChild = child_1->createChildSceneNode();
	tempChild->setTag("3");

	SNode->moveRecursive(ae::Vector2f(100, 100));

	if(child_1->getPosition() != ae::Vector2f(100, 100) ||
	   child_1->getChildByTag("1")->getPosition() != ae::Vector2f(100, 100) ||
	   child_1->getChildByTag("2")->getPosition() != ae::Vector2f(100, 100) ||
	   child_1->getChildByTag("3")->getPosition() != ae::Vector2f(100, 100))
	{
	    ERROR("Error while moving children");
	}
    }
    END_TEST
}

void SceneNode_Test::rotateRecursive_Test()
{
    BEGIN_TEST
    {	
	auto SNode = ae::SceneNode::create();
	auto child_1 = SNode->createChildSceneNode();
    
	auto tempChild = child_1->createChildSceneNode();
	tempChild->setTag("1");
	tempChild = child_1->createChildSceneNode();
	tempChild->setTag("2");
	tempChild = child_1->createChildSceneNode();
	tempChild->setTag("3");
    
	SNode->rotateRecursive(100);
    
	if(child_1->getRotation() != 100 ||
	   child_1->getChildByTag("1")->getRotation() != 100 ||
	   child_1->getChildByTag("2")->getRotation() != 100 ||
	   child_1->getChildByTag("3")->getRotation() != 100)
	{
	    ERROR("Error while rotating children");
	}
    }
    END_TEST
}

void SceneNode_Test::scaleRecursive_Test()
{
    BEGIN_TEST
    {	
	auto SNode = ae::SceneNode::create();
	auto child_1 = SNode->createChildSceneNode();

	auto tempChild = child_1->createChildSceneNode();
	tempChild->setTag("1");
	tempChild = child_1->createChildSceneNode();
	tempChild->setTag("2");
	tempChild = child_1->createChildSceneNode();
	tempChild->setTag("3");

	SNode->scaleRecursive(ae::Vector2f(0.5, 0.5));

	if(child_1->getScale() != ae::Vector2f(0.5, 0.5) ||
	   child_1->getChildByTag("1")->getScale() != ae::Vector2f(0.5, 0.5) ||
	   child_1->getChildByTag("2")->getScale() != ae::Vector2f(0.5, 0.5) ||
	   child_1->getChildByTag("3")->getScale() != ae::Vector2f(0.5, 0.5))
	{
	    ERROR("Error while scaling children");
	}
    }
    END_TEST
}

void SceneNode_Test::beginTests()
{
    MESSAGE("SceneNode test block:");    

    SceneNodeConstructor_Test();
    createChildSceneNode_Test();
    addChild_Test();
    removeChild_Test();
    removeChildren_Test();
    rebaseToNewParent_Test();
    rebaseChildrenToNewParent_Test();
    attachObject_Test();
    detachObject_Test();
    set_get_DrawOrder_Test();
    set_get_Tag_Test();
    setOriginRecursive_Test();
    setScaleRecursive_Test();
    setPositionRecursive_Test();
    setRotationRecursive_Test();
    moveRecursive_Test();
    rotateRecursive_Test();
    scaleRecursive_Test();

    MESSAGE("End SceneNode test block"); 
}
