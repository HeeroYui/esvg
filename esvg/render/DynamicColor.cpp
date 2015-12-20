/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <esvg/debug.h>
#include <esvg/render/DynamicColor.h>
#include <esvg/LinearGradient.h>
#include <esvg/esvg.h>

#undef __class__
#define __class__	"render:DynamicColorLinear"

esvg::render::DynamicColorLinear::DynamicColorLinear(const std::string& _link, const mat2& _mtx) :
  m_colorName(_link),
  m_matrix(_mtx),
  m_viewPort(vec2(9999999999.0,9999999999.0),vec2(-9999999999.0,-9999999999.0)) {
	
}

void esvg::render::DynamicColorLinear::setViewPort(const std::pair<vec2, vec2>& _viewPort) {
	m_viewPort = _viewPort;
}


static vec2 getIntersect(const vec2& _point1,
                         const vec2& _vect1,
                         const vec2& _point2,
                         const vec2& _vect2) {
	float diviseur = _vect1.x() * _vect2.y() - _vect1.y() * _vect2.x();
	if(diviseur != 0.0f) {
		float mmm = (   _vect1.x() * _point1.y()
		              - _vect1.x() * _point2.y()
		              - _vect1.y() * _point1.x()
		              + _vect1.y() * _point2.x()
		            ) / diviseur;
		return vec2(_point2 + _vect2 * mmm);
	}
	ESVG_ERROR("Get divider / 0.0f");
	return _point2;
}

// TODO : This can optimize ... really slow ... and not linear
etk::Color<float,4> esvg::render::DynamicColorLinear::getColor(const ivec2& _pos) {
	if (m_data.size() < 2) {
		return etk::color::purple;
	}
	float ratio = 0.0f;
	if (m_unit == gradientUnits_userSpaceOnUse) {
		vec2 vectorBase = m_pos2 - m_pos1;
		vec2 vectorOrtho(vectorBase.y(), -vectorBase.x());
		vec2 intersec = getIntersect(m_pos1,                   vectorBase,
		                             vec2(_pos.x(), _pos.y()), vectorOrtho);
		float baseSize = vectorBase.length();
		vec2 vectorBaseDraw = intersec - m_pos1;
		float baseDraw = vectorBaseDraw.length();
		ratio = baseDraw / baseSize;
		switch(m_spread) {
			case spreadMethod_pad:
				if (vectorBase.dot(vectorBaseDraw) < 0) {
					ratio *= -1.0;
				}
				break;
			case spreadMethod_reflect:
				ratio -= float((int32_t(ratio)>>1)<<1);
				if (ratio > 1.0f) {
					ratio = 2.0f-ratio;
				}
				break;
			case spreadMethod_repeat:
				if (vectorBase.dot(vectorBaseDraw) < 0) {
					ratio *= -1.0;
				}
				ratio -= float(int32_t(ratio));
				if (ratio <0.0f) {
					ratio = 1.0f-std::abs(ratio);
				}
				break;
		}
	} else {
		// in the basic vertion of the gradient the color is calculated with the ration in X and Y in the bonding box associated (it is rotate with the object..
		vec2 intersecX = getIntersect(m_pos1,                   m_axeX,
		                              vec2(_pos.x(), _pos.y()), m_axeY);
		vec2 intersecY = getIntersect(m_pos1,                   m_axeY,
		                              vec2(_pos.x(), _pos.y()), m_axeX);
		vec2 vectorBaseDrawX = intersecX - m_pos1;
		vec2 vectorBaseDrawY = intersecY - m_pos1;
		float baseDrawX = vectorBaseDrawX.length();
		float baseDrawY = vectorBaseDrawY.length();
		if (m_axeX.dot(vectorBaseDrawX) < 0) {
			baseDrawX *= -1.0f;
		}
		if (m_axeY.dot(vectorBaseDrawY) < 0) {
			baseDrawY *= -1.0f;
		}
		if (m_baseSize.x()+m_baseSize.y() != 0.0f) {
			if (    m_baseSize.x() != 0.0f
			     && m_baseSize.y() != 0.0f) {
				ratio = (baseDrawX*m_baseSize.y() + baseDrawY*m_baseSize.x())/(m_baseSize.x()*m_baseSize.y()*2.0f);
			} else if (m_baseSize.x() != 0.0f) {
				ratio = baseDrawX/m_baseSize.x();
			} else {
				ratio = baseDrawY/m_baseSize.y();
			}
		} else {
			ratio = 1.0f;
		}
		switch(m_spread) {
			case spreadMethod_pad:
				// nothing to do ...
				break;
			case spreadMethod_reflect:
				ratio = std::abs(ratio);
				ratio -= float((int32_t(ratio)>>1)<<1);
				if (ratio > 1.0f) {
					ratio = 2.0f-ratio;
				}
				break;
			case spreadMethod_repeat:
				ratio -= float(int32_t(ratio));
				if (ratio <0.0f) {
					ratio = 1.0f-std::abs(ratio);
				}
				break;
		}
	}
	if (ratio <= m_data[0].first*0.01f) {
		return m_data[0].second;
	}
	if (ratio >= m_data.back().first*0.01f) {
		return m_data.back().second;
	}
	for (size_t iii=1; iii<m_data.size(); ++iii) {
		if (ratio <= m_data[iii].first*0.01f) {
			float localRatio = ratio - m_data[iii-1].first*0.01f;
			localRatio = localRatio / ((m_data[iii].first - m_data[iii-1].first) * 0.01f);
			return etk::Color<float,4>(m_data[iii-1].second.r() * (1.0-localRatio) + m_data[iii].second.r() * localRatio,
			                           m_data[iii-1].second.g() * (1.0-localRatio) + m_data[iii].second.g() * localRatio,
			                           m_data[iii-1].second.b() * (1.0-localRatio) + m_data[iii].second.b() * localRatio,
			                           m_data[iii-1].second.a() * (1.0-localRatio) + m_data[iii].second.a() * localRatio);
		}
	}
	return etk::color::green;
}

