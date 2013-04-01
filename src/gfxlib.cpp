#include "gfxlib.h"

#include <iostream>
#include <SFML/Graphics.hpp>

extern sf::RenderWindow* gWindow;
extern sf::Font* gFont;

/*
var. 1 = #1240AB = rgb(18,64,171) - textinput
var. 2 = #2A4480 = rgb(42,68,128) - normal
var. 3 = #06266F = rgb(6,38,111) - border
var. 4 = #4671D5 = rgb(70,113,213) - hover
var. 5 = #6C8CD5 = rgb(108,140,213) - active

border +
active +
textInput 
hover +
normal
*/

static sf::Color textColor(255, 255, 255, 255);
static sf::Color elementColor(42,68,128,255);
static sf::Color pressedElemColor(108,140,213,255);
static sf::Color hoverElemColor(70,113,213,255);
static sf::Color checkboxColor(255, 255, 255, 255);
static sf::Color borderColor(6,38,111,255);

void gfxDrawRectangle(float x, float y, float width, float height, int style)
{
	sf::RectangleShape shape(sf::Vector2f(width-2, height-2));
	shape.setOutlineThickness(1);
	shape.setOutlineColor(borderColor);
	shape.setPosition(sf::Vector2f(x+1, y+1));
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
	sf::FloatRect r = text.getLocalBounds();
	sf::Vector2f pos = sf::Vector2f(x - (r.left+r.width)/2.0f, y - text.getCharacterSize()/2.0f);
	pos = sf::Vector2f((int)pos.x, (int)pos.y);
	text.setPosition(pos);
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
		_Text.setPosition(sf::Vector2f(pos.x + xDiff, pos.y));
		gWindow->draw(_Text);
	} 
}
