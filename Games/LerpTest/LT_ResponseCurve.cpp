/*
 *  LT_ResponseCurve
 *  VectorStorm
 *
 *  Created by Trevor Powell on 15-08-2011.
 *  Copyright 2011 Trevor Powell.  All rights reserved.
 *
 */

#include "LT_ResponseCurve.h"

ltResponseCurve::ltResponseCurve( const vsString &name, const vsString &equation ):
	m_name(name),
	m_equation(equation)
{
}

ltResponseCurve_Linear::ltResponseCurve_Linear():
	ltResponseCurve("Linear", "t = t")
{
}

float
ltResponseCurve_Linear::GetCurveValue( float t )
{
	return t;
}

ltResponseCurve_Quadratic::ltResponseCurve_Quadratic():
	ltResponseCurve("Quadratic", "t = t^2")
{
}

float
ltResponseCurve_Quadratic::GetCurveValue( float t )
{
	return t*t;
}

ltResponseCurve_Cubic::ltResponseCurve_Cubic():
	ltResponseCurve("Cubic", "t = t^3")
{
}

float
ltResponseCurve_Cubic::GetCurveValue( float t )
{
	return t*t*t;
}

ltResponseCurve_Hermite::ltResponseCurve_Hermite():
	ltResponseCurve("Hermite", "t = 3t^2 - 2t^3")
{
}

float
ltResponseCurve_Hermite::GetCurveValue( float t )
{
	return (3.f*t*t) - (2.f*t*t*t);
}

