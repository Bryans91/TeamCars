#include "Car.h"

Car::Car(b2World* world) {
	//create car body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	CarBody = world->CreateBody(&bodyDef);

	b2Vec2 vertices[8];
	vertices[0].Set(1.5, 0);
	vertices[1].Set(3, 2.5);
	vertices[2].Set(2.8, 5.5);
	vertices[3].Set(1, 10);
	vertices[4].Set(-1, 10);
	vertices[5].Set(-2.8, 5.5);
	vertices[6].Set(-3, 2.5);
	vertices[7].Set(-1.5, 0);
	b2PolygonShape polygonShape;
	polygonShape.Set(vertices, 8);
	b2Fixture* fixture = CarBody->CreateFixture(&polygonShape, 0.1f);//shape, density
}

b2Vec2 Car::getForwardVelocity() {
	b2Vec2 currentForwardNormal = CarBody->GetWorldVector(b2Vec2(0, 1));
	return b2Dot(currentForwardNormal, CarBody->GetLinearVelocity()) * currentForwardNormal;
}

b2Vec2 Car::getLateralVelocity() {
	b2Vec2 currentRightNormal = CarBody->GetWorldVector(b2Vec2(1, 0));
	return b2Dot(currentRightNormal, CarBody->GetLinearVelocity()) * currentRightNormal;
}

//tire class variables
float m_maxForwardSpeed = 20;  // 100;
float m_maxBackwardSpeed = -5; // -20;
float m_maxDriveForce = 30;    // 150;

//tire class function
void Car::updateDrive(int controlState) {
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

void Car::updateTurn(int controlState) {
	float desiredTorque = 0;
	switch (controlState & (TDC_LEFT | TDC_RIGHT)) {
	case TDC_LEFT:  desiredTorque = 15;  break;
	case TDC_RIGHT: desiredTorque = -15; break;
	default:;//nothing
	}
	CarBody->ApplyTorque(desiredTorque, true);
}

void Car::updateFriction() {
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
