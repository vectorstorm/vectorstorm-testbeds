/*
 *  LT_Camera
 *  MMORPG2
 *
 *  Created by Trevor Powell on 18-08-2011.
 *  Copyright 2011 Trevor Powell.  All rights reserved.
 *
 */

#ifndef LT_CAMERA_H
#define LT_CAMERA_H

class ltCamera : public vsCamera3D
{
public:
	ltCamera();

	virtual void Update( float timeStep );
};

#endif /* LT_CAMERA_H */

