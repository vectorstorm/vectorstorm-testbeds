/*
 *  CT_Game.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 10/02/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#include "CT_Game.h"

#include "CT_Mode2D.h"
#include "CT_Mode3D.h"
#include "CT_SafeZone.h"

REGISTER_GAME("CameraTest", ctGame);

ctGame::ctGame():
    m_font(NULL),
	m_vgknowhow(NULL),
	m_cameraName(NULL)
{
}

ctGame::~ctGame()
{
}

void
ctGame::Init()
{
	m_sceneCount = 2;

	Parent::Init();

	{
		m_font = new vsFont("FreeSerif.txt");
		vsDisplayList *list = m_font->CreateString(FontContext_2D, "http://www.vgknowhow.com/", 30.f, Justification_Right, -1, vsColor(0.4f,0.4f,0.4f,1.f));
		m_vgknowhow = new vsSprite( list );
		m_vgknowhow->RegisterOnScene(1);

		vsVector2D br = vsSystem::GetScreen()->GetScene(1)->GetBottomRightCorner();
		br -= vsVector2D::One * 60.f;

		m_vgknowhow->SetPosition( br );
	}
	m_safeZone = new ctSafeZone;
	m_safeZone->RegisterOnScene(1);

	m_mode[Mode_2D] = new ctMode2D;
	m_mode[Mode_3D] = new ctMode3D;

	SetMode( Mode_2D );

}

void
ctGame::Deinit()
{
	SetGameMode( NULL );

	for ( int i = 0; i < MODE_MAX; i++ )
	{
		vsDelete( m_mode[i] );
	}

	vsDelete( m_vgknowhow );
	vsDelete( m_font );
	vsDelete( m_safeZone );
	vsDelete( m_cameraName );

	Parent::Deinit();
}

void
ctGame::Update( float timeStep )
{
	// todo:  Look for a button press to switch from 2D to 3D mode.

	/*if ( vsInput::Instance()->WasPressed(CID_B) )
	{
		if ( m_currentMode == Mode_2D )
			SetMode( Mode_3D );
		else
			SetMode( Mode_2D );
	}*/
}

void
ctGame::SetMode( int mode )
{
	m_currentMode = mode;
	SetGameMode( m_mode[mode] );
}

void
ctGame::SetCameraName( const vsString &cameraName )
{
	vsDelete(m_cameraName)

	vsVector2D tr = vsSystem::GetScreen()->GetScene(1)->GetTopRightCorner();
	tr.x -= 60.f;
	tr.y += 60.f;

	m_cameraName = new vsSprite(m_font->CreateString2D(cameraName, 35.f, Justification_Right));
	m_cameraName->RegisterOnScene(1);
	m_cameraName->SetPosition( tr );
}

