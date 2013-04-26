#include "gfxlib.h"

#include <iostream>
#include <SFML/Graphics.hpp>

extern sf::RenderWindow* gWindow;
extern sf::Font* gFont;

static sf::Color textColor(255, 255, 255, 255);
static sf::Color elementColor(85, 87, 82, 255);
static sf::Color pressedElemColor(108,140,213,255);
static sf::Color hoverElemColor(70,113,213,255);
static sf::Color checkboxColor(255, 255, 255, 255);
static sf::Color borderColor(6,38,111,255);

void gfxDrawRectangle(float x, float y, float width, float height, int style, bool border)
{
	sf::RectangleShape shape(sf::Vector2f(width, height));
	shape.setPosition(sf::Vector2f(x, y));

	if(border)
	{
		shape.setSize(sf::Vector2f(width-2, height-2));
		shape.setPosition(sf::Vector2f(x+1, y+1));
	}

	if(style == GFX_STYLE_NONE)
	    shape.setFillColor(elementColor);
	else if(style == GFX_STYLE_ELEM_PRESSED)
	    shape.setFillColor(pressedElemColor);
	else if(style == GFX_STYLE_ELEM_HOVER)
	    shape.setFillColor(hoverElemColor);
	else if(style == GFX_STYLE_SCROLL_AREA)
		shape.setFillColor(sf::Color(70,70,70));
	else if(style == GFX_STYLE_SLIDER_THUMB)
		shape.setFillColor(sf::Color(104, 106, 101));
	else if(style == GFX_STYLE_SLIDER_BG)
		shape.setFillColor(sf::Color(50, 50, 50));
	else if(style == GFX_STYLE_TEXTBOX_BG)
		shape.setFillColor(sf::Color(110, 110, 110));
	else if(style == GFX_STYLE_CAN_DRAG)
		shape.setFillColor(sf::Color(50, 100, 50));
	else if(style == GFX_STYLE_CAN_NOT_DRAG)
		shape.setFillColor(sf::Color(100, 50, 50));
	else if(style == GFX_STYLE_SELECTED)
		shape.setFillColor(sf::Color(0, 0, 255));
	else
		shape.setFillColor(checkboxColor);
	    
	gWindow->draw(shape);

	if(!border)
		return;

	sf::Color lighterBorderColor(117, 119, 114);
	sf::Color darkerBorderColor(53, 55, 50);
	
	sf::RectangleShape upBorder(sf::Vector2f(width, 1));
	upBorder.setPosition(sf::Vector2f(x, y));
	upBorder.setFillColor(lighterBorderColor);
	gWindow->draw(upBorder);

	sf::RectangleShape leftBorder(sf::Vector2f(1, height));
	leftBorder.setPosition(sf::Vector2f(x, y));
	leftBorder.setFillColor(lighterBorderColor);
	gWindow->draw(leftBorder);

	sf::RectangleShape downBorder(sf::Vector2f(width, 1));
	downBorder.setPosition(sf::Vector2f(x, y+height-1));
	downBorder.setFillColor(darkerBorderColor);
	gWindow->draw(downBorder);

	sf::RectangleShape rightBorder(sf::Vector2f(1, height));
	rightBorder.setPosition(sf::Vector2f(x+width-1, y));
	rightBorder.setFillColor(darkerBorderColor);
	gWindow->draw(rightBorder);
}

void gfxPrint(float x, float y, const char* str, int style, int pipePos, int pipe2Pos)
{
	const int charSize = 16;
	const int pipeSize = 12;

	sf::Text text(str, *gFont, charSize);
	sf::FloatRect r = text.getLocalBounds();
	sf::Vector2f pos = sf::Vector2f(x - (r.left+r.width)/2.0f, y - text.getCharacterSize()/2.0f);
	pos = sf::Vector2f((int)pos.x, (int)pos.y);
	text.setPosition(pos);
	text.setColor(textColor);

	if(pipePos != -1)
	{
		float p1 = text.findCharacterPos(pipePos).x;
		float p2 = text.findCharacterPos(pipe2Pos).x;
		gfxDrawRectangle(p1, pos.y-pipeSize/2, p2-p1, pipeSize*2, GFX_STYLE_ELEM_HOVER, false);
	}

	gWindow->draw(text);

	if(pipePos >= 0 && pipePos == pipe2Pos)
	{
		std::string curr = str;
		curr = curr.substr(0, pipePos);
		sf::Text currText(curr+".", *gFont, charSize);
		sf::Text _Text("|", *gFont, charSize);
		sf::Text dotText(".", *gFont, charSize);

		_Text.setColor(sf::Color(0, 0, 255));
		float xDiff = currText.getLocalBounds().width - dotText.getLocalBounds().width;
		_Text.setPosition(sf::Vector2f(pos.x + xDiff, pos.y));
		//gWindow->draw(_Text);
		gfxDrawRectangle(pos.x + xDiff-2, pos.y-pipeSize/2, 2, pipeSize*2, GFX_STYLE_ELEM_PRESSED);
	} 
}

int gfxCharAt( float x, float y, const char* str, int style, float mouseX )
{
	const int charSize = 16;

	sf::Text text(str, *gFont, charSize);
	sf::FloatRect r = text.getLocalBounds();
	sf::Vector2f pos = sf::Vector2f(x - (r.left+r.width)/2.0f, y - text.getCharacterSize()/2.0f);
	pos = sf::Vector2f((int)pos.x, (int)pos.y);
	text.setPosition(pos);
	int character = 0;
	while(character < text.getString().getSize() && text.findCharacterPos(character+1).x < mouseX+2)
		character++;
	return character;
}

#ifdef WIN32

#include <windows.h>
#include <gl/gl.h>

#else

// todo: linux gl headers

#endif

void gfxScissor(int x, int y, int width, int height)
{
	glEnable(GL_SCISSOR_TEST);
	glScissor(x,gWindow->getSize().y - y - height,width,height);
}

void gfxDisableScissor()
{
	glDisable(GL_SCISSOR_TEST);
}
