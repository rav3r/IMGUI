#include "gfxlib.h"

#include <iostream>
#include <SFML/Graphics.hpp>

extern sf::RenderWindow* gWindow;
extern sf::Font* gFont;

/*
   var. 1 = #5C0DAC = rgb(92,13,172) +
   var. 2 = #542881 = rgb(84,40,129)
   var. 3 = #3A0470 = rgb(58,4,112) +
   var. 4 = #8B42D6 = rgb(139,66,214) +
   var. 5 = #9F69D6 = rgb(159,105,214) +
*/

static sf::Color textColor(159, 105, 214, 255);
static sf::Color elementColor(58, 4, 112, 255);
static sf::Color pressedElemColor(139, 66, 214, 255);
static sf::Color hoverElemColor(92, 13, 172, 255);
static sf::Color checkboxColor(159, 105, 214, 255);

void gfxDrawRectangle(float x, float y, float width, float height, int style)
{
	sf::RectangleShape shape(sf::Vector2f(width, height));
	shape.setPosition(sf::Vector2f(x, y));
	if(style == GFX_STYLE_NONE)
	    shape.setFillColor(elementColor);
	else if(style == GFX_STYLE_ELEM_PRESSED)
	    shape.setFillColor(pressedElemColor);
	else if(style == GFX_STYLE_ELEM_HOVER)
	    shape.setFillColor(hoverElemColor);
	else
		shape.setFillColor(checkboxColor);
	    
	gWindow->draw(shape);
}

void gfxPrint(float x, float y, const char* str, int style, int pipePos)
{
	const int charSize = 16;

	sf::Text text(str, *gFont, charSize);
	text.setPosition(sf::Vector2f(x, y));
	text.setColor(textColor);

	gWindow->draw(text);

	if(pipePos >= 0)
	{
		std::string curr = str;
		curr = curr.substr(0, pipePos);
		sf::Text currText(curr+".", *gFont, charSize);
		sf::Text _Text("_", *gFont, charSize);
		sf::Text dotText(".", *gFont, charSize);

		_Text.setColor(sf::Color::Red);
		float xDiff = currText.getLocalBounds().width - dotText.getLocalBounds().width;
		_Text.setPosition(sf::Vector2f(x + xDiff, y));
		gWindow->draw(_Text);
	} 
}
