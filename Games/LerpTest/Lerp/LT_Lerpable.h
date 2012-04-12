/*
 *  LT_Lerpable
 *  MMORPG2
 *
 *  Created by Trevor Powell on 17-08-2011.
 *  Copyright 2011 Trevor Powell.  All rights reserved.
 *
 */

#ifndef LT_LERPABLE_H
#define LT_LERPABLE_H

#include "LT_ResponseCurve.h"

class ltLerpable: public vsSprite
{
	typedef vsSprite Parent;

	vsString		m_name;
	bool			m_selected;

	void RebuildGraphics();
protected:
	vsFont *		m_font;
	ltResponseCurve * m_curve;
public:
	ltLerpable( const vsString &name );
	virtual ~ltLerpable();

	virtual void Init();
	virtual void Deinit();
	virtual void SetCurve( ltResponseCurve *curve ) { m_curve = curve; }
};


class ltLerpableNone: public ltLerpable
{
public:
	ltLerpableNone(): ltLerpable("None") {}

	virtual void Update( float timeStep ) {}
};


#endif /* end of include guard: LT_LERPABLE_H */

