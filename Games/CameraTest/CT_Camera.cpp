/*
 *  CT_Camera.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 3/01/11.
 *  Copyright 2011 Trevor Powell. All rights reserved.
 *
 */

#include "CT_Camera.h"

#include "CT_CameraScript.h"
#include "CT_CameraScript3D.h"

#include "CT_Game.h"

ctCamera::ctCamera():
	m_sprite(NULL)
{
	m_cameraScript.AddItem( new ctCameraScript_Overview );
	m_cameraScript.AddItem( new ctCameraScript_BasicFollow );
	m_cameraScript.AddItem( new ctCameraScript_LerpFollow );
	m_cameraScript.AddItem( new ctCameraScript_SpringFollow );
	m_cameraScript.AddItem( new ctCameraScript_LooseFollow );
	m_cameraScript.AddItem( new ctCameraScript_LooseLead );
	m_cameraScript.AddItem( new ctCameraScript_LooseLeadGround );

	m_currentScript = 0;

	m_defaultFOV = 20.f;
	SetFOV(20.f);

	SetTargetPosition();
	m_cameraScript[m_currentScript]->Reset();
	ctGame::Instance()->SetCameraName( m_cameraScript[m_currentScript]->GetName() );
}

ctCamera::~ctCamera()
{
}

void
ctCamera::SetTargetPosition()
{
	if ( m_sprite )
	{
		ctCameraScript *script = m_cameraScript[m_currentScript];

		script->SetTargetPosition( m_sprite->GetPosition() + m_spriteOffset );
	}
}

void
ctCamera::Update( float timeStep )
{
	vsInput *input = vsInput::Instance();

	if ( input->WasPressed( CID_A ) )	// change scripts
	{
		m_currentScript++;
		if ( m_currentScript >= m_cameraScript.ItemCount() )
		{
			m_currentScript = 0;
		}
		SetTargetPosition();
		m_cameraScript[m_currentScript]->Reset();
		ctGame::Instance()->SetCameraName( m_cameraScript[m_currentScript]->GetName() );
	}
	if ( input->IsDown( CID_B ) )	// change offset
	{
		float leftRight = input->GetState( CID_LeftRightLAxis );
		float upDown = input->GetState( CID_UpDownLAxis );

		m_spriteOffset.x += leftRight * timeStep;
		m_spriteOffset.y += upDown * timeStep;

		SetTargetPosition();
		m_cameraScript[m_currentScript]->Reset();
	}

	float wheel = input->GetState( CID_MouseWheel );

	if ( wheel != 0.f )	// FOV change
	{
		m_defaultFOV = vsClamp( m_defaultFOV + wheel, 0.1f, 300.f );
		SetFOV( m_defaultFOV );
	}

	if ( m_sprite )
	{
		SetTargetPosition();

		ctCameraScript *script = m_cameraScript[m_currentScript];

		script->Update( timeStep );

		SetPosition( script->m_position );
		SetAngle( script->m_angle );

		if ( script->m_overrideFOV )
		{
			SetFOV( script->m_fov );
		}
		else
		{
			SetFOV( m_defaultFOV );
		}
	}
}












ctCamera3D::ctCamera3D():
m_model(NULL)
{
	//m_cameraScript.AddItem( new ctCameraScript_Overview );
	m_cameraScript.AddItem( new ctCameraScript3D_BasicFollow );
	//m_cameraScript.AddItem( new ctCameraScript_LerpFollow );
	//m_cameraScript.AddItem( new ctCameraScript_SpringFollow );
	//m_cameraScript.AddItem( new ctCameraScript_LooseFollow );
	//m_cameraScript.AddItem( new ctCameraScript_LooseLead );
	//m_cameraScript.AddItem( new ctCameraScript_LooseLeadGround );

	m_currentScript = 0;

	m_defaultFOV = DEGREES(60.f);
	SetFOV( DEGREES(60.f) );

	m_modelOffset.Set(0.f,1.f,-2.f);

	SetTargetPosition();
	m_cameraScript[m_currentScript]->Reset();
	ctGame::Instance()->SetCameraName( m_cameraScript[m_currentScript]->GetName() );
}

ctCamera3D::~ctCamera3D()
{
}

void
ctCamera3D::SetTargetPosition()
{
	if ( m_model )
	{
		ctCameraScript3D *script = m_cameraScript[m_currentScript];

		script->SetTargetPosition( m_model->GetPosition() + m_modelOffset );
	}
}

void
ctCamera3D::Update( float timeStep )
{
	vsInput *input = vsInput::Instance();

	if ( input->WasPressed( CID_A ) )	// change scripts
	{
		m_currentScript++;
		if ( m_currentScript >= m_cameraScript.ItemCount() )
		{
			m_currentScript = 0;
		}
		SetTargetPosition();
		m_cameraScript[m_currentScript]->Reset();
		ctGame::Instance()->SetCameraName( m_cameraScript[m_currentScript]->GetName() );
	}
	/*if ( input->IsDown( CID_B ) )	// change offset
	{
		float leftRight = input->GetState( CID_LeftRightLAxis );
		float upDown = input->GetState( CID_UpDownLAxis );

		m_spriteOffset.x += leftRight * timeStep;
		m_spriteOffset.y += upDown * timeStep;

		SetTargetPosition();
		m_cameraScript[m_currentScript]->Reset();
	}*/

	float wheel = input->GetState( CID_MouseWheel );

	if ( wheel != 0.f )	// FOV change
	{
		wheel *= DEGREES(2.f);
		m_defaultFOV = vsClamp( m_defaultFOV + wheel, DEGREES(10.f), DEGREES(120.f) );
		SetFOV( m_defaultFOV );
	}

	if ( m_model )
	{
		SetTargetPosition();

		ctCameraScript3D *script = m_cameraScript[m_currentScript];

		script->Update( timeStep );

		SetPosition( script->m_position );
		SetOrientation( script->m_orientation );

		if ( script->m_overrideFOV )
		{
			SetFOV( script->m_fov );
		}
		else
		{
			SetFOV( m_defaultFOV );
		}
	}
}

