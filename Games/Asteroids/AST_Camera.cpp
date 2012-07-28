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
	m_transform.SetTranslation(vsVector2D(0.0f,0.0f));
	m_transform.SetScale(vsVector2D(80.0f,80.0f));	// we see 800 units around the position.  (vertically)
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
		m_transform.SetTranslation( vsInterpolate( 0.05f, m_transform.GetTranslation(), m_trackSprite->m_transform.GetTranslation() ) );
		m_transform.SetAngle( vsInterpolate( 0.05f, m_transform.GetAngle(), m_trackSprite->m_transform.GetAngle() ) );
	}
}
