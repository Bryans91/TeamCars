#include <iostream>
#include <string>
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include "View.h"
		
	View::View(){
		GroundTexture->loadFromFile("ground.png");
		BoxTexture->loadFromFile("box.png");
		Window = new sf::RenderWindow(sf::VideoMode(800, 600, 32), "Test");
		Window->setFramerateLimit(60);
	}

	sf::RenderWindow* View::getWindow(){
		return Window;
	}

	sf::Texture View::getGround(){
		return *GroundTexture;
	}

	sf::Texture View::getBox(){
		return *BoxTexture;
	}
	
	void View::drawObjects(b2World* World , float SCALE){
		World->Step(1 / 60.f, 8, 3);

		Window->clear(sf::Color::White);
		int BodyCount = 0;
		for (b2Body* BodyIterator = World->GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
		{
			if (BodyIterator->GetType() == b2_dynamicBody)
			{
				sf::Sprite Sprite;
				Sprite.setTexture(*BoxTexture);
				Sprite.setOrigin(16.f, 16.f);
				Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
				Sprite.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
				Window->draw(Sprite);
				++BodyCount;
			}
			else
			{
				sf::Sprite GroundSprite;
				GroundSprite.setTexture(*GroundTexture);
				GroundSprite.setOrigin(400.f, 8.f);
				GroundSprite.setPosition(BodyIterator->GetPosition().x * SCALE, BodyIterator->GetPosition().y * SCALE);
				GroundSprite.setRotation(180 / b2_pi * BodyIterator->GetAngle());
				Window->draw(GroundSprite);
			}
		}
		Window->display();
	}