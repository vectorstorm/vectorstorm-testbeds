/*
 *  BNG_Game.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 28/04/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#include "SST_Game.h"
#include "SST_Fireworks.h"
#include "SST_Hud.h"
#include "SST_ModeInGame.h"
#include "SST_Rocket.h"
#include "SST_Sequence.h"


//#define RECORDING_ALLOWED		// disabled in most builds, so people don't accidentally tromp over the initial data when they're trying to figure out how to play.

REGISTER_GAME("StarShot", sstGame);


sstGame::sstGame()
{
	m_sceneCount = 2;
}

void
sstGame::Init()
{
	Parent::Init();

	m_modeInGame = new sstModeInGame(this);

	SetGameMode( m_modeInGame );
}

void
sstGame::Deinit()
{
	SetGameMode( NULL );
	vsDelete( m_modeInGame );

	Parent::Deinit();
}

void
sstGame::Update(float timeStep)
{
	UNUSED(timeStep);
}
