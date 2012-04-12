/*
 *  SMX_Rope.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 11/02/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#include "SMX_Rope.h"

#include "SMX_ModeInGame.h"

smxRope::smxRope( int segments )
{
	vsAssert(segments > 0, "Tried to create a rope with zero segments!");

	m_spawned = false;
	m_segmentCount = segments;
	m_body = new b2Body*[m_segmentCount];

	m_jointCount = segments-1;
	m_joint = new b2Joint*[m_jointCount];

	m_startJoint = NULL;
	m_endJoint = NULL;

	m_extendLength = 0;

	m_allowAutoAttach = false;
}

smxRope::~smxRope()
{
	LinkStartTo(NULL);
	LinkEndTo(NULL);

	vsDeleteArray(m_body);
	vsDeleteArray(m_joint);
}

void
smxRope::Update( float timeStep )
{
	UNUSED(timeStep);

	if ( m_allowAutoAttach && !m_endJoint )
	{
		/*b2ContactNode *node = m_body[m_segmentCount-1]->GetContactList();

		while ( node )
		{
			void *ud = node->other->GetUserData();

			b2Contact *contact = node->contact;
			b2Manifold *m = contact->GetManifolds();

			b2ContactPoint *cp = &m->points[0];
			vsVector2D pos( cp->position.x, cp->position.y );

			LinkEndTo((vsCollisionObject *)ud, pos );
			return;

			node = node->next;
		}*/
	}
}

void
smxRope::Draw( vsRenderQueue *queue )
{
	UNUSED(queue);
/*	if ( m_spawned )
	{
		if ( m_endJoint &&  m_startJoint )
			list->SetColor( vsColor::LightBlue );
		else
			list->SetColor( vsColor::Green );
		list->MoveTo( vsVector2D(m_body[0]->m_position.x, m_body[0]->m_position.y) );

		for ( int i = 1; i < m_segmentCount; i++ )
		{
			list->LineTo( vsVector2D(m_body[i]->m_position.x, m_body[i]->m_position.y) );
		}

		if ( m_endJoint )
		{
			b2Vec2 p = m_endJoint->GetAnchor1();
			list->LineTo( vsVector2D(p.x,p.y) );
		}
	}*/
}

void
smxRope::Spawn( const vsVector2D &start, const vsVector2D &end )
{
	UNUSED(start);
	UNUSED(end);
	/*vsAssert(!m_spawned, "Rope is already spawned!");
	// okay.  We're going from start to end.

	m_spawned = true;

	b2World *w = vsCollisionSystem::Instance()->GetWorld();

	b2BoxDef			boxDef;
	b2BodyDef			bodyDef;
	b2RevoluteJointDef	jointDef;

	vsVector2D delta = end - start;
	float length = delta.Length();
	float lengthPerSegment = length / (m_segmentCount+1);
	vsVector2D direction = delta * (1.0f / length);

	vsVector2D point = start + (direction * lengthPerSegment * 0.5f);
	vsAngle a = vsAngle::FromForwardVector(direction);

	boxDef.extents.Set(5.f,lengthPerSegment + 5.0f);
	boxDef.density = 0.1f;
	boxDef.friction = 0.2f;
	boxDef.categoryBits = smxColFlag_Rope;
	boxDef.maskBits = 0xffff & ~ColFlag_Particle;
	boxDef.groupIndex = -1;

	bodyDef.AddShape(&boxDef);

	for ( int i = 0; i < m_segmentCount; i++ )
	{
		vsVector2D bPos = point + direction * lengthPerSegment * i;
		vsVector2D jPos = start + direction * lengthPerSegment * i;

		bodyDef.position.Set(bPos.x,bPos.y);

		m_body[i] = w->CreateBody(&bodyDef);

		if ( i > 0 )
		{
			jointDef.anchorPoint.Set(jPos.x,jPos.y);
			jointDef.body1 = m_body[i-1];
			jointDef.body2 = m_body[i];
			w->CreateJoint(&jointDef);
		}
	}*/
}

void
smxRope::Despawn()
{
	vsAssert(m_spawned, "Rope isn't spawned!");
}

void
smxRope::LinkStartTo( vsCollisionObject *target )
{
	b2Body *b = NULL;

	if ( target )
		b = target->GetBody();

	LinkStartToBody(b);
}

void
smxRope::LinkStartToBody( b2Body *body )
{
	UNUSED(body);
	/*b2World *w = vsCollisionSystem::Instance()->GetWorld();

	if ( m_startJoint )
	{
		w->DestroyJoint(m_startJoint);
		m_startJoint = NULL;
	}

	if ( body )
	{
		b2RevoluteJointDef	jointDef;
		b2Vec2 jPos = m_body[0]->m_position;
		jointDef.anchorPoint.Set(jPos.x,jPos.y);
		jointDef.body1 = body;
		jointDef.body2 = m_body[0];
		m_startJoint = w->CreateJoint(&jointDef);
	}*/
}

void
smxRope::LinkEndTo( vsCollisionObject *target )
{
	UNUSED(target);
/*	b2Vec2 p = m_body[m_segmentCount-1]->m_position;
	vsVector2D pos(p.x,p.y);
	LinkEndTo( target, pos );*/
}

void
smxRope::LinkEndTo( vsCollisionObject *target, const vsVector2D &pos )
{
	UNUSED(target);
	UNUSED(pos);
	/*b2World *w = vsCollisionSystem::Instance()->GetWorld();

	if ( m_endJoint )
	{
		w->DestroyJoint(m_endJoint);
		m_endJoint = NULL;
	}

	if ( target )
	{
		b2RevoluteJointDef	jointDef;
		//b2Vec2 jPos = m_body[m_segmentCount-1]->m_position;
		jointDef.anchorPoint.Set(pos.x,pos.y);
		jointDef.body1 = target->GetBody();
		jointDef.body2 = m_body[m_segmentCount-1];
		m_endJoint = w->CreateJoint(&jointDef);
	}*/
}



