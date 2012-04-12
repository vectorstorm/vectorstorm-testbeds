/*
 *  AST_ModeTitleScreen.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 29/12/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#include "AST_ModeTitleScreen.h"

#include "AST_Game.h"
#include "AST_HowToPlay.h"

enum
{
	NewGame,
	HowToPlay,
	Options,
	Quit
};

static vsVector2D titlePos(-600.0f,-300.0f);
static vsVector2D menuPos(100.0f,100.0f);

static const float c_transitionDuration = 1.0f;

astModeTitleScreen::astModeTitleScreen( astGame *game ):
	m_game(game)
{
	m_menu = new vsSimpleMenu( 4, 25.0f, 35.0f, 20.0f );
	m_menu->SetItemLabel(NewGame, "New Game");
	m_menu->SetItemLabel(HowToPlay, "How To Play");
	m_menu->SetItemLabel(Options, "Options");
	m_menu->SetItemLabel(Quit, "Quit");

	m_title = new vsSprite( vsBuiltInFont::CreateString(vsString("Asteroids"), 60.0f, 80.0f) );
}

astModeTitleScreen::~astModeTitleScreen()
{
	delete m_title;
	delete m_menu;
}

void
astModeTitleScreen::Init()
{
	m_title->RegisterOnScene(1);

	m_menu->Enter();
	m_menu->RegisterOnScene(1);

	m_game->ShowHUD(false);
	m_game->ShowAsteroids(true);

	m_transitioningIn = true;
	m_transitioningOut = false;
	m_transitionTimer = 0.f;

	m_iniTitlePos = titlePos;
	m_iniMenuPos = menuPos;

	m_iniTitlePos.y = vsSystem::GetScreen()->GetScene(1)->GetTopLeftCorner().y;
	m_iniMenuPos.x = vsSystem::GetScreen()->GetScene(1)->GetTopRightCorner().x;

	m_menu->SetPosition(m_iniMenuPos);
	m_title->SetPosition(m_iniTitlePos);
	m_title->SetColor(vsColor::Black);
}

void
astModeTitleScreen::Deinit()
{
	m_title->Extract();
	m_menu->Extract();

	m_transitioningIn = false;
	m_transitioningOut = false;
}

void
astModeTitleScreen::Update( float timeStep )
{
	UNUSED(timeStep);

	if ( m_transitioningIn || m_transitioningOut )
	{

		m_transitionTimer += timeStep;

		if ( m_transitionTimer > c_transitionDuration )
		{
			m_transitionTimer = c_transitionDuration;
			if ( m_transitioningOut )
			{
				if ( m_transitioningOutTo == NewGame )
					m_game->SetModeInGame();
				else if ( m_transitioningOutTo == HowToPlay )
					m_game->SetModeHowTo();
			}
			m_transitioningIn = false;
			m_transitioningOut = false;
		}

		float timer = m_transitionTimer;
		if ( m_transitioningOut )
			timer = c_transitionDuration - m_transitionTimer;

		float transitionFraction = timer / c_transitionDuration;
		transitionFraction = (3.0f * transitionFraction * transitionFraction) - (2.0f * transitionFraction * transitionFraction * transitionFraction);

		m_menu->SetPosition( vsInterpolate(transitionFraction, m_iniMenuPos, menuPos) );
		m_title->SetPosition( vsInterpolate(transitionFraction, m_iniTitlePos, titlePos) );
		m_title->SetColor( vsInterpolate(transitionFraction, vsColor::Black, vsColor::White) );
	}

	if ( !m_transitioningIn && !m_transitioningOut )
	{
		if ( m_menu->WasActionTaken() )
		{
			vsSimpleMenuAction a = m_menu->GetAction();

			if ( a.type == vsSimpleMenuAction::Type_Select )
			{
				switch( a.menuItemId )
				{
					case NewGame:
						m_transitioningOut = true;
						m_transitionTimer = 0.f;
						m_transitioningOutTo = NewGame;
						break;
					case HowToPlay:
						m_transitioningOut = true;
						m_transitionTimer = 0.f;
						m_transitioningOutTo = HowToPlay;
						m_game->GetHowToPlayScreen()->Show(true);
						break;
					case Quit:
						core::SetExitToMenu();
						break;
					default:
						break;
				}
			}
		}
	}
}


