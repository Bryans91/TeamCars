#include <iostream>
#include <string>
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

class View{

public:
	sf::Texture* GroundTexture;
	sf::Texture* BoxTexture;
	sf::RenderWindow* Window;

	View();

	sf::RenderWindow* getWindow();

	sf::Texture getGround();

	sf::Texture getBox();

	void drawObjects(b2World* World, float SCALE);

};