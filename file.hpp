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
	void breakBlock(int, int);

	sf::Vector2f blockSize() const { return { (float)nBlockWidth, (float)nBlockHeight }; }
	sf::Vector2f offset() const { return vOffset; }
	sf::Vector2f blockOffset() const { return vBlockOffset; }

	void calculateOffsets(const sf::Vector2f&);
	void generateTerrain(const int&);
	void displayWorld(sf::RenderWindow&);

private:
	const int nBlockWidth = 64;
	const int nBlockHeight = 64;

	const int nScreenWidth  = sf::VideoMode::getFullscreenModes()[0].width;
	const int nScreenHeight = sf::VideoMode::getFullscreenModes()[0].height;

	const int nVisibleBlocksX = nScreenWidth / nBlockWidth;
	const int nVisibleBlocksY = nScreenHeight / nBlockHeight;

	const int nWorldWidth = nVisibleBlocksX + 1;
	const int nWorldHeight = 128;
	
	const int nSeedPointsDist = 40;
	const int nSeedSize = nWorldWidth / nSeedPointsDist + 4;

	int8_t* nWorld;
	std::vector<float> fDirtSeed;
	std::vector<float> fStoneSeed;

	sf::Vector2f vOffset { 0.0f, 0.0f };
	sf::Vector2f vBlockOffset { 0.0f, 0.0f };

	void fillSeed(std::vector<float>&, int, int, int);

	std::vector<sf::Vector2i> brokenBlocks;
	std::vector<sf::Vector3i> placedBlocks;

	sf::Texture t1;
	sf::Sprite dirt;

	sf::Texture t2;
	sf::Sprite stone;
};

#endif /* world_hpp */