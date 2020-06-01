#include "file.hpp"
#include <SFML/Graphics.hpp>

#ifndef player_hpp
#define player_hpp

class Player
{
public:
	Player(World&);
	~Player();

	sf::Vector2f pos() const { return vPos; }
	void left();
	void right();
	void jump();
	void down();

	void move();
	void collision(const float& fElapsedTime);
	void defineRayAngle(sf::Vector2f vMouse);
	void castRay();
	void display(sf::RenderWindow& window);

private:
	World& world;

	sf::Vector2f vPos { 284.0f, 40.0f };
	sf::Vector2f vVel { 0.0f, 0.0f };
	sf::Vector2f vAcc { 0.0f, 0.0f };
	sf::Vector2f vFriction { 0.9f, 1.0f };
	sf::Vector2f vMaxVel { 48.0f, 48.0f };
	sf::Vector2f vScreenPos;

	bool bOnGround;

	float fMaxReachability = 7.0f;
	float fRayAngle;

	sf::Vector2f vTestPoint;
	void drawRay(sf::RenderWindow& window);

	sf::RectangleShape playerRect;
};

#endif /* player_hpp */