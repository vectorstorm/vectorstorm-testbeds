/*
 *  LAG_Game.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 10/02/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#ifndef CT_GAME_H
#define CT_GAME_H

class ctCamera;
class ctSafeZone;
class ctStage;
class ctPlayer;

class ctGame : public coreGame, public vsSingleton<ctGame>
{
	enum
	{
		Mode_2D,
		Mode_3D,
		MODE_MAX
	};

	typedef coreGame Parent;

	coreGameMode *			m_mode[MODE_MAX];

	vsFont *		m_font;
	vsSprite *		m_vgknowhow;
	vsSprite *		m_cameraName;
	ctSafeZone *	m_safeZone;

	int				m_currentMode;

	void			SetMode( int mode );

public:

					ctGame();
	virtual			~ctGame();

	virtual void	Init();
	virtual void	Deinit();

	void			SetCameraName( const vsString &name );

	virtual void	Update( float timeStep );
};

#endif // CT_GAME_H

