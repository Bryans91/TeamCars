#include <iostream>

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

static const float SCALE = 30.f;

enum {
	TDC_LEFT = 0x1,
	TDC_RIGHT = 0x2,
	TDC_UP = 0x4,
	TDC_DOWN = 0x8
};

b2Body* CarBody;
b2Body* BallBody;
int m_controlState;

/* Create the base for the boxes to land */
void CreateGround(b2World& World, float X, float Y);

/* Create the boxes */
void CreateBall(b2World& World, int MouseX, int MouseY);

b2Vec2 getForwardVelocity();
b2Vec2 getLateralVelocity();
void updateDrive(int controlState);
void updateTurn(int controlState);
void updateFriction();


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

	CreateCar(World, 600, 200);


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

			/*updateFriction();
			updateDrive(m_controlState);
			updateTurn(m_controlState);*/

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
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		//{
		//	m_controlState &= ~TDC_RIGHT;
		//	m_controlState |= TDC_LEFT;
		//	//Body->ApplyForce(b2Vec2(1, 0), Body->GetWorldPoint( b2Vec2(1, 1)), true);
		//}
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		//{
		//	m_controlState &= ~TDC_LEFT;
		//	m_controlState |= TDC_RIGHT;
		//	//Body->ApplyForce(b2Vec2(0, 1), Body->GetWorldPoint(b2Vec2(1, 1)), true);
		//}
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		//{
		//	m_controlState &= ~TDC_DOWN;
		//	m_controlState |= TDC_UP;
		//	//Body->ApplyForce(Body->GetWorldVector(b2Vec2(0, 5)), Body->GetWorldCenter(), true);
		//}
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		//{
		//	m_controlState &= ~TDC_UP;
		//	m_controlState |= TDC_DOWN;
		//	//Body->ApplyForce(Body->GetWorldVector(b2Vec2(0, 5)), Body->GetWorldCenter(), true);
		//}

		//if (m_controlState != 0){
		updateFriction();
		updateDrive(m_controlState);
		updateTurn(m_controlState);
		//}

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
			if (BodyIterator == CarBody)
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
	BallBodyDef.linearDamping = 0.3;
	BallBody = World.CreateBody(&BallBodyDef);

	b2CircleShape CircleShape;
	CircleShape.m_p.Set(0, 0);
	CircleShape.m_radius = (32 / 2) / SCALE;
	b2FixtureDef BallFixtureDef;
	BallFixtureDef.friction = 1.f;
	BallFixtureDef.density = 0.015f;
	BallFixtureDef.shape = &CircleShape;
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

b2Vec2 getForwardVelocity() {
	b2Vec2 currentForwardNormal = CarBody->GetWorldVector(b2Vec2(0, 1));
	return b2Dot(currentForwardNormal, CarBody->GetLinearVelocity()) * currentForwardNormal;
}

b2Vec2 getLateralVelocity() {
	b2Vec2 currentRightNormal = CarBody->GetWorldVector(b2Vec2(1, 0));
	return b2Dot(currentRightNormal, CarBody->GetLinearVelocity()) * currentRightNormal;
}

//tire class variables
float m_maxForwardSpeed = 20;  // 100;
float m_maxBackwardSpeed = -5; // -20;
float m_maxDriveForce = 30;    // 150;

//tire class function
void updateDrive(int controlState) {
	//find desired speed
	float desiredSpeed = 0;
	switch (controlState & (TDC_UP | TDC_DOWN)) {
	case TDC_UP:   desiredSpeed = m_maxForwardSpeed;  break;
	case TDC_DOWN: desiredSpeed = m_maxBackwardSpeed; break;
	default: return;//do nothing
	}

	//find current speed in forward direction
	b2Vec2 currentForwardNormal = CarBody->GetWorldVector(b2Vec2(0, 1));
	float currentSpeed = b2Dot(getForwardVelocity(), currentForwardNormal);

	//apply necessary force
	float force = 0;
	if (desiredSpeed > currentSpeed)
		force = m_maxDriveForce;
	else if (desiredSpeed < currentSpeed)
		force = -m_maxDriveForce;
	else
		return;
	CarBody->ApplyForce(force * currentForwardNormal, CarBody->GetWorldCenter(), true);
}

void updateTurn(int controlState) {
	float desiredTorque = 0;
	switch (controlState & (TDC_LEFT | TDC_RIGHT)) {
	case TDC_LEFT:  desiredTorque = 15;  break;
	case TDC_RIGHT: desiredTorque = -15; break;
	default:;//nothing
	}
	CarBody->ApplyTorque(desiredTorque, true);
}

void updateFriction() {
	//lateral linear velocity
	float maxLateralImpulse = 2.5f;
	b2Vec2 impulse = CarBody->GetMass() * -getLateralVelocity();
	if (impulse.Length() > maxLateralImpulse)
		impulse *= maxLateralImpulse / impulse.Length();
	CarBody->ApplyLinearImpulse(impulse, CarBody->GetWorldCenter(), true);

	//angular velocity
	CarBody->ApplyAngularImpulse(0.1f * CarBody->GetInertia() * -CarBody->GetAngularVelocity(), true);

	//forward linear velocity
	b2Vec2 currentForwardNormal = getForwardVelocity();
	float currentForwardSpeed = currentForwardNormal.Normalize();
	float dragForceMagnitude = -2 * currentForwardSpeed;
	CarBody->ApplyForce(dragForceMagnitude * currentForwardNormal, CarBody->GetWorldCenter(), true);
}

void CreateCar(b2World& World, int X, int Y){
	//movebox
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
	BodyDef.type = b2_dynamicBody;
	CarBody = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((43 / 2) / SCALE, (80 / 2) / SCALE);
	b2FixtureDef Car;
	Car.density = 0.2f;
	Car.friction = 0.7f;
	Car.shape = &Shape;
	CarBody->CreateFixture(&Car);
}