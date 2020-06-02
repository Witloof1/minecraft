#include "player.hpp"

Player::Player(World& _world) : world(_world)
{
	playerRect.setSize(world.blockSize());

	if (vMaxVel.x > world.blockSize().x)
		std::cout << "vMaxVel.x should be smaller than world.blockSize().x" << std::endl;
	if (vMaxVel.y > world.blockSize().y)
		std::cout << "vMaxVel.y should be smaller than world.blockSize().y" << std::endl;
}

Player::~Player()
{
}

void Player::left()  { vAcc.x -= 12.0f; }
void Player::right() { vAcc.x += 12.0f; }
void Player::jump()  { if (bOnGround) vVel.y -= 36.0f; }
void Player::down()  { vPos.x = round(vPos.x); }

void Player::move()
{
	// gravity
	vAcc.y += 2.0f;

	vVel += vAcc;
	vVel.x *= vFriction.x;
	vVel.y *= vFriction.y;

	// Clamp velocities
	if (vVel.x > vMaxVel.x)
		vVel.x = vMaxVel.x;
	else if (vVel.x < -vMaxVel.x)
		vVel.x = -vMaxVel.x;

	if (vVel.y > vMaxVel.y)
		vVel.y = vMaxVel.x;
	else if (vVel.y < -vMaxVel.x)
		vVel.y = -vMaxVel.x;

	vAcc = { 0.0f, 0.0f };

	vScreenPos = { (vPos.x - world.offset().x) * world.blockSize().x, (vPos.y - world.offset().y) * world.blockSize().y };
}

void Player::collision(const float& fElapsedTime)
{
	float fNewPosX = vPos.x + vVel.x * fElapsedTime - (int)world.offset().x;
	float fNewPosY = vPos.y + vVel.y * fElapsedTime;

	if (vVel.x <= 0)	// moving left
	{
		if (world.getBlock(fNewPosX, vPos.y) != 0 || world.getBlock(fNewPosX, vPos.y + 0.999f) != 0)
		{
			fNewPosX = (int)fNewPosX + 1;
			vVel.x = 0;
		}
	}
	else 				// moving left
	{
		if (world.getBlock(fNewPosX + 1.0f, vPos.y) != 0 || world.getBlock(fNewPosX + 1.0f, vPos.y + 0.999f) != 0)
		{
			fNewPosX = (int)fNewPosX;
			vVel.x = 0;
		}

	}

	bOnGround = false;
	vFriction.x = 0.4f;
	if (vVel.y <= 0)	// moving up
	{
		if (world.getBlock(fNewPosX, fNewPosY) != 0 || world.getBlock(fNewPosX + 0.999f, fNewPosY) != 0)
		{
			fNewPosY = (int)fNewPosY + 1;
			vVel.y = 0;
		}
	}
	else 				// moving down
	{
		if (world.getBlock(fNewPosX, fNewPosY + 1.0f) != 0 || world.getBlock(fNewPosX + 0.999f, fNewPosY + 1.0f) != 0)
		{
			fNewPosY = (int)fNewPosY;
			vVel.y = 0;
			bOnGround = true;
			vFriction.x = 0.6f;
		}
	}

	vPos.x = fNewPosX + (int)world.offset().x;
	vPos.y = fNewPosY;
}

void Player::calculateRayAngle(sf::Vector2f vMouse)
{
	sf::Vector2f vToMouse = vMouse - vScreenPos;
	sf::Vector2f vUp { 0.0f, 1.0f };

	fRayAngle = (3.14159265f / 2) + atan2(vToMouse.y, vToMouse.x) - atan2(vUp.y, vUp.x);
}

void Player::castRay()
{
	sf::Vector2f vTestPoint;
	for (float fProg = 0.0f; fProg < fMaxReachability; fProg+=0.1)
	{
		vTestPoint.x = vPos.x - world.offset().x + cos(fRayAngle) * fProg;
		vTestPoint.y = vPos.y + sin(fRayAngle) * fProg;

		if (world.getBlock(vTestPoint.x, vTestPoint.y) != 0)
			break;
	}

	vEndOfRay = vTestPoint;
}

void Player::breakBlock()
{
	world.breakBlock(vEndOfRay.x, vEndOfRay.y);
}

void Player::drawRay(sf::RenderWindow& window)
{
	sf::Vertex ray[2]
	{
		sf::Vertex(vScreenPos),
		sf::Vertex({ vEndOfRay.x * world.blockSize().x, (vEndOfRay.y - world.offset().y) * world.blockSize().y })
	};

	sf::CircleShape circle(10, 12);
	circle.setPosition(vEndOfRay.x * world.blockSize().x - 5, (vEndOfRay.y - world.offset().y) * world.blockSize().y - 5);
	window.draw(circle);

	window.draw(ray, 2, sf::Lines);
}

void Player::display(sf::RenderWindow& window)
{
	playerRect.setPosition(vScreenPos);
	window.draw(playerRect); 

	drawRay(window);
}