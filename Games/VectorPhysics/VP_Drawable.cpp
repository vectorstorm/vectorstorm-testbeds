/*
 *  VP_Drawable.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/02/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#include "VP_Drawable.h"
#include "VP_Game.h"

static vsColor s_color[vpDrawable::DRAWMODE_MAX] =
{
	vsColor::Blue,
	vsColor::Yellow,
	vsColor::White
};

const vsColor &	vpDrawable::GetColorForDrawMode( int mode )
{
	vsAssert( mode < vpDrawable::DRAWMODE_MAX, "Illegal draw mode specified!" );
	return s_color[mode];
}

vpDrawable::vpDrawable( vpGame *game, int drawMode ):
	m_game(game)
{
	m_sampleCount = 0;
	//m_shader = new vsJitterShader(0.15f,0.05f);
	m_object = NULL;
	m_objectCount = 0;

	m_colorPulse = 0.f;
	m_color = GetColorForDrawMode(drawMode);
	m_drawMode = drawMode;
	m_error = 0.f;

	m_hilighted = false;
	m_drawing = true;
	m_toDelete = false;
}

vpDrawable::~vpDrawable()
{
	for ( int i = 0; i < m_objectCount; i++ )
		vsDelete(m_object[i]);
	vsDeleteArray(m_object);
	//vsDelete(m_shader);
}

vsVector3D
vpDrawable::CalculateOptimisedPositionForIndex(int index, int ignoringOptimisedIndex)
{
	int preIndex = 0;
	int postIndex = m_optimisedIndexCount;

	for ( int i = 0; i < m_optimisedIndexCount; i++ )
	{
		if ( i != ignoringOptimisedIndex )
		{
			int optIndex = m_optimisedIndex[i];

			if ( optIndex <= index )
				preIndex = optIndex;
			if ( optIndex >= index )
			{
				postIndex = optIndex;
				break;
			}
		}
	}

	vsVector3D correctPos = m_samplePos[index];

	if ( preIndex == index || postIndex == index )
		return correctPos;

	// okay.  We're on an optimised line segment from preIndex to postIndex;  we want to find where
	// on that line segment is closest to 'correctPos'.

	vsVector3D segmentA = m_samplePos[preIndex];
	vsVector3D segmentB = m_samplePos[postIndex];
	vsVector3D segmentDirection = segmentB - segmentA;
	float segmentLength = segmentDirection.Length();
	segmentDirection *= 1.0f / segmentLength;

	// Use a dot product to find the projection of 'segmentA->correctPos' onto 'segmentA->segmentB', and use that to pick
	// the nearest spot on the line segment.

	float projectionLength = segmentDirection.Dot( correctPos - segmentA );
	// clamp it into (0..segmentLength).
	projectionLength = vsClamp(projectionLength, 0.f, segmentLength);

	return (segmentA + (segmentDirection * projectionLength));	// done!
}

void
vpDrawable::Optimise_Rigid()
{
	m_error = 0.f;

	// okay.  We need to force ourselves down to just 32 indices, trying to minimise our error value.
	//
	// First, initialise our optimised indices to match the unoptimised ones.

	for ( int i = 0; i < m_sampleCount; i++ )
		m_optimisedIndex[i] = i;
	m_optimisedIndexCount = m_sampleCount;


	// Here's the general strategy we're going to follow:
	//
	// First, remove all zero-error indices.
	//
	// Then, repeatedly remove a single index until we're down to 32.

	float s_epsilon = 0.1f * 0.1f;

	for ( int i = 0; i < m_optimisedIndexCount; i++ )
	{
		if ( CalculateOptimisedError(i) < s_epsilon )
			RemoveOptimisedIndex(i);
	}

	while( m_optimisedIndexCount > 32 )
	{
		OptimiseOutOneIndex();
	}

	float overallAllowableError = 0.05f * m_optimisedIndexCount;
	while( m_error < (overallAllowableError*overallAllowableError) && m_optimisedIndexCount > 2 )
	{
		overallAllowableError = 0.02f * m_optimisedIndexCount;
		OptimiseOutOneIndex();
	}
}

void
vpDrawable::Optimise_Rope()
{
	// with rope, we want a very even distribution of points, so we need to calculate distance

	float totalDistance = 0.f;

	float sampleDistance[MAX_SAMPLES];

	for ( int i = 0; i < m_sampleCount-1; i++ )
	{
		const vsVector3D &start = m_samplePos[i];
		const vsVector3D &end = m_samplePos[i+1];

		sampleDistance[i] = (end-start).Length();
		totalDistance += sampleDistance[i];
	}

	// okay, now we need to divide the rope up into 32 equal parts.

	float ropeSegmentLengths = 0.2f;
	float distanceBucket = 0.f;

	m_optimisedIndex[0] = 0;
	m_optimisedIndexCount = 1;

	for ( int i = 1; i < m_sampleCount-1; i++ )
	{
		distanceBucket += sampleDistance[i];

		if ( distanceBucket > ropeSegmentLengths )
		{
			distanceBucket -= ropeSegmentLengths;

			vsAssert(m_optimisedIndexCount < MAX_SAMPLES, "Too many rope segments!");

			m_optimisedIndex[m_optimisedIndexCount++] = i;
		}
	}
}

void
vpDrawable::OptimiseOutOneIndex()
{
	// By breaking up the task into smaller segments, each one becomes much easier.
	//
	// This function merely needs to remove one of our optimised indices.  To do this,
	// we will test what happens if we remove each of our indices, and in the end, we
	// will remove the index that causes the least overall error.

	float	leastError = 10000000.0f;	// arbitrary huge number
	int		leastErrorIndex = 1;

	for( int i = 1; i < m_optimisedIndexCount-1; i++ )	// not allowed to remove the first or last indices!
	{
		float err = CalculateOptimisedError(i);

		if ( err < leastError )
		{
			leastError = err;
			leastErrorIndex = i;
		}
	}

	// now, remove the index selected as causing the least error.
	RemoveOptimisedIndex(leastErrorIndex);

	m_error = leastError;
}

void
vpDrawable::RemoveOptimisedIndex(int id)
{
	for ( int i = id; i < m_optimisedIndexCount-1; i++ )
	{
		m_optimisedIndex[i] = m_optimisedIndex[i+1];
	}
	m_optimisedIndexCount--;
}

float
vpDrawable::CalculateOptimisedError(int ignoringOptimisedIndex)
{
	// this one is easy.  We just get each of our samples, find out what its optimised equivalent is, and
	// sum the squared error.

	float sse = 0.f;	// sum of squared error

	for ( int i = 0; i < m_sampleCount; i++ )
	{
		vsVector3D optPos = CalculateOptimisedPositionForIndex(i, ignoringOptimisedIndex);
		vsVector3D errorVec = optPos - m_samplePos[i];
		sse += errorVec.SqLength();
	}

	return sse;
}

void
vpDrawable::ConvertToPhysics()
{
	if ( m_drawMode == DrawMode_Rope )
		ConvertToPhysics_Rope();
	else
		ConvertToPhysics_Rigid();
}

void
vpDrawable::ConvertToPhysics_Rigid()
{
	m_objectCount = 1;
	m_object = new vsCollisionObject *[1];

	bool isStatic = false;
	if ( m_drawMode == DrawMode_Static )
		isStatic = true;

	m_object[0] = new vsCollisionObject( ColFlag_Player, ColFlag_All, isStatic );
	// for each line of our optimised model, add a box to our collision object!

	for ( int i = 0; i < m_optimisedIndexCount-1; i++ )
	{
		const vsVector2D &start = m_samplePos[m_optimisedIndex[i]];
		const vsVector2D &end = m_samplePos[m_optimisedIndex[i+1]];

		vsVector2D direction(end.x-start.x,end.y-start.y);
		vsAngle ang = vsAngle::FromForwardVector(direction);
		float length = direction.Length();

		if ( length > 0.f )
		{
			float density = 1.0f;
			float width = 0.15f;

			if ( m_drawMode == DrawMode_Static )
				density = 0.f;

			vsVector2D extent( width, length );
			vsVector2D midpoint = 0.5f * (start + end);

			m_object[0]->AddBox( extent, midpoint, ang, density );
		}
	}

	m_object[0]->SetCollisionsActive(true);
	m_object[0]->SetUserData(this);
	m_object[0]->SetResponder(this);
}

void
vpDrawable::ConvertToPhysics_Rope()
{
	m_objectCount = 0;
	// for each line of our optimised model, add a box to our collision object!

	m_object = new vsCollisionObject *[m_optimisedIndexCount-1];

	for ( int i = 0; i < m_optimisedIndexCount-1; i++ )
	{
		const vsVector2D &start = m_samplePos[m_optimisedIndex[i]];
		const vsVector2D &end = m_samplePos[m_optimisedIndex[i+1]];

		vsVector2D direction = end-start;
		vsAngle ang = vsAngle::FromForwardVector(direction);
		float length = direction.Length();

		vsAssert( length != 0.f, "Error, rope segment with 0 length!  Fix the optimisation code!" );

		{
			m_object[m_objectCount] = new vsCollisionObject( ColFlag_Player, ColFlag_All );

			float density = 0.5f;
			float width = 0.15f;

			vsVector2D extent( width, length + 0.1f );
			vsVector2D midpoint = 0.5f * (start + end);

			m_object[m_objectCount]->SetBox( extent, midpoint, ang, density );
			m_object[m_objectCount]->SetCollisionsActive(true);
			m_object[m_objectCount]->SetUserData(this);
			m_object[m_objectCount]->SetResponder(this);
			m_objectCount++;
		}
	}

	for ( int i = 0; i < m_objectCount-1; i++ )
	{
		m_object[i]->AddRotationJointTo( m_object[i+1], m_samplePos[m_optimisedIndex[i+1]] );
	}
}

void
vpDrawable::Update( float timeStep )
{
	vsInput *input = m_game->GetInput();

	if ( m_drawing )
	{
		m_colorPulse += timeStep * 2.0f;

		if ( m_colorPulse > 1.0f )
			m_colorPulse -= 1.0f;

		if ( !input->IsDown(CID_MouseLeftButton) )
		{
			if ( m_drawMode == DrawMode_Rope )
				Optimise_Rope();
			else
				Optimise_Rigid();
			ConvertToPhysics();
			m_drawing = false;
		}
	}

	if ( m_drawing )
	{
		vsVector2D mousePos = input->GetMousePosition();

		vsVector2D delta = mousePos - m_samplePos[m_sampleCount-1];

		const float c_error = 0.01f;

		if ( m_sampleCount < MAX_SAMPLES )
		{
			if ( delta.SqLength() > c_error * c_error )
			{
				m_samplePos[m_sampleCount++] = mousePos;
			}
		}
	}
	else
	{
		// not drawing;  probably under physics control.

		for ( int i = 0; i < m_objectCount; i++ )
			m_object[i]->Update(timeStep);
	}
}

void
vpDrawable::Draw( vsRenderQueue *queue )
{
	if ( m_toDelete )
		return;

	if ( m_sampleCount >= 2 )
	{
		vsDisplayList *list = queue->GetGenericList();

		if ( m_drawing )
		{
			vsColor c = m_color;
			float pulse = m_colorPulse;
			c.a = pulse;

			list->SetColor(c);
			int i = 0;
			list->MoveTo( m_samplePos[i] );

			for ( i = 1; i < m_sampleCount; i++ )
			{
				pulse -= (1.0f/30.0f);
				if ( pulse < 0.0f )
					pulse += 1.0f;
				c.a = vsInterpolate(pulse, 0.8f, 0.3f);
				list->SetColor(c);

				list->LineTo(m_samplePos[i]);
			}
		}
		else
		{
			//list->SetShader(m_shader);
			if ( m_hilighted )
				list->SetColor(vsColor::Red);
			else
				list->SetColor(m_color);

			if ( m_drawMode == DrawMode_Rope )
			{
				list->MoveTo( m_object[0]->GetPosition() );
				for ( int i = 1; i < m_objectCount; i++ )
				{
					list->LineTo( m_object[i]->GetPosition() );
				}
			}
			else	// static
			{
				vsTransform2D t;
				t.m_position = ( m_object[0]->GetPosition() );
				t.m_angle = ( m_object[0]->GetAngle() );

				list->PushTransform(t);
				list->VertexArray( m_samplePos, m_sampleCount );
				list->LineStrip( m_optimisedIndex, m_optimisedIndexCount );
				list->PopTransform();
			}
			//list->SetShader(NULL);
		}
	}
}

void
vpDrawable::DestroyCallback()
{
	SetToDelete();

	// also need to shut down all my collision objects now, as some will be being shut down by the main collision system.

	for ( int i = 0; i < m_objectCount; i++ )
		m_object[i]->SetCollisionsActive(false);
}

