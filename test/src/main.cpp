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
    auto node = layer->getRootSceneNode()->createChildSceneNode("firstNode", 0);
    node->setPositionRecursive(renderWindow->getSize().x / 2,
			       renderWindow->getSize().y / 2);

    ae::SceneManager smgr;
    smgr.addScene(scene);
    
    ae::Texture texture;
    texture.loadFromFile("test_image.png");

    
    std::shared_ptr<ae::Sprite> sprites[1000];
    for(int i = 0; i < 1000; ++i) {
	sprites[i] = std::make_shared<ae::Sprite>(std::to_string(i), texture, true);
	node->attachObject(sprites[i], 0);
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
	    node->move(-0.1, 0);
	}
	if (ae::Keyboard::isKeyPressed(ae::Keyboard::Right)) {
	    node->move(0.1, 0);
	}
	if (ae::Keyboard::isKeyPressed(ae::Keyboard::Up)) {
	    node->move(0, -0.1);
	}
	if (ae::Keyboard::isKeyPressed(ae::Keyboard::Down)) {
	    node->move(0, 0.1);
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

