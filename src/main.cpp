#include <iostream>
#include <SFML/Graphics.hpp>

#include "gfxlib.h"

#include "IMGUI.h"

sf::RenderWindow* gWindow = 0;
sf::Font* gFont = 0;

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	gWindow = &window;

	sf::Font font;
	gFont = &font;
	if(!font.loadFromFile("Amble-Bold.ttf"))
        	return EXIT_FAILURE;

	igContext gui;

	bool checkboxValue = true;
	float vSliderValue = 0.0f;
	float hSliderValue = 0.0f;
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();
		    if(event.type == sf::Event::MouseMoved)
		    {
		        gui.mouseX = event.mouseMove.x;
		        gui.mouseY = event.mouseMove.y;
		    }
			if(event.type == sf::Event::MouseButtonPressed)
			{
				if(event.mouseButton.button == sf::Mouse::Button::Left)
					gui.leftDown = true;
			}
			if(event.type == sf::Event::MouseButtonReleased)
			{
				if(event.mouseButton.button == sf::Mouse::Button::Left)
					gui.leftDown = false;
			}
		}

		window.clear(sf::Color(128, 128, 128, 255));
		gui.Begin();

		if(gui.Checkbox(GEN_ID(checkboxValue), 300, 100, 10, 10, checkboxValue))
		{
			checkboxValue = !checkboxValue;
		}

		if(checkboxValue)
		{
			if(gui.Button(GEN_NULL_ID, 100, 100, 100, 30, "Click me 1!"))
				std::cout << "button 1 clicked\n";
			if(gui.Button(GEN_NULL_ID, 100, 150, 100, 30, "Click me 2!"))
				std::cout << "button 2 clicked\n";
			if(gui.Button(GEN_NULL_ID, 100, 200, 100, 30, "Click me 3!"))
				std::cout << "button 3 clicked\n";
			if(gui.Button(GEN_NULL_ID, 100, 250, 100, 30, "Click me 4!"))
				std::cout << "button 4 clicked\n";
			if(gui.Button(GEN_NULL_ID, 100, 300, 100, 30, "Click me 5!"))
				std::cout << "button 5 clicked\n";
		}

		if(gui.VSlider(GEN_ID(vSliderValue), 10, 10, 20, 400, 0.3f, vSliderValue))
			std::cout << "V val changed: "<<vSliderValue<<"\n";
		if(gui.HSlider(GEN_ID(hSliderValue), 100, 10, 400, 20, 0.1f, hSliderValue))
			std::cout << "H val changed: "<<hSliderValue<<"\n";

		gui.End();
		window.display();

	}

	return 0;
}
