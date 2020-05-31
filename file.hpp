#include <math.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdint>
#include <limits.h>

#ifndef world_hpp
#define world_hpp

class World
{
public:

	World();
	~World();

	int8_t getBlock(int, int) const;
	void setBlock(int, int, int8_t);

	sf::Vector2f blockSize() const { return { (float)nBlockWidth, (float)nBlockHeight }; }
	sf::Vector2f offset() const { return vOffset; }

	void calculateOffsets(const sf::Vector2f&);
	void generateTerrain(const int&);
	void displayWorld(sf::RenderWindow&);

private:
	const int nBlockWidth = 32;
	const int nBlockHeight = 32;

	const int nScreenWidth  = sf::VideoMode::getFullscreenModes()[0].width;
	const int nScreenHeight = sf::VideoMode::getFullscreenModes()[0].height;

	const int nVisibleBlocksX = nScreenWidth / nBlockWidth;
	const int nVisibleBlocksY = nScreenHeight / nBlockHeight;

	const int nWorldWidth = nVisibleBlocksX;
	const int nWorldHeight = 128;
	
	const int nSeedPointsDist = 40;
	const int nSeedSize = nWorldWidth / nSeedPointsDist + 3;

	int8_t* nWorld;
	std::vector<float> fDirtSeed;
	std::vector<float> fStoneSeed;

	sf::Vector2f vOffset { 0.0f, 0.0f };
	sf::Vector2f vBlockOffset { 0.0f, 0.0f };

	void fillSeed(std::vector<float>&, int, int, int);

	sf::RectangleShape block;
};

#endif /* world_hpp */