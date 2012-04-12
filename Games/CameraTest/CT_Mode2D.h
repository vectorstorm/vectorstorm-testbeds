/*
 *  CT_Mode2D.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/01/11.
 *  Copyright 2011 Trevor Powell. All rights reserved.
 *
 */

#ifndef CT_MODE_2D_H
#define CT_MODE_2D_H

class ctCamera;
class ctPlayer;
class ctStage;

class ctMode2D : public coreGameMode
{
	typedef coreGameMode Parent;
	
	ctCamera *		m_camera;
	ctStage *		m_stage;
	ctPlayer *		m_player;
	
public:
	
	ctMode2D();
	
	virtual void Init();
	virtual void Deinit();
};

#endif // CT_MODE_2D_H

