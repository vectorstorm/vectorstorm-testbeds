/*
 *  AST_ModeInGame.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 29/12/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#include "AST_ModeInGame.h"

#include "AST_Game.h"
#include "AST_PlayerShip.h"

astModeInGame::astModeInGame(astGame *game, astPlayerShip *player):
	m_game(game),
	m_player(player)
{
}

astModeInGame::~astModeInGame()
{
}

void
astModeInGame::Init()
{
	m_game->ShowHUD(true);
}

void
astModeInGame::Deinit()
{
}

void
astModeInGame::Update(float timeStep)
{
	if ( !m_player->IsSpawned() )	// if no player is currently on the level
	{
		if ( m_game->GetPlayerLivesRemaining() )		// check if we have any lives left
		{
			m_game->AttemptPlayerSpawn(timeStep);
		}
		else						// no lives left
		{
			// if we haven't decided the game is over, put our "Game Over" message into the HUD and set
			// the 'game over' flag.
			
			m_game->SetModeGameOver();
			return;
		}
	}
}


