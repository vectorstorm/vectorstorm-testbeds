/*
 *  SMX_Maze.h
 *  Munchers
 *
 *  Created by Trevor Powell on 26/01/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#ifndef SMX_MAZE_H
#define SMX_MAZE_H

class vsDisplayList;

enum MazeDirection
{
	MD_North,
	MD_South,
	MD_West,
	MD_East,
	MD_MAX
};

struct munCell
{
	int			column;
	int			row;
	vsVector2D	position;

	int			distance;		// used for pathfinding;  distance from wherever was set as the center
	bool		distanceCalculated;

	bool	wall[MD_MAX];
	munCell *neighbor[MD_MAX];
	bool	inMaze;	// used during construction;

	bool	hasDot;
};

class munWallCollisionObject : public vsCollisionObject
{
	typedef vsCollisionObject Parent;

	munCell *		m_a;
	munCell *		m_b;
public:

	munWallCollisionObject( munCell *a, munCell *b, const vsVector2D &start, const vsVector2D &end );

	virtual void	DestroyCallback();

	munCell *		GetCellA() { return m_a; }
	munCell *		GetCellB() { return m_b; }
};

class smxMaze : public vsEntity
{
	vsTransform2D m_transform;

	bool	m_collision;

	bool	m_fading;
	float	m_fadeStart;
	float	m_fadeEnd;
	float	m_fadeTime;
	float	m_fadeTimer;
	float	m_opacity;

	int		m_columns;
	int		m_rows;

	float	m_width;
	float	m_height;

	munCell m_outside;
	munCell	*m_cell;


	munWallCollisionObject **m_wall;
	int			m_wallCount;

	int		m_cellCount;

	void	GenerateAldousBroder();	// generate the maze using Aldous-Broder

	void	GenerateCollision();	// generate the maze's collision data

	void	AddWall( int wallId, const vsVector2D &start, const vsVector2D &end, munCell *a, munCell *b );
	void	AddWall( int wallId, int x, int y, MazeDirection dir );

	void	GetWallCoords( int x, int y, MazeDirection dir, vsVector2D *wallPosA, vsVector2D *wallPosB );

	void	Fade(float to, float time) { m_fading = true; m_fadeStart = m_opacity; m_fadeEnd = to; m_fadeTime = time; m_fadeTimer = 0.f; }
public:

					smxMaze(float width, float height, int columns, int rows, bool withCollision = true);
	virtual			~smxMaze();

	virtual void	Generate(int columns, int rows);

	void			SetTransform( const vsTransform2D &t ) { m_transform = t; }

	munCell *		GetCellId(int x, int y);
	munCell *		GetCellForPosition( const vsVector2D &pos );

	int				GetWallCount();

	void			SetCenterForPathfinding( munCell *cell );

	void			FadeIn(float time) { Fade(1.0f,time); }
	void			FadeOut(float time) { Fade(0.0f,time); }

	virtual void	Update( float timeStep );
	virtual void	_Draw( vsDisplayList *list );
};

#endif // SMX_MAZE_H
