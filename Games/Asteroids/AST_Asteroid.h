/*
 *  AST_Asteroid.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 9/04/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#ifndef AST_ASTEROID_H
#define AST_ASTEROID_H

class vsCamera2D;
class vsDisplayList;

class astAsteroid : public vsPhysicsSprite
{
	typedef vsPhysicsSprite Parent;

	int  m_type;

	bool m_spawned;

public:

	enum Type
	{
		Large,
		Medium,
		Small,
		Type_MAX
	};
	astAsteroid(vsDisplayList *list, Type type);

//	void Spawn( const vsVector2D & where );
//	void Spawn();
	void Spawn(const vsVector2D &where = vsVector2D::Zero, const vsVector2D &baseVelocity = vsVector2D::Zero, float radius = 400.0f, bool ensureNoOverlap = true);
	void Despawn();
	bool IsSpawned() { return m_spawned; }

	virtual void Update( float timeStep );
	virtual bool	CollisionCallback( const colEvent &event );
	virtual void	DestroyCallback();
};

class astAsteroidPool : public vsSingleton<astAsteroidPool>
{
	vsDisplayList *			m_largeList;
	vsDisplayList *			m_mediumList;
	vsDisplayList *			m_smallList;

	astAsteroid	**	m_largeAsteroidPool;
	astAsteroid	**	m_mediumAsteroidPool;
	astAsteroid	**	m_smallAsteroidPool;

	int						m_largeAsteroidPoolSize;
	int						m_mediumAsteroidPoolSize;
	int						m_smallAsteroidPoolSize;

	float				m_opacity;
	float				m_fadingFrom;
	float				m_fadingTo;
	float				m_fadeTimer;
	bool				m_fading;

	int					m_respawnAsteroidCount;
	bool				m_respawning;

	void				FadeTo(float opacity) { m_fading = true; m_fadingFrom = m_opacity; m_fadingTo = opacity; m_fadeTimer = 0.f; }

public:

	astAsteroidPool();
	~astAsteroidPool();

	astAsteroid *	SpawnAsteroid( astAsteroid::Type type );
	astAsteroid *	SpawnAsteroid( astAsteroid::Type type, const vsVector2D &where, const vsVector2D &baseVelocity, float radius, bool noOverlap );
	void			WrapAround( vsCamera2D *camera );
	void			DespawnAllAsteroids();

	void			Update( float timeStep );

	void			RespawnAsteroids(int count);	// fades out, despawns, respawns, and fades in.

	void			FadeIn() { FadeTo(1.0f); }
	void			FadeOut() { FadeTo(0.0f); }

	int				GetSpawnedAsteroidCount();
};


#endif // AST_ASTEROID_H
