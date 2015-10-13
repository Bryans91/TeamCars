#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

class Level{
private:
	b2World* world;
public:
	Level();
	b2World getWorld();

};