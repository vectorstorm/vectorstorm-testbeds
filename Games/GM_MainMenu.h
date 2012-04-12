/*
 *  GM_MainMenu.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 18/03/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#ifndef GM_MAINMENU_H
#define GM_MAINMENU_H

class mmStretchSprite : public vsSprite
{
	float			m_timer;
	
public:
	mmStretchSprite( vsDisplayList *list, float delay );

	virtual void	Update(float timeStep);
};

class gmMainMenu : public coreGame
{
	typedef coreGame Parent;
	
	vsShader *	m_shader;
	vsEntity *	m_shaderEntity;
	
	vsSprite *	m_title;
	vsSprite *	m_letter[11];
	
	vsSprite *	m_build;
	vsSprite *	m_buildName;
	
	vsSprite *	m_stripes[2];
	
	vsSprite **	m_menuSprite;
	
	vsMusic *	m_music;
	vsSoundSample *	m_tickSound;
	
	float		m_pulseTimer;
	
	int			m_menuSpriteCount;
	int			m_selection;
	
	void				BuildTitleSprite();
	
public:
	
				gmMainMenu();
//	virtual		~gmMainMenu();
		
	virtual void		Update(float timeStep);
	
	virtual void		Init();
	virtual void		Deinit();
};

#endif // GM_MAINMENU_H
