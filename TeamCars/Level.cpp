#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include "Level.h"

	Level::Level(){
		b2Vec2 gravity(0.f, 0.f);
		world = new b2World(gravity);	
	}

	b2World Level::getWorld(){
		return *world;
	}

