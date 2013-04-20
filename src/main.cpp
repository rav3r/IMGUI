#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

#include "gfxlib.h"
#include "gfxRenderer.h"

#include "IMGUI.h"

sf::RenderWindow* gWindow = 0;
sf::Font* gFont = 0;

std::string IntToStr(int k)
{
	std::ostringstream o;
	o << k;
	return o.str();
}

class DragInt: public igDraggable
{
private:
	int value;
public:
	DragInt(int k) { value = k; }
	DragInt(const DragInt& o) { value = o.value; }
	void Add(int i) { value += i; }
	operator int() { return value; }
};

class DragOther: public igDraggable 
{
	// can't be dragged
};

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

	float leftSliderValue = 0.0f;
	float rightSliderValue = 0.0f;

	sf::Clock timer;
	
	while (window.isOpen())
	{
		sf::Event event;
		
		for(int i=0; i<10 && window.pollEvent(event); i++)
		{
			if(event.type == sf::Event::Closed)
				window.close();
			if(event.type == sf::Event::MouseWheelMoved)
			{
				gui.mouseWheel += event.mouseWheel.delta;
			}
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

		window.clear(sf::Color(0, 0, 0, 255));

		sf::Vector2u size = gWindow->getSize();
		
		guiRend.time = timer.getElapsedTime().asMilliseconds();

		gui.Begin();

		static int leftScrollbarOffset = 0;
		gui.BeginScrollArea(GEN_NULL_ID, 10, 10, 120, size.y-20, leftScrollbarOffset);

		gui.Button(GEN_NULL_ID, "Button 1");
		gui.Button(GEN_NULL_ID, "Button 2");
		if(gui.Checkbox(GEN_NULL_ID, checkboxValue, "checkbox"))
			checkboxValue = !checkboxValue;
		if(checkboxValue)
		{
			gui.Button(GEN_NULL_ID, "Button 3");
			gui.Button(GEN_NULL_ID, "Button 4");
			gui.Button(GEN_NULL_ID, "Button 1");
			gui.Button(GEN_NULL_ID, "Button 2");
			gui.Button(GEN_NULL_ID, "Button 3");
			gui.Button(GEN_NULL_ID, "Button 4");
			gui.Button(GEN_NULL_ID, "Button 1");
			gui.Button(GEN_NULL_ID, "Button 2");
			gui.Button(GEN_NULL_ID, "Button 3");
			gui.Button(GEN_NULL_ID, "Button 4");
		}
		static std::string text1 = "Edit me!";
		gui.TextBox(GEN_NULL_ID, text1);
		static std::string text2 = "Or me!";
		gui.TextBox(GEN_NULL_ID, text2);
		gui.Button(GEN_NULL_ID, "Button 1");
		gui.Button(GEN_NULL_ID, "Button 2");
		gui.Button(GEN_NULL_ID, "Button 3");
		gui.Button(GEN_NULL_ID, "Button 4");
		gui.Button(GEN_NULL_ID, "Button 1");
		gui.Button(GEN_NULL_ID, "Button 2");
		gui.Button(GEN_NULL_ID, "Button 3");
		gui.Button(GEN_NULL_ID, "Button 4");
		gui.Button(GEN_NULL_ID, "Button -5");
		gui.Button(GEN_NULL_ID, "Button -4");
		gui.Button(GEN_NULL_ID, "Button -3");
		gui.Button(GEN_NULL_ID, "Button -2");
		gui.Button(GEN_NULL_ID, "Button -1");

		gui.EndScrollArea(true);

		static int rightScrollbarOffset = 0;
		gui.BeginScrollArea(GEN_NULL_ID, size.x - 230, 10, 220, size.y-20, rightScrollbarOffset);

		gui.Button(GEN_NULL_ID, "Button A");
		gui.Button(GEN_NULL_ID, "Button B");
		if(gui.Checkbox(GEN_NULL_ID, checkboxValue, "checkbox"))
			checkboxValue = !checkboxValue;
		gui.Button(GEN_NULL_ID, "Button 3");
		gui.Button(GEN_NULL_ID, "Button 4");
		gui.Button(GEN_NULL_ID, "Button 1");
		gui.Button(GEN_NULL_ID, "Button 2");
		gui.Button(GEN_NULL_ID, "Button 3");
		gui.Button(GEN_NULL_ID, "Button 4");
		gui.Button(GEN_NULL_ID, "Button 1");
		gui.Button(GEN_NULL_ID, "Button 2");
		gui.Button(GEN_NULL_ID, "Button 3");
		gui.Button(GEN_NULL_ID, "Button 4");
		gui.Button(GEN_NULL_ID, "Button 1");
		gui.Button(GEN_NULL_ID, "Button 2");
		gui.Button(GEN_NULL_ID, "Button 3");
		gui.Button(GEN_NULL_ID, "Button 4");
		gui.Button(GEN_NULL_ID, "Button 1");
		gui.Button(GEN_NULL_ID, "Button 2");
		gui.Button(GEN_NULL_ID, "Button 3");
		gui.Button(GEN_NULL_ID, "Button 4");
		gui.Button(GEN_NULL_ID, "Button -5");
		gui.Indent();
		gui.Button(GEN_NULL_ID, "Button -4");
		gui.Button(GEN_NULL_ID, "Button -3");
		gui.Button(GEN_NULL_ID, "Button -2");
		gui.Button(GEN_NULL_ID, "Button -1");
		

		gui.Indent();
		static DragInt drag1Val(1);
		if(DragInt* val=gui.Drag<DragInt>(GEN_NULL_ID, IntToStr(drag1Val).c_str(), drag1Val))
		{
			drag1Val.Add(*val);
		}
		static DragInt drag2Val(2);
		if(DragInt* val=gui.Drag<DragInt>(GEN_NULL_ID, IntToStr(drag2Val).c_str(), drag2Val))
		{
			drag2Val.Add(*val);
		}
		static DragInt drag3Val(3);
		if(DragInt* val=gui.Drag<DragInt>(GEN_NULL_ID, IntToStr(drag3Val).c_str(), drag3Val))
		{
			drag3Val.Add(*val);
		}

		static DragOther otherVal;
		if(DragOther* val=gui.Drag<DragOther>(GEN_NULL_ID, "Disabled drag", otherVal))
		{
			std::cout << "Bug in program :(\n";
		}
		gui.Unindent();
		gui.Unindent();

		gui.EndScrollArea(false);

		/*

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
			if(igButton b = gui.Button(GEN_NULL_ID, 100, 100, 100, 30, "Click me 1!"))
			{
				if(b.onClicked)
					std::cout << "button 1 clicked\n";
			}
			if(igButton b = gui.Button(GEN_NULL_ID, 100, 150, 100, 30, "Click me 2!"))
			{
				if(b.onClicked)
					std::cout << "button 2 clicked\n";
			}
			if(igButton b = gui.Button(GEN_NULL_ID, 100, 200, 100, 30, "AVOCADO!"))
			{
				if(b.onClicked)
					std::cout << "button 3 clicked\n";
			}
			if(igButton b = gui.Button(GEN_NULL_ID, 100, 250, 100, 30, "IMGUI!"))
			{
				if(b.onClicked)
					std::cout << "button 4 clicked\n";
			}
			if(igButton b = gui.Button(GEN_NULL_ID, 100, 300, 100, 30, "Click me 5!"))
			{
				if(b.onClicked)
					std::cout << "button 5 clicked\n";
			}
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
*/
		gui.End();

		//gfxDrawRectangle(objectX, objectY, objectWidth, objectHeight, GFX_STYLE_CHECKBOX);

		window.display();
	}

	return 0;
}
