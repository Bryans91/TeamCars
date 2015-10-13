#include <iostream>
#include <Box2D/Box2D.h>
#include "Car.h"




	Car::Car(b2World world){	
		const float SCALE = 30.f;
		BodyDef->position = b2Vec2(600 / SCALE, 300 / SCALE);
		BodyDef->type = b2_dynamicBody;
		body = world.CreateBody(this->BodyDef);
	}


	b2Body* Car::getBody(){
		return body;
	}


	void Car::torqueLeft(){
		body->ApplyTorque(-1, true);
	}

	void Car::torqueRight(){
		body->ApplyTorque(1, true);
	}

	void Car::force(){
		body->ApplyForce(this->body->GetWorldVector(b2Vec2(0, 5)), this->body->GetWorldCenter(), true);
	}
