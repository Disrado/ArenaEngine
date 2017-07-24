#include <SceneNode_Test.hpp>
#include <AE/Graphics.hpp>
#include <memory>
#include <iostream>
#include <string>

bool SceneNode_Test::SceneNodeConstructor_Test()
{
    auto SNode = ae::SceneNode();

    if(SNode.getDrawOrder() == 0
       && SNode.getTag() == ""
       && SNode.getPosition() == ae::Vector2f(0, 0)
       && SNode.getScale() == ae::Vector2f(1, 1)
       && SNode.getOrigin() == SNode.getPosition()
       && SNode.getRotation() == 0.0)
     	return true;
    else {
	std::cerr <<" \n\tIn SceneNode()"
		  << "\n\t\tIncorrect values of default parameters" << std::endl;
	
	return false;
    }
}

bool SceneNode_Test::createChildSceneNode_Test()
{
    auto SNode = std::make_shared<ae::SceneNode>();

    auto childNode = SNode->createChildSceneNode();

    if(!childNode) {
	std::cerr <<" \n\tIn createChildSceneNode()"
		  << "\n\t\tIncorrect return value" << std::endl;
	return false;
    }

    if(childNode->getParent() != SNode) {
	std::cerr <<" \n\tIn createChildSceneNode()"
		  << "\n\t\tIncorrect value of parent in child node" << std::endl;
	return false;
    }

    if(SNode->getChildrenCount() == 0) {
	std::cerr <<" \n\tIn createChildSceneNode()"
		  << "\n\t\tError while child edding as child to parent" << std::endl;
	return false;
    }

    return true;
}

bool SceneNode_Test::addChild_Test()
{
    auto SNode = std::make_shared<ae::SceneNode>();

    auto childNode = SNode->createChildSceneNode();

    SNode->addChild(childNode);

    if(childNode->getParent() != SNode) {
	std::cerr <<" \n\tIn createChildSceneNode()"
		  << "\n\t\tIncorrect value of parent in child node" << std::endl;
	return false;
    }
    
    if(SNode->getChildrenCount() == 0) {
	std::cerr <<" \n\tIn addChild()"
		  << "\n\t\tError while child edding as child to parent" << std::endl;
	return false;
    }

    return true;
}

bool SceneNode_Test::removeChild_Test()
{
    auto SNode = std::make_shared<ae::SceneNode>();
    auto childNode = SNode->createChildSceneNode();
    childNode->setTag("child_1");
    
    SNode->addChild(childNode);
    SNode->removeChild(childNode);

    SNode->addChild(childNode);
    SNode->removeChild("child_1");
    
    if(childNode->getParent() == SNode) {
     	std::cerr << " \n\tIn removeChild()"
     		  << "\n\t\tIncorrect value of parent in child node" << std::endl;
     	return false;
    }
    
    if(SNode->getChildrenCount() != 0) {
	std::cerr <<" \n\tIn removeChild()"
		  << "\n\t\tError while child deleting child from parent" << std::endl;
	return false;
    }
    
    return true;
}

bool SceneNode_Test::removeChildren_Test()
{
    auto SNode = std::make_shared<ae::SceneNode>();

    for(int i(0); i < 10; ++i)
	SNode->createChildSceneNode();

    SNode->removeChildren();

    if(SNode->getChildrenCount() != 0) {
	std::cerr <<" \n\tIn removeChild()"
		  << "\n\t\tError while deleting childredn from parent" << std::endl;
	return false;
    }
    
    return true;
}

bool SceneNode_Test::destroyChildrenRecursive_Test()
{
    auto SNode = std::make_shared<ae::SceneNode>();
    auto child_1_1 = SNode->createChildSceneNode();
    auto child_1_2 = SNode->createChildSceneNode();

    auto child_2_1 = child_1_1->createChildSceneNode();
    auto child_2_2 = child_1_2->createChildSceneNode();

    auto child_3_1 = child_2_1->createChildSceneNode();
    auto child_3_2 = child_2_2->createChildSceneNode();
    
    auto sprite = std::make_shared<ae::Sprite>();
    child_3_2->attachObject(sprite);

    SNode->destroyChildrenRecursive();

    if(SNode->getChildrenCount() != 0 &&
       child_1_1->getChildrenCount() != 0 &&
       child_1_2->getChildrenCount() != 0 &&
       child_2_1->getChildrenCount() != 0 &&
       child_2_2->getChildrenCount() != 0) {
	std::cerr <<" \n\tIn removeChild()"
		  << "\n\t\tError while deleting childredn from parent" << std::endl;
	return false;
    }

    if(child_3_2->detachObject() != nullptr) {
	std::cerr <<" \n\tIn removeChild()"
		  << "\n\t\tError while removing object from node" << std::endl;
	return false;
    }
    
    return true;
}

bool SceneNode_Test::rebaseToNewParent_Test()
{
    auto firstParent = std::make_shared<ae::SceneNode>();
    auto secondParent = std::make_shared<ae::SceneNode>();

    auto childNode = firstParent->createChildSceneNode();
    childNode->setTag("childNode");
    
    firstParent->addChild(childNode);
    
    childNode->rebaseToNewParent(secondParent);

    if(childNode->getParent() == firstParent) {
	std::cerr <<" \n\tIn rebaseToNewParent()"
		  << "\n\t\tFirst parent was not been removed from childNode parent" << std::endl;
	return false;
    }

    if (childNode->getParent() != secondParent) {
	std::cerr <<" \n\tIn rebaseToNewParent()"
		  << "\n\t\tSecondParent was not been set as parent of childNode" << std::endl;
	return false;
    }
    
    if(firstParent->getChildByTag("childNode")) {
	std::cerr <<" \n\tIn rebaseToNewParent()"
		  << "\n\t\tChild node was not been deleted from first parent" << std::endl;
	return false;
    }

    if (!secondParent->getChildByTag("childNode")) {
	std::cerr <<" \n\tIn rebaseToNewParent()"
		  << "\n\t\tChild node has not been added to new parent" << std::endl;
	return false;
    }
    
    return true;
}

