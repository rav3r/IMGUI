#include "gfxlib.h"

#include <SFML/Graphics.hpp>

extern sf::RenderWindow* gWindow;

void gfxDrawRectangle(float x, float y, float width, float height, int style)
{
	sf::RectangleShape shape(sf::Vector2f(width, height));
	shape.setPosition(sf::Vector2f(x, y));
	shape.setFillColor(sf::Color::Green);

	gWindow->draw(shape);
}