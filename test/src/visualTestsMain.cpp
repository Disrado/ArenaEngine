#include <AE/Graphics.hpp>
#include <AE/Window.hpp>
#include <AE/System.hpp>

#include <memory>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>


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

    node[0] = layer->getRootSceneNode()->createChildSceneNode("0", 0);
    node[1] = layer->getRootSceneNode()->createChildSceneNode("1", 0);
    node[2] = layer->getRootSceneNode()->createChildSceneNode("2", 0);
    node[3] = layer->getRootSceneNode()->createChildSceneNode("3", 0);
    
    node[0]->setPosition(node[0]->getParent()->getPosition().x - 100,
			 node[0]->getParent()->getPosition().y - 100);
    node[1]->setPosition(node[1]->getParent()->getPosition().x + 100,
			 node[1]->getParent()->getPosition().y - 100);
    node[2]->setPosition(node[2]->getParent()->getPosition().x - 100,
			 node[2]->getParent()->getPosition().y + 100);
    node[3]->setPosition(node[3]->getParent()->getPosition().x + 100,
			 node[3]->getParent()->getPosition().y + 100);

    node[0]->setDrawOrder(4);
    node[1]->setDrawOrder(1);
    node[2]->setDrawOrder(2);
    node[3]->setDrawOrder(3);

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
            layer->getRootSceneNode()->getChild("3")->rotateRecursive(5);
        }
        if (ae::Keyboard::isKeyPressed(ae::Keyboard::K)) {
	    layer->getRootSceneNode()->destroyChild(node[std::rand() % 4]);
        }
        
        renderWindow->clear();
        smgr.updateCurrentScene();
        smgr.drawCurrentScene(renderWindow);
	renderWindow->display();
        
	currentTime = clock.restart().asSeconds();
	fps = 1.f / lastTime;
	lastTime = currentTime;

        std:: cout << fps << std::endl;
    }
}
