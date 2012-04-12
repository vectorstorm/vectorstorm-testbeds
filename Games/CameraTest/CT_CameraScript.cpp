/*
 *  CT_CameraScript.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 3/01/11.
 *  Copyright 2011 Trevor Powell. All rights reserved.
 *
 */

#include "CT_CameraScript.h"

#include "CT_Player.h"	// for scripts which need more information about the player.

/*
 * A static camera, showing the full level.
 *
 */

void
ctCameraScript_Overview::Update( float timeStep )
{
	m_position.Set( 0.f, -5.f );
	m_fov = 50.f;
	m_overrideFOV = true;
}

/*
 * The simplest possible follow camera, in 3D games, commonly known as "camera on a stick".
 *
 */

void
ctCameraScript_BasicFollow::Update( float timeStep )
{
	m_position = m_targetPosition;
	//m_fov = 10.f;
}

/*
 * A camera which attaches a spring to the player, and is dragged around by it.
 *
 */

ctCameraScript_SpringFollow::ctCameraScript_SpringFollow():
	m_spring( vsVector2D::One * 90.f, 0.1f )
{
}

void
ctCameraScript_SpringFollow::Reset()
{
	m_spring.SetPosition( m_targetPosition );
	m_spring.SetCenter( m_targetPosition );
}

void
ctCameraScript_SpringFollow::Update( float timeStep )
{
	m_spring.SetCenter( m_targetPosition );
	m_position = m_spring.Update( timeStep );
}

/*
 * A camera which interpolates toward the player.
 *
 */

void
ctCameraScript_LerpFollow::Reset()
{
	m_position = m_targetPosition;
}

void
ctCameraScript_LerpFollow::Update( float timeStep )
{
	m_position = vsInterpolate( 0.1f, m_position, m_targetPosition );
}

/*
 * Our first camera script with two states:  It starts in "unfollow" state, where it sits still.  If the player moves too 
 * far, it switches to "follow" state, where it uses a spring to follow.  If the player stays relatively still for a few 
 * seconds, the camera will switch back to "unfollow" state.
 *
 */

ctCameraScript_LooseFollow::ctCameraScript_LooseFollow():
m_spring( vsVector2D::One * 60.f, 0.1f )
{
}

void
ctCameraScript_LooseFollow::Reset()
{
	m_following = false;
	m_unfollowTimer = 0.f;
	m_spring.SetPosition( m_targetPosition );
	m_spring.SetCenter( m_targetPosition );
	m_position = m_targetPosition;
}

void
ctCameraScript_LooseFollow::Update( float timeStep )
{
	vsVector2D delta = m_targetPosition - m_position;
	
	if ( delta.Length() > 5.f )
	{
		m_following = true;
		m_unfollowTimer = 3.f;
	}
	
	if ( m_following )
	{
		m_spring.SetCenter( m_targetPosition );
		m_unfollowTimer -= timeStep;
		
		if ( m_unfollowTimer <= 0.f )
		{
			m_following = false;
		}
	}
	
	m_position = m_spring.Update( timeStep );
}

/*
 * LooseLead is like LooseFollow, except that it tries to move ahead of the player, so that the player can see what's in front of him.
 *
 */

ctCameraScript_LooseLead::ctCameraScript_LooseLead():
m_spring( vsVector2D::One * 60.f, 0.1f )
{
}

void
ctCameraScript_LooseLead::Reset()
{
	m_following = false;
	m_spring.SetPosition( m_targetPosition );
	m_spring.SetCenter( m_targetPosition );
	m_position = m_targetPosition;
}

void
ctCameraScript_LooseLead::Update( float timeStep )
{
	float delta = m_targetPosition.x - m_position.x;
	
	if ( vsFabs(delta) > 4.f )
	{
		m_following = true;
	}
	
	if ( m_following )
	{
		vsVector2D predictedPosition = m_targetPosition;
		predictedPosition.x += ctPlayer::Instance()->GetVelocity().x * 2.f;
		m_spring.SetCenter( predictedPosition );
		
		if ( vsFabs(ctPlayer::Instance()->GetVelocity().x) <= 0.f )
		{
			m_following = false;
		}
	}
	else
	{
		m_spring.SetCenter( m_position );
	}
	
	m_position = m_spring.Update( timeStep );
}

/*
 * LooseLeadGround is like LooseLead, except that it tries to stay down at the level of the ground.
 *
 */

ctCameraScript_LooseLeadGround::ctCameraScript_LooseLeadGround():
m_spring( vsVector2D::One * 60.f, 0.1f )
{
}

void
ctCameraScript_LooseLeadGround::Reset()
{
	m_following = false;
	m_spring.SetPosition( m_targetPosition );
	m_spring.SetCenter( m_targetPosition );
	m_position = m_targetPosition;
}

void
ctCameraScript_LooseLeadGround::Update( float timeStep )
{
	float delta = m_targetPosition.x - m_position.x;
	
	if ( vsFabs(delta) > 4.f )
	{
		m_following = true;
	}
	
	if ( m_following )
	{
		vsVector2D predictedPosition = m_targetPosition;

		if ( ctPlayer::Instance()->InTheAir() )
		{
			predictedPosition.y = m_lastGroundHeight;
		}
		
		predictedPosition.x += ctPlayer::Instance()->GetVelocity().x * 2.f;
		m_spring.SetCenter( predictedPosition );
		
		if ( vsFabs(ctPlayer::Instance()->GetVelocity().x) <= 0.f )
		{
			m_following = false;
		}
	}
	else
	{
		m_spring.SetCenter( m_position );
	}

	if ( !ctPlayer::Instance()->InTheAir() )
	{
		m_lastGroundHeight = m_targetPosition.y;
	}
	
	m_position = m_spring.Update( timeStep );
}
