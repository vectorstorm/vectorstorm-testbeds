/*
 *  PREF_Game.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 28/12/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#ifndef PREF_GAME_H
#define PREF_GAME_H


class prefGame: public coreGame
{
	typedef coreGame Parent;

	enum State
	{
		Top,
		Screen,
		Sound,
		Control
	};

	int					m_state;

	vsSimpleMenu *			m_topMenu;
	vsSimpleMenu *			m_screenMenu;
	vsSimpleMenu *			m_soundMenu;
	vsSimpleMenu *			m_controlMenu;

	vsSprite *			m_controlHelpText;

	DeviceControl		*m_deviceControl;
	int					m_controlCount;

	int					m_resolution;
	int					m_resolutionCount;
	bool				m_fullscreen;
	bool				m_bloom;

	int					m_controlBeingPolled;
	bool				m_polling;

	int					m_effectVolume;
	int					m_musicVolume;

	void				ResetValues();		// set local values from the system preferences
	void				SetValues();		// set menu values to match our local values.
	void				UpdateValues();		// save our local values back into the system preferences

	vsString			MakeVolumeString( int value );

	vsString			MakeControlString( int control );

	void				UpdateControlScreen();

public:

	prefGame();

	virtual void		Update(float timeStep);

	virtual void		Init();
	virtual void		Deinit();
};

#endif // PREF_GAME_H

