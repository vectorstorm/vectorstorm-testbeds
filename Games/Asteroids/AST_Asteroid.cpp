/*
 *  AST_Asteroid.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 9/04/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#include "AST_Asteroid.h"
#include "AST_Game.h"

astAsteroid::astAsteroid(vsDisplayList *list, astAsteroid::Type type):
vsPhysicsSprite( list, 1.0f, ColFlag_Enemy, ColFlag_Enemy | ColFlag_Shot | ColFlag_Player | ColFlag_Particle ),
m_type(type),
m_spawned(false)
{
}

void
astAsteroid::Spawn(const vsVector2D &where_in, const vsVector2D &baseVelocity, float radius_in, bool noOverlap)
{
	vsAssert( !m_spawned, "Tried to spawn an asteroid that was already spawned!" );

	int tries = 0;

	while ( !m_spawned )
	{
		float rx = vsRandom::GetFloat( -radius_in, radius_in );
		float ry = vsRandom::GetFloat( -radius_in, radius_in );
		rx += where_in.x;
		ry += where_in.y;

		vsVector2D where(rx,ry);
		vsVector2D nextPos = where + vsVector2D(2.0f,2.0f);
		float radius = GetBoundingCircle().radius;
		radius *= 2.0f;

		if ( !noOverlap || tries++ > 10 /*||
			!core::GetGame()->GetCollision()->CollisionTest( where, nextPos, radius, m_colObject->GetTestFlags() )*/ )
		{
			SetCollisionsActive(true);
			SetPosition(where);

			float rotAmt = vsRandom::GetFloat( -1.0f, 1.0f );
			SetAngularVelocity(rotAmt);

			float motionX = vsRandom::GetFloat( -5.0f, 5.0f );
			float motionY = vsRandom::GetFloat( -5.0f, 5.0f );
			SetVelocity( baseVelocity + vsVector2D( motionX, motionY ) );

			/*m_colObject->SetTransform( m_transform );
			m_colObject->Teleport();
			m_colObject->SetMass( 0.1f * radius );	// we're 5 ships heavy  (A player ship is '1' unit heavy)
			m_colObject->SetCollisionsActive(true);
			*/

			RegisterOnScene(0);

			m_spawned = true;
		}
	}
}

void
astAsteroid::Despawn()
{
	vsAssert( m_spawned, "Tried to despawn an asteroid that wasn't spawned!" );

	SetCollisionsActive(false);
	Extract();

	m_spawned = false;
}

void
astAsteroid::Update(float timeStep)
{
	if ( m_spawned )
	{
		Parent::Update(timeStep);
	}
}

bool
astAsteroid::CollisionCallback( const colEvent &event )
{
 	if ( event.colFlags & ColFlag_Shot )
	{
		int pointValue[Type_MAX] = { 300, 200, 100 };

		SetDestroyed();
		astGame::Instance()->AddScore(pointValue[m_type]);

		return true;
	}

	return Parent::CollisionCallback(event);
}

void
astAsteroid::DestroyCallback()
{
	vsVector2D basePos = GetPosition();
	vsVector2D baseVel = GetVelocity();

	Despawn();
	Type nextType = (Type)(m_type + 1);
	if ( nextType < Type_MAX )
	{
		astAsteroidPool::Instance()->SpawnAsteroid( nextType, basePos, baseVel,
												   GetBoundingCircle().radius * 1.0f, false );
		astAsteroidPool::Instance()->SpawnAsteroid( nextType, basePos, baseVel,
												   GetBoundingCircle().radius * 1.0f, false );
	}

	int particles[Type_MAX] = {25, 20, 15};
	astGame::Instance()->GetShrapnelEmitter()->SpawnImmediateBurst(particles[m_type], GetPosition(), 10.0f, 2.0f);
}

