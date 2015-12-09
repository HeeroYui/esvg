/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */


#include <esvg/debug.h>
#include <esvg/Base.h>
#include <math.h>

const float esvg::kappa90(0.5522847493f);

#undef __class__
#define __class__	"PaintState"

esvg::PaintState::PaintState() :
  fill(std::pair<etk::Color<float,4>, std::string>(etk::color::black, "")),
  stroke(std::pair<etk::Color<float,4>, std::string>(etk::color::none, "")),
  strokeWidth(1.0f),
  flagEvenOdd(false),
  lineCap(esvg::cap_butt),
  lineJoin(esvg::join_miter),
  miterLimit(4.0f),
  viewPort(255,255),
  opacity(1.0) {
	
}

void esvg::PaintState::clear() {
	fill = std::pair<etk::Color<float,4>, std::string>(etk::color::black, "");
	stroke = std::pair<etk::Color<float,4>, std::string>(etk::color::none, "");
	strokeWidth = 1.0;
	viewPort.setValue(255,255);
	flagEvenOdd = false;
	lineJoin = esvg::join_miter;
	lineCap = esvg::cap_butt;
	miterLimit = 4.0f;
	opacity = 1.0;
}


#undef __class__
#define __class__	"Base"


esvg::Base::Base(PaintState _parentPaintState) {
	// copy the parent painting properties ...
	m_paint = _parentPaintState;
}

void esvg::Base::parseTransform(const std::shared_ptr<exml::Element>& _element) {
	if (_element == nullptr) {
		return;
	}
	std::string inputString = _element->getAttribute("transform");
	if (inputString.size() == 0) {
		return;
	}
	SVG_VERBOSE("find transform : \"" << inputString << "\"");
	for (int32_t iii=0; iii<inputString.size(); iii++) {
		if (inputString[iii] == ',') {
			inputString[iii] = ' ';
		}
	}
	SVG_VERBOSE("find transform : \"" << inputString << "\"");
	double matrix[6];
	float angle, xxx, yyy;
	int32_t n;
	const char * pointerOnData = inputString.c_str();
	while (*pointerOnData) {
		if (sscanf(pointerOnData, "matrix (%lf %lf %lf %lf %lf %lf) %n", &matrix[0], &matrix[1], &matrix[2], &matrix[3], &matrix[4], &matrix[5], &n) == 6) {
			m_transformMatrix = mat2(matrix);
		} else if (sscanf(pointerOnData, "translate (%f %f) %n", &xxx, &yyy, &n) == 2) {
			m_transformMatrix *= etk::mat2Translate(vec2(xxx, yyy));
			SVG_VERBOSE("Translate : " << xxx << ", " << yyy);
		} else if (sscanf(pointerOnData, "translate (%f) %n", &xxx, &n) == 1) {
			m_transformMatrix *= etk::mat2Translate(vec2(xxx, 0));
			SVG_VERBOSE("Translate : " << xxx << ", " << 0);
		} else if (sscanf(pointerOnData, "scale (%f %f) %n", &xxx, &yyy, &n) == 2) {
			m_transformMatrix *= etk::mat2Scale(vec2(xxx, yyy));
			SVG_VERBOSE("Scale : " << xxx << ", " << yyy);
		} else if (sscanf(pointerOnData, "scale (%f) %n", &xxx, &n) == 1) {
			m_transformMatrix *= etk::mat2Scale(xxx);
			SVG_VERBOSE("Scale : " << xxx << ", " << xxx);
		} else if (sscanf(pointerOnData, "rotate (%f %f %f) %n", &angle, &xxx, &yyy, &n) == 3) {
			angle = angle / 180 * M_PI;
			m_transformMatrix *= etk::mat2Translate(vec2(-xxx, -yyy));
			m_transformMatrix *= etk::mat2Rotate(angle);
			m_transformMatrix *= etk::mat2Translate(vec2(xxx, yyy));
		} else if (sscanf(pointerOnData, "rotate (%f) %n", &angle, &n) == 1) {
			angle = angle / 180 * M_PI;
			SVG_VERBOSE("rotate : " << angle << "rad, " << (angle/M_PI*180) << "°");
			m_transformMatrix *= etk::mat2Rotate(angle);
		} else if (sscanf(pointerOnData, "skewX (%f) %n", &angle, &n) == 1) {
			angle = angle / 180 * M_PI;
			SVG_VERBOSE("skewX : " << angle << "rad, " << (angle/M_PI*180) << "°");
			m_transformMatrix *= etk::mat2Skew(vec2(angle, 0.0f));
		} else if (sscanf(pointerOnData, "skewY (%f) %n", &angle, &n) == 1) {
			angle = angle / 180 * M_PI;
			SVG_VERBOSE("skewY : " << angle << "rad, " << (angle/M_PI*180) << "°");
			m_transformMatrix *= etk::mat2Skew(vec2(0.0f, angle));
		} else {
			break;
		}
		pointerOnData += n;
	}
}

