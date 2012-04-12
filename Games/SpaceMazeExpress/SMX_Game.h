/*
 *  SMX_Game.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 10/02/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#ifndef SMX_GAME_H
#define SMX_GAME_H

class smxGame : public coreGame
{
	typedef coreGame Parent;
	enum
	{
		MODE_InGame,
		MODE_MAX
	};
	
	coreGameMode *			m_mode[MODE_MAX];

public:
	
					smxGame();
	virtual			~smxGame();
	
	virtual void	Init();
	virtual void	Deinit();
};

#endif // SMX_GAME_H

