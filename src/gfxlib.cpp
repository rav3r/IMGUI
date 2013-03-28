#include "gfxlib.h"

#include <SFML/Graphics.hpp>

extern sf::RenderWindow* gWindow;
extern sf::Font* gFont;

void gfxDrawRectangle(float x, float y, float width, float height, int style)
{
	sf::RectangleShape shape(sf::Vector2f(width, height));
	shape.setPosition(sf::Vector2f(x, y));
	shape.setFillColor(sf::Color::Green);

	gWindow->draw(shape);
}

void gfxPrint(float x, float y, const char* str, int style)
{
	sf::Text text(str, *gFont, 16);
	text.setPosition(sf::Vector2f(x, y));
	
	gWindow->draw(text);
}
