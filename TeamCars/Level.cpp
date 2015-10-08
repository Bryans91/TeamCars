#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

class Level{

	b2World* world ;

	Level(){
		b2Vec2 gravity(0.f, 0.f);
		b2World world(gravity);
		
	}

	b2World* getWorld(){
		return this->world;
	}


};