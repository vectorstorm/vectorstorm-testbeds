/*
 *  AST_Camera.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 25/03/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#ifndef AST_CAMERA_H
#define AST_CAMERA_H

class astCamera : public vsCamera2D
{
	vsSprite *		m_trackSprite;
	bool			m_trackPlayer;
	
public:
	astCamera();
	virtual ~astCamera();
	
	void			SetTrackSprite( vsSprite *track ) { m_trackSprite = track; }
	
	virtual void	Update( float timeStep );
};


#endif // AST_CAMERA_H
