/*
 *  AST_HowToPlay.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 1/01/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#include "AST_HowToPlay.h"

astHowToPlay::astHowToPlay():
	vsSprite(NULL)
{
	SetVisible(false);
	m_show = false;
	
	m_title = new vsSprite(vsBuiltInFont::CreateString("Instructions", 35.0f, 55.0f, Justification_Center));
	m_turningText = new vsSprite(vsBuiltInFont::CreateString("* Push <LEFT> and <RIGHT> to turn", 25.0f, 35.0f, Justification_Center));
	m_shootingText = new vsSprite(vsBuiltInFont::CreateString("* Press <A> to shoot", 25.0f, 35.0f, Justification_Center));
	m_thrustingText = new vsSprite(vsBuiltInFont::CreateString("* Press <B> to thrust", 25.0f, 35.0f, Justification_Center));
	m_extraText = new vsSprite(vsBuiltInFont::CreateString("Destroy all the asteroids", 25.0f, 35.0f, Justification_Center));
	m_extraExtraText = new vsSprite(vsBuiltInFont::CreateString("To reach the next level!", 25.0f, 35.0f, Justification_Center));
	
	vsVector2D titlePos(0.f,-300.f);
	vsVector2D controlPos(0.f,-100.f);
	vsVector2D extraPos(0.f,100.f);
	vsVector2D spacing(0.f,50.f);

	m_title->SetPosition( titlePos );
	m_turningText->SetPosition( controlPos );
	m_shootingText->SetPosition( controlPos + spacing );
	m_thrustingText->SetPosition( controlPos + 2.f * spacing );
	m_extraText->SetPosition( extraPos );
	m_extraExtraText->SetPosition( extraPos + spacing );
	
	AddChild(m_title);
	AddChild(m_turningText);
	AddChild(m_shootingText);
	AddChild(m_thrustingText);
	AddChild(m_extraText);
	AddChild(m_extraExtraText);
}

astHowToPlay::~astHowToPlay()
{
}

void
astHowToPlay::Show(bool show)
{
	m_show = show;
	if ( show )
		SetVisible(true);
}

void
astHowToPlay::Update( float timeStep )
{
	if ( m_show )
		m_showFraction = vsMin(m_showFraction + timeStep, 1.f);
	else
		m_showFraction = vsMax(m_showFraction - timeStep, 0.f);
	
	SetColor( vsInterpolate( m_showFraction, vsColor::Black, vsColor::White ) );
}


