/*
 *  SST_ModeInGame.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 1/01/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#include "SST_ModeInGame.h"

#include "SST_Fireworks.h"
#include "SST_Game.h"
#include "SST_Hud.h"
#include "SST_Rocket.h"
#include "SST_Sequence.h"

sstModeInGame::sstModeInGame(sstGame *game):
m_game(game)
{
	m_hud = new sstHud(this);

	// Init Screen
	m_hud->RegisterOnScene(1);

	m_beatArraySize = 8192;
	m_beatCount = 0;
	m_nextBeat = 0;
	m_beat = new float[m_beatArraySize];

	m_score = 0;
	m_latestScore = 0;
	m_latestScoreTimer = 0.f;
	m_musicTimer = 0.f;

	Load("BigBoom.beats");

	m_recording = false;
	m_sequenceOnLeft = false;
	m_intermission = false;
	m_scoredThisFrame = false;

	m_rocketThrust = new vsSoundSample("RocketThrust.wav");
	m_rocketPop = new vsSoundSample("RocketPop.wav");
	m_crowdMurmur = new vsSoundSample("SmallAudienceChatter.wav");

	m_rocketsFired = 0;
	m_rocketsDestroyed = 0;
	m_rocketTotalAccuracy = 0.f;
	m_ended = false;
	m_started = false;

	//m_emitter = new vsPhysicsEmitter("Boom", 0.1f, 800);
	m_emitter = new sstFireworks(5000);
	m_emitter->RegisterOnScene( 0 );

	for ( int i = 0; i < MAX_ROCKETS; i++ )
		m_rocket[i] = new sstRocket(this);

	for ( int i = 0; i < MAX_SEQUENCES; i++ )
		m_sequence[i] = new sstSequence(this);

	m_beatSequencePos = new int[m_beatArraySize];
	int seqPos = 0;
	float c_sequenceDelta = 1.0f;
	bool simults = false;
	for ( int i = 0; i < m_beatArraySize; i++ )
	{
		if ( i > 0 && m_beat[i-1] + c_sequenceDelta < m_beat[i] )
			seqPos = 0;
		if ( i > 0 && m_beat[i] == m_beat[i-1] )
			simults = true;
		else if ( simults )
		{
			seqPos = 0;
			simults = false;
		}

		m_beatSequencePos[i] = seqPos++;
	}

	m_music = new vsMusic("BigBoom.mp3", false);

	m_background = new vsSprite( vsDisplayList::Load("Background") );
	m_background->RegisterOnScene(0);
	m_background->SetColor(vsColor::PureWhite);
}

sstModeInGame::~sstModeInGame()
{
	vsDelete(m_background);

	for ( int i = 0; i < MAX_SEQUENCES; i++ )
		vsDelete(m_sequence[i]);
	for ( int i = 0; i < MAX_ROCKETS; i++ )
		vsDelete(m_rocket[i]);
	vsDelete(m_emitter);
	vsDeleteArray(m_beat);
	vsDeleteArray(m_beatSequencePos);
	vsDelete(m_hud);
	vsDelete(m_music);
	vsDelete(m_rocketThrust);
	vsDelete(m_rocketPop);
	vsDelete(m_crowdMurmur);
}

void
sstModeInGame::Init()
{
}

void
sstModeInGame::Deinit()
{
}

void
sstModeInGame::Save( const char *filename )
{
	vsFile *saveFile = new vsFile(filename, vsFile::MODE_Write);
	vsRecord r;

	r.SetLabel(vsString("BeatCount"));
	r.SetInt( m_beatCount );

	saveFile->Record( &r );

	r.SetLabel(vsString("BeatTime"));
	for ( int i = 0; i < m_beatCount; i++ )
	{
		r.SetFloat( m_beat[i] );
		saveFile->Record( &r );
	}

	delete saveFile;
}

void
sstModeInGame::Load( const char *filename )
{
	vsFile *saveFile = new vsFile(filename, vsFile::MODE_Read);
	vsRecord r;

	m_beatCount = 0;

	if ( !saveFile->AtEnd() )
	{
		vsAssert( saveFile->Record(&r), "Failed to write out record!" );
		vsAssert( r.GetLabel().AsString() == vsString("BeatCount"), "Didn't find expected BeatCount record!" );
		vsAssert( m_beatArraySize >= r.GetToken(0).AsInteger(), "Too many beats specified in data file!" );

		while ( saveFile->Record(&r) )
		{
			vsAssert( r.GetLabel().AsString() == vsString("BeatTime"), "Found unexpected label!" );

			m_beat[ m_beatCount++ ] = r.GetToken(0).AsFloat();
		}
	}

	delete saveFile;
}

void
sstModeInGame::Update(float timeStep)
{
	m_scoredThisFrame = false;

	if ( m_started )
	{
		if ( !m_music->IsActuallyPlaying() )
		{
			if ( !m_ended )
				m_crowdMurmur->Play();
			m_ended = true;
		}
	}
	else if ( !m_music->IsPlaying() )
	{
		m_musicStartTick = vsTimerSystem::Instance()->GetCurrentMillis();

		m_music->Start();
		m_started = true;
		m_ended = false;
	}

	//m_musicTimer = (sysTimer::Instance()->GetCurrentMillis()-m_musicStartTick)/1000.0f;
	m_musicTimer = m_music->GetTime();
	m_latestScoreTimer += timeStep;

	m_emitter->Update(timeStep);

	vsInput *input = m_game->GetInput();

#if defined(RECORDING_ALLOWED)
	if ( input->WasPressed(CID_Left) )
	{
		m_music->Rewind();
		m_musicTimer = 0.f;
		m_nextBeat = 0;
		m_musicStartTick = sysTimer::Instance()->GetCurrentMillis();
		/*m_musicTimer -= 5.0f;
		 if ( m_musicTimer < 0.f )
		 m_musicTimer = 0.f;
		 m_music->GoToTime( m_musicTimer );*/

		if ( m_recording )
		{
			m_beatCount = 0;
		}
	}
	if ( input->WasPressed(CID_Up) )
	{
		m_recording = true;
	}
	if ( input->WasPressed(CID_Down) )
	{
		m_recording = false;
	}
	/*	if ( input->WasPressed(CID_Right) )
	 {
	 m_musicTimer += 5.0f;
	 m_music->GoToTime( m_musicTimer );
	 }*/

	if ( m_recording )
	{
		if ( input->WasPressed(CID_A) && m_beatCount < m_beatArraySize )
		{
			vsColor c = vsRandom::GetColor(0.5f, 1.0f);

			TriggerExplosion( vsRandom::GetVector2D(200.0f), c );
			m_beat[m_beatCount++] = m_musicTimer;
		}
		if ( input->WasPressed(CID_B) )
		{
			for ( int i = 0; i < 5; i++ )
			{
				vsColor c = vsRandom::GetColor(0.5f,1.0f);
				if ( m_beatCount < m_beatArraySize )
				{
					TriggerExplosion( vsRandom::GetVector2D(200.0f), c );
					m_beat[m_beatCount++] = m_musicTimer;
				}
			}
		}

		if ( input->WasPressed(CID_Right) )
		{
			Save("SavedBeats.txt");
		}
	}
	else
