/*
 *  SMX_Game.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 10/02/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#include "SMX_Game.h"
#include "SMX_ModeInGame.h"

//REGISTER_GAME("SpaceMazeExpress", smxGame)

smxGame::smxGame()
{
}

smxGame::~smxGame()
{
}

void
smxGame::Init()
{
	vsCollisionSystem::Instance()->SetWorldBounds(-500.f, 500.f, -450.f, 450.f);

	Parent::Init();

	m_mode[MODE_InGame] = new smxModeInGame(this);

	vsCollisionSystem::Instance()->SetGravity( vsVector2D(0.0f,100.0f) );
	SetGameMode(m_mode[MODE_InGame]);
}

void
smxGame::Deinit()
{
	SetGameMode(NULL);

	for ( int i = 0; i < MODE_MAX; i++ )
		vsDelete( m_mode[i] );

	Parent::Deinit();
}

