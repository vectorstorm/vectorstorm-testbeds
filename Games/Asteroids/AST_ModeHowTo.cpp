/*
 *  AST_ModeHowTo.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 1/01/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#include "AST_ModeHowTo.h"
#include "AST_HowToPlay.h"
#include "AST_Game.h"

astModeHowTo::astModeHowTo(astGame *game):
	m_game(game),
	m_turningShip(NULL),
	m_thrustingShip(NULL)
{
//	m_turningShip = vsSprite::Load("PlayerShip");
//	m_turningShip->SetColor(vsColor::White);
}

astModeHowTo::~astModeHowTo()
{
//	delete m_turningShip;
}

void
astModeHowTo::Init()
{
//	m_turningShip->RegisterOnScene(1);
}

void
astModeHowTo::Deinit()
{
//	m_turningShip->Extract();
}

void
astModeHowTo::Update(float timeStep)
{
	UNUSED(timeStep);
	
	if ( m_game->GetInput()->WasPressed(CID_A) )
	{
		m_game->SetModeTitleScreen();
		m_game->GetHowToPlayScreen()->Show(false);
	}
}


