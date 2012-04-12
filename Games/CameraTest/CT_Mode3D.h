/*
 *  CT_Mode3D.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/01/11.
 *  Copyright 2011 Trevor Powell. All rights reserved.
 *
 */

#ifndef CT_MODE_3D_H
#define CT_MODE_3D_H

class ctCamera3D;
class ctPlayer3D;
class ctStage3D;


class ctMode3D : public coreGameMode
{
	typedef coreGameMode Parent;
	
	ctCamera3D *		m_camera;
	ctStage3D *			m_stage;
	ctPlayer3D *		m_player;
	
public:
	ctMode3D();
	
	virtual void Init();
	virtual void Deinit();
};

#endif // CT_MODE_3D_H
