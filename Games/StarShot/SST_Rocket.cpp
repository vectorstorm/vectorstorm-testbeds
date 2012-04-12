/*
 *  SST_Rocket.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 29/04/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#include "SST_Rocket.h"
#include "SST_Game.h"
#include "SST_ModeInGame.h"
#include "SST_Sequence.h"

sstRocket::sstRocket(sstModeInGame *game):
vsSprite( vsDisplayList::Load("Rocket") ),
m_gameMode(game)
{
	SetColor( vsColor::Red );
	m_hose = 0.0f;
	m_spawned = false;
}

sstRocket::~sstRocket()
{
}

void
sstRocket::Spawn( const vsVector2D &targetPos, float timeToSpawn, sstSequence *sequence, int id )
{
	m_gameMode->TriggerRocket();
	
	vsColor color = vsRandom::GetColor(0.5f, 1.0f);
	SetColor(color);
	
	m_seq = sequence;
	m_seqId = id;
	
	m_targetPos = targetPos;

	vsVector2D iniVelocity = vsRandom::GetVector2D(50.0f);
	iniVelocity.y = vsRandom::GetFloat(-900.0f, -700.0f) / timeToSpawn;
	
	vsVector2D iniPosition = m_targetPos - (iniVelocity * timeToSpawn);
	SetPosition( iniPosition );
	
	m_timeSinceTarget = -timeToSpawn;	// starts big negative.  Will hit zero as we hit our target point, and go positive as we pass it.
	
	m_velocity = iniVelocity;
	m_transform.m_angle = vsAngle::FromForwardVector(m_velocity);
	
	RegisterOnScene(0);
	
	m_spawned = true;
}

bool
sstRocket::ConsiderDetonating()
{
	bool detonating = false;
	float timeOffset = (m_timeSinceTarget);
	if ( timeOffset < 0.f )
		timeOffset = -timeOffset;
	
	float maxTimeOffset = 0.1f;
	
	if ( timeOffset < maxTimeOffset )
	{
		m_gameMode->RegisterBeatResult(true, 1.0f - (timeOffset/maxTimeOffset));
		Detonate();
		
		detonating = true;
		//points = (int)(100.f * (1.0f - (timeOffset/maxTimeOffset)));
	}
	
	return detonating;
}

void
sstRocket::Detonate()
{
	m_gameMode->TriggerExplosion( GetPosition(), m_color );
	m_seq->RegisterDetonation(m_seqId);
	Despawn();
}

void
sstRocket::Despawn()
{
	Extract();
	
	m_spawned = false;
}

void
sstRocket::Update( float timeStep )
{
	Parent::Update( timeStep );
	
	m_timeSinceTarget += timeStep;

	if ( m_timeSinceTarget < 0.f )	// we haven't hit our target yet
	{
		SetPosition( m_targetPos + (m_velocity * m_timeSinceTarget) );
		
		const float c_spawnRate = 100.0f;	// a hundred particles per second
		m_hose += timeStep * c_spawnRate;
		
		if ( m_hose > 1.0f )
		{
			int hoseCount = (int)m_hose;
			m_hose -= hoseCount;
			m_gameMode->SpawnImmediate( hoseCount, GetPosition() /*+ vsVector2D(0.0f,5.0f)*/, -m_velocity, vsColor(0.2f,0.2f,0.2f,1.0f), 1.0f );
		}
	}
	else
	{
		// we've passed our target;  gravity!
		
		m_velocity += vsVector2D(0.f,300.f * timeStep);
		m_transform.m_angle += m_velocity.x * DEGREES(10.0f) * timeStep;
		SetPosition( GetPosition() + m_velocity * timeStep );

		if ( GetPosition().y > 900.0f )	// off the bottom of the screen
		{
			Despawn();
			m_gameMode->RegisterBeatResult(false);	// failed!
		}
	}
}


