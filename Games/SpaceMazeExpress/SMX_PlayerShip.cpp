/*
 *  SMX_PlayerShip.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 25/03/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#include "SMX_PlayerShip.h"
#include "SMX_Game.h"
#include "SMX_Rope.h"


smxPlayerShip::smxPlayerShip( vsPhysicsEmitter *thrustEmitter ):
vsPhysicsSprite( vsDisplayList::Load("PlayerShip"), 1.0f, ColFlag_Player, ColFlag_Enemy )
{
	m_thrust = vsSprite::Load("PlayerThrust");
	m_leftAttitude = vsSprite::Load("PlayerAttitude");
	m_rightAttitude = vsSprite::Load("PlayerAttitude");
	m_leftRearAttitude = vsSprite::Load("PlayerAttitude");
	m_rightRearAttitude = vsSprite::Load("PlayerAttitude");

	m_joint = NULL;

	SetCollisionsActive(false);

	for ( int i = 0; i < Thruster_MAX; i++ )
	{
		m_thrustAmt[i] = 0.f;
	}

	SetColor(vsColor::Blue);

	m_thrust->SetColor(vsColor::Red);
	AddChild( m_thrust );
	m_thrust->m_transform.SetTranslation(0,9);

	AddChild( m_leftAttitude );
	AddChild( m_rightAttitude );
	AddChild( m_leftRearAttitude );
	AddChild( m_rightRearAttitude );

	m_leftAttitude->m_transform.SetTranslation( 5.0f, -8.0f );
	m_leftAttitude->m_transform.SetAngle( DEGREES(30.0f) );
	m_leftRearAttitude->m_transform.SetTranslation( 12.0f, 8.0f );
	m_leftRearAttitude->m_transform.SetAngle( DEGREES(-30.0f) );
	//	m_leftAttitude->m_transform.SetTranslation( 7.5, 0 );

	m_rightAttitude->m_transform.SetTranslation( -5.0f, -8.0f );
	m_rightAttitude->m_transform.SetAngle( DEGREES(-30.0f) );
	//	m_rightAttitude->m_transform.SetTranslation( -7.5, 0 );
	m_rightAttitude->m_transform.SetScale(-1.0f,1.0f);

	m_rightRearAttitude->m_transform.SetTranslation( -12.0f, 8.0f );
	m_rightRearAttitude->m_transform.SetScale(-1.0f,1.0f);
	m_rightRearAttitude->m_transform.SetAngle( DEGREES(30.0f) );

	m_isSpawned = false;

	m_emitter = thrustEmitter;

	m_rope = new smxRope(30);
	m_rope->RegisterOnScene(0);
	m_ropeRetracting = false;
}

smxPlayerShip::~smxPlayerShip()
{
	vsDelete(m_rope);
	delete m_thrust;
	delete m_leftAttitude;
	delete m_rightAttitude;
	delete m_leftRearAttitude;
	delete m_rightRearAttitude;

}

void
smxPlayerShip::Spawn( const vsVector2D &pos_in )
{
	SetCollisionsActive(true);
	SetPosition( pos_in );
	SetVelocity( vsVector2D::Zero );
	SetAngularVelocity( 0.f );
	RegisterOnScene(0);
	m_spawnInvulnerable = true;
	//	m_colObject->Teleport();

	m_timeSinceSpawn = 0.f;

	m_isSpawned = true;
}

void
smxPlayerShip::Despawn()
{
	Extract();
	SetCollisionsActive(false);

	m_emitter->SetSpawnRate(0.0f);
	m_emitter->SpawnImmediateBurst(60, GetPosition(), 100.0f, 30.0f, vsColor::Blue);

	m_isSpawned = false;
	m_delayBeforeSpawn = 2.0f;	// no respawning for at least two seconds.
	m_timeSinceSpawn = 0.f;

	m_rope->Despawn();
}

bool
smxPlayerShip::AttemptSpawn(float timeStep, const vsVector2D &position)
{
	m_delayBeforeSpawn -= timeStep;

	if ( m_delayBeforeSpawn <= 0.f )
	{
		//float radius = m_colObject->GetBoundingCircle().radius * 4.0f;
		//vsVector2D nextPos = position + vsVector2D(20.0f,20.0f);
		//if (!core::GetGame()->GetCollision()->CollisionTest( position, nextPos, radius, m_colObject->GetTestFlags() ) )
		Spawn(position);
	}
	return m_isSpawned;
}

void
smxPlayerShip::HandleThrusters( float timeStep )
{
	UNUSED(timeStep);

	vsInput *input = core::GetGame()->GetInput();

	ControlID thrusterControl[Thruster_MAX] =
	{
		CID_B,
		CID_LeftRightLAxis,
		CID_LeftRightLAxis
	};

	/*
	 SetColor 1.0 1.0 1.0 0.8
	 MoveTo -0.75 -1.8
	 LineTo 7.5 -3.0
	 LineTo 0.75 1.8
	 */

	for( int i = 0; i < Thruster_MAX; i++ )
	{
		float desired = input->GetState( thrusterControl[i] );
		float current = m_thrustAmt[i];

		if ( i == 1 )	// left
			desired = vsMax(0.f, -desired);
		else
			desired = vsMax(0.f, desired);

		m_thrustAmt[i] = vsInterpolate( 0.15f, current, desired );
	}

	m_thrust->m_transform.SetScale( m_thrustAmt[Thruster_Main], m_thrustAmt[Thruster_Main] );
	m_leftAttitude->m_transform.SetScale( m_thrustAmt[Thruster_Left], m_thrustAmt[Thruster_Left] );
	m_rightAttitude->m_transform.SetScale( -m_thrustAmt[Thruster_Right], m_thrustAmt[Thruster_Right] );

	m_rightRearAttitude->m_transform.SetScale( -m_thrustAmt[Thruster_Left], m_thrustAmt[Thruster_Left] );
	m_leftRearAttitude->m_transform.SetScale( m_thrustAmt[Thruster_Right], m_thrustAmt[Thruster_Right] );
}

