/*
 *  CT_SafeZone.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 3/01/11.
 *  Copyright 2011 Trevor Powell. All rights reserved.
 *
 */

#include "CT_SafeZone.h"

void
ctSafeZone::Draw( vsRenderQueue *queue )
{
	vsScene *scene = queue->GetScene();

	vsBox2D box( scene->GetTopLeftCorner(), scene->GetBottomRightCorner() );

	float marginSize = box.Height() * 0.1f;
	box.Expand( marginSize );

	vsVector3D v[4] =
	{
		vsVector2D(box.GetMin().x, box.GetMin().y),
		vsVector2D(box.GetMax().x, box.GetMin().y),
		vsVector2D(box.GetMin().x, box.GetMax().y),
		vsVector2D(box.GetMax().x, box.GetMax().y)
	};
	int i[5] = {0, 1, 3, 2, 0};

	vsDisplayList *list = queue->GetGenericList();

	list->VertexArray(v,4);
	list->LineStripArray(i,5);
	list->ClearVertexArray();
}
