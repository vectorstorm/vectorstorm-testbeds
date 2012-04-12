/*
 *  LT_CameraMove
 *  MMORPG2
 *
 *  Created by Trevor Powell on 18-08-2011.
 *  Copyright 2011 Trevor Powell.  All rights reserved.
 *
 */


#include "LT_CameraMove.h"
#include "LT_Camera.h"
#include "LT_Game.h"
#include "LT_Scenes.h"

ltCameraMove::ltCameraMove():
	ltLerpable("Camera Motion"),
	m_timer(0.f),
	m_forward(false)
{
	m_box = new vsModel;

	vsColor color(0.0f,0.0f,0.5f,1.0f);
	vsColor lineColor(1.0f,1.0f,1.0f,1.0f);

	vsBox3D box( vsVector3D::One * -10.f,
			vsVector3D::One * 10.f );

	//	box += vsVector3D::ZAxis * 60.f;

	m_box->AddFragment( vsMakeOutlineBox3D( box, "White", &lineColor ) );
	m_box->AddFragment( vsMakeSolidBox3D( box, "BlueLit" ) );
	m_box->SetPosition( vsVector3D::ZAxis * 60.f );


	vsBox3D floorBox( vsVector3D(-100.f, -20.f, -100.f),
			vsVector3D(200.f, -10.2f, 200.f) );
	m_world = new vsModel;
	m_world->AddFragment( vsMakeTexturedBox3D( floorBox, "Wall" ) );
}

ltCameraMove::~ltCameraMove()
{
	vsDelete( m_box );
	vsDelete( m_world );
}

void
ltCameraMove::Init()
{
	m_box->RegisterOnScene(Scene_World);
	m_world->RegisterOnScene(Scene_World);
	Parent::Init();

	m_timer = 0.f;
	m_forward = false;
}

void
ltCameraMove::Deinit()
{
	Parent::Deinit();
	m_box->Extract();
	m_world->Extract();
}

void
ltCameraMove::Update( float timeStep )
{
	if ( vsInput::Instance()->WasPressed(CID_A) )
	{
		m_forward = !m_forward;
	}
	m_timer += timeStep * (m_forward ? 1.0f : -1.0f);
	m_timer = vsClamp(0.f, m_timer, 1.f);

	float lerpValue = m_curve->GetCurveValue(m_timer);

	ltCamera *camera = ltGame::Instance()->GetCamera();

	const vsVector3D start( 30.0f, 10.0f, 0.0f );
	const vsVector3D end( 120.0f, 50.0f, 0.0f );

	vsVector3D position( vsInterpolate( lerpValue, start, end ) );
	vsQuaternion orientation( m_box->GetPosition() - position, vsVector3D::YAxis );

	camera->SetPosition( position );
	camera->SetOrientation( orientation );
}
