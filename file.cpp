#include "file.hpp"

uint32_t nLehmer = 0; 	// seed
uint32_t Lehmer32()		// random number generator
{
	nLehmer += 0xe120fc15;
	uint64_t tmp;
	tmp = (uint64_t)nLehmer * 0x4a39b70d;
	uint32_t m1 = (tmp >> 32) ^ tmp;
	tmp = (uint64_t)m1 * 0x12fad5c9;
	uint32_t m2 = (tmp >> 32) ^ tmp;
	return m2;
}

int cerp(float y1, float y2, float a)	// cosine interpolation
{
	float g = (1 - cos(3.14159265 * a)) / 2;
	
	return (1 - g) * y1 + a * y2;
}

World::World()
{
	nWorld = new int8_t[nWorldWidth * nWorldHeight];
	fDirtSeed.reserve(nSeedSize);
	fStoneSeed.reserve(nSeedSize);

	brokenBlocks.reserve(100);
	placedBlocks.reserve(100);

	std::cout << "nSeedSize: " << nSeedSize << std::endl;
	std::cout << "nVisibleBlocksX: " << nVisibleBlocksX << std::endl;
	std::cout << "nScreenWidth: " << nScreenWidth << std::endl;

	if(!t1.loadFromFile("resources/textures/blocks/dirt.jpg"))
		std::cout << "Faile to load dirt" << std::endl;

	if(!t2.loadFromFile("resources/textures/blocks/stone.jpeg"))
		std::cout << "Faile to load stone" << std::endl;

	dirt.setTexture(t1);
	stone.setTexture(t2);

	dirt.setScale((float)nBlockWidth / t1.getSize().x, (float)nBlockHeight / t1.getSize().y);
	stone.setScale((float)nBlockWidth / t2.getSize().x, (float)nBlockHeight / t2.getSize().y);
}

World::~World()
{
	delete[] nWorld;
}

int8_t World::getBlock(int x, int y) const
{
	if (x >= 0 && x < nWorldWidth && y >= 0 && y < nWorldHeight)
		return nWorld[y * nWorldWidth + x];
	return -1;
}

void World::setBlock(int x, int y, int8_t blockID)
{
	if (x >= 0 && x < nWorldWidth && y >= 0 && y < nWorldHeight)
		nWorld[y * nWorldWidth + x] = blockID;
}

void World::breakBlock(int x, int y)
{
	std::cout << "block broken at: (" << (int)vOffset.x + x << ", " << y << ")" << std::endl;
	brokenBlocks.push_back({(int)vOffset.x + x, y});
}

void World::fillSeed(std::vector<float>& fSeed, int min, int max, int nOffset)
{
	for (int i = 0; i != nSeedSize; ++i)
	{
		nLehmer = i + nOffset;
		fSeed[i] = min + Lehmer32() % (max - min);
	}
}

void World::calculateOffsets(const sf::Vector2f& vPlayerPos)
{
	vOffset.x = vPlayerPos.x - (float)nVisibleBlocksX / 2.0f;
	vOffset.y = vPlayerPos.y - (float)nVisibleBlocksY / 2.0f;

	vBlockOffset.x = (vOffset.x - (int)vOffset.x) * nBlockWidth;
	vBlockOffset.y = (vOffset.y - (int)vOffset.y) * nBlockHeight;
}

void World::generateTerrain(const int& nPlayerPosX)
{
	int nOffset = nPlayerPosX / nSeedPointsDist;

	fillSeed(fDirtSeed, 40, 80, nOffset);
	fillSeed(fStoneSeed, 30, 60, nOffset);

	for (int i = 0; i != nSeedSize - 1; ++i)
	{
		for (int p = 0; p != nSeedPointsDist; ++p)
		{
			int dirtHeight = nWorldHeight - cerp(fDirtSeed[i], fDirtSeed[i + 1], (float)p / (float)nSeedPointsDist);
			int stoneHeight = nWorldHeight - cerp(fStoneSeed[i], fStoneSeed[i + 1], (float)p / (float)nSeedPointsDist);

			int x = -nSeedPointsDist + i * nSeedPointsDist + p - nPlayerPosX % nSeedPointsDist;
			int y = 0;
			for (; y < dirtHeight; ++y)
				setBlock(x, y, 0); // from top to dirtHeight -> sky

			for (; y < stoneHeight; ++y)
				setBlock(x, y, 1); // from dirtHeight to stoneHeight -> dirt

			for (; y < nWorldHeight; ++y)
				setBlock(x, y, 2); // from stoneHeight to bottom -> stone

		}
	}

	for (const sf::Vector2i& block : brokenBlocks)
		setBlock(block.x - (int)vOffset.x, block.y, 0);

	for (const sf::Vector3i& block : placedBlocks)
		setBlock(block.x - (int)vOffset.x, block.y, block.z);
}

void World::displayWorld(sf::RenderWindow& window)
{
	for (int x = -1; x < nVisibleBlocksX + 1; x++)
	{
		for (int y = 0; y < nVisibleBlocksY + 2; y++)
		{
			int8_t nTileID = getBlock(x, y + vOffset.y);
			switch (nTileID)
			{
				case 0:
					break;
				case 1:
					dirt.setPosition(x * nBlockWidth - vBlockOffset.x, y * nBlockHeight - vBlockOffset.y);
					window.draw(dirt);
					break;
				case 2:
					stone.setPosition(x * nBlockWidth - vBlockOffset.x, y * nBlockHeight - vBlockOffset.y);
					window.draw(stone);
					break;
				default:
					sf::RectangleShape block({ (float)nBlockWidth, (float)nBlockHeight });
					block.setPosition(x * nBlockWidth - vBlockOffset.x, y * nBlockHeight - vBlockOffset.y);
					block.setOutlineThickness(1);
					block.setFillColor(sf::Color::Red);
					window.draw(block);
			}
		}
	}
}