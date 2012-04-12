/*
 *  AST_ModeGameOver.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 29/12/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#ifndef AST_MODEGAMEOVER_H
#define AST_MODEGAMEOVER_H

class astGame;

class astModeGameOver : public coreGameMode
{
	astGame *				m_game;
	vsSprite *				m_gameOverSprite;
	
	float					m_timer;
	float					m_fade;
	bool					m_exitting;
	
public:
	
			astModeGameOver( astGame *game );
	virtual ~astModeGameOver();
	
	void	Init();
	void	Deinit();
	
	void	Update(float timeStep);
};

#endif // AST_MODEGAMEOVER_H

