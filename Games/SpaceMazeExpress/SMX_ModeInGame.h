/*
 *  SMX_ModeInGame.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 10/02/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#ifndef SMX_MODEINGAME_H
#define SMX_MODEINGAME_H

class smxGame;
class smxMaze;
class smxPlayerShip;

#define BOX_COUNT (40)

const int smxColFlag_Rope = ColFlag_User1;

class smxModeInGame : public coreGameMode
{
	smxGame *			m_game;
	smxMaze *			m_maze;
	smxPlayerShip *		m_player;
	vsPhysicsSprite *		m_box[BOX_COUNT];

	vsPhysicsEmitter	*		m_sparkEmitter;

public:
					smxModeInGame( smxGame * game );
	virtual			~smxModeInGame();

	virtual void	Init();
	virtual void	Deinit();

	virtual void	Update( float timeStep );
};

#endif // SMX_MODEINGAME_H