void esvg::Base::parsePosition(const std::shared_ptr<const exml::Element>& _element, vec2 &_pos, vec2 &_size) {
	_pos.setValue(0,0);
	_size.setValue(0,0);
	
	if (_element == nullptr) {
		return;
	}
	std::string content = _element->getAttribute("x");
	if (content.size()!=0) {
		_pos.setX(parseLength(content));
	}
	content = _element->getAttribute("y");
	if (content.size()!=0) {
		_pos.setY(parseLength(content));
	}
	content = _element->getAttribute("width");
	if (content.size()!=0) {
		_size.setX(parseLength(content));
	}
	content = _element->getAttribute("height");
	if (content.size()!=0) {
		_size.setY(parseLength(content));
	}
}


std::pair<float, enum esvg::distance> esvg::Base::parseLength2(const std::string& _dataInput) {
	SVG_VERBOSE(" lenght : '" << _dataInput << "'");
	float n = stof(_dataInput);
	std::string unit;
	for (int32_t iii=0; iii<_dataInput.size(); ++iii) {
		if(    (_dataInput[iii]>='0' && _dataInput[iii]<='9')
		    || _dataInput[iii]=='+'
		    || _dataInput[iii]=='-'
		    || _dataInput[iii]=='.') {
			continue;
		}
		unit = std::string(_dataInput, iii);
		break;
	}
	SVG_VERBOSE(" lenght : '" << n << "' => unit=" << unit);
	// note : ";" is for the parsing of the style elements ...
	if(unit.size() == 0) {
		return std::make_pair(n, esvg::distance_pixel);
	} else if (unit[0] == '%') {     // xxx %
		return std::make_pair(n, esvg::distance_pourcent);
	} else if (    unit[0] == 'e'
	            && unit[1] == 'm') { // xxx em
		return std::make_pair(n, esvg::distance_element);
	} else if (    unit[0] == 'e'
	            && unit[1] == 'x') { // xxx ex
		return std::make_pair(n, esvg::distance_ex);
	} else if (    unit[0] == 'p'
	            && unit[1] == 'x') { // xxx px
		return std::make_pair(n, esvg::distance_pixel);
	} else if (    unit[0] == 'p'
	            && unit[1] == 't') { // xxx pt
		return std::make_pair(n, esvg::distance_point);
	} else if (    unit[0] == 'p'
	            && unit[1] == 'c') { // xxx pc
		return std::make_pair(n, esvg::distance_pc);
	} else if (    unit[0] == 'm'
	            && unit[1] == 'm') { // xxx mm
		return std::make_pair(n, esvg::distance_millimeter);
	} else if (    unit[0] == 'c'
	            && unit[1] == 'm') { // xxx cm
		return std::make_pair(n, esvg::distance_centimeter);
	} else if (    unit[0] == 'i'
	            && unit[1] == 'n') { // xxx in
		return std::make_pair(n, esvg::distance_inch);
	}
	return std::make_pair(0.0f, esvg::distance_pixel);
}


float esvg::Base::parseLength(const std::string& _dataInput) {
	std::pair<float, enum esvg::distance> value = parseLength2(_dataInput);
	SVG_VERBOSE(" lenght : '" << value.first << "' => unit=" << value.second);
	float font_size = 20.0f;
	switch (value.second) {
		case esvg::distance_pourcent:
			return value.first / 100.0 * m_paint.viewPort.x();
		case esvg::distance_element:
			return value.first * font_size;
		case esvg::distance_ex:
			return value.first / 2.0f * font_size;
		case esvg::distance_pixel:
			return value.first;
		case esvg::distance_point:
			return value.first * 1.25f;
		case esvg::distance_pc:
			return value.first * 15.0f;
		case esvg::distance_millimeter:
			return value.first * 3.543307f;
		case esvg::distance_centimeter:
			return value.first * 35.43307f;
		case esvg::distance_inch:
			return value.first * 90.0f;
	}
	return 0.0f;
}

