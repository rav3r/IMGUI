#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

#include "gfxlib.h"
#include "igRenderer.h"

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

class DragTreeNode: public igDraggable
{
private:
	std::vector<DragTreeNode*> children;
	DragTreeNode* parent;
	std::string name;
	bool expand;
public:
	DragTreeNode(std::string n, DragTreeNode* p=0): parent(p), name(n), expand(true) {}

	void RemoveChild(DragTreeNode* child)
	{
		for(int i=0; i<children.size(); i++)
			if(child == children[i])
				children.erase(children.begin()+i);
		child->parent = 0;
	}

	void AddChild(DragTreeNode* child)
	{
		if(child->parent == this) return;
		DragTreeNode* prevParent = this->parent;
		while(prevParent)
		{
			if(prevParent == child)
				return;
			prevParent = prevParent->parent;
		}
		if(child->parent)
			child->parent->RemoveChild(child);
		child->parent = this;
		children.push_back(child);
	}

	void DoGUI(igContext &gui, int indent=0)
	{
		if(indent!=0) gui.Space(indent);

		if(children.empty())
			gui.Space(16);
		else
		{
			if(gui.Checkbox(GEN_ID(*this), expand, 0))
				expand = !expand;
		}
		if(DragTreeNode* child = gui.Drag<DragTreeNode>(GEN_ID(*this), name.c_str(), *this))
		{
			AddChild(child);
		}
		if(!expand)
			return;
		//gui.Indent();
		for(int i=0; i<children.size(); i++)
			children[i]->DoGUI(gui, indent+16+marginX);
		//gui.Unindent();
	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	sf::View view;
	gWindow = &window;

	sf::Font font;
	gFont = &font;
	if(!font.loadFromFile("data/SourceSansPro-Regular.otf"))
        	return EXIT_FAILURE;

	igRenderer guiRend;
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

	DragTreeNode* root = new DragTreeNode("root");
	int id=0;
	
	while (window.isOpen())
	{
		sf::Event event;
		
		for(int i=0; i<10 && window.pollEvent(event); i++)
		{
			gui.shift = event.key.shift;
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
					gui.ArrowLeftDown();
				if(event.key.code == sf::Keyboard::Right)
					gui.ArrowRightDown();
				if(event.key.code == sf::Keyboard::Delete)
					gui.charEntered = 127;
			}
		}

		window.clear(sf::Color(0, 0, 0, 255));

		sf::Vector2u size = gWindow->getSize();
		
		guiRend.time = timer.getElapsedTime().asMilliseconds();

		gui.Begin();

		static int leftScrollbarOffset = 0;
		gui.BeginScrollArea(GEN_NULL_ID, 10, 100, 220, size.y-200, leftScrollbarOffset);

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
		gui.TextBox(GEN_NULL_ID, text1, 100);
		gui.Button(GEN_NULL_ID, "do it!");
		static std::string text2 = "Or me!";
		gui.TextBox(GEN_NULL_ID, text2);
		for(int i=0; i<50; i++)
		{
			if(gui.Button(GEN_IID(i), "Button 1").onClicked)
			{
				std::cout << "clicked " << i<<"\n";
			}
		}

		gui.EndScrollArea(true);

		static int rightScrollbarOffset = 0;
		gui.BeginScrollArea(GEN_NULL_ID, size.x - 230, 10, 220, size.y-20, rightScrollbarOffset);

		root->DoGUI(gui);

		gui.Separator();
		if(gui.Button(GEN_NULL_ID, "Add node").onClicked)
		{
			id++;
			root->AddChild(new DragTreeNode("Node nr "+IntToStr(id)));
		}

		gui.EndScrollArea(false);

		gui.End();

		window.display();
	}

	return 0;
}