bool SceneNode_Test::rebaseChildrenToNewParent_Test()
{
    auto first_SNode = std::make_shared<ae::SceneNode>();
    auto second_SNode = std::make_shared<ae::SceneNode>();
    
    for(int i(0); i < 10; ++i)
	first_SNode->createChildSceneNode();

    first_SNode->rebaseChildrenToNewParent(second_SNode);

    if(first_SNode->getChildrenCount() != 0) {
	std::cerr <<" \n\tIn rebaseChildrenToNewParent()"
		  << "\n\t\tChildren was not been removed from parent " << std::endl;
	return false;
    }

    if (second_SNode->getChildrenCount() == 0) {
	std::cerr <<" \n\tIn rebaseChildrenToNewParent()"
		  << "\n\t\tChildren was not been added to new parent" << std::endl;
	return false;
    }
    
    return true;
}

bool SceneNode_Test::attachObject_Test()
{
    auto SNode = std::make_shared<ae::SceneNode>();
	
    auto sprite = std::make_shared<ae::Sprite>();

    SNode->attachObject(sprite);

    if (SNode->detachObject() != sprite) {
	std::cerr <<" \n\tIn attachObject()"
		  << "\n\t\tObject was not been added" << std::endl;
	return false;
    } 
    
    return true;
}

bool SceneNode_Test::detachObject_Test()
{
    auto SNode = std::make_shared<ae::SceneNode>();
	
    auto sprite = std::make_shared<ae::Sprite>();

    SNode->attachObject(sprite);

    if (SNode->detachObject() != sprite) {
	std::cerr <<" \n\tIn attachObject()"
		  << "\n\t\tObject was not been added" << std::endl;
	return false;
    } 
    
    return true;
}

bool SceneNode_Test::set_get_DrawOrder_Test()
{
    auto SNode = std::make_shared<ae::SceneNode>();
    auto child_1 = SNode->createChildSceneNode();

    child_1->setDrawOrder(4);
    
    if (child_1->getDrawOrder() != 4) {
	std::cerr <<" \n\tIn set_get_DrawOrder()"
		  << "\n\t\tError while change drawOrder" << std::endl;
	return false;
    }    
    
    return true;
}

bool SceneNode_Test::set_get_Tag_Test()
{
    auto SNode = std::make_shared<ae::SceneNode>();
    auto child_1 = SNode->createChildSceneNode();

    child_1->setTag("child");
    
    if (child_1->getTag() != "child") {
	std::cerr <<" \n\tIn set_get_Tag_Test()"
		  << "\n\t\tError while change tag" << std::endl;
	return false;
    }
    
    return true;
}

bool SceneNode_Test::setOriginRecursive_Test()
{
    auto SNode = std::make_shared<ae::SceneNode>();
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
       child_1->getChildByTag("3")->getOrigin() != ae::Vector2f(100, 100)) {
	std::cerr <<" \n\tIn setOriginRecursive()"
		  << "\n\t\tError while set origins to children" << std::endl;
	return false;
    }
        
    return true;
}

bool SceneNode_Test::setScaleRecursive_Test()
{
    auto SNode = std::make_shared<ae::SceneNode>();
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
       child_1->getChildByTag("3")->getScale() != ae::Vector2f(0.5, 0.5)) {
	std::cerr <<" \n\tIn setScaleRecursive()"
		  << "\n\t\tError while set scale to children" << std::endl;
	return false;
    }
            
    return true;
}

bool SceneNode_Test::setPositionRecursive_Test()
{
    auto SNode = std::make_shared<ae::SceneNode>();
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
       child_1->getChildByTag("3")->getPosition() != ae::Vector2f(100, 100)) {
	std::cerr <<" \n\tIn setPositionRecursive()"
		  << "\n\t\tError while set positions to children" << std::endl;
	return false;
    }
    
    return true;
}

bool SceneNode_Test::setRotationRecursive_Test()
{
    auto SNode = std::make_shared<ae::SceneNode>();
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
       child_1->getChildByTag("3")->getRotation() != 100) {
	std::cerr <<" \n\tIn setRotationRecursive()"
		  << "\n\t\tError while set rotation to children" << std::endl;
	return false;
    }

    return true;
}

bool SceneNode_Test::moveRecursive_Test()
{
    auto SNode = std::make_shared<ae::SceneNode>();
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
       child_1->getChildByTag("3")->getPosition() != ae::Vector2f(100, 100)) {
	std::cerr <<" \n\tIn moveRecursive()"
		  << "\n\t\tError while moving children" << std::endl;
	return false;
    }

    return true;
}

bool SceneNode_Test::rotateRecursive_Test()
{
    auto SNode = std::make_shared<ae::SceneNode>();
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
       child_1->getChildByTag("3")->getRotation() != 100) {
	std::cerr <<" \n\tIn rotateRecursive()"
		  << "\n\t\tError while rotating children" << std::endl;
	return false;
    }

    return true;
}

bool SceneNode_Test::scaleRecursive_Test()
{
    auto SNode = std::make_shared<ae::SceneNode>();
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
       child_1->getChildByTag("3")->getScale() != ae::Vector2f(0.5, 0.5)) {
	std::cerr <<" \n\tIn scaleRecursive()"
		  << "\n\t\tError while scaling children" << std::endl;
	return false;
    }
    
    return true;
}
