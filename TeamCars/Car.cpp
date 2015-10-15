#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

class Car {
	b2Body* m_body;
public:
	Car(b2World* world) {
		//create car body
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		m_body = world->CreateBody(&bodyDef);

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
		b2Fixture* fixture = m_body->CreateFixture(&polygonShape, 0.1f);//shape, density
	}
};