void esvg::render::DynamicColorLinear::generate(esvg::Document* _document) {
	if (_document == nullptr) {
		ESVG_ERROR("Get nullptr input for document");
		return;
	}
	std::shared_ptr<esvg::Base> base = _document->getReference(m_colorName);
	if (base == nullptr) {
		ESVG_ERROR("Can not get base : '" << m_colorName << "'");
		return;
	}
	std::shared_ptr<esvg::LinearGradient> gradient = std::dynamic_pointer_cast<esvg::LinearGradient>(base);
	if (gradient == nullptr) {
		ESVG_ERROR("Can not cast in a linear gradient: '" << m_colorName << "' ==> wrong type");
		return;
	}
	ESVG_INFO("get for color linear:");
	gradient->display(2);
	m_unit = gradient->m_unit;
	m_spread = gradient->m_spread;
	ESVG_INFO("    viewport = {" << m_viewPort.first << "," << m_viewPort.second << "}");
	vec2 size = m_viewPort.second - m_viewPort.first;
	
	esvg::Dimension dimPos1 = gradient->getPosition1();
	m_pos1 = dimPos1.getPixel(size);
	if (dimPos1.getType() == esvg::distance_pourcent) {
		m_pos1 += m_viewPort.first;
	}
	esvg::Dimension dimPos2 = gradient->getPosition2();
	m_pos2 = dimPos2.getPixel(size);
	if (dimPos2.getType() == esvg::distance_pourcent) {
		m_pos2 += m_viewPort.first;
	}
	// in the basic vertion of the gradient the color is calculated with the ration in X and Y in the bonding box associated (it is rotate with the object..
	vec2 delta = m_pos2 - m_pos1;
	if (delta.x() < 0.0f) {
		m_axeX = vec2(-1.0f, 0.0f);
	} else {
		m_axeX = vec2(1.0f, 0.0f);
	}
	if (delta.y() < 0.0f) {
		m_axeY = vec2(0.0f, -1.0f);
	} else {
		m_axeY = vec2(0.0f, 1.0f);
	}
	// Move the positions ...
	m_pos1 = m_matrix * m_pos1;
	m_pos2 = m_matrix * m_pos2;
	m_axeX = m_matrix.applyScaleRotation(m_axeX);
	m_axeY = m_matrix.applyScaleRotation(m_axeY);
	// in the basic vertion of the gradient the color is calculated with the ration in X and Y in the bonding box associated (it is rotate with the object..
	vec2 intersecX = getIntersect(m_pos1, m_axeX,
	                              m_pos2, m_axeY);
	vec2 intersecY = getIntersect(m_pos1, m_axeY,
	                              m_pos2, m_axeX);
	m_baseSize = vec2((m_pos1 - intersecX).length(),
	                  (m_pos1 - intersecY).length());
	// get all the colors
	m_data = gradient->getColors(_document);
}


#undef __class__
#define __class__	"render:DynamicColor"

std::shared_ptr<esvg::render::DynamicColor> esvg::render::createColor(std::pair<etk::Color<float,4>, std::string> _color, const mat2& _mtx) {
	// Check if need to create a color:
	if (    _color.first.a() == 0x00
	     && _color.second == "") {
	     return nullptr;
	}
	if (_color.second != "") {
		return std::make_shared<esvg::render::DynamicColorLinear>(_color.second, _mtx);
	}
	return std::make_shared<esvg::render::DynamicColorUni>(_color.first);
}