astAsteroidPool::astAsteroidPool():
m_opacity(0.f),
m_fadingFrom(0.f),
m_fadingTo(0.f),
m_fadeTimer(0.f),
m_fading(false),
m_respawning(false)
{
	m_largeList = vsDisplayList::Load("Asteroid");
	m_mediumList = vsDisplayList::Load("MediumAsteroid");
	m_smallList = vsDisplayList::Load("SmallAsteroid");

	m_largeAsteroidPoolSize		=	12;
	m_mediumAsteroidPoolSize	=	m_largeAsteroidPoolSize * 2;
	m_smallAsteroidPoolSize		=	m_mediumAsteroidPoolSize * 2;

	m_largeAsteroidPool			=	new astAsteroid *[m_largeAsteroidPoolSize];
	m_mediumAsteroidPool		=	new astAsteroid *[m_mediumAsteroidPoolSize];
	m_smallAsteroidPool			=	new astAsteroid *[m_smallAsteroidPoolSize];

	for ( int i = 0; i < m_largeAsteroidPoolSize; i++ )
		m_largeAsteroidPool[i] = new astAsteroid(m_largeList->CreateInstance(), astAsteroid::Large);
	for ( int i = 0; i < m_mediumAsteroidPoolSize; i++ )
		m_mediumAsteroidPool[i] = new astAsteroid(m_mediumList->CreateInstance(), astAsteroid::Medium);
	for ( int i = 0; i < m_smallAsteroidPoolSize; i++ )
		m_smallAsteroidPool[i] = new astAsteroid(m_smallList->CreateInstance(), astAsteroid::Small);
}

astAsteroidPool::~astAsteroidPool()
{
	for ( int i = 0; i < m_largeAsteroidPoolSize; i++ )
		delete m_largeAsteroidPool[i];
	for ( int i = 0; i < m_smallAsteroidPoolSize; i++ )
		delete m_smallAsteroidPool[i];
	for ( int i = 0; i < m_mediumAsteroidPoolSize; i++ )
		delete m_mediumAsteroidPool[i];

	delete [] m_largeAsteroidPool;
	delete [] m_mediumAsteroidPool;
	delete [] m_smallAsteroidPool;

	delete m_largeList;
	delete m_mediumList;
	delete m_smallList;
}

void
astAsteroidPool::Update(float timeStep)
{
	if ( m_fading )
	{
		m_fadeTimer += timeStep;

		if ( m_fadeTimer > 1.0f )
		{
			m_fadeTimer = 1.0f;
			m_fading = false;

			if ( m_respawning )
			{
				DespawnAllAsteroids();

				for ( int i = 0; i < m_respawnAsteroidCount; i++ )
					SpawnAsteroid( astAsteroid::Large );

				m_respawning = false;

				FadeIn();
			}
		}

		m_opacity = vsInterpolate( m_fadeTimer, m_fadingFrom, m_fadingTo );

		vsColor c = vsColor::White * m_opacity;

		for ( int i = 0; i < m_largeAsteroidPoolSize; i++ )
			m_largeAsteroidPool[i]->SetColor(c);

		for ( int i = 0; i < m_mediumAsteroidPoolSize; i++ )
			m_mediumAsteroidPool[i]->SetColor(c);

		for ( int i = 0; i < m_smallAsteroidPoolSize; i++ )
			m_smallAsteroidPool[i]->SetColor(c);
	}
}

astAsteroid *
astAsteroidPool::SpawnAsteroid( astAsteroid::Type type )
{
	int listSize = 0;

	if ( type == astAsteroid::Large )
	{
		astAsteroid **list = m_largeAsteroidPool;
		listSize = m_largeAsteroidPoolSize;

		for( int i = 0; i < listSize; i++ )
		{
			if ( !list[i]->IsSpawned() )
			{
				list[i]->Spawn();
				return list[i];
			}
		}
	}
	else if ( type == astAsteroid::Medium )
	{
		astAsteroid **list = m_mediumAsteroidPool;
		listSize = m_mediumAsteroidPoolSize;

		for( int i = 0; i < listSize; i++ )
		{
			if ( !list[i]->IsSpawned() )
			{
				list[i]->Spawn();
				return list[i];
			}
		}
	}
	else if ( type == astAsteroid::Small )
	{
		astAsteroid **list = m_smallAsteroidPool;
		listSize = m_smallAsteroidPoolSize;

		for( int i = 0; i < listSize; i++ )
		{
			if ( !list[i]->IsSpawned() )
			{
				list[i]->Spawn();
				return list[i];
			}
		}
	}
	return NULL;
}

