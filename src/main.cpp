#include <SFML/Graphics.hpp>

#include "gfxlib.h"

sf::RenderWindow* gWindow = 0;
sf::Font* gFont = 0;

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	gWindow = &window;

	sf::Font font;
	gFont = &font;
	if(!font.loadFromFile("SourceCodePro-Light.otf"))
        	return EXIT_FAILURE;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color(32, 64, 128, 255));
		
		gfxDrawRectangle(100, 100, 5, 100, 0);

		gfxPrint(10, 10, "Zazolc gesla jazn .,?!-|", 0);

		window.display();

	}

	return 0;
}
