/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <esvg/debug.h>
#include <esvg/LinearGradient.h>
#include <esvg/RadialGradient.h>
#include <esvg/render/Path.h>
#include <esvg/render/Weight.h>
#include <esvg/esvg.h>

#undef __class__
#define __class__	"RadialGradient"

esvg::RadialGradient::RadialGradient(PaintState _parentPaintState) :
  esvg::Base(_parentPaintState),
  m_center(vec2(50,50), esvg::distance_pourcent),
  m_radius(50, esvg::distance_pourcent),
  m_focal(vec2(50,50), esvg::distance_pourcent),
  m_unit(gradientUnits_objectBoundingBox),
  m_spread(spreadMethod_pad) {
	
}

esvg::RadialGradient::~RadialGradient() {
	
}


bool esvg::RadialGradient::parseXML(const std::shared_ptr<exml::Element>& _element, mat2& _parentTrans, vec2& _sizeMax) {
	// line must have a minimum size...
	//m_paint.strokeWidth = 1;
	if (_element == nullptr) {
		return false;
	}
	
	// ---------------- get unique ID ----------------
	m_id = _element->getAttribute("id");
	
	//parseTransform(_element);
	//parsePaintAttr(_element);
	
	// add the property of the parrent modifications ...
	m_transformMatrix *= _parentTrans;
	
	std::string contentX = _element->getAttribute("cx");
	std::string contentY = _element->getAttribute("cy");
	if (    contentX != ""
	     && contentY != "") {
		m_center.set(contentX, contentY);
	}
	contentX = _element->getAttribute("r");
	if (contentX != "") {
		m_radius.set(contentX);
	}
	contentX = _element->getAttribute("fx");
	contentY = _element->getAttribute("fy");
	if (    contentX != ""
	     && contentY != "") {
		m_focal.set(contentX, contentY);
	}
	contentX = _element->getAttribute("gradientUnits");
	if (contentX == "userSpaceOnUse") {
		m_unit = gradientUnits_userSpaceOnUse;
	} else {
		m_unit = gradientUnits_objectBoundingBox;
		if (    contentX.size() != 0
		     && contentX != "objectBoundingBox") {
			ESVG_ERROR("Parsing error of 'gradientUnits' ==> not suported value: '" << contentX << "' not in : {userSpaceOnUse/objectBoundingBox} use objectBoundingBox");
		}
	}
	contentX = _element->getAttribute("spreadMethod");
	if (contentX == "reflect") {
		m_spread = spreadMethod_reflect;
	} else if (contentX == "repeat") {
		m_spread = spreadMethod_repeat;
	} else {
		m_spread = spreadMethod_pad;
		if (    contentX.size() != 0
		     && contentX != "pad") {
			ESVG_ERROR("Parsing error of 'spreadMethod' ==> not suported value: '" << contentX << "' not in : {reflect/repeate/pad} use pad");
		}
	}
	// note: xlink:href is incompatible with subNode "stop"
	m_href = _element->getAttribute("xlink:href");
	if (m_href.size() != 0) {
		m_href = std::string(m_href.begin()+1, m_href.end());
	}
	// parse all sub node :
	for(int32_t iii=0; iii<_element->size() ; iii++) {
		std::shared_ptr<exml::Element> child = _element->getElement(iii);
		if (child == nullptr) {
			// can be a comment ...
			continue;
		}
		if (child->getValue() == "stop") {
			float offset = 100;
			etk::Color<float,4> stopColor = etk::color::none;
			std::string content = child->getAttribute("offset");
			if (content.size()!=0) {
				std::pair<float, enum esvg::distance> tmp = parseLength2(content);
				if (tmp.second == esvg::distance_pixel) {
					// special case ==> all time % then no type define ==> % in [0.0 .. 1.0]
					offset = tmp.first*100.0f;
				} else if (tmp.second != esvg::distance_pourcent) {
					ESVG_ERROR("offset : " << content << " res=" << tmp.first << "," << tmp.second << " Not support other than pourcent %");
				} else {
					offset = tmp.first;
				}
			}
			content = child->getAttribute("stop-color");
			if (content.size()!=0) {
				stopColor = parseColor(content).first;
				ESVG_VERBOSE(" color : \"" << content << "\"  == > " << stopColor);
			}
			content = child->getAttribute("stop-opacity");
			if (content.size()!=0) {
				float opacity = parseLength(content);
				opacity = std::avg(0.0f, opacity, 1.0f);
				stopColor.setA(opacity);
				ESVG_VERBOSE(" opacity : \"" << content << "\"  == > " << stopColor);
			}
			m_data.push_back(std::pair<float, etk::Color<float,4>>(offset, stopColor));
		} else {
			ESVG_ERROR("(l " << child->getPos() << ") node not suported : \"" << child->getValue() << "\" must be [stop]");
		}
	}
	if (m_data.size() != 0) {
		if (m_href != "") {
			ESVG_ERROR("(l " << _element->getPos() << ") node can not have an xlink:href element with sub node named: stop ==> removing href");
			m_href = "";
		}
	}
	return true;
}

void esvg::RadialGradient::display(int32_t _spacing) {
	ESVG_DEBUG(spacingDist(_spacing) << "RadialGradient center=" << m_center << " focal=" << m_focal << " radius=" << m_radius);
	for (auto &it : m_data) {
		ESVG_DEBUG(spacingDist(_spacing+1) << "STOP: offset=" << it.first << " color=" << it.second);
	}
}

void esvg::RadialGradient::draw(esvg::Renderer& _myRenderer, mat2& _basicTrans, int32_t _level) {
	ESVG_VERBOSE(spacingDist(_level) << "DRAW esvg::RadialGradient");
}

const esvg::Dimension& esvg::RadialGradient::getCenter() {
	return m_center;
}

const esvg::Dimension& esvg::RadialGradient::getFocal() {
	return m_focal;
}

const esvg::Dimension1D& esvg::RadialGradient::getRadius() {
	return m_radius;
}

const std::vector<std::pair<float, etk::Color<float,4>>>& esvg::RadialGradient::getColors(esvg::Document* _document) {
	if (m_href == "") {
		return m_data;
	}
	if (_document == nullptr) {
		ESVG_ERROR("Get nullptr input for document");
		return m_data;
	}
	std::shared_ptr<esvg::Base> base = _document->getReference(m_href);
	if (base == nullptr) {
		ESVG_ERROR("Can not get base : '" << m_href << "'");
		return m_data;
	}
	std::shared_ptr<esvg::RadialGradient> gradientR = std::dynamic_pointer_cast<esvg::RadialGradient>(base);
	if (gradientR == nullptr) {
		std::shared_ptr<esvg::LinearGradient> gradientL = std::dynamic_pointer_cast<esvg::LinearGradient>(base);
		if (gradientL == nullptr) {
			ESVG_ERROR("Can not cast in a linear/radial gradient: '" << m_href << "' ==> wrong type");
			return m_data;
		}
		return gradientL->getColors(_document);
	}
	return gradientR->getColors(_document);
}



