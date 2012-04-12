/*
 *  SMX_ModeInGame.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 10/02/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#include "SMX_ModeInGame.h"

#include "SMX_Game.h"
#include "SMX_Maze.h"
#include "SMX_PlayerShip.h"
#include "SMX_Rope.h"

smxModeInGame::smxModeInGame( smxGame *game ):
	m_game(game)
{
}

smxModeInGame::~smxModeInGame()
{
}

void
smxModeInGame::Init()
{

	m_maze = new smxMaze( 1000, 1000, 3, 3 );
	m_maze->RegisterOnScene(0);
	m_maze->FadeIn(0.5f);

	m_sparkEmitter = new vsPhysicsEmitter("Thrust", 0.1f, 160, 0);
	m_player = new smxPlayerShip(m_sparkEmitter);
	m_player->RegisterOnScene(0);
	m_player->Spawn( vsVector2D::Zero );

	for ( int i = 0; i < BOX_COUNT; i++ )
	{
		m_box[i] = new vsPhysicsSprite(vsDisplayList::Load("Box"), 0.25f, ColFlag_Enemy, ColFlag_All, true);
		m_box[i]->SetColor(vsRandom::GetColor(0.7f,1.0f));
		m_box[i]->SetCollisionsActive(true);
		m_box[i]->SetPosition( vsVector2D(vsRandom::GetFloat(-500.f,500.f), vsRandom::GetFloat(100.f) + 200.f) );
		m_box[i]->RegisterOnScene(0);
	}
}

void
smxModeInGame::Deinit()
{
	for ( int i = 0; i < BOX_COUNT; i++ )
		vsDelete( m_box[i] );
	vsDelete( m_player );
//	vsDelete( m_rope );
	vsDelete( m_sparkEmitter );
	vsDelete( m_maze );
}

void
smxModeInGame::Update( float timeStep )
{
	m_sparkEmitter->Update( timeStep );
}

