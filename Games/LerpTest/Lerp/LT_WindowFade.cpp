/*
 *  LT_WindowFade
 *  MMORPG2
 *
 *  Created by Trevor Powell on 17-08-2011.
 *  Copyright 2011 Trevor Powell.  All rights reserved.
 *
 */

#include "LT_WindowFade.h"
#include "LT_Scenes.h"

ltWindowFade::ltWindowFade():
	ltLerpable("Window Fade"),
	m_timer(0.f),
	m_forward(false)
{
	m_window = new vsSprite;
}

ltWindowFade::~ltWindowFade()
{
	vsDelete( m_window );
}

void
ltWindowFade::Init()
{
	m_window->RegisterOnScene(Scene_UI);
	Parent::Init();

	m_timer = 0.f;
	m_forward = false;
}

void
ltWindowFade::Deinit()
{
	Parent::Deinit();
	m_window->Extract();
}

void
ltWindowFade::Update( float timeStep )
{
	if ( vsInput::Instance()->WasPressed(CID_A) )
	{
		m_forward = !m_forward;
	}
	m_timer += timeStep * (m_forward ? 1.0f : -1.0f);
	m_timer = vsClamp(0.f, m_timer, 1.f);

	float lerpValue = m_curve->GetCurveValue(m_timer);

	m_window->ClearFragments();

	vsColor color(0.0f,0.0f,0.5f,1.0f);
	vsColor lineColor(1.0f,1.0f,1.0f,1.0f);

	color *= lerpValue;
	lineColor *= lerpValue;

	vsBox2D box( vsVector2D::One * -300.f,
			vsVector2D::One * 300.f );

	m_window->AddFragment( vsMakeSolidBox2D( box, "Solid", &color ) );
	m_window->AddFragment( vsMakeOutlineBox2D( box, "Solid", &lineColor ) );

}
