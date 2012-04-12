/*
 *  TB_Game.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/09/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

// The TestBed game is just a simple place for quick testing.  I've been using it as a
// place to do testing of the collision system, so I can quickly set up collision test
// cases and run them over and over again as I fix the collision and physics systems.
//
// The TestBed game only exists in Debug builds of VectorStorm, as it's not a real game.

#ifndef TB_GAME_H
#define TB_GAME_H

class tbMap;

class tbGame : public coreGame
{
	typedef coreGame Parent;

	vsModel *	m_sphere;
	vsModel *	m_cube;

	vsModel *	m_lightSphere;
	vsModel *	m_lightSphereB;

	tbMap *		m_map;
	vsSprite *  m_background;
	vsSprite *	m_title;
	vsSprite *	m_texture;
	vsSprite *	m_texture2;
	vsEntity *	m_networkDisplay;
	vsCamera3D *	m_camera;

	vsLight *	m_light;
	vsLight *	m_lightB;
	vsLight *	m_lightC;
	vsAngle		m_lightAngle;
	vsAngle		m_lightAngleB;

	vsPhysicsEmitter *m_e;

public:

		tbGame();

	virtual void	Init();
	virtual void	Deinit();

	virtual void	Update( float timeStep );
};

//#endif // _DEBUG

#endif // TB_GAME_H

