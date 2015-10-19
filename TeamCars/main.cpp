#include <iostream>

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

#include "Car.h"

static const float SCALE = 30.f;

enum {
	TDC_LEFT = 0x1,
	TDC_RIGHT = 0x2,
	TDC_UP = 0x4,
	TDC_DOWN = 0x8
};

Car* car;
//b2Body* CarBody;
b2Body* BallBody;
int m_controlState;

/* Create the base for the boxes to land */
void CreateGround(b2World& World, float X, float Y);

/* Create the boxes */
void CreateBall(b2World& World, int MouseX, int MouseY);

//b2Vec2 getForwardVelocity();
//b2Vec2 getLateralVelocity();
//void updateDrive(int controlState);
//void updateTurn(int controlState);
//void updateFriction();


void CreateCar(b2World& World, int X, int Y);

int main()
{
	m_controlState = 0;
	/* Prepare the window */
	sf::RenderWindow Window(sf::VideoMode(1200, 800, 32), "Test");
	Window.setFramerateLimit(60);

	/* Prepare the world */
	b2Vec2 Gravity(0.f, 0.f);
	b2World World(Gravity);
	CreateGround(World, 400.f, 700.f);


	/** Prepare textures */
	sf::Texture GroundTexture;
	sf::Texture BallTexture;
	sf::Texture BoxTexture;
	sf::Texture CarTexture;
	GroundTexture.loadFromFile("ground.png");
	BoxTexture.loadFromFile("box.png");
	BallTexture.loadFromFile("Sprites/Ball/ball.png");
	CarTexture.loadFromFile("Sprites/Car/bord_gt45_small.png");

	car = new Car(&World);
	//CreateCar(World, 600, 200);
	

	sf::Event event;

	while (Window.isOpen())
	{
		if (Window.pollEvent(event)){
			if (event.type == sf::Event::KeyPressed){
				switch (event.key.code){
				case sf::Keyboard::Left:
					m_controlState |= TDC_RIGHT;
					break;
				case sf::Keyboard::Right:
					m_controlState |= TDC_LEFT;
					break;
				case sf::Keyboard::Up:
					m_controlState |= TDC_UP;
					break;
				case sf::Keyboard::Down:
					m_controlState |= TDC_DOWN;
					break;
				}
			}

			/*car->updateFriction();
			car->updateDrive(m_controlState);
			car->updateTurn(m_controlState);*/

			if (event.type == sf::Event::KeyReleased){
				switch (event.key.code){
				case sf::Keyboard::Left:
					m_controlState &= ~TDC_RIGHT;
					break;
				case sf::Keyboard::Right:
					m_controlState &= ~TDC_LEFT;
					break;
				case sf::Keyboard::Up:
					m_controlState &= ~TDC_UP;
					break;
				case sf::Keyboard::Down:
					m_controlState &= ~TDC_DOWN;
					break;
				}
			}
		}

		car->updateFriction();
		car->updateDrive(m_controlState);
		car->updateTurn(m_controlState);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			int MouseX = sf::Mouse::getPosition(Window).x;
			int MouseY = sf::Mouse::getPosition(Window).y;

			CreateBall(World, MouseX, MouseY);
		}

		World.Step(1 / 60.f, 8, 3);

		Window.clear(sf::Color::White);
		int BodyCount = 0;
		for (b2Body* BodyIterator = World.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
		{
			if (BodyIterator == car->getCarBody())
			{
				sf::Sprite Sprite;
				Sprite.setTexture(CarTexture);
				Sprite.setOrigin(43 / 2, 80 / 2);
				Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
				Sprite.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
				Window.draw(Sprite);
				++BodyCount;
			}
			else if (BodyIterator == BallBody)
			{
				sf::Sprite Sprite;
				Sprite.setTexture(BallTexture);
				Sprite.setOrigin(32 / 2, 32 / 2);
				Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
				Sprite.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
				Window.draw(Sprite);
				++BodyCount;
			}
			else if (BodyIterator->GetType() == b2_staticBody)
			{
				sf::Sprite GroundSprite;
				GroundSprite.setTexture(GroundTexture);
				GroundSprite.setOrigin(800.f, 8.f);
				GroundSprite.setPosition(BodyIterator->GetPosition().x * SCALE, BodyIterator->GetPosition().y * SCALE);
				GroundSprite.setRotation(180 / b2_pi * BodyIterator->GetAngle());
				Window.draw(GroundSprite);
			}
		}
		Window.display();
	}

	return 0;
}

void CreateBall(b2World& World, int MouseX, int MouseY)
{
	b2BodyDef BallBodyDef;
	BallBodyDef.position = b2Vec2(MouseX / SCALE, MouseY / SCALE);
	BallBodyDef.type = b2_dynamicBody;
	BallBodyDef.linearDamping = 0.3F;
	BallBody = World.CreateBody(&BallBodyDef);

	b2CircleShape CircleShape;
	CircleShape.m_p.Set(0, 0);
	CircleShape.m_radius = (32 / 2) / SCALE;
	b2FixtureDef BallFixtureDef;
	BallFixtureDef.friction = 1.f;
	BallFixtureDef.density = 0.015f;
	BallFixtureDef.shape = &CircleShape;
	BallFixtureDef.restitution = 0.5f;
	BallBody->CreateFixture(&BallFixtureDef);
}



void CreateGround(b2World& World, float X, float Y)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
	BodyDef.type = b2_staticBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((1600.f / 2) / SCALE, (16.f / 2) / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 0.f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}



//void CreateCar(b2World& World, int X, int Y){
//	//movebox
//	b2BodyDef BodyDef;
//	BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
//	BodyDef.type = b2_dynamicBody;
//	CarBody = World.CreateBody(&BodyDef);
//
//	b2PolygonShape Shape;
//	Shape.SetAsBox((43 / 2) / SCALE, (80 / 2) / SCALE);
//	b2FixtureDef Car;
//	Car.density = 0.2f;
//	Car.friction = 0.7f;
//	Car.shape = &Shape;
//	CarBody->CreateFixture(&Car);
//}