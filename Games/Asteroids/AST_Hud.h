/*
 *  AST_Hud.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 9/04/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#ifndef AST_HUD_H
#define AST_HUD_H

class astGame;

class astHud : public vsEntity
{
	vsDisplayList *		m_playerShip;
	vsDisplayList *		m_scoreList;
	vsDisplayList *		m_highScoreList;
	
	astGame *			m_parent;
	
	int					m_score;		// this is the score that we've built into 'm_scoreList';  we don't need to rebuild that display list except when this value changes!
	int					m_highScore;
	
	float				m_opacity;
	float				m_fadingFrom;
	float				m_fadingTo;
	float				m_fadeTimer;
	bool				m_fading;
	
	void			BuildScoreLists();
	
	void			FadeTo(float opacity) { m_fading = true; m_fadingFrom = m_opacity; m_fadingTo = opacity; m_fadeTimer = 0.f; }
	
protected:
	virtual void	_Draw( vsDisplayList *list );
public:
	
			astHud( astGame * parent );
	virtual ~astHud();
	
	virtual void	Update(float timeStep);
	
	void			FadeIn() { FadeTo(1.0f); }
	void			FadeOut() { FadeTo(0.0f); }
	
};

#endif // AST_HUD_H
