/*
 *  AST_ModeHowTo.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 1/01/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#ifndef AST_MODEHOWTO_H
#define AST_MODEHOWTO_H

class astGame;

class astModeHowTo : public coreGameMode
{
	astGame *		m_game;
	vsSprite *		m_turningShip;
	vsSprite *		m_thrustingShip;
public:
	
					astModeHowTo(astGame *game);
					~astModeHowTo();
	
	virtual void	Init();
	virtual void	Deinit();
	
	virtual void	Update( float timeStep );
};

#endif // AST_MODEHOWTO_H

