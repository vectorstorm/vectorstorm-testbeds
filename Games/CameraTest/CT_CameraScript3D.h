/*
 *  CT_CameraScript3D.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/01/11.
 *  Copyright 2011 Trevor Powell. All rights reserved.
 *
 */

#ifndef CT_CAMERA_SCRIPT_3D_H
#define CT_CAMERA_SCRIPT_3D_H

class ctCameraScript3D
{
protected:
	vsVector3D	m_targetPosition;
public:
	
	vsVector3D		m_position;
	vsQuaternion	m_orientation;
	float			m_fov;
	bool			m_overrideFOV;
	
	
	ctCameraScript3D():
		m_targetPosition( vsVector3D::Zero ),
		m_position( vsVector3D::Zero ),
		m_orientation( vsQuaternion::Identity ),
		m_fov( 60.f ),
		m_overrideFOV( false )	// usually we let the ctCamera set the FOV, but if we want to, we can take control.
								// in a real game, the camera script would always have control of FOV, but I want
								// to be able to demonstrate the effect of changing this for the video.
	{
	}
	virtual ~ctCameraScript3D() {}
	
	void	SetTargetPosition( const vsVector3D &targetPosition ) { m_targetPosition = targetPosition; }
	
	virtual void Reset() {}
	virtual void Update( float timeStep ) = 0;
	virtual vsString	GetName() = 0;
};

class ctCameraScript3D_BasicFollow : public ctCameraScript3D
{
public:
	
	ctCameraScript3D_BasicFollow() {}
	
	virtual void Update( float timeStep );
	virtual vsString	GetName() { return "Basic Follow 3D"; }
};


#endif // CT_CAMERA_SCRIPT_3D_H

