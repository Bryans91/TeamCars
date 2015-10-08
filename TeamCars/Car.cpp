#include <iostream>
#include <Box2D/Box2D.h>

static const float SCALE = 30.f;

class Car{

	b2Body* body;
	b2BodyDef* BodyDef;

	Car(b2World world){	
		this->BodyDef->position = b2Vec2(600 / SCALE, 300 / SCALE);
		this->BodyDef->type = b2_dynamicBody;
		this->body = world.CreateBody(this->BodyDef);
	}


	b2Body* getBody(){
		return this->body;
	}


	void torqueLeft(){
		this->body->ApplyTorque(-1, true);
	}

	void  torqueRight(){
		this->body->ApplyTorque(1, true);
	}

	void force(){
		this->body->ApplyForce(this->body->GetWorldVector(b2Vec2(0, 5)), this->body->GetWorldCenter(), true);
	}
};
