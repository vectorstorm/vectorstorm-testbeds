/*
 *  LT_CameraMove
 *  MMORPG2
 *
 *  Created by Trevor Powell on 18-08-2011.
 *  Copyright 2011 Trevor Powell.  All rights reserved.
 *
 */
#ifndef LT_CAMERAMOVE_H
#define LT_CAMERAMOVE_H

#include "LT_Lerpable.h"

class ltCameraMove: public ltLerpable
{
	typedef ltLerpable Parent;
	vsModel *m_box;
	vsModel *m_world;
	float	m_timer;
	bool	m_forward;

public:
	ltCameraMove();
	virtual ~ltCameraMove();

	virtual void Init();
	virtual void Deinit();
	virtual void Update( float timestep );
};

#endif /* LT_CAMERAMOVE_H */

