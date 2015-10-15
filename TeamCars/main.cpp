#include <iostream>

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

static const float SCALE = 30.f;

/** Create the base for the boxes to land */
void CreateGround(b2World& World, float X, float Y);

/** Create the boxes */
void CreateBox(b2World& World, int MouseX, int MouseY);

void CreateBall(b2World& World, int MouseX, int MouseY);

void CreateCar(b2World& World, int X, int Y);


int main()
{
	/** Prepare the window */
	sf::RenderWindow Window(sf::VideoMode(800, 600, 32), "Test");
	Window.setFramerateLimit(60);

	/** Prepare the world */
	b2Vec2 Gravity(0.f, 0.f);
	b2World World(Gravity);
	CreateGround(World, 400.f, 500.f);

	/** Prepare textures */
	sf::Texture GroundTexture;
	sf::Texture BallTexture;
	sf::Texture BoxTexture;
	sf::Texture CarTexture;
	GroundTexture.loadFromFile("ground.png");
	BoxTexture.loadFromFile("box.png");
	BallTexture.loadFromFile("Sprites/Ball/ball.png");
	CarTexture.loadFromFile("Sprites/Car/land_dover_offender.png");



	//movebox
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(600 / SCALE, 300 / SCALE);
	BodyDef.type = b2_dynamicBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((89/2)/SCALE, (217/2)/SCALE);
	b2FixtureDef Car;
	Car.density = 0.015f;
	Car.friction = 1.f;
	Car.shape = &Shape;
	Body->CreateFixture(&Car);

	

	while (Window.isOpen())
	{

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			Body->ApplyTorque(-2, true);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			Body->ApplyTorque(2, true);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			Body->ApplyForce(Body->GetWorldVector(b2Vec2(0, -5)), Body->GetWorldCenter(), true);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			Body->ApplyForce(Body->GetWorldVector(b2Vec2(0, 5)), Body->GetWorldCenter(), true);
		}


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
			if (BodyIterator->GetType() == b2_dynamicBody)
			{
				sf::Sprite Sprite;
				Sprite.setTexture(CarTexture);
				Sprite.setOrigin(89/2, 217/2);
				Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
				Sprite.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
				Window.draw(Sprite);
				++BodyCount;
			}
			else if (BodyIterator->GetType() == b2_kinematicBody)
			{
				sf::Sprite Sprite;
				Sprite.setTexture(BallTexture);
				Sprite.setOrigin(32 / 2, 32 / 2);
				Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
				Sprite.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
				Window.draw(Sprite);
				++BodyCount;
			}
			else
			{
				sf::Sprite GroundSprite;
				GroundSprite.setTexture(GroundTexture);
				GroundSprite.setOrigin(400.f, 8.f);
				GroundSprite.setPosition(BodyIterator->GetPosition().x * SCALE, BodyIterator->GetPosition().y * SCALE);
				GroundSprite.setRotation(180 / b2_pi * BodyIterator->GetAngle());
				Window.draw(GroundSprite);
			}
		}
		Window.display();
	}

	return 0;
}

void CreateBox(b2World& World, int MouseX, int MouseY)
{

	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(MouseX / SCALE, MouseY / SCALE);
	BodyDef.type = b2_kinematicBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((50.f / 2) / SCALE, (32.f / 2) / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}

void CreateBall(b2World& World, int MouseX, int MouseY)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(MouseX / SCALE, MouseY / SCALE);
	BodyDef.type = b2_kinematicBody;
	BodyDef.linearDamping = 0.3;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2CircleShape Shape;
	Shape.m_p.Set(0, 0);
	Shape.m_radius = 1;
	b2FixtureDef FixtureDef;
	FixtureDef.friction = 0.7f;
	FixtureDef.density = 0.005f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}

void CreateGround(b2World& World, float X, float Y)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
	BodyDef.type = b2_staticBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((800.f / 2) / SCALE, (16.f / 2) / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 0.f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}

void CreateCar(b2World& World, int X, int Y){

}