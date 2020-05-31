#include "file.hpp"
#include <SFML/Graphics.hpp>

#ifndef player_hpp
#define player_hpp

class Player
{
public:
	Player(const World&);
	~Player();

	sf::Vector2f pos() const { return vPos; }
	void left();
	void right();
	void jump();

	void move();
	void collision(const float& fElapsedTime);
	void display(sf::RenderWindow& window);

private:
	const World& world;

	sf::Vector2f vPos { 128.0f, 40.0f };
	sf::Vector2f vVel { 0.0f, 0.0f };
	sf::Vector2f vAcc { 0.0f, 0.0f };
	sf::Vector2f vFriction { 0.9f, 1.0f };

	bool bOnGround;

	sf::RectangleShape playerRect;
};

#endif /* player_hpp */