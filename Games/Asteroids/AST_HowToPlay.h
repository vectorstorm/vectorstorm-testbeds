/*
 *  AST_HowToPlay.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 1/01/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#ifndef AST_HOWTOPLAY_H
#define AST_HOWTOPLAY_H

class astHowToPlay : public vsSprite
{
	vsSprite *	m_title;
	vsSprite *	m_ship;
	vsSprite *	m_turningText;
	vsSprite *	m_thrustingText;
	vsSprite *	m_shootingText;
	vsSprite *	m_extraText;
	vsSprite *	m_extraExtraText;
	
	float	m_showFraction;	// [0..1] from invisible to visible
	
	bool	m_show;
	
public:
	
			astHowToPlay();
			~astHowToPlay();

	void	Show(bool show);	// transition in/out nicely.
	
	bool	FullyVisible() { return (m_showFraction == 1.0f); }
	
	virtual void	Update(float timeStep);
};

#endif // AST_HOWTOPLAY_H


