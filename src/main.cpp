#include <SFML/Graphics.hpp>

#include "gfxlib.h"

sf::RenderWindow* gWindow = 0;

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	gWindow = &window;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		
		gfxDrawRectangle(100, 100, 5, 100, 0);

		window.display();

	}

	return 0;
}