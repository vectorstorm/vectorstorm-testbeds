/*
 *  SMX_Rope.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 11/02/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#ifndef SMX_ROPE_H
#define SMX_ROPE_H

class b2Body;
class b2Joint;

class vsCollisionObject;

class smxRope : public vsEntity
{
	b2Body **			m_body;
	int					m_segmentCount;

	b2Joint **			m_joint;
	int					m_jointCount;

	b2Joint *			m_startJoint;
	b2Joint *			m_endJoint;

	float				m_extendLength;

	bool				m_allowAutoAttach;
	bool				m_spawned;

public:

						smxRope( int segments );
	virtual				~smxRope();

	virtual void		Update(float timeStep);
	virtual void		Draw(vsRenderQueue *queue);

	void				LinkStartToBody( b2Body *body );
	void				LinkStartTo( vsCollisionObject *sprite );
	void				LinkEndTo( vsCollisionObject *sprite );
	void				LinkEndTo( vsCollisionObject *sprite, const vsVector2D &pos );

	void				Extend();
	void				Retract();

	void				AllowAutoAttach(bool allow) { m_allowAutoAttach = allow; }

	bool				IsSpawned() { return m_spawned; }
	void				Spawn( const vsVector2D &start, const vsVector2D &end );
	void				Despawn();
};

#endif // SMX_ROPE_H

