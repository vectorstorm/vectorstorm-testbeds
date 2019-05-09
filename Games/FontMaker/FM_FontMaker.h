/*
 *  FM_FontMaker.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 28/12/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#ifndef FM_FONTMAKER_H
#define FM_FONTMAKER_H

#if 0
class fmFontMaker : public coreGame
{
	typedef coreGame Parent;

	vsFont *		m_font;
	vsFont *		m_fontB;
	vsFont *		m_fontC;
	vsSprite *		m_sprite;
	vsSprite *		m_guide;
	vsSprite *		m_other;
	vsSprite *		m_final;

	vsSprite *		m_muncher;

public:

					fmFontMaker();
	virtual			~fmFontMaker();

	virtual void	Init();
	virtual void	Deinit();

	virtual void	Update( float timeStep );
};

#endif

#endif // FM_FONTMAKER_H

