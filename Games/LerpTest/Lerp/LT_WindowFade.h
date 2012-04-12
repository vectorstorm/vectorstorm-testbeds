/*
 *  LT_WindowFade
 *  MMORPG2
 *
 *  Created by Trevor Powell on 17-08-2011.
 *  Copyright 2011 Trevor Powell.  All rights reserved.
 *
 */

#ifndef LT_WINDOWFADE_H
#define LT_WINDOWFADE_H

#include "LT_Lerpable.h"

class ltWindowFade: public ltLerpable
{
	typedef ltLerpable Parent;
	vsSprite *m_window;
	float	m_timer;
	bool	m_forward;

public:
	ltWindowFade();
	virtual ~ltWindowFade();

	virtual void Init();
	virtual void Deinit();
	virtual void Update( float timestep );
};

#endif /* LT_WINDOWFADE_H */

