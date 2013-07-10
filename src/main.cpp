#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

#include "igDrawing.h"

#include "IMGUI.h"

sf::RenderWindow* gWindow = 0;
sf::Font* gFont = 0;
sf::Texture* gCheckboxTex = 0;

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
			if(DrawCheckbox(gui.Checkbox(GEN_ID(*this), expand, 0), *gCheckboxTex).clicked)
				expand = !expand;
		}
		if(DragTreeNode* child = DrawDrag(gui.Drag<DragTreeNode>(GEN_ID(*this), name.c_str(), *this)).drop)
		{
			AddChild(child);
		}
		if(!expand)
			return;
		//gui.Indent();
		for(int i=0; i<children.size(); i++)
			children[i]->DoGUI(gui, indent+16+igSizing::SCROLLAREA_MARGIN_X);
		//gui.Unindent();
	}
};

const igMove& DrawSizer(const igMove& sizer, sf::Texture& tex)
{
	igRect r = sizer.rect;
	sf::Sprite spr(tex);
	spr.setPosition(sizer.prevX, sizer.prevY);
	spr.setScale((float)r.w/tex.getSize().x, (float)r.h/tex.getSize().y);
	gWindow->draw(spr);
	return sizer;
}

#include "gfxlib.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1024, 768), "TermEdit");
	sf::View view;
	gWindow = &window;

	sf::Font font;
	gFont = &font;
	if(!font.loadFromFile("data/SourceSansPro-Regular.otf"))
        	return EXIT_FAILURE;

	sf::Texture checkboxTex;
	gCheckboxTex = &checkboxTex;
	checkboxTex.loadFromFile("data/checkbox.png");

	sf::Texture sizerTex;
	sizerTex.loadFromFile("data/sizer.png");

	igContext gui;

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

	float k = 1.23f;

	float size1 = 250;
	float size2 = 250;
	
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
				if(event.key.control)
				{
					if(event.key.code == sf::Keyboard::C)
						gui.ctrlC = true;
					if(event.key.code == sf::Keyboard::X)
						gui.ctrlX = true;
					if(event.key.code == sf::Keyboard::V)
						gui.ctrlV = true;
					if(event.key.code == sf::Keyboard::A)
						gui.ctrlA = true;
				}
			}
		}

		static sf::Color bgColor(0,0,0,255);
		window.clear(bgColor);

		sf::Vector2u size = gWindow->getSize();
		
		//guiRend.time = timer.getElapsedTime().asMilliseconds();

		gui.Begin();

		static int movey = 0;

		static int leftScrollbarOffset = 0;
		DrawAreaBG(gui.BeginScrollArea(GEN_NULL_ID, size.x - size2-8-size1, 0, size2, size.y, leftScrollbarOffset, true, igColor(70/255.0f, 70/255.0f, 70/255.0f)));

		DrawButton(gui.Button(GEN_NULL_ID, "X", 32));
		DrawLabel(gui.Label("Properties", igTextAlign::LEFT));
		DrawSeparator(gui.Separator());
		DrawButton(gui.Button(GEN_NULL_ID, "Button 1"));
		DrawButton(gui.Button(GEN_NULL_ID, "Button 2"));

		std::stringstream bleh;
		bleh << k;
		std::string kk = bleh.str();

		DrawTextbox(gui.TextBox(GEN_NULL_ID, kk));

		bleh.str("");
		bleh << kk;
		bleh >> k;

		DrawLabel(gui.Label("Clear color", igTextAligns::LEFT));
		float slidVal = bgColor.r/255.0f;
		DrawLabel(gui.Label("R", igTextAligns::CENTER, 20));
		if(DrawSlider(gui.Slider(GEN_NULL_ID, slidVal, 0, 1, 120)))
			bgColor.r = slidVal*255.0f+0.5f;
		std::stringstream rs;
		rs << (int)bgColor.r;
		std::string strr = rs.str();
		DrawTextbox(gui.TextBox(GEN_NULL_ID, strr));
		slidVal = bgColor.g/255.0f;
		DrawLabel(gui.Label("G", igTextAligns::CENTER, 20));
		if(DrawSlider(gui.Slider(GEN_NULL_ID, slidVal, 0, 1)))
			bgColor.g = slidVal*255.0f+0.5f;
		slidVal = bgColor.b/255.0f;
		DrawLabel(gui.Label("B", igTextAligns::CENTER, 20));
		if(DrawSlider(gui.Slider(GEN_NULL_ID, slidVal, 0, 1)))
			bgColor.b = slidVal*255.0f+0.5f;
		if(DrawCheckbox(gui.Checkbox(GEN_NULL_ID, checkboxValue, "checkbox"), checkboxTex).clicked)
			checkboxValue = !checkboxValue;
		if(checkboxValue)
		{
			DrawButton(gui.Button(GEN_NULL_ID, "Button 3"));
			DrawButton(gui.Button(GEN_NULL_ID, "Button 4"));
			DrawButton(gui.Button(GEN_NULL_ID, "Button 1"));
			DrawButton(gui.Button(GEN_NULL_ID, "Button 2"));
			DrawButton(gui.Button(GEN_NULL_ID, "Button 3"));
			DrawButton(gui.Button(GEN_NULL_ID, "Button 4"));
			DrawButton(gui.Button(GEN_NULL_ID, "Button 1"));
			DrawButton(gui.Button(GEN_NULL_ID, "Button 2"));
			DrawButton(gui.Button(GEN_NULL_ID, "Button 3"));
			DrawButton(gui.Button(GEN_NULL_ID, "Button 4"));
		}
		static std::string text1 = "Edit me!";
		DrawTextbox(gui.TextBox(GEN_NULL_ID, text1, 100));
		DrawButton(gui.Button(GEN_NULL_ID, "do it!"));
		static std::string text2 = "Or me!";
		DrawTextbox(gui.TextBox(GEN_NULL_ID, text2));
		for(int i=0; i<50; i++)
		{
			if(DrawButton(gui.Button(GEN_IID(i), "Button 1")).clicked)
			{
				std::cout << "clicked " << i<<"\n";
			}
		}

		DrawAreaFG(gui.EndScrollArea());

		static int rightScrollbarOffset = 0;
		DrawAreaBG(gui.BeginScrollArea(GEN_NULL_ID, size.x - size1, 0, size1, size.y, rightScrollbarOffset, true,igColor(70/255.0f, 70/255.0f, 70/255.0f)));

		DrawButton(gui.Button(GEN_NULL_ID, "X", 32));
		DrawLabel(gui.Label("Hierarchy", igTextAlign::LEFT));
		DrawSeparator(gui.Separator());
		if(DrawButton(gui.Button(GEN_NULL_ID, "Add node")).clicked)
		{
			id++;
			root->AddChild(new DragTreeNode("Node nr "+IntToStr(id)));
		}
		DrawSeparator(gui.Separator());

		root->DoGUI(gui);

		DrawAreaFG(gui.EndScrollArea());

		int ddd=0;
		DrawAreaBG(gui.BeginScrollArea(GEN_NULL_ID, size.x-64-size1-size2-16, 0, 64, size.y, ddd, false, igColor(50/255.0f, 50/255.0f, 50/255.0f)));
		DrawButton(gui.Button(GEN_NULL_ID, "Hier"));
		DrawButton(gui.Button(GEN_NULL_ID, "Prop"));
		DrawButton(gui.Button(GEN_NULL_ID, "PSys"));
		DrawButton(gui.Button(GEN_NULL_ID, "Ent"));
		DrawButton(gui.Button(GEN_NULL_ID, "Tex"));
		DrawAreaFG(gui.EndScrollArea());

		int movex = size.x-size1-8-size2-8;
		DrawSizer(gui.Move(GEN_NULL_ID, movex, movey, 8, size.y, ""), sizerTex);
		movey = 0;
		size2 = size.x-size1-8-movex-8;
		if(size2<150) size2 = 150;;
		movex = size.x-size1-8;
		DrawSizer(gui.Move(GEN_NULL_ID, movex, movey, 8, size.y, ""), sizerTex);
		movey = 0;
		size1 = size.x-movex-8;
		if(size1<150) size1 = 150;

		DrawDragged(gui.End());

		window.display();
	}

	return 0;
}
