/*
 *  VP_Game.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/02/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#ifndef VP_GAME_H
#define VP_GAME_H

#define MAX_DRAWABLES	(200)
class vpDrawable;

class vpGame : public coreGame
{
	typedef coreGame Parent;
	
	int				m_drawMode;
	
	vpDrawable *	m_drawable[MAX_DRAWABLES];
	vsSprite *		m_cursor;
	vsCamera2D *		m_camera;
	
	vsSprite *		m_staticButton;
	vsSprite *		m_ropeButton;
	vsSprite *		m_normalButton;
	
public:
	
	vpGame();
	~vpGame();
	
	virtual void Init();
	virtual void Deinit();
	
	virtual void Update(float timeStep);
};

#endif // VP_GAME_H