#endif // RECORDING_ALLOWED
	{
		if ( input->WasPressed(CID_A) )
		{

			for ( int i = 0; i < MAX_ROCKETS; i++ )
			{
				if ( m_rocket[i]->IsSpawned() )
					m_rocket[i]->ConsiderDetonating();
			}
		}

		const float c_earlySequenceTime = 20.0f;
		while ( m_nextBeat < m_beatCount && m_musicTimer + c_earlySequenceTime > m_beat[m_nextBeat] )
		{
			vsAssert( m_beatSequencePos[m_nextBeat] == 0, "Error in stepping through sequence!" );
			{
				sstSequence *s = GetAvailableSequence();
				vsAssert(s != NULL, "Ran out of available beat sequences!");

				s->Init(m_sequenceOnLeft);
				m_sequenceOnLeft = !m_sequenceOnLeft;
				s->AddBeat( m_beat[m_nextBeat++] );
				while ( m_nextBeat < m_beatCount && m_beatSequencePos[m_nextBeat] != 0 )
					s->AddBeat( m_beat[m_nextBeat++] );
				s->DoneAddingBeats();

				s->Spawn();
			}

			/*			sstRocket *r = GetAvailableRocket();

			 if ( r )
			 {
			 vsVector2D pos( -400 + (m_beatSequencePos[m_nextBeat] * 50.0f), -300.0f );
			 r->Spawn( pos, c_rocketTime );
			 }
			 m_nextBeat++;*/
		}

		for ( int i = 0; i < m_beatCount; i++ )
		{
			if ( m_beat[i] > m_musicTimer )
			{
				float timeToNextBeat = m_beat[i] - m_musicTimer;
				if ( !m_intermission )
				{
					if ( timeToNextBeat > 10.0f )
						m_intermission = true;
				}
				else
				{
					if ( timeToNextBeat < 3.0f )
						m_intermission = false;
				}

				break;
			}
		}
	}
}

void
sstModeInGame::TriggerRocket()
{
	m_rocketThrust->Play();
}

void
sstModeInGame::TriggerExplosion( const vsVector2D &where, const vsColor &color )
{
	//	vsColor c = vsRandom::GetColor(0.5f, 1.0f);
	m_emitter->SpawnImmediateBurst(100, where, vsVector2D::Zero, 100.0f, 15.0f, color, 5.0f);	// explosions last a long time
	m_emitter->SpawnImmediateBurst(100, where, vsVector2D::Zero, 100.0f, 15.0f, color + vsRandom::GetColor(0.0f,0.2f), 5.0f);	// explosions last a long time

	m_rocketPop->Play();
}

void
sstModeInGame::SpawnImmediate( int count, const vsVector2D &where, const vsVector2D &vel, const vsColor &color, float lifeTime )
{
	m_emitter->SpawnImmediate(count, where, vel, 5.0f, 5.0f, color, lifeTime);
}

sstRocket *
sstModeInGame::GetAvailableRocket()
{
	for ( int i = 0; i < MAX_ROCKETS; i++ )
	{
		if ( !m_rocket[i]->IsSpawned() )
			return m_rocket[i];
	}
	return NULL;
}

sstSequence *
sstModeInGame::GetAvailableSequence()
{
	for ( int i = 0; i < MAX_SEQUENCES; i++ )
	{
		if ( !m_sequence[i]->IsSpawned() )
			return m_sequence[i];
	}
	return NULL;
}

void
sstModeInGame::RegisterBeatResult(bool success, float accuracy)
{
	m_rocketsFired++;
	if ( success )
	{
		m_rocketsDestroyed++;

		if ( !m_scoredThisFrame )
			m_latestScore = 0;

		int points = (int)(100.f * accuracy);

		m_score += points;
		m_latestScore += points;
		m_latestScoreTimer = 0.f;

		m_scoredThisFrame = true;
	}
	m_rocketTotalAccuracy += accuracy;
}

