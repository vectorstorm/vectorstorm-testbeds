/*
 *  CT_Camera.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 3/01/11.
 *  Copyright 2011 Trevor Powell. All rights reserved.
 *
 */

#ifndef CT_CAMERA_H
#define CT_CAMERA_H

class ctCameraScript;
class ctCameraScript3D;

class ctCamera: public vsCamera2D
{
	vsSprite *			m_sprite;			// the sprite we're following
	vsVector2D			m_spriteOffset;		// offset we're applying to the sprite, to determine our tracking position
	
	float				m_defaultFOV;		// when a script doesn't specify a FOV, use this one.  (This is just for the video;  real camera systems wouldn't do this)
	
	vsArrayStore<ctCameraScript>	m_cameraScript;		// array of all scripts we can use
	int								m_currentScript;	// which script ID are we currently using?
	
	void	SetTargetPosition();
	
public:
			ctCamera();
	virtual ~ctCamera();
	
	virtual void Update( float timeStep );
	
	void	Follow( vsSprite *sprite ) { m_sprite = sprite; }
};

class ctCamera3D: public vsCamera3D
{
	vsModel *			m_model;			// the model we're following
	vsVector3D			m_modelOffset;		// offset we're applying to the sprite, to determine our tracking position
	
	vsFont *			m_font;				// font to be used for the text sprite.
	vsSprite *			m_textSprite;		// this sprite contains renderable text, telling us which script is currently active
	
	float				m_defaultFOV;		// when a script doesn't specify a FOV, use this one.  (This is just for the video;  real camera systems wouldn't do this)
	
	vsArrayStore<ctCameraScript3D>	m_cameraScript;		// array of all scripts we can use
	int								m_currentScript;	// which script ID are we currently using?
	
	void	SetTargetPosition();
	void	DisplayCameraScriptName();		// shows the name of the currently selected script.
	
public:
	ctCamera3D();
	virtual ~ctCamera3D();
	
	virtual void Update( float timeStep );
	
	void	Follow( vsModel *model ) { m_model = model; }
};

#endif // CT_CAMERA_H