void
smxPlayerShip::HandleSpawnTimer( float timeStep )
{
	m_timeSinceSpawn += timeStep;

	const float c_invulnerableTime = 3.0f;	// three seconds of invulnerability

	if ( m_spawnInvulnerable )
	{
		float scale = 1.0f;
		vsColor c = vsColor::Blue;
		if ( m_timeSinceSpawn > c_invulnerableTime )
		{
			SetCollisionsActive(true);
			m_spawnInvulnerable = false;
		}
		else if ( m_timeSinceSpawn > 2.0f )
		{
			float frac = (m_timeSinceSpawn - 2.0f);

			float theta = frac * TWOPI;
			float pulseAmt = vsFabs(vsSin(theta));

			c = vsInterpolate(pulseAmt, vsColor::LightBlue, vsColor::White);
		}
		else if ( m_timeSinceSpawn > 1.0f )
		{
			float frac = (m_timeSinceSpawn - 1.0f);

			float theta = frac * TWOPI;
			float pulseAmt = vsFabs(vsSin(theta));

			c = vsInterpolate(pulseAmt, vsColor::LightBlue, vsColor::White);
		}
		else
		{
			c = vsInterpolate(m_timeSinceSpawn, vsColor::Black, vsColor::LightBlue);
		}

		const float c_scaleTime = 3.0f;
		if ( m_timeSinceSpawn < c_scaleTime )
		{
			float frac = m_timeSinceSpawn / c_scaleTime;
			frac = 1.0f - ((frac-1.f) * (frac-1.f));									// slam in, ease out
			//frac = (3.0f * frac * frac) - (2.0f * frac * frac * frac);			// ease in, ease out

			scale = vsInterpolate( frac, 5.0f, 1.0f );
		}

		SetColor(c);
		m_transform.SetScale( vsVector2D(scale,scale) );
	}
}

void
smxPlayerShip::Update( float timeStep )
{
	vsInput *input = core::GetGame()->GetInput();

	//vsVector2D lStick = input->GetLeftStick();
	float steering = input->GetLeftStick().x;

	AddTorque( m_object->GetAngularVelocity() * -6.0f * m_object->GetMomentOfInertia());		// angular damping
	AddTorque( DEGREES(1080.0f) * steering * m_object->GetMomentOfInertia() );		// rotation from attitude jets

	AddForce( m_object->GetVelocity() * -0.3f * m_object->GetMass() );				// damping

	//if ( lStick.y > 0.f )
	if ( input->IsDown(CID_B) )
	{
		float acceleration = 300.0f;
		AddForce( m_transform.GetAngle().GetForwardVector() * acceleration /* lStick.y*/ * m_object->GetMass() );	// thrust
		m_emitter->SetSpawnRate( 30.0f );
	}
	else
	{
		m_emitter->SetSpawnRate(0.0f);
	}

	HandleSpawnTimer(timeStep);

	HandleThrusters(timeStep);

	Parent::Update( timeStep );

	vsVector2D forwardVec = m_transform.GetAngle().GetForwardVector();
	m_emitter->SetSpawnPosition( GetPosition() - forwardVec * 30.0f, 10.0f );
	m_emitter->SetSpawnColor( vsColor::Red );

	if ( input->IsDown(CID_B) )
		m_emitter->SetSpawnVelocity( forwardVec * -200.0f, 60.0f );


	m_rope->AllowAutoAttach( !input->IsDown(CID_A) );
	if ( input->IsDown(CID_A) )
	{
		//m_ropeRetracting = !m_ropeRetracting;

		m_rope->LinkEndTo(NULL);
	}

	if ( m_ropeRetracting )
	{
		//m_spoolJoint->SetMotorSpeed(10.0f);
	}
	else
	{
		//m_spoolJoint->SetMotorSpeed(0.0f);
	}
}

bool
smxPlayerShip::CollisionCallback( const colEvent &event )
{
	/*if ( event.colFlags & ColFlag_Enemy )
	{
		if ( !m_spawnInvulnerable )
			SetDestroyed();
		return true;
	}*/
	UNUSED(event);

	return false;
}

void
smxPlayerShip::DestroyCallback()
{
	Despawn();
}
