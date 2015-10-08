#include <iostream>

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

static const float SCALE = 30.f;

class GameController{

	View* view;
	Car* car;
	Level* level;

	GameController(){
		this->view = new View();
		this->level = new Level();
		this->car = new Car(this->level->getWorld());
		gameLoop(this->level->getWorld(), this->view->getWindow(), this->car->getBody());


	}

	~GameController(){
		//DESTRUCTOR	
	}


	void gameLoop(b2World World , sf::RenderWindow Window,b2Body* Body){
		while (Window.isOpen())
		{

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				this->car->torqueLeft();
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				this->car->torqueRight();
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				this->car->force();
			}


			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				int MouseX = sf::Mouse::getPosition(Window).x;
				int MouseY = sf::Mouse::getPosition(Window).y;

				CreateBox(World, MouseX, MouseY);
			}
			

			this->view->drawObjects(World,SCALE);
		}
	}



	void CreateBox(b2World& World, int MouseX, int MouseY)
	{
		b2BodyDef BodyDef;
		BodyDef.position = b2Vec2(MouseX / SCALE, MouseY / SCALE);
		BodyDef.type = b2_dynamicBody;
		b2Body* Body = World.CreateBody(&BodyDef);

		b2PolygonShape Shape;
		Shape.SetAsBox((50.f / 2) / SCALE, (32.f / 2) / SCALE);
		b2FixtureDef FixtureDef;
		FixtureDef.density = 1.f;
		FixtureDef.friction = 0.7f;
		FixtureDef.shape = &Shape;
		Body->CreateFixture(&FixtureDef);
	}

};