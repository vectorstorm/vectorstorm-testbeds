/*
 *  SST_Rocket.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 29/04/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#ifndef SST_ROCKET_H
#define SST_ROCKET_H

class sstModeInGame;
class sstSequence;

class sstRocket : public vsSprite
{
	typedef vsSprite Parent;
	
	sstModeInGame *	m_gameMode;
	
	vsVector2D	m_targetPos;
	vsVector2D	m_velocity;
	
	sstSequence *m_seq;
	int			m_seqId;
	
	float		m_hose;
	float		m_timeSinceTarget;
	bool		m_spawned;
	
public:
	
	sstRocket( sstModeInGame *mode );
	~sstRocket();
	
	void	Spawn(const vsVector2D &targetPos, float timeToExplode, sstSequence *mySequence, int id);		// how long until we're supposed to go boom?
	void	Despawn();
	bool	ConsiderDetonating();	// returns how many points we think we're worth
	void	Detonate();
	
	bool	IsSpawned() { return m_spawned; }
	
	virtual void Update( float timeStep );
};

#endif // SST_ROCKET_H


