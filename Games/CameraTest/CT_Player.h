/*
 *  CT_Player.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 3/01/11.
 *  Copyright 2011 Trevor Powell. All rights reserved.
 *
 */

#ifndef CT_PLAYER_H
#define CT_PLAYER_H


class ctPlayer : public vsSprite, public vsSingleton<ctPlayer>
{
	vsBox2D		m_world;

	vsVector2D	m_velocity;

public:

	ctPlayer( const vsBox2D &worldDims );
	virtual ~ctPlayer();

	const vsVector2D &	GetVelocity() { return m_velocity; }

	bool		InTheAir();

	virtual void Update( float timeStep );
};

class ctPlayer3D : public vsModel, public vsSingleton<ctPlayer3D>
{
	vsBox3D		m_world;

	vsVector3D	m_velocity;

public:

	ctPlayer3D( const vsBox3D &worldDims );
	virtual ~ctPlayer3D();

	const vsVector3D &	GetVelocity() { return m_velocity; }

	bool		InTheAir();

	virtual void Update( float timeStep );
};


#endif // CT_PLAYER_H