void esvg::Base::parsePaintAttr(const std::shared_ptr<const exml::Element>& _element) {
	if (_element == nullptr) {
		return;
	}
	/*
	bool fillNone = false;
	bool strokeNone = false;
	*/
	std::string content;
	// ---------------- get unique ID ----------------
	m_id = _element->getAttribute("id");
	// ---------------- stroke ----------------
	content = _element->getAttribute("stroke");
	if (content.size()!=0) {
		m_paint.stroke = parseColor(content);
		/*
		if (m_paint.stroke.first.a() == 0) {
			strokeNone = true;
		}
		*/
	}
	content = _element->getAttribute("stroke-width");
	if (content.size()!=0) {
		m_paint.strokeWidth = parseLength(content);
	}
	content = _element->getAttribute("stroke-opacity");
	if (content.size()!=0) {
		float opacity = parseLength(content);
		opacity = std::avg(0.0f, opacity, 1.0f);
		m_paint.stroke.first.setA(opacity);
	}
	
	content = _element->getAttribute("stroke-dasharray");
	if (content.size()!=0) {
		if (content == "none" ) {
			// OK, Nothing to do ...
		} else {
			SVG_TODO(" 'stroke-dasharray' not implemented ...");
		}
	}
	content = _element->getAttribute("stroke-linecap");
	if (content.size()!=0) {
		if (content == "butt" ) {
			m_paint.lineCap = esvg::cap_butt;
		} else if (content == "round" ) {
			m_paint.lineCap = esvg::cap_round;
		} else if (content == "square" ) {
			m_paint.lineCap = esvg::cap_square;
		} else {
			m_paint.lineCap = esvg::cap_butt;
			SVG_ERROR("not know stroke-linecap value : \"" << content << "\", not in [butt,round,square]");
		}
	}
	content = _element->getAttribute("stroke-linejoin");
	if (content.size()!=0) {
		if (content == "miter" ) {
			m_paint.lineJoin = esvg::join_miter;
		} else if (content == "round" ) {
			m_paint.lineJoin = esvg::join_round;
		} else if (content == "bevel" ) {
			m_paint.lineJoin = esvg::join_bevel;
		} else {
			m_paint.lineJoin = esvg::join_miter;
			SVG_ERROR("not know stroke-linejoin value : \"" << content << "\", not in [miter,round,bevel]");
		}
	}
	content = _element->getAttribute("stroke-miterlimit");
	if (content.size()!=0) {
		float tmp = parseLength(content);
		m_paint.miterLimit = std::max(0.0f, tmp);
	}
	// ---------------- FILL ----------------
	content = _element->getAttribute("fill");
	if (content.size()!=0) {
		m_paint.fill = parseColor(content);
		/*
		if (m_paint.fill.a() == 0) {
			fillNone = true;
		}
		*/
	}
	content = _element->getAttribute("fill-opacity");
	if (content.size()!=0) {
		float opacity = parseLength(content);
		opacity = std::avg(0.0f, opacity, 1.0f);
		m_paint.fill.first.setA(opacity);
	}
	content = _element->getAttribute("fill-rule");
	if (content.size()!=0) {
		if (content == "nonzero") {
			m_paint.flagEvenOdd = false;
		} else if (content == "evenodd" ) {
			m_paint.flagEvenOdd = true;
		} else {
			SVG_ERROR("not know fill-rule value : \"" << content << "\", not in [nonzero,evenodd]");
		}
	}
	// ---------------- opacity ----------------
	content = _element->getAttribute("opacity");
	if (content.size()!=0) {
		m_paint.opacity = parseLength(content);
		m_paint.opacity = std::avg(0.0f, m_paint.opacity, 1.0f);
	}
	// Note : No parsing of 'style' it is already converted in attribute before...
	// check if somewere none is set to the filling:
	/*
	if (fillNone == true) {
		m_paint.fill.setA(0.0f);
	}
	if (strokeNone == true) {
		m_paint.stroke.setA(0.0f);
	}
	*/
}

std::pair<etk::Color<float,4>, std::string> esvg::Base::parseColor(const std::string& _inputData) {
	std::pair<etk::Color<float,4>, std::string> localColor(etk::color::white, "");
	
	if(    _inputData.size() > 4
	    && _inputData[0] == 'u'
	    && _inputData[1] == 'r'
	    && _inputData[2] == 'l'
	    && _inputData[3] == '(') {
		if (_inputData[4] == '#') {
			std::string color(_inputData.begin() + 5, _inputData.end()-1);
			localColor = std::pair<etk::Color<float,4>, std::string>(etk::color::none, color);
		} else {
			SVG_ERROR(" pb in parsing the color : \"" << _inputData << "\"  == > url(XXX) is not supported now ...");
		}
	} else {
		localColor = std::pair<etk::Color<float,4>, std::string>(_inputData, "");
	}
	SVG_VERBOSE("Parse color : \"" << _inputData << "\"  == > " << localColor.first << " " << localColor.second);
	return localColor;
}

bool esvg::Base::parseXML(const std::shared_ptr<exml::Element>& _element, mat2& _parentTrans, vec2& _sizeMax) {
	// TODO : UNDERSTAND why nothing is done here ...
	// Parse basic elements (ID...):
	m_id = _element->getAttribute("id");
	_sizeMax = vec2(0.0f, 0.0f);
	return false;
}


const char * esvg::Base::spacingDist(int32_t _spacing) {
	static const char *tmpValue = "                                                                                ";
	if (_spacing>20) {
		_spacing = 20;
	}
	return tmpValue + 20*4 - _spacing*4;
}

void esvg::Base::draw(esvg::Renderer& _myRenderer, mat2& _basicTrans, int32_t _level) {
	SVG_WARNING(spacingDist(_level) << "DRAW esvg::Base ... ==> No drawing availlable");
}



const std::string& esvg::Base::getId() const {
	return m_id;
}

void esvg::Base::setId(const std::string& _newId) {
	// TODO : Check if it is UNIQUE ...
	m_id = _newId;
}

