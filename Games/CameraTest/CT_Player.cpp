/*
 *  CT_Player.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 3/01/11.
 *  Copyright 2011 Trevor Powell. All rights reserved.
 *
 */

#include "CT_Player.h"

#include "CT_Stage.h"

ctPlayer::ctPlayer( const vsBox2D &dims ):
	m_world(dims)
{
	AddChild( vsSprite::Load("Player.vsp") );

	BuildBoundingBox();
}

ctPlayer::~ctPlayer()
{
}

void
ctPlayer::Update( float timeStep )
{
	vsInput *i = vsInput::Instance();

	if ( i->IsDown( CID_A ) || i->IsDown( CID_B ) )	// no movement while a modifier is down.  In CT, that's for adjusting cameras.
		return;

	float leftRight = i->GetState( CID_LeftRightLAxis );

	vsVector2D position = GetPosition();

	const float c_speed = 5.f;	// 5 meters per second, maximum speed
	float acceleration = 20.f;	// 10 meters per second per second, maximum acceleration

	float heightAboveGround = ctStage::Instance()->HeightAboveGround( GetPosition(), GetBoundingBox() );

	// Am I in the air?  If so, slow down our acceleration, for slower air control.  Also, apply gravity.
	if ( heightAboveGround > 0.1f )
	{
		acceleration = 8.f;
		float c_gravity = 50.f;	// adjust to taste.  (Remember, down is POSITIVE Y in our coordinate system)

		if ( i->IsDown( CID_Up ) && m_velocity.y < 0.f )
			c_gravity = 20.f;

		m_velocity.y += c_gravity * timeStep;
	}
	else
	{
		// I'm not in the air, so snap me to the floor, then test if I want to jump.

		position.y += heightAboveGround;

		bool jump = i->WasPressed( CID_Up );
		m_velocity.y = 0.f;

		if ( jump )
		{
			// arbitrary jump initial velocity.  Larger numbers == higher jumps.
			//
			// It's possible to solve a parabola to learn the correct initial velocity for a particular
			// jump height, but you really don't want to do that;  jumping never feels right if you use
			// real physics.

			m_velocity.y = -15.f;
		}
	}

	float desiredHorizontalVelocity = leftRight * c_speed;
	float deltaHorizontalVelocity = desiredHorizontalVelocity - m_velocity.x;
	deltaHorizontalVelocity = vsClamp( deltaHorizontalVelocity, -acceleration * timeStep, acceleration * timeStep );
	m_velocity.x += deltaHorizontalVelocity;

	position += m_velocity * timeStep;

	position.x = vsClamp( position.x, m_world.GetMin().x, m_world.GetMax().x );
	position.y = vsClamp( position.y, m_world.GetMin().y, m_world.GetMax().y );

	// If I'm on the ground, no vertical velocity any more.
	if ( position.y >= 0.f )
	{
		m_velocity.y = 0.f;
	}

	SetPosition( ctStage::Instance()->ClipAgainstStage( GetPosition(), position, GetBoundingBox() ) );

}

bool
ctPlayer::InTheAir()
{
	// Someday we might have a real map.  But for now, the floor is always at zero, so let's just test that.
	//
	// Remember, in our coordinate system, up is NEGATIVE y.

	return ( ctStage::Instance()->HeightAboveGround( GetPosition(), GetBoundingBox() ) > 0.2f );
	//return (GetPosition().y < 0.f);
}

























ctPlayer3D::ctPlayer3D( const vsBox3D &dims ):
m_world(dims)
{
	AddChild( vsModel::Load("Player3D.vsp") );

	BuildBoundingBox();
}

ctPlayer3D::~ctPlayer3D()
{
}

void
ctPlayer3D::Update( float timeStep )
{
	vsInput *i = vsInput::Instance();

	if ( i->IsDown( CID_A ) || i->IsDown( CID_B ) )	// no movement while a modifier is down.  In CT, that's for adjusting cameras.
		return;

	vsVector3D input = vsVector3D(i->GetState( CID_LeftRightLAxis ),
                                  0.f,
                                  i->GetState( CID_UpDownLAxis ));

	vsVector3D position = GetPosition();

	const float c_speed = 5.f;	// 5 meters per second, maximum speed
	float acceleration = 20.f;	// 10 meters per second per second, maximum acceleration

	float heightAboveGround = ctStage3D::Instance()->HeightAboveGround( GetPosition(), GetBoundingBox() );

	// Am I in the air?  If so, slow down our acceleration, for slower air control.  Also, apply gravity.
	if ( heightAboveGround > 0.1f )
	{
		acceleration = 8.f;
		float c_gravity = 50.f;	// adjust to taste.  (Remember, down is POSITIVE Y in our coordinate system)

		if ( i->IsDown( CID_Up ) && m_velocity.y < 0.f )
			c_gravity = 20.f;

		m_velocity.y += c_gravity * timeStep;
	}
	else
	{
		// I'm not in the air, so snap me to the floor, then test if I want to jump.

		position.y += heightAboveGround;

		bool jump = i->WasPressed( CID_Up );
		m_velocity.y = 0.f;

		if ( jump )
		{
			// arbitrary jump initial velocity.  Larger numbers == higher jumps.
			//
			// It's possible to solve a parabola to learn the correct initial velocity for a particular
			// jump height, but you really don't want to do that;  jumping never feels right if you use
			// real physics.

			m_velocity.y = -15.f;
		}
	}

	vsVector3D desiredHorizontalVelocity = input * c_speed;
	vsVector3D deltaHorizontalVelocity = desiredHorizontalVelocity - m_velocity;
    if ( deltaHorizontalVelocity.Length() > acceleration * timeStep )
    {
        deltaHorizontalVelocity.Normalise();
        deltaHorizontalVelocity *= acceleration * timeStep;
    }
	m_velocity += deltaHorizontalVelocity;

	position += m_velocity * timeStep;

	position.x = vsClamp( position.x, m_world.GetMin().x, m_world.GetMax().x );
	position.y = vsClamp( position.y, m_world.GetMin().y, m_world.GetMax().y );

	// If I'm on the ground, no vertical velocity any more.
	if ( position.y >= 0.f )
	{
		m_velocity.y = 0.f;
	}

	SetPosition( ctStage3D::Instance()->ClipAgainstStage( GetPosition(), position, GetBoundingBox() ) );

}

bool
ctPlayer3D::InTheAir()
{
	// Someday we might have a real map.  But for now, the floor is always at zero, so let's just test that.
	//
	// Remember, in our coordinate system, up is NEGATIVE y.

	return ( ctStage3D::Instance()->HeightAboveGround( GetPosition(), GetBoundingBox() ) > 0.2f );
	//return (GetPosition().y < 0.f);
}


