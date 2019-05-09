/*
 *  LT_Graph
 *  MMORPG2
 *
 *  Created by Trevor Powell on 15-08-2011.
 *  Copyright 2011 Trevor Powell.  All rights reserved.
 *
 */

#include "LT_Graph.h"
#include "LT_ResponseCurve.h"

ltGraph::ltGraph():
	Parent(),
	m_curve(NULL),
	m_curveName(NULL),
	m_equationText(NULL)
{
	m_material = new vsMaterial("White");
	m_font = new vsFont("FreeSans.txt");

}

ltGraph::~ltGraph()
{
	vsDelete( m_curveName );
	vsDelete( m_equationText );
	vsDelete( m_font );
	vsDelete( m_material );
}

void
ltGraph::SetResponseCurve( ltResponseCurve *curve )
{
	m_curve = curve;

	SetCurveName( curve->GetName() );
	SetEquationText( curve->GetEquationText() );
}

void
ltGraph::DynamicDraw( vsRenderQueue *queue )
{
	vsDisplayList *list = queue->MakeTemporaryBatchList( m_material, 10 * 1024 );

	list->SetColor(vsColor::White);

	const int c_lineSegments = 100;
	float xIncrement = m_box.Width() / (c_lineSegments - 1);
	float yIncrement = m_box.Height();

	list->SetColor( vsColor::Blue );
	list->MoveTo( m_box.TopLeft() );
	list->LineTo( m_box.TopRight() );
	list->LineTo( m_box.BottomRight() );
	list->LineTo( m_box.BottomLeft() );
	list->LineTo( m_box.TopLeft() );

	list->SetColor( vsColor::Green );
	for( int i = 0; i < c_lineSegments; i++ )
	{
		float t = i / (float)c_lineSegments;
		float curveValue = m_curve->GetCurveValue(t);

		vsVector2D cursor( i*xIncrement, curveValue*yIncrement );
		cursor.x += m_box.GetMin().x;
		cursor.y = m_box.GetMax().y - cursor.y;

		if ( i == 0 )
			list->MoveTo(cursor);
		else
			list->LineTo(cursor);
	}
}

void
ltGraph::SetCurveName( const vsString &curveName )
{
	vsDelete(m_curveName)

	vsVector2D top = m_box.TopLeft();
	top.y += 20.f;

	vsFontRenderer fr(m_font, 35.f);
	m_curveName = new vsSprite(fr.DisplayList2D(curveName));
	AddChild( m_curveName );
	m_curveName->SetPosition( top );
}

void
ltGraph::SetEquationText( const vsString &equationText )
{
	vsDelete(m_equationText)

	vsVector2D bottom = m_box.BottomRight();
	bottom.y -= 20.f;

	vsFontRenderer fr(m_font,35.f);
	fr.SetJustificationType(Justification_Right);
	m_equationText = new vsSprite(fr.DisplayList2D(equationText));
	AddChild( m_equationText );
	m_equationText->SetPosition( bottom );
}

