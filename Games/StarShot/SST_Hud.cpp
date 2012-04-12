/*
 *  BNG_Hud.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 28/04/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#include "SST_Hud.h"
#include "SST_ModeInGame.h"

sstHud::sstHud( sstModeInGame *game ):
m_parent(game)
{
	vsString playing("Playing");
	vsString recording("Recording");
	vsString intermission("Intermission");
	vsString starting("Starting");
	vsString ended("Ended");
	
	m_playing = vsBuiltInFont::CreateString(playing, 30.0f, 50.0f, Justification_Right);
	m_recording = vsBuiltInFont::CreateString(recording, 30.0f, 50.0f, Justification_Right);
	m_intermission = vsBuiltInFont::CreateString(intermission, 30.0f, 50.0f, Justification_Center);

	m_starting = vsBuiltInFont::CreateString(starting, 30.0f, 50.0f, Justification_Right);
	m_ended = vsBuiltInFont::CreateString(ended, 30.0f, 50.0f, Justification_Right);
}

sstHud::~sstHud()
{
	delete m_playing;
	delete m_recording;
	delete m_intermission;
	delete m_starting;
	delete m_ended;
}

void
sstHud::Update( float timeStep )
{
	if ( m_parent->IsIntermission() || m_parent->IsOver() )
		m_intermissionBrightness = vsMin( 1.0f, m_intermissionBrightness + timeStep );
	else
		m_intermissionBrightness = vsMax( 0.0f, m_intermissionBrightness - timeStep );
}

void
sstHud::Draw( vsRenderQueue *queue )
{
	vsDisplayList *list = queue->GetGenericList();
	
	float leftEdge = vsSystem::GetScreen()->GetScene(1)->GetTopLeftCorner().x;
	float rightEdge = vsSystem::GetScreen()->GetScene(1)->GetTopRightCorner().x;
	
	vsTransform2D t;
	t.m_position.Set(rightEdge - 10.0f,-440.0f);
	list->SetColor( vsColor::LightBlue );

//	list->MoveTo( vsVector2D(-1000.0f, -300.0f) );
//	list->LineTo( vsVector2D(1000.0f, -300.0f) );
	

	list->PushTransform( t );
	if ( m_parent->IsOver() )
		list->Append( *m_ended );
	else 
	{
		if ( m_parent->IsRecording() )
			list->Append( *m_recording );
		else if ( m_parent->IsStarted() )
			list->Append( *m_playing );
		else
			list->Append( *m_starting );
		
		float mTime = m_parent->GetMusicTime();
		int mins = (int)(mTime / 60.0f);
		mTime -= mins * 60.0f;
		int secs = (int)mTime;
		mTime -= secs;
		int millis = (int)(mTime * 1000.0f);
		
		vsString timeStr = vsFormatString("%02d:%02d.%03d", mins, secs, millis);
		vsDisplayList *timeList = vsBuiltInFont::CreateString(timeStr, 50.0f, 50.0f, Justification_Right);
		
		t.m_position.Set(0.f,70.0f);
		list->PushTransform( t );
		list->Append( *timeList );
		list->PopTransform();

		delete timeList;
}
	list->PopTransform();
	
	t.m_position.Set(leftEdge + 10.0f,-440.0f);
	list->PushTransform( t );

	vsString scoreStr = vsFormatString("%08d", m_parent->GetScore());
	vsDisplayList *score = vsBuiltInFont::CreateString(scoreStr, 30.0f, 30.0f, Justification_Left);
	list->Append( *score );
	delete score;

	float colorAlpha = 1.0f - (0.5f * m_parent->GetLatestScoreTimer());
	if ( colorAlpha > 0.0f )
	{
		vsColor c = vsColor::LightBlue;
		c.a *= colorAlpha;
		
		t.m_position.Set(160.f,70.0f);
		list->PushTransform( t );
		scoreStr = vsFormatString("+%d", m_parent->GetLatestScore());
		score = vsBuiltInFont::CreateString(scoreStr, 50.0f, 50.0f, Justification_Center);
		list->SetColor(c);
		list->Append( *score );
		delete score;
		list->PopTransform();
	}
	list->PopTransform();
	
	if ( m_intermissionBrightness )
	{
		list->SetColor( vsColor(0.0f, 0.0f, 1.0f, m_intermissionBrightness) );
		if ( m_parent->IsIntermission() )
		{
			t.m_position.Set(0.f,-80.f);
			list->PushTransform(t);
			list->Append( *m_intermission );
			list->PopTransform();
		}
		
		sstModeInGame *g = m_parent;
		int destroyed = g->GetRocketsDestroyed();
		int fired = g->GetRocketsFired();
		int perc = 0;
		if ( destroyed > 0 )
			perc = (100 * destroyed) / fired;
		int accPerc = (int)(100 * g->GetRocketsAccuracy());
		vsDisplayList *hits = vsBuiltInFont::CreateString(STR("Hits: %d/%d (%d%%)", destroyed, fired, perc), 30.0f, 30.0f, Justification_Center);
		vsDisplayList *acc = vsBuiltInFont::CreateString(STR("Accuracy: %d%%", accPerc), 30.0f, 30.0f, Justification_Center);
		
		t.m_position.Set(0.f,0.f);
		list->PushTransform(t);
		list->SetColor( vsColor(0.0f, 0.0f, 1.0f, m_intermissionBrightness) );
		list->Append( *hits );
		list->PopTransform();

		t.m_position.Set(0.f,50.f);
		list->PushTransform(t);
		list->SetColor( vsColor(0.0f, 0.0f, 1.0f, m_intermissionBrightness) );
		list->Append( *acc );
		list->PopTransform();
		
		delete acc;
		delete hits;
	}
//	list->SetColor( vsColor::White );
//	list->MoveTo( vsVector2D(0.0f,0.0f) );
//	list->LineTo( vsVector2D(50.0f, 50.0f) );
}
