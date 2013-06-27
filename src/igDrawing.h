#ifndef __IGDRAWING_H__
#define __IGDRAWING_H__

#include "igWidget.h"

#include <SFML/Graphics.hpp>

const igButton&		DrawButton(const igButton& button);
const igCheckbox&	DrawCheckbox(const igCheckbox& checkbox, sf::Texture& thickTexture);
const igTextbox&	DrawTextbox(const igTextbox& textbox);
const igLabel&		DrawLabel(const igLabel& label);
const igSlider&		DrawSlider(const igSlider& slider);
const igMove&		DrawMove(const igMove& move);
const igSeparator&	DrawSeparator(const igSeparator& separator);
const igAreaBG&		DrawAreaBG(const igAreaBG& scrollArea);
const igAreaFG&		DrawAreaFG(const igAreaFG& scrollArea);
const igDragged&	DrawDragged(const igDragged& dragged);

void				DrawDrag(const igDrag<void>& drag);

template <class R>
const igDrag<R>&	DrawDrag(const igDrag<R>& drag)
{
	igDrag<void> tmp;
	tmp.rect	= drag.rect;
	tmp.hover	= drag.hover;
	tmp.down	= drag.down;
	tmp.title	= drag.title;
	DrawDrag(tmp);
	return drag;
}

#endif