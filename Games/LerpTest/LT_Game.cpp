/*
 *  LT_Game
 *  VectorStorm
 *
 *  Created by Trevor Powell on 16-08-2011.
 *  Copyright 2011 Trevor Powell.  All rights reserved.
 *
 */

#include "LT_Game.h"
#include "LT_Camera.h"
#include "LT_Graph.h"
#include "LT_Lerpable.h"
#include "LT_ResponseCurve.h"
#include "LT_Scenes.h"
#include "LT_CameraMove.h"
#include "LT_PlayerControl.h"
#include "LT_WindowFade.h"
#include "LT_WindowSlide.h"

REGISTER_GAME("LerpTest", ltGame);

ltGame::ltGame():
	m_lerpable(NULL),
	m_lerpableCount(0),
	m_currentLerpable(0),
	m_curve(NULL),
	m_curveCount(0),
	m_currentCurve(0),
	m_graph(NULL)
{
	m_curveCount = 4;
	m_curve = new ltResponseCurve*[m_curveCount];
	m_curve[0] = new ltResponseCurve_Linear;
	m_curve[1] = new ltResponseCurve_Quadratic;
	m_curve[2] = new ltResponseCurve_Cubic;
	m_curve[3] = new ltResponseCurve_Hermite;
}

ltGame::~ltGame()
{
	for( int i = 0; i < m_curveCount; i++ )
	{
		vsDelete( m_curve[i] );
	}
	vsDeleteArray( m_curve );
}

void
ltGame::Init()
{
	m_sceneCount = 3;
	Parent::Init();

	m_camera = new ltCamera;
	vsSystem::GetScreen()->GetScene( Scene_World )->SetCamera3D( m_camera );

	m_graph = new ltGraph;
	m_graph->RegisterOnScene(Scene_Graph);
	m_graph->SetBox( vsBox2D( vsVector2D(400.f, -400.f), vsVector2D(600.f, -200.f) ) );
	m_graph->SetResponseCurve( m_curve[m_currentCurve] );

	m_lerpableCount = 5;
	m_lerpable = new ltLerpable*[m_lerpableCount];
	m_lerpable[0] = new ltLerpableNone;
	m_lerpable[1] = new ltWindowFade;
	m_lerpable[2] = new ltWindowSlide;
	m_lerpable[3] = new ltCameraMove;
	m_lerpable[4] = new ltPlayerControl;

	m_light = new vsLight;
	m_light->SetType( vsLight::Type_Point );
	m_light->SetPosition( vsVector3D( 15.f, 15.f, -30.f ));
	m_light->SetColor( vsColor::White );
	m_light->SetAmbientColor( vsColor::White * 0.2f );
	vsSystem::GetScreen()->GetScene( Scene_World )->AddLight( m_light );

	vsVector2D baseLerpablePos = vsSystem::GetScreen()->GetScene(1)->GetTopLeftCorner() + vsVector2D(20.f, 40.f);
	vsVector2D offset(0.f, 40.f);

	for ( int i = 0; i < m_lerpableCount; i++ )
	{
		m_lerpable[i]->SetPosition(baseLerpablePos + offset * i);
		m_lerpable[i]->RegisterOnScene(Scene_Graph);
		m_lerpable[i]->SetCurve( m_curve[ m_currentCurve ] );
	}
}

void
ltGame::Deinit()
{
	vsSystem::GetScreen()->GetScene( Scene_World )->SetCamera3D( NULL );
	vsSystem::GetScreen()->GetScene( Scene_World )->RemoveLight( m_light );
	vsDelete( m_graph );

	for (int i = 0; i < m_lerpableCount; i++)
	{
		vsDelete( m_lerpable[i] );
	}
	vsDeleteArray( m_lerpable );

	vsDelete( m_camera );
	vsDelete( m_light );

	Parent::Deinit();
}

void
ltGame::Update( float timeStep )
{
	if ( vsInput::Instance()->WasPressed(CID_B) )
	{
		m_currentCurve++;
		if ( m_currentCurve >= m_curveCount )
		{
			m_currentCurve = 0;
		}
		m_graph->SetResponseCurve( m_curve[m_currentCurve] );
		m_lerpable[ m_currentLerpable ]->SetCurve( m_curve[ m_currentCurve ] );

	}

	int lerpIncrement = 0;
	if ( vsInput::Instance()->WasPressed(CID_Up) )
	{
		lerpIncrement--;
	}
	else if ( vsInput::Instance()->WasPressed(CID_Down) )
	{
		lerpIncrement++;
	}
	int nextLerpable = vsClamp( 0, m_currentLerpable + lerpIncrement, m_lerpableCount-1 );

	if ( nextLerpable != m_currentLerpable )
	{
		m_lerpable[ m_currentLerpable ]->Deinit();
		m_currentLerpable = nextLerpable;
		m_lerpable[ m_currentLerpable ]->Init();
		m_lerpable[ m_currentLerpable ]->SetCurve( m_curve[ m_currentCurve ] );
	}
}
