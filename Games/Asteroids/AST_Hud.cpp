/*
 *  AST_Hud.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 9/04/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#include "AST_Hud.h"
#include "AST_Game.h"

// HUD is actually an acronym for 'Heads Up Display'.  But I can't be
// fussed to hold down the shift key for that long, so this class is
// now and forever known as 'astHud'.

astHud::astHud( astGame * parent ):
	m_playerShip(NULL),
	m_scoreList(NULL),
	m_highScoreList(NULL),
	m_parent(parent),
	m_score(-1),
	m_highScore(-1),
	m_opacity(0.f),
	m_fadingFrom(0.f),
	m_fadingTo(0.f),
	m_fading(false)
{
	m_playerShip = vsDisplayList::Load("PlayerShip");

	BuildScoreLists();
}

astHud::~astHud()
{
	delete m_playerShip;
	if ( m_scoreList )
		delete m_scoreList;
	if ( m_highScoreList )
		delete m_highScoreList;
}

void
astHud::BuildScoreLists()
{
	int newScore = m_parent->GetScore();
	int newHighScore = m_parent->GetHighScore();

	if ( !m_scoreList || newScore != m_score )
	{
		if ( m_scoreList )
			delete m_scoreList;

		vsString scoreString = vsFormatString("%08d", newScore);
		m_scoreList = vsBuiltInFont::CreateString( scoreString, 30.0f );
		m_score = newScore;
	}
	if ( !m_highScoreList || newHighScore != m_highScore )
	{
		if ( m_highScoreList )
			delete m_highScoreList;

		vsString highscoreString = vsFormatString("%08d", newHighScore);
		m_highScoreList = vsBuiltInFont::CreateString( highscoreString, 20.0f );
		m_highScore = newHighScore;
	}
}

void
astHud::_Draw( vsDisplayList *list )
{
	// by default, our viewport extends -500 -> 500 vertically, and something more than that horizontally (based on aspect ratio)

	BuildScoreLists();

	vsTransform2D t = vsTransform2D::Zero;
	t.SetTranslation( vsVector2D(-460.f, -400.f) );

	list->SetColor( vsColor::Blue * m_opacity * m_opacity );	// ease in, slam out

	int livesRemaining = m_parent->GetPlayerLivesRemaining();
	if ( livesRemaining > 5 )
		livesRemaining = 5;	// only draw up to five icons, max.
	for ( int i = 0; i < livesRemaining; i++ )	// draw each of our player icons
	{
		list->PushTransform(t);
		list->Append(*m_playerShip);
		list->PopTransform();

		t.SetTranslation( t.GetTranslation() + vsVector2D(50.f,0.f) );
	}

	list->SetColor( vsColor::Blue * m_opacity );
	if ( m_scoreList )
	{
		t.SetTranslation(vsVector2D(-520.f, -440.f));
		list->PushTransform(t);
		list->Append(*m_scoreList);
		list->PopTransform();
	}

	if ( m_highScoreList )
	{
		t.SetTranslation(vsVector2D(-40.f, -440.f));
		list->PushTransform(t);
		list->Append(*m_highScoreList);
		list->PopTransform();
	}
}

void
astHud::Update(float timeStep)
{
	if ( m_fading )
	{
		m_fadeTimer += timeStep;

		if ( m_fadeTimer > 1.0f )
		{
			m_fadeTimer = 1.0f;
			m_fading = false;
		}

		m_opacity = vsInterpolate( m_fadeTimer, m_fadingFrom, m_fadingTo );
	}
}


