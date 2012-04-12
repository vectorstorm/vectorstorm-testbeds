/*
 *  LT_Game
 *  VectorStorm
 *
 *  Created by Trevor Powell on 16-08-2011.
 *  Copyright 2011 Trevor Powell.  All rights reserved.
 *
 */

#ifndef LT_GAME_H
#define LT_GAME_H

class ltCamera;
class ltGraph;
class ltLerpable;
class ltResponseCurve;

class ltGame : public coreGame, public vsSingleton<ltGame>
{
	typedef coreGame Parent;

	ltCamera *			m_camera;
	ltLerpable **		m_lerpable;
	int					m_lerpableCount;
	int					m_currentLerpable;

	ltResponseCurve **	m_curve;
	int					m_curveCount;
	int					m_currentCurve;

	ltGraph *			m_graph;

	vsLight *			m_light;

public:

						ltGame();
	virtual				~ltGame();

	virtual void		Init();
	virtual void		Deinit();

	virtual void		Update( float timeStep );

	ltCamera *			GetCamera() { return m_camera; }
};

#endif // LT_GAME_H


