/*
 *  SMX_Maze.cpp
 *  Munchers
 *
 *  Created by Trevor Powell on 26/01/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#include "SMX_Maze.h"

static int directionOffsetX[MD_MAX] =
{
	0,
	0,
	-1,
	1
};

static int directionOffsetY[MD_MAX] =
{
	-1,
	1,
	0,
	0
};

static MazeDirection oppositeDirection[MD_MAX] =
{
	MD_South,
	MD_North,
	MD_East,
	MD_West
};

smxMaze::smxMaze(float width, float height, int columns, int rows, bool withCollision):
    m_collision(withCollision),
	m_fading(false),
	m_opacity(0.0f),
	m_width(width),
	m_height(height)
{
	m_transform = vsTransform2D::Zero;
	m_cell = NULL;
	m_wall = NULL;
	Generate(columns, rows);
}

smxMaze::~smxMaze()
{
	for ( int i = 0; i < m_wallCount; i++ )
		vsDelete(m_wall[i]);
	vsDeleteArray(m_wall);

	vsDeleteArray(m_cell);
}

void
smxMaze::Generate(int columns, int rows)
{
	vsDeleteArray(m_cell);

	m_columns = columns;
	m_rows = rows;
	m_cellCount = columns*rows;
	m_cell = new munCell[columns*rows];

	float cellWidth = m_width / m_columns;
	float cellHeight = m_height / m_rows;

	float halfCellWidth = cellWidth * 0.5f;
	float halfCellHeight = cellHeight * 0.5f;

	for ( int x = 0; x < m_columns; x++ )
		for ( int y = 0; y < m_rows; y++ )
		{
			munCell *c = GetCellId(x,y);

			for ( int i = 0; i < MD_MAX; i++ )
			{
				c->column = x;
				c->row = y;
				c->position.x = (-.5f * m_width) + (cellWidth * x) + halfCellWidth;
				c->position.y = (-.5f * m_height) + (cellHeight * y) + halfCellHeight;

				c->wall[i] = true;
				c->neighbor[i] = GetCellId( x + directionOffsetX[i],
										   y + directionOffsetY[i] );
				c->inMaze = false;
				c->hasDot = false;
			}
		}

	GenerateAldousBroder();

	if ( m_collision )
		GenerateCollision();
}

void
smxMaze::GenerateAldousBroder()
{
	// maze generation using Aldous-Broder algorithm
	// http://www.astrolog.org/labyrnth/algrithm.htm#perfect

	int carvedCells = 0;
	int totalCellsToCarve = m_columns * m_rows;

	int curX = vsRandom::GetInt(m_columns);
	int curY = vsRandom::GetInt(m_rows);

	munCell *curCell = GetCellId( curX, curY );
	curCell->inMaze = true;
	carvedCells++;

	while ( carvedCells < totalCellsToCarve )
	{
		int direction;
		int nextX, nextY;
		do
		{
			direction = vsRandom::GetInt(MD_MAX);
			nextX = curX + directionOffsetX[direction];
			nextY = curY + directionOffsetY[direction];

		}while( nextX < 0 || nextY < 0 || nextX >= m_columns || nextY >= m_rows );

		// okay.  We're going from 'cur' to 'next'.

		munCell *nextCell = GetCellId( nextX, nextY );

		if ( !nextCell->inMaze )	// next cell isn't in the maze!  Let's carve into it.
		{
			curCell->wall[direction] = false;
			nextCell->wall[ oppositeDirection[direction] ] = false;
			nextCell->inMaze = true;
			carvedCells++;
		}

		curCell = nextCell;
		curX = nextX;
		curY = nextY;
	}
}

void
smxMaze::GenerateCollision()
{
	vsDeleteArray(m_wall);
	m_wallCount = 4;

	for ( int x = 0; x < m_columns; x++ )
	{
		for ( int y = 0; y < m_rows; y++ )
		{
			munCell *cell = GetCellId(x,y);
			if ( x < m_columns-1 && cell->wall[MD_East] )
				m_wallCount++;
			if ( y < m_rows-1 && cell->wall[MD_South] )
				m_wallCount++;
		}
	}

	m_wall = new munWallCollisionObject *[m_wallCount];
	for ( int i = 0; i < m_wallCount; i++ )
		m_wall[i] = NULL;

	int nextWall = 0;

	float left = -m_width * .5f;
	float right = -left;
	float top = -m_height * .5f;
	float bottom = -top;

	vsVector2D topLeft(left,top);
	vsVector2D topRight(right,top);
	vsVector2D bottomLeft(left,bottom);
	vsVector2D bottomRight(right,bottom);

	//colLine line;
	AddWall( nextWall++, topLeft, topRight, NULL, NULL );
	AddWall( nextWall++, topRight, bottomRight, NULL, NULL );
	AddWall( nextWall++, bottomRight, bottomLeft, NULL, NULL );
	AddWall( nextWall++, bottomLeft, topLeft, NULL, NULL );

	for ( int x = 0; x < m_columns; x++ )
	{
		for ( int y = 0; y < m_rows; y++ )
		{
			munCell *cell = GetCellId(x,y);
			if ( x < m_columns-1 && cell->wall[MD_East] )
			{
				AddWall(nextWall++,x,y,MD_East);
			}
			if ( y < m_rows-1 && cell->wall[MD_South] )
			{
				AddWall(nextWall++,x,y,MD_South);
			}
		}
	}
}

munCell *
smxMaze::GetCellId( int x, int y )
{
	munCell *result = NULL;

	if ( x >= 0 && x < m_columns && y >= 0 && y < m_rows )
	{
		result = &m_cell[x + (y*m_columns)];
	}

	return result;
}

munCell *
smxMaze::GetCellForPosition( const vsVector2D &pos )
{
	vsVector2D start( m_width * -.5f, m_height * -.5f );
	vsVector2D p = pos;

	p -= start;	// now we're relative to the 'start' point.

	float cellWidth = m_width / m_columns;
	float cellHeight = m_height / m_rows;

	int column = (int)(p.x / cellWidth);
	int row = (int)(p.y / cellHeight);

	column = vsClamp( column, 0, m_columns );
	row = vsClamp( row, 0, m_rows );

	return GetCellId( column, row );
}

void
smxMaze::SetCenterForPathfinding( munCell *cell )
{
	for ( int i = 0; i < m_cellCount; i++ )
	{
		m_cell[i].distanceCalculated = false;
		m_cell[i].distance = 2000;		// arbitrary large number
	}

	cell->distance = 0;
	cell->distanceCalculated = true;

	for ( int distance = 0; distance < 80; distance++ )
	{
		for ( int i = 0; i < m_cellCount; i++ )
		{
			if ( !m_cell[i].distanceCalculated )
			{
				for ( int j = 0; j < MD_MAX; j++ )
				{
					munCell *n = m_cell[i].neighbor[j];
					if ( !m_cell[i].wall[j] && n && n->distanceCalculated && n->distance == distance)
					{
						m_cell[i].distance = distance+1;
						m_cell[i].distanceCalculated = true;
					}
				}
			}
		}
	}
}

void
smxMaze::GetWallCoords( int x, int y, MazeDirection dir, vsVector2D *a, vsVector2D *b )
{
	vsVector2D start( m_width * -.5f, m_height * -.5f );

	float cellWidth = m_width / m_columns;
	float cellHeight = m_height / m_rows;

	if ( dir == MD_East || dir == MD_West)
	{
		*a = start + vsVector2D( cellWidth * x, cellHeight * y );
		if ( dir == MD_East )
			a->x += cellWidth;

		*b = *a + vsVector2D( 0.f, cellHeight );
	}
	else if ( dir == MD_North || dir == MD_South)
	{
		*a = start + vsVector2D( cellWidth * x, cellHeight * y );
		if ( dir == MD_South )
			a->y += cellHeight;
		*b = *a + vsVector2D( cellWidth, 0.f );
	}
	else
		vsAssert(0, "Unknown wall direction requested!");
}

void
smxMaze::Update( float timeStep )
{
	if ( m_fading )
	{
		m_fadeTimer += timeStep;

		if ( m_fadeTimer > m_fadeTime )
		{
			m_fadeTimer = m_fadeTime;
			m_fading = false;
		}

		m_opacity = vsInterpolate( m_fadeTimer / m_fadeTime, m_fadeStart, m_fadeEnd );
	}
}

void
smxMaze::_Draw( vsDisplayList *list )
{
	bool pushedTransform = false;

	//if ( m_transform != vsTransform2D::Zero )
	{
		pushedTransform = true;
		list->PushTransform(m_transform);
	}

	vsVector2D start( m_width * -.5f, m_height * -.5f );

	//float cellWidth = m_width / m_columns;
	//float cellHeight = m_height / m_rows;

	//float halfCellWidth = cellWidth * 0.5f;
	//float halfCellHeight = cellHeight * 0.5f;

	vsColor c = vsInterpolate( m_opacity, vsColor::Black, vsColor::Blue );
	list->SetColor(c);

	list->MoveTo( start );
	list->LineTo( start + vsVector2D(m_width, 0.f) );
	list->LineTo( start + vsVector2D(m_width, m_height) );
	list->LineTo( start + vsVector2D(0.f, m_height) );
	list->LineTo( start );

	vsVector2D pointA, pointB;

	for ( int x = 0; x < m_columns; x++ )
	{
		for ( int y = 0; y < m_rows; y++ )
		{
			munCell *cell = GetCellId( x,y );

			// draw right and bottom walls of this cell.

			if ( x < m_columns-1 && cell->wall[MD_East] )
			{
				//vsVector2D pointA = start + vsVector2D( cellWidth * x + cellWidth, cellHeight * y /*- halfCellHeight*/ );
				//vsVector2D pointB = pointA + vsVector2D( 0.f, cellHeight );
				GetWallCoords( x, y, MD_East, &pointA, &pointB );

				list->MoveTo( pointA );
				list->LineTo( pointB );
			}
			if ( y < m_rows-1 && cell->wall[MD_South] )
			{
				//vsVector2D pointA = start + vsVector2D( cellWidth * x /*- halfCellWidth*/, cellHeight * y + cellHeight );
				//vsVector2D pointB = pointA + vsVector2D( cellWidth, 0.f );
				GetWallCoords( x, y, MD_South, &pointA, &pointB );

				list->MoveTo( pointA );
				list->LineTo( pointB );
			}
		}
	}

	/*c = vsInterpolate( m_opacity, vsColor::Black, vsColor::Red * 0.5f );
	list->SetColor(c);

	vsTransform2D t;

	for ( int x = 0; x < m_columns; x++ )
	{
		for ( int y = 0; y < m_rows; y++ )
		{
			munCell *cell = GetCellId( x,y );

			if ( cell->hasDot )
			{
				t.m_position.Set( cell->x, cell->y );
				list->PushTransform(t);
				list->Append(*m_gemList);
				list->PopTransform();
			}
		}
	}*/

	if ( pushedTransform )
		list->PopTransform();
}

