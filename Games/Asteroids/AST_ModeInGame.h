/*
 *  AST_ModeInGame.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 29/12/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#ifndef AST_MODEINGAME_H
#define AST_MODEINGAME_H


class astPlayerShip;
class astGame;

class astModeInGame : public coreGameMode
{
	astGame *		m_game;
	astPlayerShip *	m_player;
	
public:
	astModeInGame(astGame *game, astPlayerShip *player);
	virtual ~astModeInGame();
	
	virtual void	Init();		// called when this game mode should initialise its data and get ready to run
	virtual void	Deinit();	// called when this game mode needs to shut itself down and delete its data
	
	virtual void	Update( float timeStep );
	
};

#endif // AST_MODEINGAME_H