astAsteroid *
astAsteroidPool::SpawnAsteroid( astAsteroid::Type type, const vsVector2D &where, const vsVector2D &baseVelocity,
							   float radius, bool noOverlap )
{
	int listSize = 0;

	if ( type == astAsteroid::Large )
	{
		astAsteroid **list = m_largeAsteroidPool;
		listSize = m_largeAsteroidPoolSize;

		for( int i = 0; i < listSize; i++ )
		{
			if ( !list[i]->IsSpawned() )
			{
				list[i]->Spawn(where, baseVelocity, radius, noOverlap);
				return list[i];
			}
		}
	}
	else if ( type == astAsteroid::Medium )
	{
		astAsteroid **list = m_mediumAsteroidPool;
		listSize = m_mediumAsteroidPoolSize;

		for( int i = 0; i < listSize; i++ )
		{
			if ( !list[i]->IsSpawned() )
			{
				list[i]->Spawn(where, baseVelocity, radius, noOverlap);
				return list[i];
			}
		}
	}
	else if ( type == astAsteroid::Small )
	{
		astAsteroid **list = m_smallAsteroidPool;
		listSize = m_smallAsteroidPoolSize;

		for( int i = 0; i < listSize; i++ )
		{
			if ( !list[i]->IsSpawned() )
			{
				list[i]->Spawn(where, baseVelocity, radius, noOverlap);
				return list[i];
			}
		}
	}
	return NULL;
}

void
astAsteroidPool::WrapAround( vsCamera2D *camera )
{
	vsVector2D v;
	float r;

	for ( int i = 0; i < m_smallAsteroidPoolSize; i++ )
	{
		if ( m_smallAsteroidPool[i]->IsSpawned() )
		{
			v = m_smallAsteroidPool[i]->GetPosition();
			r = m_smallAsteroidPool[i]->GetBoundingRadius();

			if ( camera->WrapAround(v,r) )
			{
				m_smallAsteroidPool[i]->SetPosition( v );
				//m_smallAsteroidPool[i]->Teleport();
			}
		}
	}

	for ( int i = 0; i < m_mediumAsteroidPoolSize; i++ )
	{
		if ( m_mediumAsteroidPool[i]->IsSpawned() )
		{
			v = m_mediumAsteroidPool[i]->GetPosition();
			r = m_mediumAsteroidPool[i]->GetBoundingRadius();

			if ( camera->WrapAround(v,r) )
			{
				m_mediumAsteroidPool[i]->SetPosition( v );
				//m_mediumAsteroidPool[i]->Teleport();
			}
		}
	}

	for ( int i = 0; i < m_largeAsteroidPoolSize; i++ )
	{
		if ( m_largeAsteroidPool[i]->IsSpawned() )
		{
			v = m_largeAsteroidPool[i]->GetPosition();
			r = m_largeAsteroidPool[i]->GetBoundingRadius();

			if ( camera->WrapAround(v,r) )
			{
				m_largeAsteroidPool[i]->SetPosition( v );
				//m_largeAsteroidPool[i]->Teleport();
			}
		}
	}
}

void
astAsteroidPool::DespawnAllAsteroids()
{
	for ( int i = 0; i < m_largeAsteroidPoolSize; i++ )
		if ( m_largeAsteroidPool[i]->IsSpawned() )
			m_largeAsteroidPool[i]->Despawn();

	for ( int i = 0; i < m_mediumAsteroidPoolSize; i++ )
		if ( m_mediumAsteroidPool[i]->IsSpawned() )
			m_mediumAsteroidPool[i]->Despawn();

	for ( int i = 0; i < m_smallAsteroidPoolSize; i++ )
		if ( m_smallAsteroidPool[i]->IsSpawned() )
			m_smallAsteroidPool[i]->Despawn();
}

int
astAsteroidPool::GetSpawnedAsteroidCount()
{
	int spawnCount = 0;

	for ( int i = 0; i < m_largeAsteroidPoolSize; i++ )
		if ( m_largeAsteroidPool[i]->IsSpawned() )
			spawnCount++;

	for ( int i = 0; i < m_mediumAsteroidPoolSize; i++ )
		if ( m_mediumAsteroidPool[i]->IsSpawned() )
			spawnCount++;

	for ( int i = 0; i < m_smallAsteroidPoolSize; i++ )
		if ( m_smallAsteroidPool[i]->IsSpawned() )
			spawnCount++;

	return spawnCount;
}

void
astAsteroidPool::RespawnAsteroids(int count)
{
	m_respawning = true;
	m_respawnAsteroidCount = count;

	FadeOut();
}
