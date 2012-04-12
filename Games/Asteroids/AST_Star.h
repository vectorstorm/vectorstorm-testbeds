/*
 *  AST_Star.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 10/04/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#ifndef AST_STAR_H
#define AST_STAR_H

class astStar : public vsSprite
{
	vsCamera2D *m_camera;
	
public:
	
	astStar( vsCamera2D *camera );
	
	virtual void	Update(float timeStep);
};

#endif // AST_STAR_H
