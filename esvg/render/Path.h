/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <etk/math/Vector2D.h>
#include <esvg/render/Element.h>
#include <esvg/render/PointList.h>
#include <ememory/memory.h>
#ifdef DEBUG
	#include <esvg/render/SegmentList.h>
#endif

namespace esvg {
	namespace render {
		class Path {
			public:
				std::vector<ememory::SharedPtr<esvg::render::Element>> m_listElement;
				#ifdef DEBUG
					esvg::render::SegmentList m_debugInformation;
				#endif
			public:
				Path() {
					
				}
				
				~Path() {
					
				}
				void clear();
				void stop();
				void close(bool _relative=false);
				void moveTo(bool _relative, const vec2& _pos);
				void lineTo(bool _relative, const vec2& _pos);
				void lineToH(bool _relative, float _posX);
				void lineToV(bool _relative, float _posY);
				void curveTo(bool _relative, const vec2& _pos1, const vec2& _pos2, const vec2& _pos);
				void smoothCurveTo(bool _relative, const vec2& _pos2, const vec2& _pos);
				void bezierCurveTo(bool _relative, const vec2& _pos1, const vec2& _pos);
				void bezierSmoothCurveTo(bool _relative, const vec2& _pos);
				void ellipticTo(bool _relative,
				                const vec2& _radius,
				                float _angle,
				                bool _largeArcFlag,
				                bool _sweepFlag,
				                const vec2& _pos);
				void display(int32_t _spacing);
				esvg::render::PointList generateListPoints(int32_t _level, int32_t _recurtionMax = 10, float _threshold = 0.25f);
		};
	}
}

