/*
 *  BNG_Hud.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 28/04/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#ifndef BNG_HUD_H
#define BNG_HUD_H

class sstModeInGame;

class sstHud : public vsEntity
{
	sstModeInGame *m_parent;
	
	vsDisplayList *m_playing;
	vsDisplayList *m_recording;
	vsDisplayList *m_starting;
	vsDisplayList *m_ended;
	vsDisplayList *m_intermission;
	
	float			m_intermissionBrightness;
public:
	sstHud( sstModeInGame *game );
	~sstHud();
	
	void	Update(float timeStep);
	void	Draw(vsRenderQueue *queue);
};

#endif // BNG_HUD_H

