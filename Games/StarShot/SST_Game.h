/*
 *  BNG_Game.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 28/04/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

class sstModeInGame;

class sstGame : public coreGame, public vsSingleton<sstGame>
{
	typedef coreGame Parent;
	
	sstModeInGame *m_modeInGame;
	
public:
	
	sstGame();
	
	virtual void Init();
	virtual void Deinit();
	
	void	Update(float timeStep);
};

