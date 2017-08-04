#include <SceneNode_Test.hpp>
#include <AE/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>

int main()
{
    /* Unit tests
    std::vector<Test*> testClasses =
    {
	new SceneNode_Test()
	//...
    };

    for(auto itr : testClasses)
	itr->beginTests();

    for(auto &itr : testClasses)
	delete itr;
    */

    // Visual test

    auto renderWindow = std::shared_ptr<ae::RenderWindow>( new ae::RenderWindow({720, 480},
										"AE_Test"));
//    renderWindow->setSize(renderWindow->getSize());

    
    
    auto scene = std::make_shared<ae::Scene>();
    auto layer = scene->createLayer(0, "firstLayer");
    std::shared_ptr<ae::SceneNode> node[4];

    layer->getRootSceneNode()->setPosition(renderWindow->getSize().x / 2,
					   renderWindow->getSize().y / 2);

    
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

    
	    
    ae::SceneManager smgr;
    smgr.addScene(scene);
    
    ae::Texture texture;
    texture.loadFromFile("test_image.png");

    
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
	    layer->getRootSceneNode()->moveRecursive(-0.1, 0);
	}
	if (ae::Keyboard::isKeyPressed(ae::Keyboard::Right)) {
	    layer->getRootSceneNode()->moveRecursive(0.1, 0);
	}
	if (ae::Keyboard::isKeyPressed(ae::Keyboard::Up)) {
	    layer->getRootSceneNode()->moveRecursive(0, -0.1);
	}
	if (ae::Keyboard::isKeyPressed(ae::Keyboard::Down)) {
	    layer->getRootSceneNode()->moveRecursive(0, 0.1);
	} 
	
        renderWindow->clear();
        smgr.drawCurrentScene(renderWindow);
	renderWindow->display();

	currentTime = clock.restart().asSeconds();
	fps = 1.f / lastTime;
	lastTime = currentTime;

	std:: cout << fps << std::endl;
    }
}

