/*
 *  LT_WindowSlide
 *  MMORPG2
 *
 *  Created by Trevor Powell on 17-08-2011.
 *  Copyright 2011 Trevor Powell.  All rights reserved.
 *
 */

#include "LT_WindowSlide.h"
#include "LT_Scenes.h"

ltWindowSlide::ltWindowSlide():
	ltLerpable("Window Slide"),
	m_window(NULL),
	m_timer(0.f),
	m_forward(false)
{
	m_window = new vsSprite;
}

ltWindowSlide::~ltWindowSlide()
{
	vsDelete( m_window );
}

void
ltWindowSlide::Init()
{
	Parent::Init();

	vsColor color(0.0f,0.0f,0.5f,1.0f);
	vsColor lineColor(1.0f,1.0f,1.0f,1.0f);

	vsBox2D box( vsVector2D::One * -300.f,
			vsVector2D::One * 300.f );

	m_window->AddFragment( vsMakeSolidBox2D( box, "Solid", &color ) );
	m_window->AddFragment( vsMakeOutlineBox2D( box, "Solid", &lineColor ) );
	m_window->RegisterOnScene(Scene_UI);

	m_timer = 0.f;
	m_forward = false;
}

void
ltWindowSlide::Deinit()
{
	m_window->Extract();
	Parent::Deinit();
}

void
ltWindowSlide::Update( float timeStep )
{
	if ( vsInput::Instance()->WasPressed(CID_A) )
	{
		m_forward = !m_forward;
	}
	m_timer += timeStep * (m_forward ? 1.0f : -1.0f);
	m_timer = vsClamp(0.f, m_timer, 1.f);

	float lerpValue = m_curve->GetCurveValue(m_timer);

	vsVector2D left(-300.f,0.f);
	vsVector2D right(300.f,0.f);

	m_window->SetPosition( vsInterpolate( lerpValue, left, right ) );
}

