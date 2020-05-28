#include "file.hpp"
#include <SFML/Graphics.hpp>

#ifndef player_hpp
#define player_hpp

class Player
{
public:
	Player(const World&);
	~Player();

	sf::Vector2f pos() { return vPos; }
	void left();
	void right();
	void jump();
	void t_down();

	void move(const float& fElapsedTime);
	void collision(const float& fElapsedTime);
	void display(sf::RenderWindow& window);

	void t_setVel(sf::Vector2f vel) { vVel = vel; }

private:
	const World& world;

	sf::Vector2f vPos { 64.0f, 64.0f };
	sf::Vector2f vVel { 0.0f, 0.0f };
	sf::Vector2f vAcc { 0.0f, 0.0f };

	sf::RectangleShape playerRect;
};

#endif /* player_hpp */