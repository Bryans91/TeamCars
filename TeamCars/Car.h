#pragma once
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

class Car {
private:
	b2Body* CarBody;
	enum {
		TDC_LEFT = 0x1,
		TDC_RIGHT = 0x2,
		TDC_UP = 0x4,
		TDC_DOWN = 0x8
	};
public:
	Car(b2World* world);
	b2Vec2 getForwardVelocity();
	b2Vec2 getLateralVelocity();
	void updateDrive(int controlState);
	void updateTurn(int controlState);
	void updateFriction();
};