/*
 *  SMX_Game.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 10/02/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#include "LAG_Game.h"

//REGISTER_GAME("LagTest", lagGame);

class lagSprite : public vsSprite
{
	bool m_buttonDown;

public:

	lagSprite(): m_buttonDown(false) {}
	virtual ~lagSprite() {}

	virtual void Update( float timeStep )
	{
		m_buttonDown = vsInput::Instance()->GetState( CID_A );
	}

	virtual void DynamicDraw( vsRenderQueue *queue )
	{
		if ( m_buttonDown )
		{
			vsScene *scene = queue->GetScene();

			vsVector3D v[4] =
			{
				scene->GetTopLeftCorner(),
				scene->GetTopRightCorner(),
				scene->GetBottomLeftCorner(),
				scene->GetBottomRightCorner()
			};

			vsDisplayList *list = queue->GetGenericList();

			int tf[4] = {0, 1, 3, 2};

			list->SetColor(vsColor::Red);
			list->VertexArray(v,4);
			list->TriangleFanArray(tf,4);
//			list->MoveTo(tl);
//			list->LineTo(br);
		}
	}
};

lagGame::lagGame()
{
}

lagGame::~lagGame()
{
}

void
lagGame::Init()
{
	Parent::Init();

	m_sprite = new lagSprite;
	m_sprite->RegisterOnScene(0);
}

void
lagGame::Deinit()
{
	vsDelete( m_sprite );

	Parent::Deinit();
}

