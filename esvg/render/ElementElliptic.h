/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __ESVG_RENDER_ELEMENT_ELLIPTIC_H__
#define __ESVG_RENDER_ELEMENT_ELLIPTIC_H__

#include <etk/types.h>
#include <etk/math/Vector2D.h>
#include <esvg/render/Element.h>

namespace esvg {
	namespace render {
		class ElementElliptic : public esvg::render::Element {
			private:
				float m_angle;
				bool m_largeArcFlag;
				bool m_sweepFlag;
			public:
				ElementElliptic(bool _relative,
				                const vec2& _radius, // in m_pos1
				                float _angle,
				                bool _largeArcFlag,
				                bool _sweepFlag,
				                const vec2& _pos);
			public:
				virtual std::string display() const;
		};
	}
}

#endif

