/*
 *  LT_ResponseCurve
 *  VectorStorm
 *
 *  Created by Trevor Powell on 15-08-2011.
 *  Copyright 2011 Trevor Powell.  All rights reserved.
 *
 */

#ifndef LT_RESPONSECURVE_H
#define LT_RESPONSECURVE_H

class ltResponseCurve
{
	vsString m_name;
	vsString m_equation;
public:
	ltResponseCurve( const vsString &name, const vsString &equation );
	virtual ~ltResponseCurve() {}

	const vsString & GetName() { return m_name; }
	const vsString & GetEquationText() { return m_equation; }

	virtual float GetCurveValue( float t ) = 0;
};

class ltResponseCurve_Linear: public ltResponseCurve
{
public:
	ltResponseCurve_Linear();
	virtual float GetCurveValue( float t );
};

class ltResponseCurve_Quadratic: public ltResponseCurve
{
public:
	ltResponseCurve_Quadratic();
	virtual float GetCurveValue( float t );
};

class ltResponseCurve_Cubic: public ltResponseCurve
{
public:
	ltResponseCurve_Cubic();
	virtual float GetCurveValue( float t );
};

class ltResponseCurve_Hermite: public ltResponseCurve
{
public:
	ltResponseCurve_Hermite();
	virtual float GetCurveValue( float t );
};


#endif //LT_RESPONSECURVE_H

