/*
 *  LT_PlayerControl
 *  MMORPG2
 *
 *  Created by Trevor Powell on 19-08-2011.
 *  Copyright 2011 Trevor Powell.  All rights reserved.
 *
 */
#ifndef LT_PLAYERCONTROL_H
#define LT_PLAYERCONTROL_H

#include "LT_Lerpable.h"

class ltPlayerControl : public ltLerpable
{
	typedef ltLerpable Parent;

	static const int ENEMY_COUNT = 10;
	vsSprite *	m_player;
	vsSprite *	m_target;
	vsSprite *	m_enemy[ENEMY_COUNT];

	vsSprite *  m_text;
	bool		m_ok;

	void SetText( float value );
	void PlaceGamePieces();
	bool Intersects( vsSprite *a, vsSprite *b );

public:
	ltPlayerControl();
	virtual ~ltPlayerControl();

	virtual void Init();
	virtual void Deinit();
	virtual void Update( float timestep );

};

#endif /* LT_PLAYERCONTROL_H */


