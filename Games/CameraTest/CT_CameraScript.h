/*
 *  CT_CameraScript.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 3/01/11.
 *  Copyright 2011 Trevor Powell. All rights reserved.
 *
 */

#ifndef CT_CAMERASCRIPT_H
#define CT_CAMERASCRIPT_H

class ctCameraScript
{
protected:
	vsVector2D	m_targetPosition;
public:

	vsVector2D	m_position;
	vsAngle		m_angle;
	float		m_fov;
	bool		m_overrideFOV;


	ctCameraScript():
		m_targetPosition( vsVector2D::Zero ),
		m_position( vsVector2D::Zero ),
		m_angle( vsAngle::Zero ),
		m_fov( 30.f ),
		m_overrideFOV( false )	// usually we let the ctCamera set the FOV, but if we want to, we can take control.
								// in a real game, the camera script would always have control of FOV, but I want
								// to be able to demonstrate the effect of changing this for the video.
	{
	}
	virtual ~ctCameraScript() {}

	void	SetTargetPosition( const vsVector2D &targetPosition ) { m_targetPosition = targetPosition; }

	virtual void Reset() {}
	virtual void Update( float timeStep ) = 0;
	virtual vsString	GetName() = 0;
};

class ctCameraScript_Overview : public ctCameraScript
{
public:

	ctCameraScript_Overview() {}

	virtual void Update( float timeStep );
	virtual vsString	GetName() { return "Overview"; }
};

class ctCameraScript_BasicFollow : public ctCameraScript
{
public:

	ctCameraScript_BasicFollow() {}

	virtual void Update( float timeStep );
	virtual vsString	GetName() { return "Basic Follow"; }
};

class ctCameraScript_LerpFollow : public ctCameraScript
{
public:

	ctCameraScript_LerpFollow() {}

	virtual void Reset();
	virtual void Update( float timeStep );
	virtual vsString	GetName() { return "Lerp Follow"; }
};

class ctCameraScript_SpringFollow : public ctCameraScript
{
	vsSpring2D	m_spring;
public:

	ctCameraScript_SpringFollow();

	virtual void Reset();
	virtual void Update( float timeStep );
	virtual vsString	GetName() { return "Spring Follow"; }
};

class ctCameraScript_LooseFollow : public ctCameraScript
{
	vsSpring2D	m_spring;
	float		m_unfollowTimer;
	bool		m_following;
public:

	ctCameraScript_LooseFollow();

	virtual void Reset();
	virtual void Update( float timeStep );
	virtual vsString	GetName() { return "Loose Follow"; }
};

class ctCameraScript_LooseLead : public ctCameraScript
{
	vsSpring2D	m_spring;
	bool		m_following;
public:

	ctCameraScript_LooseLead();

	virtual void Reset();
	virtual void Update( float timeStep );
	virtual vsString	GetName() { return "Loose Lead"; }
};

class ctCameraScript_LooseLeadGround : public ctCameraScript
{
	vsSpring2D	m_spring;
	float		m_lastGroundHeight;
	bool		m_following;
public:

	ctCameraScript_LooseLeadGround();

	virtual void Reset();
	virtual void Update( float timeStep );
	virtual vsString	GetName() { return "Loose Lead Ground"; }
};

#endif // CT_CAMERASCRIPT_H

