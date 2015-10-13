#include <iostream>
#include "Car.h"
#include "Level.h"
#include "View.h"
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>



class GameController{

private:
	View* view;
	Car* car;
	Level* level;
    float SCALE;


public:
	
	GameController();

	void gameLoop(b2World* World, sf::RenderWindow* Window, b2Body* Body);

	void CreateBox(b2World* World, int MouseX, int MouseY);

};