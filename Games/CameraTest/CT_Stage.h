/*
 *  CT_Stage.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 3/01/11.
 *  Copyright 2011 Trevor Powell. All rights reserved.
 *
 */

#ifndef CT_STAGE_H
#define CT_STAGE_H

class ctStage : public vsSprite, public vsSingleton<ctStage>
{
	vsBox2D						m_bounds;
	vsLinkedListStore<vsBox2D>	m_box;

	void		AddInvisibleBox( const vsBox2D &box );

	float		ResolveCollision( const vsVector2D &start, const vsVector2D &end, const vsBox2D &box );

public:

	ctStage( const vsBox2D &dimensions );
	virtual ~ctStage();

	void		AddBox( const vsBox2D &box );	// extra obstruction

	vsVector2D	ClipAgainstStage( const vsVector2D &start, const vsVector2D &end, const vsBox2D &box );	// move the box from 'start' to 'end', returning the final position.
	float		HeightAboveGround( const vsVector2D &pos, const vsBox2D &box );
};

class ctStage3D : public vsModel, public vsSingleton<ctStage3D>
{
	vsBox3D						m_bounds;
	vsLinkedListStore<vsBox3D>	m_box;

	void		AddInvisibleBox( const vsBox3D &box );

	float		ResolveCollision( const vsVector3D &start, const vsVector3D &end, const vsBox3D &box );

public:

	ctStage3D( const vsBox3D &dimensions );
	virtual ~ctStage3D();

	void		AddBox( const vsBox3D &box );	// extra obstruction

	vsVector3D	ClipAgainstStage( const vsVector3D &start, const vsVector3D &end, const vsBox3D &box );	// move the box from 'start' to 'end', returning the final position.
	float		HeightAboveGround( const vsVector3D &pos, const vsBox3D &box );
};

#endif // CT_STAGE_H
