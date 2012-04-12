/*
 *  CT_Mode2D.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/01/11.
 *  Copyright 2011 Trevor Powell. All rights reserved.
 *
 */

#include "CT_Mode2D.h"

#include "CT_Camera.h"
#include "CT_Player.h"
#include "CT_Stage.h"



ctMode2D::ctMode2D()
{
}

void
ctMode2D::Init()
{
	Parent::Init();
	
	vsBox2D dims( vsVector2D(-30.f, -15.f), vsVector2D(30.f, 0.f) );
	
	m_stage = new ctStage( dims );
	m_player = new ctPlayer( dims );
	m_player->SetPosition( vsVector2D(-25.f, 0.f) );
	
	m_stage->RegisterOnScene(0);
	m_player->RegisterOnScene(0);
	
	m_camera = new ctCamera;
	m_camera->Follow( m_player );
	vsSystem::GetScreen()->GetScene(0)->Set3D(false);
	vsSystem::GetScreen()->GetScene(0)->SetCamera2D(m_camera);
	
	
	m_stage->AddBox( vsBox2D( vsVector2D(0.f,-2.f), vsVector2D(5.f,-1.f) ) );
	m_stage->AddBox( vsBox2D( vsVector2D(10.f,-4.f), vsVector2D(15.f,-3.f) ) );
	m_stage->AddBox( vsBox2D( vsVector2D(18.f,-5.f), vsVector2D(25.f,-4.f) ) );
}

void
ctMode2D::Deinit()
{
	vsSystem::GetScreen()->GetScene(0)->SetCamera2D(NULL);
	
	vsDelete( m_player );
	vsDelete( m_stage );
	vsDelete( m_camera );
	
	Parent::Deinit();
}
