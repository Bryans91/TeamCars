#include <iostream>
#include <Box2D/Box2D.h>

class Car{
private:
	b2Body* body;
	b2BodyDef* BodyDef;

public:
	Car(b2World world);

	b2Body* getBody();

	void torqueLeft();

	void  torqueRight();

	void force();
};