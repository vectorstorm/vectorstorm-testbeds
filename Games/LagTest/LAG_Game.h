/*
 *  LAG_Game.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 10/02/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#ifndef LAG_GAME_H
#define LAG_GAME_H

class lagGame : public coreGame
{
	typedef coreGame Parent;
	
	vsSprite *		m_sprite;

public:
	
					lagGame();
	virtual			~lagGame();
	
	virtual void	Init();
	virtual void	Deinit();
};

#endif // LAG_GAME_H

