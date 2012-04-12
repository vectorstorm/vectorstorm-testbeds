/*
 *  SST_ModeInGame.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 1/01/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#ifndef SST_MODEINGAME_H
#define SST_MODEINGAME_H

class sstGame;
class sstHud;
class sstRocket;
class sstSequence;
class sstFireworks;

#define MAX_ROCKETS (64)
#define MAX_SEQUENCES (64)

class sstModeInGame : public coreGameMode
{
	sstGame *			m_game;
	
	sstFireworks *		m_emitter;
	vsMusic *			m_music;
	
	vsSoundSample *			m_rocketThrust;
	vsSoundSample *			m_rocketPop;
	vsSoundSample *			m_crowdMurmur;
	
	sstRocket *			m_rocket[MAX_ROCKETS];
	sstSequence *		m_sequence[MAX_SEQUENCES];
	
	sstHud *			m_hud;
	
	unsigned long		m_musicStartTick;
	float				m_musicTimer;
	
	float *				m_beat;
	int *				m_beatSequencePos;
	int					m_beatCount;
	int					m_beatArraySize;
	int					m_nextBeat;
	
	int					m_rocketsFired;
	int					m_rocketsDestroyed;
	float				m_rocketTotalAccuracy;
	int					m_score;
	int					m_latestScore;
	float				m_latestScoreTimer;
	
	int					m_nextStringStartsAt;
	
	bool				m_sequenceOnLeft;
	
	bool				m_recording;
	bool				m_intermission;
	
	bool				m_scoredThisFrame;
	bool				m_started;
	bool				m_ended;
	
	vsSprite *			m_background;
	
	void				Load( const char *filename );
	void				Save( const char *filename );
	
	sstSequence *		GetAvailableSequence();
	
public:
	
public:
	sstModeInGame( sstGame *game );
	virtual ~sstModeInGame();
	
	virtual void	Init();
	virtual void	Deinit();
	
	virtual void	Update(float timeStep);
	
	float			GetMusicTime() { return m_musicTimer; }
	bool			IsRecording() { return m_recording; }
	sstRocket *		GetAvailableRocket();
	
	bool			IsStarted() { return m_started; }
	bool			IsIntermission() { return m_intermission; }
	bool			IsOver() { return m_ended; }
	
	void			RegisterBeatResult( bool success, float accuracy = 0.f );
	
	int				GetScore() { return m_score; }
	int				GetLatestScore() { return m_latestScore; }
	float			GetLatestScoreTimer() { return m_latestScoreTimer; }
	int				GetRocketsFired() { return m_rocketsFired; }
	int				GetRocketsDestroyed() { return m_rocketsDestroyed; }
	float			GetRocketsAccuracy() { return m_rocketTotalAccuracy / (float)m_rocketsDestroyed; }
	
	void			TriggerExplosion( const vsVector2D &where, const vsColor &color );
	void			TriggerRocket();
	void			SpawnImmediate( int count, const vsVector2D &where, const vsVector2D &vel, const vsColor &color, float lifetime );
	
};

#endif // SST_MODEINGAME_H

