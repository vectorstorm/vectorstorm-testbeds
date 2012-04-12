/*
 *  SST_Sequence.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 7/05/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#include "SST_Sequence.h"
#include "SST_Game.h"
#include "SST_ModeInGame.h"
#include "SST_Rocket.h"


sstSequence::sstSequence(sstModeInGame *mode):
m_mode(mode)
{
	m_spawned = false;
	Init(false);
}

sstSequence::~sstSequence()
{
}

void
sstSequence::Init( bool onRight )
{
	m_beatCount = 0;
	m_nextBeat = 0;
	
	m_onRight = onRight;
}

void
sstSequence::AddBeat( float time )
{
	vsAssert( m_beatCount < MAX_BEATS, "Too many beats in this sequence!" );
	m_targetActive[ m_beatCount ] = true;
	m_rocket[ m_beatCount ] = NULL;
	m_beat[ m_beatCount++ ] = time;
}

void
sstSequence::DoneAddingBeats()
{
	if ( !m_beatCount )
		return;
	
	m_width = m_beatCount * vsRandom::GetFloat( 70.0f, 100.0f );
	float halfWidth = 0.5f * m_width;
	float offsetPerBeat = (m_beatCount > 1)?m_width / (m_beatCount-1):0.f;
	float offsetPerBeatFortyFive = offsetPerBeat * 0.707f;	// if we're doing a 45 degree offset, adjust our distance so the stars aren't really far apart.
		
	vsVector2D movingPosition( -halfWidth, 0.f );
	
	for ( int i = 0; i < m_beatCount; i++ )
	{
		int dir = vsRandom::GetInt(4);
		if ( dir == 0 )
			movingPosition += vsVector2D( offsetPerBeatFortyFive, offsetPerBeatFortyFive );
		else if ( dir == 1 )
			movingPosition += vsVector2D( offsetPerBeatFortyFive, -offsetPerBeatFortyFive );
		else
			movingPosition += vsVector2D( offsetPerBeat, 0.f );
			
		m_segment[i].m_position = movingPosition;//.Set( -halfWidth + (i * offsetPerBeat), 0.f );
		m_segment[i].m_time = m_beat[i];
	}
	
	m_start = m_mode->GetMusicTime();
	m_end = m_beat[m_beatCount-1] + 8.0f;
	
	m_transformA.m_position = vsRandom::GetVector2D(300.0f);
	if ( m_onRight )
		m_transformA.m_position += vsVector2D(700.0f,0.f);
	else
		m_transformA.m_position -= vsVector2D(700.0f,0.f);
		
	float speed = vsRandom::GetFloat( 30.0f, 60.0f );
	float rotSpeed = vsRandom::GetFloat( -200.0f, 200.0f );
	
	float deltaTime = m_end - m_start;
	
	m_transformB.m_position = m_transformA.m_position;
	m_transformB.m_position += vsRandom::GetVector2D( speed * deltaTime );
	
	m_transformA.m_angle.Set( DEGREES(vsRandom::GetFloat(-180.0f, 180.0f)) );
	m_transformB.m_angle.Set( DEGREES(m_transformA.m_angle.GetDegrees() + rotSpeed * deltaTime) );
	
	ClampBeats();
}

void
sstSequence::ClampBeats()
{
	float margin = 100.f;
	
	vsScreen *screen = vsSystem::GetScreen();
	
	float fov = screen->GetScene(0)->GetFOV();
	float screenHeight = fov;
	float screenWidth = screenHeight * screen->GetAspectRatio();
	
	float topEdge = (screenHeight * -.5f) + 100.0f + margin;
	float botEdge = 0.f;	// never go below the halfway point, because that's just not nice.  :)
	float leftEdge = (screenWidth * -0.5f) + margin;
	float rightEdge = (screenWidth * 0.5f) - margin;
//	bool clampedSuccessfully = false;
	
	// if we're supposed to be on the right, clamp us onto the right side.
	
	if ( m_onRight )
		leftEdge = 0.f;
	else
		rightEdge = 0.f;
	
	vsVector2D worldBeatPos[MAX_BEATS];
	vsVector2D topLeft(10000.0f,10000.0f);		// arbitrary large values
	vsVector2D botRight(-1000.0f,-10000.0f);	// arbitrary small values
	
	for ( int i = 0; i < m_beatCount; i++ )
	{
		vsTransform2D t = GetTransformAtTime( m_segment[i].m_time );
		worldBeatPos[i] = t.ApplyTo( m_segment[i].m_position );
		
		if ( worldBeatPos[i].x < topLeft.x )
			topLeft.x = worldBeatPos[i].x;
		if ( worldBeatPos[i].x > botRight.x )
			botRight.x = worldBeatPos[i].x;
		if ( worldBeatPos[i].y < topLeft.y )
			topLeft.y = worldBeatPos[i].y;
		if ( worldBeatPos[i].y > botRight.y )
			botRight.y = worldBeatPos[i].y;
	}
	
	float width = botRight.x - topLeft.x;
	float height = botRight.y - topLeft.y;
	
	float usableScreenWidth = rightEdge - leftEdge;
	float usableScreenHeight = botEdge - topEdge;
	
	if ( width > usableScreenWidth || height > usableScreenHeight )
	{
		// we're never going to make this fit;  make our translation smaller!
		//printf("Too big!  Try again!\n");
		return DoneAddingBeats();
	}
	
//	float widthRange = usableScreenWidth - width;
//	float heightRange = usableScreenHeight - height;
	
	vsVector2D minAdjustment = vsVector2D::Zero;
	vsVector2D maxAdjustment = vsVector2D::Zero;
	if ( topLeft.x < leftEdge )
	{
		minAdjustment.x = leftEdge - topLeft.x;
		maxAdjustment.x = rightEdge - botRight.x;
	}
	else if ( botRight.x > rightEdge )
	{
		minAdjustment.x = rightEdge - botRight.x;
		maxAdjustment.x = leftEdge - topLeft.x;
	}

	if ( topLeft.y < topEdge )
	{
		minAdjustment.y = topEdge - topLeft.y;
		maxAdjustment.y = botEdge - botRight.y;
	}
	else if ( botRight.y > botEdge )
	{
		minAdjustment.y = botEdge - botRight.y;
		maxAdjustment.y = topEdge - topLeft.y;
	}
	
	vsVector2D adjustment( vsRandom::GetFloat(minAdjustment.x, maxAdjustment.x), 
						  vsRandom::GetFloat(minAdjustment.y, maxAdjustment.y) );
	
	m_transformA.m_position += adjustment;
	m_transformB.m_position += adjustment;
}

vsTransform2D
sstSequence::GetTransformAtTime( float time )
{
	float timeFraction = (time - m_start) / ( m_end - m_start );
	
	return vsInterpolate( timeFraction, m_transformA, m_transformB );
}

void
sstSequence::Update( float timeStep )
{
	UNUSED(timeStep);	// no using of timeStep in this case;  we want to use actual times from sysTimer.
	
	float musicTime = m_mode->GetMusicTime();
		
	m_currentTransform = GetTransformAtTime( musicTime );
	
	const float c_rocketTime = 3.0f;
	while ( m_nextBeat < m_beatCount && musicTime + c_rocketTime > m_beat[m_nextBeat] )
	{
		sstRocket *r = m_mode->GetAvailableRocket();
		
		if ( r )
		{
			m_rocket[m_nextBeat] = r;
			
			vsTransform2D transformAtImpact = GetTransformAtTime( m_beat[m_nextBeat] );
			r->Spawn( transformAtImpact.ApplyTo(m_segment[m_nextBeat].m_position), m_beat[m_nextBeat] - musicTime, this, m_nextBeat );
		}
		m_nextBeat++;
	}
	
	if ( musicTime > m_end )
		Despawn();
}

float
sstSequence::GetBrightnessForTimeDelta( float delta )
{
	if ( delta < 0.f )
		delta = -delta;
	
	const float fadeFinishTime = 2.0f;
	const float holdTime = 0.5f;
	
	if ( delta > fadeFinishTime )
		return 0.f;
	else if ( delta < holdTime )
		return 1.f;
	
	float fadeTime = fadeFinishTime - holdTime;
	
	return 1.f - ((delta-holdTime)/fadeTime);
}

void
sstSequence::DrawStar( vsDisplayList *list, int id )
{
	if ( m_targetActive[id] )
	{
		vsVector2D pos = m_currentTransform.ApplyTo( m_segment[id].m_position );
		
		vsVector2D tl( -5.0f, -5.0f );
		vsVector2D tr( 5.0f, -5.0f );
		vsVector2D bl( -5.0f, 5.0f );
		vsVector2D br( 5.0f, 5.0f );
		
		list->MoveTo( pos + tl );
		list->LineTo( pos + br );
		list->MoveTo( pos + tr );
		list->LineTo( pos + bl );
	}
}

void
sstSequence::DrawTarget( vsDisplayList *list, int id )
{
	if ( m_targetActive[id] )
	{
		vsVector2D pos = m_currentTransform.ApplyTo( m_segment[id].m_position );
		
		list->SetColor( m_segment[id].m_color );
		list->MoveTo( pos + vsVector2D(0.0f,-15.0f));
		list->LineTo( pos + vsVector2D(15.0f,0.0f));
		list->LineTo( pos + vsVector2D(0.0f,15.0f));
		list->LineTo( pos + vsVector2D(-15.0f,0.0f));
		list->LineTo( pos + vsVector2D(0.0f,-15.0f));
	}
}

void
sstSequence::DrawConnectingLine( vsDisplayList *list, int a, int b )
{
	vsVector2D posA = m_currentTransform.ApplyTo( m_segment[a].m_position );
	vsVector2D posB = m_currentTransform.ApplyTo( m_segment[b].m_position );

	list->SetColor( m_segment[a].m_color );
	list->MoveTo( posA );
	list->SetColor( m_segment[b].m_color );
	list->LineTo( posB );
}

void
sstSequence::Draw( vsRenderQueue *queue )
{
	vsDisplayList *list = queue->GetGenericList();
	
	float musicTime = m_mode->GetMusicTime();
	vsColor baseColor = vsColor::Blue;
//	vsColor c = vsColor::Blue;
	for ( int i = 0; i < m_beatCount; i++ )
		m_segment[i].m_color = vsColor::Blue * GetBrightnessForTimeDelta( musicTime - m_segment[i].m_time );
	
	float starBrightness = 0.5f;
	
	float starFadeInTime = 2.0f;
	float starFadeOutTime = 2.0f;
	
	if ( musicTime < m_start + starFadeInTime )
	{
		starBrightness = vsInterpolate( (musicTime-m_start) / starFadeInTime, 0.f, starBrightness );
	}
	else if ( musicTime < m_end - starFadeOutTime )
	{
		starBrightness = 0.5f;
	}
	else
	{
		float startFadingOutTime = m_end - starFadeOutTime;
		
		starBrightness = vsInterpolate( (musicTime-startFadingOutTime) / starFadeOutTime, starBrightness, 0.f );
	}
	
	list->SetColor( vsColor(1.0f, 1.0f, 1.0f, starBrightness) );
	for ( int i = 0; i < m_beatCount; i++ )
	{
		DrawStar( list, i );
	}
	
	for ( int i = 0; i < m_beatCount; i++ )
	{
		DrawTarget( list, i );
		
		if ( i < m_beatCount-1 )
		{
			DrawConnectingLine( list, i, i+1 );
		}
	}
}

void
sstSequence::Spawn()
{
	RegisterOnScene(0);
	m_spawned = true;
}

void
sstSequence::Despawn()
{
	Extract();
	m_spawned = false;
}



