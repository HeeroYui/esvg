/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/math/Vector2D.hpp>
#include <esvg/render/Element.hpp>

namespace esvg {
	namespace render {
		class ElementLineToH : public esvg::render::Element {
			public:
				ElementLineToH(bool _relative, float _posX);
			public:
				virtual std::string display() const;
		};
	}
}

