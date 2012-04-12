/*
 *  AST_PlayerShip.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 25/03/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#include "AST_PlayerShip.h"
#include "AST_PlayerShot.h"
#include "AST_Game.h"

astPlayerShip::astPlayerShip( vsPhysicsEmitter *thrustEmitter ):
vsPhysicsSprite( vsDisplayList::Load("PlayerShip"), 1.0f, ColFlag_Player, ColFlag_All )
{
	m_thrust = new vsSprite( vsDisplayList::Load("PlayerThrust") );
	m_leftAttitude = new vsSprite( vsDisplayList::Load("PlayerAttitude") );
	m_rightAttitude = new vsSprite( vsDisplayList::Load("PlayerAttitude") );
	m_leftRearAttitude = new vsSprite( vsDisplayList::Load("PlayerAttitude") );
	m_rightRearAttitude = new vsSprite( vsDisplayList::Load("PlayerAttitude") );

	SetCollisionsActive(false);

	for ( int i = 0; i < Thruster_MAX; i++ )
	{
		m_thrustAmt[i] = 0.f;
	}

	SetColor(vsColor::Blue);

	m_thrust->SetColor(vsColor::Red);
	AddChild( m_thrust );
	m_thrust->m_transform.m_position.Set(0,0.9f);

	AddChild( m_leftAttitude );
	AddChild( m_rightAttitude );
	AddChild( m_leftRearAttitude );
	AddChild( m_rightRearAttitude );

	m_leftAttitude->m_transform.m_position.Set( 0.50f, -0.80f );
	m_leftAttitude->m_transform.m_angle.Set( DEGREES(30.0f) );
	m_leftRearAttitude->m_transform.m_position.Set( 1.2f, 0.8f );
	m_leftRearAttitude->m_transform.m_angle.Set( DEGREES(-30.0f) );
//	m_leftAttitude->m_transform.m_position.Set( 7.5, 0 );

	m_rightAttitude->m_transform.m_position.Set( -0.5f, -0.8f );
	m_rightAttitude->m_transform.m_angle.Set( DEGREES(-30.0f) );
//	m_rightAttitude->m_transform.m_position.Set( -7.5, 0 );
	m_rightAttitude->m_transform.m_scale.Set(-1.0f,1.0f);

	m_rightRearAttitude->m_transform.m_position.Set( -1.2f, 0.8f );
	m_rightRearAttitude->m_transform.m_scale.Set(-1.0f,1.0f);
	m_rightRearAttitude->m_transform.m_angle.Set( DEGREES(30.0f) );

	for ( int i = 0; i < c_maxShots; i++ )
	{
		m_shotList[i] = new astPlayerShot();
		m_shotList[i]->SetColor( vsColor::Yellow );
	}

	m_isSpawned = false;

	m_emitter = thrustEmitter;
}

astPlayerShip::~astPlayerShip()
{
	delete m_thrust;
	delete m_leftAttitude;
	delete m_rightAttitude;
	delete m_leftRearAttitude;
	delete m_rightRearAttitude;

	for ( int i = 0; i < c_maxShots; i++ )
		delete m_shotList[i];
}

void
astPlayerShip::Spawn( const vsVector2D &pos_in )
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
astPlayerShip::Despawn()
{
	Extract();
	SetCollisionsActive(false);

	m_emitter->SetSpawnRate(0.0f);
	m_emitter->SpawnImmediateBurst(60, GetPosition(), 10.0f, 3.0f, vsColor::Blue);

	m_isSpawned = false;
	m_delayBeforeSpawn = 2.0f;	// no respawning for at least two seconds.
	m_timeSinceSpawn = 0.f;
}

bool
astPlayerShip::AttemptSpawn(float timeStep, const vsVector2D &position)
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
astPlayerShip::HandleThrusters( float timeStep )
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

	m_thrust->m_transform.m_scale.Set( m_thrustAmt[Thruster_Main], m_thrustAmt[Thruster_Main] );
	m_leftAttitude->m_transform.m_scale.Set( m_thrustAmt[Thruster_Left], m_thrustAmt[Thruster_Left] );
	m_rightAttitude->m_transform.m_scale.Set( -m_thrustAmt[Thruster_Right], m_thrustAmt[Thruster_Right] );

	m_rightRearAttitude->m_transform.m_scale.Set( -m_thrustAmt[Thruster_Left], m_thrustAmt[Thruster_Left] );
	m_leftRearAttitude->m_transform.m_scale.Set( m_thrustAmt[Thruster_Right], m_thrustAmt[Thruster_Right] );
}

void
astPlayerShip::HandleSpawnTimer( float timeStep )
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
		m_transform.m_scale = vsVector2D(scale,scale);
	}
}

void
astPlayerShip::Update( float timeStep )
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
		float acceleration = 30.0f;
		AddForce( m_transform.m_angle.GetForwardVector() * acceleration /* lStick.y*/ * m_object->GetMass() );	// thrust
		m_emitter->SetSpawnRate( 30.0f );
	}
	else
	{
		m_emitter->SetSpawnRate(0.0f);
	}

	HandleSpawnTimer(timeStep);

	bool shooting = (!m_spawnInvulnerable && input->WasPressed(CID_A));

	if ( shooting )
	{
		for ( int i = 0; i < c_maxShots; i++ )
		{
			if ( !m_shotList[i]->IsSpawned() )
			{
				vsVector2D forwardVec = m_transform.m_angle.GetForwardVector();
				vsVector2D muzzlePos = GetPosition() + (forwardVec * 1.8f);
				vsVector2D shotVelocity = GetVelocity() + (forwardVec * 40.0f);
				m_shotList[i]->Spawn( muzzlePos, shotVelocity );
				break;
			}
		}
	}

	HandleThrusters(timeStep);

	Parent::Update( timeStep );

	vsVector2D forwardVec = m_transform.m_angle.GetForwardVector();
	m_emitter->SetSpawnPosition( GetPosition() - forwardVec * 3.0f, 1.0f );
	m_emitter->SetSpawnColor( vsColor::Red );

	if ( input->IsDown(CID_B) )
		m_emitter->SetSpawnVelocity( forwardVec * -20.0f, 6.0f );
}

bool
astPlayerShip::CollisionCallback( const colEvent &event )
{
	if ( event.colFlags & ColFlag_Enemy )
	{
		if ( !m_spawnInvulnerable )
			SetDestroyed();
		return true;
	}

	return false;
}

void
astPlayerShip::DestroyCallback()
{
	Despawn();
}
