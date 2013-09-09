/*
 *  AST_Star.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 10/04/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#include "AST_Star.h"

astStar::astStar( vsCamera2D *camera ):
vsSprite(vsDisplayList::Load("Star")),
m_camera(camera)
{
	SetColor( vsColor(1.0f,1.0f,1.0f,0.5f) );	// faint white
}

void
astStar::Update(float timeStep)
{
	UNUSED(timeStep);

	vsVector2D v = GetPosition();
	if ( m_camera->WrapAround( v, 0.f ) )
		SetPosition(v);
}
