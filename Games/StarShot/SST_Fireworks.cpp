/*
 *  SST_Fireworks.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 1/05/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#include "SST_Fireworks.h"

sstFireworks::sstFireworks( int count ):
vsEntity()
{
	m_particle = new sstFireworkParticle[count];
	m_particleCount = count;
	
	m_vertexBuffer = new vsRenderBuffer(vsRenderBuffer::Type_Stream);
	m_colorBuffer = new vsRenderBuffer(vsRenderBuffer::Type_Stream);
	m_indexBuffer = new vsRenderBuffer(vsRenderBuffer::Type_Stream);
	
	m_vertexArray = new vsVector3D[m_particleCount*2];
	m_colorArray = new vsColor[m_particleCount*2];
	m_indexArray = new uint16[m_particleCount*2];
	
//	for ( int i = 0; i < count; i++ )
//		m_particle[i].m_used = false;
	
	m_usedList = new sstFireworkParticle;
	m_unusedList = new sstFireworkParticle;

	for ( int i = 0; i < count; i++ )
		m_unusedList->Append(&m_particle[i]);
}

sstFireworks::~sstFireworks()
{
	vsDeleteArray( m_vertexArray );
	vsDeleteArray( m_colorArray );
	vsDeleteArray( m_indexArray );
	vsDelete( m_vertexBuffer );
	vsDelete( m_colorBuffer );
	vsDelete( m_indexBuffer );
	vsDelete( m_usedList );
	vsDelete( m_unusedList );
	vsDeleteArray( m_particle );
	m_particle = NULL;
}

void
sstFireworks::Update( float timeStep )
{
	sstFireworkParticle *p = m_usedList->m_next;
	
	while( p && p != m_usedList )
	{
		sstFireworkParticle *next = p->m_next;
		
		p->m_position += p->m_velocity * timeStep;
		p->m_velocity += vsVector2D(0.0f,30.81f) * timeStep;
		
		p->m_life += timeStep;
		
		if ( p->m_life > p->m_lifeTime )
		{
			p->Extract();
			m_unusedList->Append(p);
		}
		p = next;
	}
}

void
sstFireworks::Draw( vsRenderQueue *queue )
{
/*	vsVector2D tl(-3.f,-3.f);
	vsVector2D tr(3.f,-3.f);
	vsVector2D bl(-3.f,3.f);
	vsVector2D br(3.f,3.f);*/
	
	vsDisplayList *list = queue->GetGenericList();
	
	vsColor c;
	vsVector2D pos;
	vsVector2D dest;
	
	sstFireworkParticle *p = m_usedList->m_next;
	int i = 0;
	
	while( p && p != m_usedList )
	{
		float backVelTime = 0.5f;
		if ( p->m_life < backVelTime )
			backVelTime = p->m_life;
		pos = p->m_position;
		dest = pos - (p->m_velocity * backVelTime);
		float lifeFraction = p->m_life / p->m_lifeTime;
		
		c = vsInterpolate( lifeFraction, p->m_color, vsColor::Black );
		
		m_vertexArray[i] = pos;
		m_vertexArray[i+1] = dest;
		m_colorArray[i] = c;
		m_colorArray[i+1].Set(c.r,c.g,c.b,0.f);
		m_indexArray[i] = i;
		m_indexArray[i+1] = i+1;
		
		i+=2;
		
/*		list->SetColor( c );
		
		list->MoveTo( pos );
		c.a = 0.0f;
		list->SetColor( c );
		list->LineTo( dest );*/
		
		p = p->m_next;
	}
	
	if ( i > 0 )
	{
		m_vertexBuffer->SetArray(m_vertexArray,i);
		m_colorBuffer->SetArray(m_colorArray,i);
		m_indexBuffer->SetArray(m_indexArray,i);
		
		list->VertexBuffer(m_vertexBuffer);
		list->ColorBuffer(m_colorBuffer);
		list->LineListBuffer(m_indexBuffer);
		
//		list->VertexArray(m_vertexArray, i);
//		list->ColorArray(m_colorArray, i);
		
//		list->LineList(m_indexArray, i);
		list->ClearColorArray();
		list->ClearVertexArray();
	}
}

void
sstFireworks::Spawn(const vsVector2D &pos, const vsVector2D &vel, const vsColor &color, float lifeTime)
{
	sstFireworkParticle *p = m_unusedList->m_next;
	
	if ( p && p != m_unusedList )
	{
		p->m_life = 0.f;
		p->m_lifeTime = lifeTime;
		p->m_color = color;
		p->m_position = pos;
		p->m_velocity = vel;
		
		p->Extract();
		m_usedList->Append(p);
	}
}

void
sstFireworks::SpawnImmediate( int spawnCount, const vsVector2D &pos, const vsVector2D &vel, float speed, float radius, const vsColor &color, float lifeTime )
{
	for ( int i = 0; i < spawnCount; i++ )
	{
		vsVector2D offsetFromCenter = vsRandom::GetVector2D( 1.0f );
		vsVector2D position = pos + (radius * offsetFromCenter);
		vsVector2D velocity = vel + (vsRandom::GetVector2D(radius) * speed);
		
		Spawn( position, velocity, color, lifeTime );
	}
}


void
sstFireworks::SpawnImmediateBurst( int spawnCount, const vsVector2D &pos, const vsVector2D &vel, float speed, float radius, const vsColor &color, float lifeTime )
{
	for ( int i = 0; i < spawnCount; i++ )
	{
		vsVector2D offsetFromCenter = vsRandom::GetVector2D( 1.0f );
		vsVector2D position = pos + (radius * offsetFromCenter);
		vsVector2D velocity = vel + (offsetFromCenter * speed);
		
		Spawn( position, velocity, color, lifeTime );
	}
}

sstFireworkParticle::sstFireworkParticle()
{
	m_next = m_prev = this;
}


void
sstFireworkParticle::Append( sstFireworkParticle *other )
{
	other->m_next = m_next;
	other->m_prev = this;
	
	m_next->m_prev = other;
	m_next = other;
}

void
sstFireworkParticle::Extract()
{
	if ( m_prev )
		m_prev->m_next = m_next;
	if ( m_next )
		m_next->m_prev = m_prev;
	
	m_prev = m_next = this;
}