void
smxMaze::AddWall( int wallId, const vsVector2D &start, const vsVector2D &end, munCell *a, munCell *b )
{
	vsAssert( m_wall[wallId] == NULL, "Error:  Tried to overwrite an existing collision object!" );

	m_wall[wallId] = new munWallCollisionObject(a,b,start,end);

	m_wall[wallId]->SetCollisionsActive(true);
}


void
smxMaze::AddWall( int wallId, int x, int y, MazeDirection dir )
{
	vsVector2D a, b;

	GetWallCoords(x, y, dir, &a, &b);

	munCell *aCell = GetCellId( x, y );
	munCell *bCell = aCell->neighbor[dir];

	AddWall( wallId, a, b, aCell, bCell );
}

int
smxMaze::GetWallCount()
{
	int count = 0;

	for ( int x = 0; x < m_columns; x++ )
	{
		for ( int y = 0; y < m_rows; y++ )
		{
			munCell *c= GetCellId(x,y);

			if ( c->wall[MD_East] )
				count++;
			if ( c->wall[MD_South] )
				count++;
		}
	}

	return count;
}



munWallCollisionObject::munWallCollisionObject( munCell *a, munCell *b, const vsVector2D &start, const vsVector2D &end ):
	vsCollisionObject( ColFlag_World, ColFlag_All ),
	m_a(a),
	m_b(b)
{
	vsVector2D topLeft(	vsMin(start.x, end.x), vsMin(start.y, end.y) );
	vsVector2D bottomRight(	vsMax(start.x, end.x), vsMax(start.y, end.y) );

	vsVector2D mid = 0.5f * (topLeft + bottomRight);
	vsVector2D extents = bottomRight - topLeft;

	const float min_width = 15.0f;
	const float min_height = 15.0f;

	if ( extents.x < min_width )
		extents.x = min_width;
	if ( extents.y < min_height )
		extents.y = min_height;

	vsVector2D halfExtents = extents * 0.5f;

	topLeft = mid - halfExtents;
	bottomRight = mid + halfExtents;

	SetBox(topLeft, bottomRight, 0.f);	// we're static!
}

void
munWallCollisionObject::DestroyCallback()
{
	if ( m_a && m_b )
	{
		for ( int i = 0; i < MD_MAX; i++ )
		{
			if ( m_a->neighbor[i] == m_b )
				m_a->wall[i] = false;	// we've broken this wall!
			if ( m_b->neighbor[i] == m_a )
				m_b->wall[i] = false;
		}
	}

	Parent::DestroyCallback();
}

