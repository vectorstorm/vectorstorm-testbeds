/*
 *  CT_CameraScript3D.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/01/11.
 *  Copyright 2011 Trevor Powell. All rights reserved.
 *
 */

#include "CT_CameraScript3D.h"

/*
 * The simplest possible 3D follow camera, commonly known as "camera on a stick".
 *
 */

void
ctCameraScript3D_BasicFollow::Update( float timeStep )
{
	m_position = m_targetPosition;
	//m_fov = 10.f;
}

