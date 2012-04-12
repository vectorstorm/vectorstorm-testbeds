/*
 *  AST_ModeTitleScreen.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 29/12/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#ifndef AST_MODETITLESCREEN_H
#define AST_MODETITLESCREEN_H

class astGame;

class astModeTitleScreen : public coreGameMode
{
	astGame *	m_game;

	vsSprite *	m_title;
	vsSimpleMenu *	m_menu;

	vsVector2D	m_iniTitlePos;
	vsVector2D	m_iniMenuPos;

	float		m_transitionTimer;

	bool		m_transitioningIn;
	bool		m_transitioningOut;

	int			m_transitioningOutTo;

public:

	astModeTitleScreen( astGame *game );
	~astModeTitleScreen();

	void Init();
	void Deinit();

	void Update( float timeStep );
};

#endif // AST_MODETITLESCREEN_H
