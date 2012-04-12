/*
 *  AST_GameMode.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 25/03/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#include "AST_Game.h"
#include "AST_ModeHowTo.h"
#include "AST_ModeInGame.h"
#include "AST_ModeGameOver.h"
#include "AST_ModeTitleScreen.h"
#include "AST_Asteroid.h"
#include "AST_Camera.h"
#include "AST_HowToPlay.h"
#include "AST_Hud.h"
#include "AST_PlayerShip.h"
#include "AST_Star.h"

#if TARGET_OS_IPHONE
#include "Wedge.h"
#endif

REGISTER_GAME("Asteroids", astGame);

astGame::astGame()
{
	m_sceneCount = 2;	// we need two layers;  one for the game, one for our HUD.
}

void
astGame::Init()
{
	Parent::Init();

	m_score = 0;
	m_highScore = 0;
	m_playersLeft = 3;

	m_music = new vsMusic("tt2.ogg");
	m_music->Start();

	// Init emitters
	m_sparkEmitter = new vsPhysicsEmitter("Thrust", 0.1f, 160, 0);
	m_shrapnelEmitter = new vsPhysicsEmitter("Shrapnel", 0.1f, 160, 0);

	// Init Player
	m_player = new astPlayerShip(m_sparkEmitter);

	// Init Camera
	m_camera = new astCamera;
	m_camera->SetTrackSprite( m_player );

	// Init Hud
	m_hud = new astHud(this);
	m_hud->RegisterOnScene(1);

	vsSystem::GetScreen()->GetScene(0)->SetCamera2D( m_camera );

	// Add some asteroids, just for fun.

	m_asteroids = new astAsteroidPool;


	// Add some stars, for frame of reference.
	m_starCount = 6;
	m_star = new astStar *[m_starCount];
	for ( int i = 0; i < m_starCount; i++ )
	{
		m_star[i] = new astStar(m_camera);

		float rx = vsRandom::GetFloat(-400.0f,400.0f);
		float ry = vsRandom::GetFloat(-400.0f,400.0f);

		m_star[i]->m_transform.m_position.Set( rx, ry );
		m_star[i]->RegisterOnScene(0);
	}

	m_howToPlayScreen = new astHowToPlay;
	m_howToPlayScreen->RegisterOnScene(1);

	m_mode[MODE_InGame] = new astModeInGame(this, m_player);
	m_mode[MODE_GameOver] = new astModeGameOver(this);
	m_mode[MODE_Title] = new astModeTitleScreen(this);
	m_mode[MODE_HowTo] = new astModeHowTo(this);

	SetModeTitleScreen();

	StartLevel();
}

void
astGame::Deinit()
{
	SetGameMode(NULL);
	vsSystem::GetScreen()->GetScene(0)->SetCamera2D( NULL );

	for ( int i = 0; i < m_starCount; i++ )
		vsDelete(m_star[i]);
	vsDeleteArray(m_star);

	vsDelete(m_asteroids);
	vsDelete(m_camera);
	vsDelete(m_hud);

	for ( int i = 0; i < MODE_MAX; i++ )
		vsDelete(m_mode[i]);

	vsDelete(m_player);
	vsDelete(m_howToPlayScreen);
	vsDelete(m_sparkEmitter);
	vsDelete(m_shrapnelEmitter);
	vsDelete(m_music);

	Parent::Deinit();
}

void
astGame::StartLevel()
{
	m_asteroids->DespawnAllAsteroids();

	vsVector2D pos = m_camera->GetPosition();

	m_asteroids->SpawnAsteroid( astAsteroid::Large, pos, vsVector2D::Zero, 400.0f, true );
	m_asteroids->SpawnAsteroid( astAsteroid::Large, pos, vsVector2D::Zero, 400.0f, true );
	m_asteroids->SpawnAsteroid( astAsteroid::Large, pos, vsVector2D::Zero, 400.0f, true );
	m_asteroids->SpawnAsteroid( astAsteroid::Large, pos, vsVector2D::Zero, 400.0f, true );
	m_asteroids->SpawnAsteroid( astAsteroid::Large, pos, vsVector2D::Zero, 400.0f, true );
	m_asteroids->SpawnAsteroid( astAsteroid::Large, pos, vsVector2D::Zero, 400.0f, true );
}

void
astGame::Update(float timeStep)
{
#if TARGET_OS_IPHONE
	switch( GetDeviceOrientation() )
	{
		case Orientation_Portrait:
			vsSystem::Instance()->SetOrientation( Orientation_Normal );
			break;
		case Orientation_PortraitUpsideDown:
			vsSystem::Instance()->SetOrientation( Orientation_Six );
			break;
		case Orientation_LandscapeLeft:
			vsSystem::Instance()->SetOrientation( Orientation_Three );
			break;
		case Orientation_LandscapeRight:
			vsSystem::Instance()->SetOrientation( Orientation_Nine );
			break;
	}
#endif // TARGET_OS_IPHONE

	m_sparkEmitter->Update(timeStep);
	m_shrapnelEmitter->Update(timeStep);

	m_asteroids->Update(timeStep);
	m_asteroids->WrapAround( m_camera );

	if ( m_asteroids->GetSpawnedAsteroidCount() == 0 )
		StartLevel();	// start the next level!  (todo:  timer to delay this!!)

	if ( m_player->IsSpawned() )
	{
		vsVector2D v = m_player->GetPosition();
		float r = m_player->GetBoundingRadius();
		if ( m_camera->WrapAround(v,r) )
		{
			m_player->SetPosition( v );
			//m_player->Teleport();
		}
	}
}

void
astGame::AddScore( int points )
{
	m_score += points;

	if ( m_score > m_highScore )
		m_highScore = m_score;
}

bool
astGame::IsPlayerSpawned()
{
	return m_player->IsSpawned();
}

bool
astGame::AttemptPlayerSpawn(float timeStep)
{
	bool spawned = m_player->AttemptSpawn( timeStep, m_camera->GetPosition() );
	if ( spawned )
		m_playersLeft--;

	return spawned;
}

void
astGame::SetModeInGame()
{
	m_playersLeft = 3;
	m_score = 0;

	SetGameMode(m_mode[MODE_InGame]);
}

void
astGame::SetModeGameOver()
{
	SetGameMode(m_mode[MODE_GameOver]);
}

void
astGame::SetModeTitleScreen()
{
	SetGameMode(m_mode[MODE_Title]);
}

void
astGame::SetModeHowTo()
{
	SetGameMode(m_mode[MODE_HowTo]);
}

void
astGame::ShowHUD(bool show)
{
	if ( show )
		m_hud->FadeIn();
	else
		m_hud->FadeOut();
}

void
astGame::ShowAsteroids(bool show)
{
	UNUSED(show);
	if ( show )
		m_asteroids->FadeIn();
	else
		m_asteroids->FadeOut();
}

void
astGame::RespawnAsteroids()
{
	m_asteroids->RespawnAsteroids(6);
}
