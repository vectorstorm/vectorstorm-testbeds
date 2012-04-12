/*
 *  LT_Lerpable
 *  MMORPG2
 *
 *  Created by Trevor Powell on 17-08-2011.
 *  Copyright 2011 Trevor Powell.  All rights reserved.
 *
 */

#include "LT_Lerpable.h"

ltLerpable::ltLerpable( const vsString &name ):
    m_name(name),
    m_selected(false),
	m_font(NULL),
    m_curve(NULL)
{
	m_font = new vsFont("FreeSerif.txt");
	RebuildGraphics();
}

ltLerpable::~ltLerpable()
{
	vsDelete( m_font );
}

void
ltLerpable::Init()
{
	m_selected = true;
	RebuildGraphics();
}

void
ltLerpable::Deinit()
{
	m_selected = false;
	RebuildGraphics();
}

void
ltLerpable::RebuildGraphics()
{
	vsColor color = vsColor::White;
	if ( !m_selected )
	{
		color.Set(0.2f,0.2f,0.2f,1.f);
	}

	ClearFragments();
	AddFragment( m_font->CreateString_Fragment( FontContext_2D, m_name, 35.f, Justification_Left, -1, color) );
}
