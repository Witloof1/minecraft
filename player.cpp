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
void Player::jump()  { vAcc.y -= 12.0f; }
void Player::t_down()  { vAcc.y += 12.0f; }

void Player::move(const float& fElapsedTime)
{
	//vAcc.y += 3.0f; // gravity

	vVel += vAcc;
	vVel.x *= 0.9f;
	vVel.y *= 0.9f;

	vPos.x += vVel.x * fElapsedTime;
	vPos.y += vVel.y * fElapsedTime;

	vAcc = { 0.0f, 0.0f };
}

void Player::display(sf::RenderWindow& window)
{
	playerRect.setPosition((vPos.x - world.offset().x) * world.blockSize().x, (vPos.y - world.offset().y) * world.blockSize().y);
	window.draw(playerRect); 
}