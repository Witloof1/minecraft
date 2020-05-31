#include "player.hpp"

Player::Player(const World& _world) : world(_world)
{
	playerRect.setSize(world.blockSize());
}

Player::~Player()
{
}

void Player::left()  { vAcc.x -= 12.0f; }
void Player::right() { vAcc.x += 12.0f; }
void Player::jump()  { if (bOnGround) vVel.y -= 36.0f; }

void Player::move()
{
	// gravity
	vAcc.y += 2.0f;

	vVel += vAcc;
	vVel.x *= vFriction.x;
	vVel.y *= vFriction.y;

	// Clamp velocities
	if (vVel.x > world.blockSize().x)
		vVel.x = world.blockSize().x;
	if (vVel.x < -world.blockSize().x)
		vVel.x = -world.blockSize().x;

	if (vVel.y > world.blockSize().y)
		vVel.y = world.blockSize().y;
	if (vVel.y < -world.blockSize().y)
		vVel.y = -world.blockSize().y;

	vAcc = { 0.0f, 0.0f };
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
			vFriction.x = 0.9f;
		}
	}

	vPos.x = fNewPosX + (int)world.offset().x;
	vPos.y = fNewPosY;
}

void Player::display(sf::RenderWindow& window)
{
	playerRect.setPosition((vPos.x - world.offset().x) * world.blockSize().x, (vPos.y - world.offset().y) * world.blockSize().y);
	window.draw(playerRect); 
}