/*
 *  SST_Sequence.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 7/05/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#ifndef SST_SEQUENCE_H
#define SST_SEQUENCE_H

class sstRocket;
class sstModeInGame;

struct sstSequenceSegment
{
	vsVector2D			m_position;
	vsColor				m_color;
	float				m_time;
};

class sstSequence : public vsEntity
{
	sstModeInGame *		m_mode;
#define MAX_BEATS (12)
	
	vsTransform2D			m_transformA;	// we go from A to B over the course of m_start to m_end
	vsTransform2D			m_transformB;
	
/*	vsVector2D			m_positionA;	
	vsAngle				m_angleA;		
	vsVector2D			m_positionB;
	vsAngle				m_angleB;
	vsVector2D			m_currentPosition;*/
	
	vsTransform2D			m_currentTransform;
	
	sstSequenceSegment  m_segment[MAX_BEATS];
	sstRocket *			m_rocket[MAX_BEATS];
	bool				m_targetActive[MAX_BEATS];
	
	float				m_beat[MAX_BEATS];
	int					m_beatCount;
	int					m_nextBeat;
	
	float				m_start;
	float				m_end;
	
	float				m_width;
	
	bool				m_onRight;
	bool				m_spawned;
	
	float				GetBrightnessForTimeDelta( float delta );
	void				DrawTarget( vsDisplayList *list, int id );
	void				DrawConnectingLine( vsDisplayList *list, int a, int b );
	void				DrawStar( vsDisplayList *list, int id );
	
	//vsAngle				GetAngleAtTime( float time );
	//vsVector2D			GetPositionAtTime( float time );
	vsTransform2D			GetTransformAtTime( float time );
	
	void				ClampBeats();
	
public:
						sstSequence(sstModeInGame *mode);
	virtual				~sstSequence();

	void				Init( bool onRight );
	void				AddBeat( float time );
	void				DoneAddingBeats();
	
	void				Spawn();
	void				Despawn();
	bool				IsSpawned() { return m_spawned; }

	virtual void		Update(float timeStep);
	virtual void		Draw( vsRenderQueue *queue );
	
	void				RegisterDetonation(int id) { m_targetActive[id] = false; }
};

#endif // SST_SEQUENCE_H

