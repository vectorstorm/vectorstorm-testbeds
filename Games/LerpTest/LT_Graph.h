/*
 *  LT_Graph
 *  MMORPG2
 *
 *  Created by Trevor Powell on 15-08-2011.
 *  Copyright 2011 Trevor Powell.  All rights reserved.
 *
 */

#ifndef LT_GRAPH_H
#define LT_GRAPH_H

class ltResponseCurve;

class ltGraph : public vsSprite
{
	typedef vsSprite Parent;

	ltResponseCurve *m_curve;
	vsMaterial *m_material;
	vsBox2D m_box;
	vsSprite *			m_curveName;
	vsSprite *			m_equationText;
	vsFont *			m_font;

	void				SetCurveName( const vsString &name );
	void				SetEquationText( const vsString &text );

public:

	ltGraph();
	virtual ~ltGraph();

	void SetBox( const vsBox2D &box ) { m_box = box; }
	void SetResponseCurve( ltResponseCurve *curve );

	virtual void DynamicDraw( vsRenderQueue *queue );
};

#endif //LT_GRAPH_H

