#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <cstdint>
#include "file.hpp"
#include "player.hpp"

const int nWIDTH  = sf::VideoMode::getFullscreenModes()[0].width;
const int nHEIGHT = sf::VideoMode::getFullscreenModes()[0].height;

int main()
{
	sf::RenderWindow window(sf::VideoMode(nWIDTH, nHEIGHT), "minecraft 2D", sf::Style::Fullscreen);
	window.setFramerateLimit(60);
	//window.setMouseCursorVisible(false);
	window.setVerticalSyncEnabled(true);

	sf::Clock clock;

	sf::Font font;
	font.loadFromFile("/System/Library/Fonts/Avenir.ttc");
	sf::Text text("", font);
	sf::Text text2("", font);
	text2.setPosition(0, 30);
	sf::Text text3("", font);
	text3.setPosition(0, 60);

	World world;
	
	Player player(world);

	sf::Vector2f vMouse;

	while (window.isOpen())
	{
		vMouse = { (float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y };

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed)
				if (event.mouseButton.button == sf::Mouse::Left)
					player.breakBlock();
		}

		if (window.hasFocus())
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				player.left();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				player.right();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				player.jump();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				player.down();
		}

		window.clear(sf::Color(135, 206, 235));

		float fElapsedTime = clock.restart().asSeconds();
		player.move();
		player.collision(fElapsedTime);
		
		world.calculateOffsets(player.pos());
		world.generateTerrain(player.pos().x);
		
		world.displayWorld(window);
		player.display(window);

		player.calculateRayAngle(vMouse);
		player.castRay();

		
		// for debugging: display fElapsedTime
		std::stringstream ss;
		ss << "Elapsed time: " << fElapsedTime;
		text.setString(ss.str());
		window.draw(text);

		// for debugging: display player x pos
		std::stringstream ss2;
		ss2 << "x: " << player.pos().x;
		text2.setString(ss2.str());
		window.draw(text2);

		// for debugging: display player y pos
		std::stringstream ss3;
		ss3 << "y: " << player.pos().y;
		text3.setString(ss3.str());
		window.draw(text3);

		window.display();
	}
}