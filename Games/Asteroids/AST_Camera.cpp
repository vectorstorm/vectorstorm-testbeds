/*
 *  AST_Camera.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 25/03/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#include "AST_Camera.h"


astCamera::astCamera()
{
	m_transform.m_position.Set(0.0f,0.0f);
	m_transform.m_scale.Set(80.0f,80.0f);	// we see 800 units around the position.  (vertically)
	m_trackSprite = NULL;
	m_trackPlayer = false;
}

astCamera::~astCamera()
{
}

void
astCamera::Update(float timeStep)
{
	UNUSED(timeStep);

	if ( vsInput::Instance()->WasPressed( CID_MouseLeftButton ) )
	{
		m_trackPlayer = !m_trackPlayer;
	}

	if ( m_trackPlayer && m_trackSprite )
	{
		m_transform.m_position = vsInterpolate( 0.05f, m_transform.m_position, m_trackSprite->m_transform.m_position );
		m_transform.m_angle = vsInterpolate( 0.05f, m_transform.m_angle, m_trackSprite->m_transform.m_angle );
	}
}
