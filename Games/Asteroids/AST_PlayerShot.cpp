/*
 *  AST_PlayerShot.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 14/04/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#include "AST_PlayerShot.h"
#include "AST_Game.h"

static const float c_shotLifetime = 2.0f;

astPlayerShot::astPlayerShot():
vsPhysicsSprite( vsDisplayList::Load("Shot"), 1.0f, ColFlag_Shot, ColFlag_Enemy ),
m_velocity(vsVector2D::Zero),
m_timer(0.f),
m_spawned(false)
{
}

void
astPlayerShot::Spawn( const vsVector2D &where, const vsVector2D &velocity )
{
	vsAssert( !m_spawned, "Tried to spawn a shot that was already spawned!" );

	SetCollisionsActive(true);

	SetPosition(where);
	SetVelocity(velocity);
	m_velocity = velocity;

	RegisterOnScene(0);
	//vsScreen::Instance()->GetScene(0)->RegisterEntityOnBottom( this );

	m_spawned = true;

	m_timer = c_shotLifetime;
}

void
astPlayerShot::Despawn()
{
	vsAssert( m_spawned, "Tried to despawn a shot that wasn't spawned!" );

	SetCollisionsActive(false);
	Extract();

	m_spawned = false;
}

void
astPlayerShot::Update(float timeStep)
{
	m_timer -= timeStep;
	if ( m_spawned )
	{
		Parent::Update(timeStep);
	}

	if ( m_timer <= 0.f )
		Despawn();
}

bool
astPlayerShot::CollisionCallback( const colEvent &event )
{
	UNUSED(event);
	SetDestroyed();

	return true;
}

void
astPlayerShot::DestroyCallback()
{
	Despawn();
}

