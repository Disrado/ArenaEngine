// #include <SceneNode_Test.cpp>
// #include <Object_Test.cpp>

/*
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
*/

#include <AE/Graphics.hpp>
#include <AE/Window.hpp>
#include <AE/System.hpp>
#include <memory>
#include <iostream>
#include <string>
#include <cstdlib>

int main()
{
    auto renderWindow =
        std::shared_ptr<ae::RenderWindow>( new ae::RenderWindow({1280, 720},
                                                                "AE_Test"));
    renderWindow->setSize(renderWindow->getSize());

    
    
    auto scene = std::make_shared<ae::Scene>();
    auto layer = scene->createLayer(0, "firstLayer");
    std::shared_ptr<ae::SceneNode> node[4];

    layer->getRootSceneNode()->setPosition(renderWindow->getSize().x / 2 - 200,
					   renderWindow->getSize().y / 2 - 200);

    
    for(int i = 0; i < 4; ++i) {
	node[i] = layer->getRootSceneNode()->createChildSceneNode(std::to_string(i), 0);
	node[i]->setDrawOrder(4 - i);
    }

    node[0]->setPosition(node[0]->getParent()->getPosition().x - 100,
			 node[0]->getParent()->getPosition().y - 100);
    node[1]->setPosition(node[1]->getParent()->getPosition().x + 100,
			 node[1]->getParent()->getPosition().y - 100);
    node[2]->setPosition(node[2]->getParent()->getPosition().x - 100,
			 node[2]->getParent()->getPosition().y + 100);
    node[3]->setPosition(node[3]->getParent()->getPosition().x + 100,
			 node[3]->getParent()->getPosition().y + 100);
    node[3]->setDrawOrder(100);

    ae::SceneManager smgr;
    smgr.addScene(scene);
    
    ae::Texture texture;
    texture.loadFromFile("test.png");

    
    std::shared_ptr<ae::Sprite> sprites[4];
    for(int i = 0; i < 4; ++i) {
	sprites[i] = std::make_shared<ae::Sprite>(std::to_string(i), texture, true);
	node[i]->attachObject(sprites[i], 0);
    }

    float currentTime;
    float fps;
    ae::Clock clock;
    float lastTime = 0;

    ae::Event event;
    while (renderWindow->isOpen()) {
        while (renderWindow->pollEvent (event)) {
            if (event.type == ae::Event::Closed)
                renderWindow->close();
        }

	if (ae::Keyboard::isKeyPressed(ae::Keyboard::Left)) {
	    layer->getRootSceneNode()->moveRecursive(-4, 0);
	}
	if (ae::Keyboard::isKeyPressed(ae::Keyboard::Right)) {
	    layer->getRootSceneNode()->moveRecursive(4, 0);
	}
	if (ae::Keyboard::isKeyPressed(ae::Keyboard::Up)) {
	    layer->getRootSceneNode()->moveRecursive(0, -4);
	}
	if (ae::Keyboard::isKeyPressed(ae::Keyboard::Down)) {
	    layer->getRootSceneNode()->moveRecursive(0, 4);
	} 
	if (ae::Keyboard::isKeyPressed(ae::Keyboard::S)) {
	    layer->getRootSceneNode()->scaleRecursive(1.1, 1.1);
	}
        if (ae::Keyboard::isKeyPressed(ae::Keyboard::U)) {
	    layer->getRootSceneNode()->scaleRecursive(-1.1, -1.1);
	}
        if (ae::Keyboard::isKeyPressed(ae::Keyboard::R)) {
	    layer->getRootSceneNode()->rotateRecursive(5);
	}
        if (ae::Keyboard::isKeyPressed(ae::Keyboard::K)) {
	    layer->getRootSceneNode()->destroyChild(node[std::rand() % 4]);
        }

        //std::cout << layer->getSceneNode("0")->getName() << std::endl;
        //std::cout << layer->getSceneNode("1")->getName() << std::endl;
        //std::cout << layer->getSceneNode("2")->getName() << std::endl;
        //std::cout << layer->getSceneNode("3")->getName() << std::endl;
        //std::cout << layer->getSceneNode("RootSceneNode")->getName() << std::endl;
        
        renderWindow->clear();
        smgr.updateCurrentScene();
        smgr.drawCurrentScene(renderWindow);
	renderWindow->display();

	currentTime = clock.restart().asSeconds();
	fps = 1.f / lastTime;
	lastTime = currentTime;

        //        std:: cout << fps << std::endl;
    }
}

