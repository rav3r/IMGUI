#include <iostream>
#include <SFML/Graphics.hpp>

#include "gfxlib.h"
#include "gfxRenderer.h"

#include "IMGUI.h"

sf::RenderWindow* gWindow = 0;
sf::Font* gFont = 0;

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	sf::View view;
	gWindow = &window;

	sf::Font font;
	gFont = &font;
	if(!font.loadFromFile("data/Cabin-Regular.otf"))
        	return EXIT_FAILURE;

	gfxRenderer guiRend;
	igContext gui(&guiRend);

	bool checkboxValue = true;
	float vSliderValue = 0.0f;
	float hSliderValue = 0.0f;
	std::string textboxValue = "Textbox";
	std::string textboxValue2 = "Another :)";

	int choice = 0;
	bool checkboxIds[5];

	float moveX = 650, moveY = 500; 

	float objectX = 100, objectY = 400, objectWidth = 100, objectHeight = 100;

	int currTab = 0;
	bool tabs[5] = {true, false, false, false, false};
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();
			if(event.type == sf::Event::Resized)
			{
				view.reset(sf::FloatRect(0, 0, event.size.width, event.size.height));
				window.setView(view);
			}
			if(event.type == sf::Event::MouseMoved)
		    {
		        gui.mouseX = event.mouseMove.x;
		        gui.mouseY = event.mouseMove.y;
		    }
			if(event.type == sf::Event::MouseButtonPressed)
			{
				if(event.mouseButton.button == sf::Mouse::Left)
					gui.leftDown = true;
			}
			if(event.type == sf::Event::MouseButtonReleased)
			{
				if(event.mouseButton.button == sf::Mouse::Left)
					gui.leftDown = false;
			}
			if(event.type == sf::Event::TextEntered)
			{
				gui.charEntered = event.text.unicode;
			}
			if(event.type == sf::Event::KeyPressed)
			{
				if(event.key.code == sf::Keyboard::Back)
					gui.backspace = true;
				if(event.key.code == sf::Keyboard::Left)
					gui.textCharPos--;
				if(event.key.code == sf::Keyboard::Right)
					gui.textCharPos++;
				if(event.key.code == sf::Keyboard::Delete)
					gui.charEntered = 127;
			}
		}

		window.clear(sf::Color(128, 128, 128, 255));
		gui.Begin();

		gui.Drag(GEN_NULL_ID, 500, 500, 40, 20, "drag");
		gui.Drag(GEN_NULL_ID, 550, 500, 40, 20, "drag");
		gui.Drag(GEN_NULL_ID, 600, 500, 40, 20, "drag");

		if(gui.Move(GEN_NULL_ID, moveX, moveY, 50, 20, "move"))
		{
			moveY = int(moveY/10+0.5f)*10;
			moveX = int(moveX/10+0.5f)*10;
			std::cout << moveX << " " << moveY << "\n";
		}

		if(gui.Checkbox(GEN_ID(checkboxValue), 300, 100, 10, 10, checkboxValue))
		{
			checkboxValue = !checkboxValue;
		}

		for(int i=0; i<5; i++)
			if(gui.Checkbox(GEN_ID(checkboxIds[i]), 300 + i*12, 130, 10, 10, choice == i))
				choice = i;

		if(checkboxValue)
		{
			if(gui.Button(GEN_NULL_ID, 100, 100, 100, 30, "Click me 1!"))
				std::cout << "button 1 clicked\n";
			if(gui.Button(GEN_NULL_ID, 100, 150, 100, 30, "Click me 2!"))
				std::cout << "button 2 clicked\n";
			if(gui.Button(GEN_NULL_ID, 100, 200, 100, 30, "AVOCADO!"))
				std::cout << "button 3 clicked\n";
			if(gui.Button(GEN_NULL_ID, 100, 250, 100, 30, "IMGUI!"))
				std::cout << "button 4 clicked\n";
			if(gui.Button(GEN_NULL_ID, 100, 300, 100, 30, "Click me 5!"))
				std::cout << "button 5 clicked\n";
		}

		if(gui.VSlider(GEN_ID(vSliderValue), 10, 10, 20, 400, 0.3f, vSliderValue))
			std::cout << "V val changed: "<<vSliderValue<<"\n";
		if(gui.HSlider(GEN_ID(hSliderValue), 100, 10, 400, 20, 0.1f, hSliderValue))
			std::cout << "H val changed: "<<hSliderValue<<"\n";

		if(gui.TextBox(GEN_ID(textboxValue), 600, 10, 100, 20, textboxValue))
			;

		if(gui.TextBox(GEN_ID(textboxValue), 600, 40, 100, 20, textboxValue))
			;

		if(gui.TextBox(GEN_ID(textboxValue2), 600, 70, 100, 20, textboxValue2))
			;

		for(int i=0; i<5; i++)
		{
			if(gui.Tab(GEN_ID(tabs[i]), 100+40*i, 50 + ((currTab == i) ? 0:2), 40, 30, "TAB", currTab == i))
			{
				currTab = i;
			}
		}
		for(int i=0; i<5; i++)
			tabs[i] = i==currTab;

		float sizerSize = 20.0f;
		float sizerX, sizerY;
		float copyX, copyY;

		copyX = sizerX = objectX - sizerSize;
		copyY = sizerY = objectY;
		if(gui.Move(GEN_NULL_ID, sizerX, sizerY, sizerSize, objectHeight, 0))
		{
			objectX = sizerX + sizerSize;
			objectWidth -= sizerX - copyX;
			if(objectWidth < 0)
			{
				objectX += objectWidth;
				objectWidth = 0;
			}
		}

		copyX = sizerX = objectX + objectWidth;
		copyY = sizerY = objectY;
		if(gui.Move(GEN_NULL_ID, sizerX, sizerY, sizerSize, objectHeight, 0))
		{
			objectWidth += sizerX - copyX;
			if(objectWidth < 0)
			{
				objectWidth = 0;
			}
		}

		copyX = sizerX = objectX;
		copyY = sizerY = objectY - sizerSize;
		if(gui.Move(GEN_NULL_ID, sizerX, sizerY, objectWidth, sizerSize, 0))
		{
			objectY = sizerY + sizerSize;
			objectHeight -= sizerY - copyY;
			if(objectHeight < 0)
			{
				objectY += objectHeight;
				objectHeight = 0;
			}
		}

		copyX = sizerX = objectX;
		copyY = sizerY = objectY + objectHeight;
		if(gui.Move(GEN_NULL_ID, sizerX, sizerY, objectWidth, sizerSize, 0))
		{
			objectHeight += sizerY - copyY;
			if(objectHeight < 0)
			{
				objectHeight = 0;
			}
		}

		gui.End();

		gfxDrawRectangle(objectX, objectY, objectWidth, objectHeight, GFX_STYLE_CHECKBOX);

		window.display();
	}

	return 0;
}
