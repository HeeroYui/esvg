/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <esvg/debug.h>
#include <esvg/Ellipse.h>
#include <esvg/render/Path.h>
#include <esvg/render/Weight.h>

esvg::Ellipse::Ellipse(PaintState _parentPaintState) : esvg::Base(_parentPaintState) {
	
}

esvg::Ellipse::~Ellipse() {
	
}

bool esvg::Ellipse::parseXML(const exml::Element& _element, mat2& _parentTrans, vec2& _sizeMax) {
	if (_element.exist() == false) {
		return false;
	}
	parseTransform(_element);
	parsePaintAttr(_element);
	
	// add the property of the parrent modifications ...
	m_transformMatrix *= _parentTrans;
	
	m_c.setValue(0,0);
	m_r.setValue(0,0);
	
	std::string content = _element.attributes["cx"];
	if (content.size()!=0) {
		m_c.setX(parseLength(content));
	}
	content = _element.attributes["cy"];
	if (content.size()!=0) {
		m_c.setY(parseLength(content));
	}
	content = _element.attributes["rx"];
	if (content.size()!=0) {
		m_r.setX(parseLength(content));
	} else {
		ESVG_ERROR("(l "<<_element.getPos()<<") Ellipse \"rx\" is not present");
		return false;
	}
	content = _element.attributes["ry"];
	if (content.size()!=0) {
		m_r.setY(parseLength(content));
	} else {
		ESVG_ERROR("(l "<<_element.getPos()<<") Ellipse \"ry\" is not present");
		return false;
	}
	_sizeMax.setValue(m_c.x() + m_r.x(), m_c.y() + m_r.y());
	
	return true;
}

void esvg::Ellipse::display(int32_t _spacing) {
	ESVG_DEBUG(spacingDist(_spacing) << "Ellipse c=" << m_c << " r=" << m_r);
}


void esvg::Ellipse::draw(esvg::Renderer& _myRenderer, mat2& _basicTrans, int32_t _level) {
	ESVG_VERBOSE(spacingDist(_level) << "DRAW esvg::Ellipse");
	if (    m_r.x()<=0.0f
	     || m_r.y()<=0.0f) {
		ESVG_VERBOSE(spacingDist(_level+1) << "Too small radius" << m_r);
		return;
	}
	esvg::render::Path listElement;
	listElement.clear();
	listElement.moveTo(false, m_c + vec2(m_r.x(), 0.0f));
	listElement.curveTo(false,
	                    m_c + vec2(m_r.x(),                m_r.y()*esvg::kappa90),
	                    m_c + vec2(m_r.x()*esvg::kappa90,  m_r.y()),
	                    m_c + vec2(0.0f,                   m_r.y()));
	listElement.curveTo(false,
	                    m_c + vec2(-m_r.x()*esvg::kappa90, m_r.y()),
	                    m_c + vec2(-m_r.x(),               m_r.y()*esvg::kappa90),
	                    m_c + vec2(-m_r.x(),               0.0f));
	listElement.curveTo(false,
	                    m_c + vec2(-m_r.x(),               -m_r.y()*esvg::kappa90),
	                    m_c + vec2(-m_r.x()*esvg::kappa90, -m_r.y()),
	                    m_c + vec2(0.0f,                   -m_r.y()));
	listElement.curveTo(false,
	                    m_c + vec2(m_r.x()*esvg::kappa90,  -m_r.y()),
	                    m_c + vec2(m_r.x(),                -m_r.y()*esvg::kappa90),
	                    m_c + vec2(m_r.x(),                0.0f));
	listElement.close();
	
	mat2 mtx = m_transformMatrix;
	mtx *= _basicTrans;
	
	esvg::render::PointList listPoints;
	listPoints = listElement.generateListPoints(_level,
	                                            _myRenderer.getInterpolationRecurtionMax(),
	                                            _myRenderer.getInterpolationThreshold());
	//listPoints.applyMatrix(mtx);
	esvg::render::SegmentList listSegmentFill;
	esvg::render::SegmentList listSegmentStroke;
	esvg::render::Weight tmpFill;
	esvg::render::Weight tmpStroke;
	ememory::SharedPtr<esvg::render::DynamicColor> colorFill = esvg::render::createColor(m_paint.fill, mtx);
	ememory::SharedPtr<esvg::render::DynamicColor> colorStroke;
	if (m_paint.strokeWidth > 0.0f) {
		colorStroke = esvg::render::createColor(m_paint.stroke, mtx);
	}
	// Check if we need to display background
	if (colorFill != nullptr) {
		listSegmentFill.createSegmentList(listPoints);
		colorFill->setViewPort(listSegmentFill.getViewPort());
		listSegmentFill.applyMatrix(mtx);
		// now, traverse the scanlines and find the intersections on each scanline, use non-zero rule
		tmpFill.generate(_myRenderer.getSize(),
		                 _myRenderer.getNumberSubScanLine(),
		                 listSegmentFill);
	}
	// check if we need to display stroke:
	if (colorStroke != nullptr) {
		listSegmentStroke.createSegmentListStroke(listPoints,
		                                          m_paint.strokeWidth,
		                                          m_paint.lineCap,
		                                          m_paint.lineJoin,
		                                          m_paint.miterLimit);
		colorStroke->setViewPort(listSegmentStroke.getViewPort());
		listSegmentStroke.applyMatrix(mtx);
		// now, traverse the scanlines and find the intersections on each scanline, use non-zero rule
		tmpStroke.generate(_myRenderer.getSize(),
		                   _myRenderer.getNumberSubScanLine(),
		                   listSegmentStroke);
	}
	// add on images:
	_myRenderer.print(tmpFill,
	                  colorFill,
	                  tmpStroke,
	                  colorStroke,
	                  m_paint.opacity);
	#ifdef DEBUG
		_myRenderer.addDebugSegment(listSegmentFill);
		_myRenderer.addDebugSegment(listSegmentStroke);
	#endif
}


