/*
 *  VP_Game.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/02/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#include "VP_Game.h"

#include "VP_Drawable.h"

#include <VS_VectorStorm.h>

REGISTER_GAME("VectorPhysics", vpGame);

vpGame::vpGame()
{
}

vpGame::~vpGame()
{
}

void
vpGame::Init()
{
	vsCollisionSystem::Instance()->SetWorldBounds(-50.f, 50.f, -50.f, 50.f);
	vsCollisionSystem::Instance()->SetNoBorder();

	Parent::Init();

	vsCollisionSystem::Instance()->SetGravity( vsVector2D(0.0f,10.0f) );

	vsSystem::Instance()->HideCursor();

	m_camera = new vsCamera2D();
	m_camera->SetFOV(20.f);	// from -10 to 10, vertically
	vsSystem::GetScreen()->GetScene(0)->SetCamera2D(m_camera);

	vsVector2D tr = vsSystem::GetScreen()->GetScene(0)->GetTopRightCorner();

	for ( int i = 0; i < MAX_DRAWABLES; i++ )
	{
		m_drawable[i] = NULL;
	}

	m_drawMode = vpDrawable::DrawMode_Static;

	m_cursor = new vsSprite( vsDisplayList::Load("Cursor") );
#if !TARGET_OS_IPHONE
	m_cursor->RegisterOnScene(0);
#endif // TARGET_OS_IPHONE
	m_cursor->SetColor(vsColor::White);

	m_staticButton = new vsSprite( vsDisplayList::Load("CursorButton") );
	m_staticButton->RegisterOnScene(0);
	m_staticButton->SetColor(vpDrawable::GetColorForDrawMode( vpDrawable::DrawMode_Static ));

	m_ropeButton = new vsSprite( vsDisplayList::Load("CursorButton") );
	m_ropeButton->RegisterOnScene(0);
	m_ropeButton->SetColor(vpDrawable::GetColorForDrawMode( vpDrawable::DrawMode_Rope ));

	m_normalButton = new vsSprite( vsDisplayList::Load("CursorButton") );
	m_normalButton->RegisterOnScene(0);
	m_normalButton->SetColor(vpDrawable::GetColorForDrawMode( vpDrawable::DrawMode_Normal ));

	m_staticButton->SetPosition( vsVector2D(tr.x - 3.f, -5.f) );
	m_normalButton->SetPosition( vsVector2D(tr.x - 3.f, 0.f) );
	m_ropeButton->SetPosition( vsVector2D(tr.x - 3.f, 5.f) );
}

void
vpGame::Deinit()
{
	vsSystem::GetScreen()->GetScene(0)->SetCamera2D(NULL);
	vsDelete(m_camera);
	vsDelete(m_cursor);
	vsDelete(m_staticButton);
	vsDelete(m_ropeButton);
	vsDelete(m_normalButton);
	for ( int i = 0; i < MAX_DRAWABLES; i++ )
	{
		if ( m_drawable[i] )
		{
			vsDelete(m_drawable[i]);
		}
	}
	Parent::Deinit();
}

void
vpGame::Update(float timeStep)
{
	UNUSED(timeStep);

	if ( GetInput()->WasPressed(CID_MouseLeftButton) )
	{
		// check if I clicked near one of the buttons
		vsVector2D mousePos = GetInput()->GetMousePosition();

		vsVector2D distToRope = mousePos - m_ropeButton->GetPosition();
		vsVector2D distToNormal = mousePos - m_normalButton->GetPosition();
		vsVector2D distToStatic = mousePos - m_staticButton->GetPosition();
		if ( distToRope.SqLength() < 1.0f )
		{
			m_drawMode = vpDrawable::DrawMode_Rope;
		}
		else if ( distToNormal.SqLength() < 1.0f )
		{
			m_drawMode = vpDrawable::DrawMode_Normal;
		}
		else if ( distToStatic.SqLength() < 1.0f )
		{
			m_drawMode = vpDrawable::DrawMode_Static;
		}
		else
		{
			for ( int i = 0; i < MAX_DRAWABLES; i++ )
			{
				if ( m_drawable[i] == NULL )
				{
					m_drawable[i] = new vpDrawable(this, m_drawMode);
					m_drawable[i]->RegisterOnScene(0);
					break;
				}
			}
		}
	}



	vsColor modeColor[3] =
	{
		vsColor::Blue,		// NORMAL
		vsColor::Yellow,	// ROPE
		vsColor::White		// STATIC
	};

	m_cursor->SetColor( modeColor[m_drawMode] );
	m_cursor->SetPosition( GetInput()->GetMousePosition() );

	bool deleting = GetInput()->WasPressed(CID_MouseRightButton);

	for ( int i = 0; i < MAX_DRAWABLES; i++ )
	{
		if ( m_drawable[i] )
		{
			if ( deleting && m_drawable[i]->IsHilighted() )
			{
				m_drawable[i]->SetToDelete();
			}
			else
			{
				m_drawable[i]->SetHilighted(false);
			}
		}
	}

#define MAX_TO_FIND (10)
	vsCollisionObject *foundColObjects[MAX_TO_FIND];

	int numFound = vsCollisionSystem::Instance()->FindColObjectsUnder( m_cursor->GetPosition(),  0.05f, foundColObjects, MAX_TO_FIND );
	if ( numFound )
	{
		for ( int i = 0; i < numFound; i++ )
		{
			vpDrawable *d = (vpDrawable *)foundColObjects[i]->GetUserData();

			d->SetHilighted(true);
		}
	}

	for ( int i = 0; i < MAX_DRAWABLES; i++ )
	{
		if ( m_drawable[i] && m_drawable[i]->IsToDelete() )
		{
			vsDelete(m_drawable[i]);
		}
	}
}

