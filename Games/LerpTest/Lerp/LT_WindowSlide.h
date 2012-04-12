/*
 *  LT_WindowSlide
 *  MMORPG2
 *
 *  Created by Trevor Powell on 17-08-2011.
 *  Copyright 2011 Trevor Powell.  All rights reserved.
 *
 */

#ifndef LT_WINDOWSLIDE_H
#define LT_WINDOWSLIDE_H

#include "LT_Lerpable.h"

class ltWindowSlide: public ltLerpable
{
	typedef ltLerpable Parent;
	vsSprite *m_window;
	float	m_timer;
	bool	m_forward;

public:
	ltWindowSlide();
	virtual ~ltWindowSlide();

	virtual void Init();
	virtual void Deinit();
	virtual void Update( float timestep );
};

#endif /* LT_WINDOWSLIDE_H */
