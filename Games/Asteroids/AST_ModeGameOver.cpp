/*
 *  AST_ModeGameOver.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 29/12/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#include "AST_ModeGameOver.h"
#include "AST_Game.h"


astModeGameOver::astModeGameOver( astGame *game ):
	m_game(game)
{
	m_gameOverSprite = new vsSprite(vsBuiltInFont::CreateString(vsString("Game Over"), 30.0f, 60.0f, Justification_Center));
	m_gameOverSprite->SetColor( vsColor::Black );
	m_gameOverSprite->m_transform.m_position.Set( 0.0f, 0.0f );
}

astModeGameOver::~astModeGameOver()
{
	delete m_gameOverSprite;
}

void
astModeGameOver::Init()
{
	m_gameOverSprite->RegisterOnScene(1);	// put the "Game Over" sprite on screen.
	m_game->ShowHUD(true);
	m_fade = 0.f;
	m_timer = 0.f;
	m_exitting = false;
}

void
astModeGameOver::Deinit()
{
	m_gameOverSprite->Extract();			// we're leaving 'Game Over' mode, so hide our sprite again.
}

void
astModeGameOver::Update(float timeStep)
{
	const float c_fadeDuration = 1.0f;	// how long it takes our 'Game Over' text to fade in or out.
	const float c_autoExitTime = 5.0f;	// how long we remain at this screen before automatically leaving.  (includes duration of the fading 'Game Over' text)

	if ( m_exitting )
		m_fade = vsMax(m_fade - (timeStep/c_fadeDuration), 0.0f);
	else
		m_fade = vsMin(m_fade + (timeStep/c_fadeDuration), 1.0f);
	
	m_timer += timeStep;
	
	m_gameOverSprite->SetColor( vsInterpolate( m_fade, vsColor::Black, vsColor::Red ) );
	
	if ( m_exitting && m_fade == 0.f )
		m_game->SetModeTitleScreen();
	
	if ( !m_exitting && (m_game->GetInput()->WasPressed(CID_A) || m_timer > c_autoExitTime) )
	{
		m_game->RespawnAsteroids();
		m_exitting = true;
	}
}

/*
 if ( !m_gameOver )
 {
 // if we haven't decided the game is over, put our "Game Over" message into the HUD and set
 // the 'game over' flag.
 
 m_gameOverSprite->m_transform.m_position.Set( -160.0f, 0.0f );
 m_gameOverSprite->RegisterOnScene(1);
 
 m_gameOver = true;
 }
 else
 {
 // wait for the player to press 'A', and let them continue when they do.
 
 }
 }
 }
 else
 {
 v = m_player->GetPosition();
 r = m_player->GetBoundingRadius();
 if ( m_camera->WrapAround(v,r) )
 {
 m_player->SetPosition( v );
 m_player->Teleport();
 }
 }
 }
 */

