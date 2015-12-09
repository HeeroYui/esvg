/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <esvg/debug.h>
#include <esvg/Text.h>

#undef __class__
#define __class__	"Text"

esvg::Text::Text(PaintState _parentPaintState) : esvg::Base(_parentPaintState) {
	
}

esvg::Text::~Text() {
	
}

bool esvg::Text::parse(const std::shared_ptr<exml::Element>& _element, mat2& _parentTrans, vec2& _sizeMax) {
	_sizeMax.setValue(0,0);
	ESVG_ERROR("NOT IMPLEMENTED");
	return false;
}

void esvg::Text::display(int32_t _spacing) {
	ESVG_DEBUG(spacingDist(_spacing) << "Text");
}


