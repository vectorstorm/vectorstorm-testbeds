/*
 *  CT_Stage.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 3/01/11.
 *  Copyright 2011 Trevor Powell. All rights reserved.
 *
 */

#include "CT_Stage.h"

ctStage::ctStage( const vsBox2D &dims )
{
	vsBox2D wallDims = dims;
	wallDims.Expand(2.f);
	
	AddFragment( vsMakeTiledTexturedBox2D(wallDims, "Wall", 5.f, vsAngle(DEGREES(7.f))) );
	AddFragment( vsMakeTiledTexturedBox2D(dims, "Empty", 8.f, vsAngle(DEGREES(-27.f))) );
//	AddFragment( vsMakeSolidBox2D(dims, "Empty") );
	
	m_boundingBox = wallDims;
	m_boundingRadius = wallDims.Width() + wallDims.Height();	// TODO:  make vsSprite able to calculate its dimensions based upon vsFragments.

	vsBox2D left( wallDims.TopLeft(), wallDims.BottomLeft() );
	left.Expand(2.f);
	AddInvisibleBox(left);

	vsBox2D right( wallDims.TopRight(), wallDims.BottomRight() );
	right.Expand(2.f);
	AddInvisibleBox(right);

	vsBox2D bottom( wallDims.BottomLeft(), wallDims.BottomRight() );
	bottom.Expand(2.f);
	AddInvisibleBox(bottom);
}

ctStage::~ctStage()
{
}

void
ctStage::AddInvisibleBox( const vsBox2D &box )
{
	m_box.AddItem( new vsBox2D(box) );
}

void
ctStage::AddBox( const vsBox2D &box )
{
	AddInvisibleBox( box );
	
	AddFragment( vsMakeTiledTexturedBox2D(box, "Wall", 5.f, vsAngle(DEGREES(-7.f))) );
}

vsVector2D	
ctStage::ClipAgainstStage( const vsVector2D &start, const vsVector2D &end, const vsBox2D &box_in )
{
	// Okay.  Let's just check our ending position against all our walls.
	
	vsBox2D finalBox = box_in + end;
	
	for ( vsLinkedListStore<vsBox2D>::Iterator iter = m_box.Begin(); iter != m_box.End(); iter++ )
	{
		vsBox2D *box = *iter;
		
		// okay.  Let's check whether we're intersecting at all.
		
		if ( box->Intersects( finalBox ) )
		{
			// I've intersected with this box!  Let's find the fastest way to get out of it.
			
			vsVector2D	offset;
			float offsetDistance = 1000000.f;
			
			float		distance[4];
			vsVector2D	direction[4] = { vsVector2D(-1.f,0.f), vsVector2D(1.f,0.f), vsVector2D(0.f,1.f), vsVector2D(0.f,-1.f) };
			
			distance[0] = finalBox.GetMax().x - box->GetMin().x;	// how far in order to move us left out of this box?
			distance[1] = box->GetMax().x - finalBox.GetMin().x;	// how far in order to move us right out of this box?
			distance[2] = box->GetMax().y - finalBox.GetMin().y;	// down
			distance[3] = finalBox.GetMax().y - box->GetMin().y;		// up
			
			for ( int i = 0; i < 4; i++ )
			{
				if ( distance[i] < offsetDistance )
				{
					offsetDistance = distance[i];
					offset = direction[i] * offsetDistance;
				}
			}
			
			
			return ClipAgainstStage( start, end+offset, box_in );
		}
	}
	
	
	return end;
}

float
ctStage::HeightAboveGround( const vsVector2D &pos, const vsBox2D &box )
{
	vsBox2D finalBox = box + pos;
	
	float minDistance = 100000.f;
	
	for ( vsLinkedListStore<vsBox2D>::Iterator iter = m_box.Begin(); iter != m_box.End(); iter++ )
	{
		vsBox2D *box = *iter;
		
		// okay.  Let's check whether we're intersecting at all.
		
		if ( box->ContainsPoint( pos ) )
		{
			return 0.f;
		}
		if ( finalBox.Intersects( *box ) && finalBox.GetMax().y <= box->GetMin().y )
		{
			minDistance = vsMin( minDistance, box->GetMin().y - pos.y );
		}
	}
	
	return minDistance;
}










ctStage3D::ctStage3D( const vsBox3D &dims )
{
	vsBox3D wallDims = dims;
	wallDims.Expand( 2.f );
	
	/*AddFragment( vsMakeTiledTexturedBox2D(wallDims, "Wall", 5.f, vsAngle(DEGREES(7.f))) );
	AddFragment( vsMakeTiledTexturedBox2D(dims, "Empty", 8.f, vsAngle(DEGREES(-27.f))) );
	//	AddFragment( vsMakeSolidBox2D(dims, "Empty") );
	
	m_boundingBox = wallDims;
	m_boundingRadius = wallDims.Width() + wallDims.Height();	// TODO:  make vsSprite able to calculate its dimensions based upon vsFragments.
	
	vsBox3D left( wallDims.TopLeft(), wallDims.BottomLeft() );
	left.Expand(2.f);
	AddInvisibleBox(left);
	
	vsBox3D right( wallDims.TopRight(), wallDims.BottomRight() );
	right.Expand(2.f);
	AddInvisibleBox(right);
	
	vsBox3D bottom( wallDims.BottomLeft(), wallDims.BottomRight() );
	bottom.Expand(2.f);
	AddInvisibleBox(bottom);*/
}

ctStage3D::~ctStage3D()
{
}

void
ctStage3D::AddInvisibleBox( const vsBox3D &box )
{
	m_box.AddItem( new vsBox3D(box) );
}

void
ctStage3D::AddBox( const vsBox3D &box )
{
	AddInvisibleBox( box );
	
	//AddFragment( vsMakeTiledTexturedBox2D(box, "Wall", 5.f, vsAngle(DEGREES(-7.f))) );
}

vsVector3D	
ctStage3D::ClipAgainstStage( const vsVector3D &start, const vsVector3D &end, const vsBox3D &box_in )
{
	// Okay.  Let's just check our ending position against all our walls.
	
	vsBox3D finalBox = box_in + end;
	
	for ( vsLinkedListStore<vsBox3D>::Iterator iter = m_box.Begin(); iter != m_box.End(); iter++ )
	{
		vsBox3D *box = *iter;
		
		// okay.  Let's check whether we're intersecting at all.
		
		if ( box->Intersects( finalBox ) )
		{
			// I've intersected with this box!  Let's find the fastest way to get out of it.
			
			vsVector3D	offset;
			float offsetDistance = 1000000.f;
			
			float		distance[6];
			vsVector3D	direction[6] = 
            { 
                vsVector3D(-1.f, 0.f, 0.f), 
                vsVector3D( 1.f, 0.f, 0.f), 
                vsVector3D( 0.f, 1.f, 0.f), 
                vsVector3D( 0.f,-1.f, 0.f),
                vsVector3D( 0.f, 0.f, 1.f), 
                vsVector3D( 0.f, 0.f,-1.f)
            };
			
			distance[0] = finalBox.GetMax().x - box->GetMin().x;	// how far in order to move us left out of this box?
			distance[1] = box->GetMax().x - finalBox.GetMin().x;	// how far in order to move us right out of this box?
			distance[2] = box->GetMax().y - finalBox.GetMin().y;	// down
			distance[3] = finalBox.GetMax().y - box->GetMin().y;	// up
			distance[4] = box->GetMax().z - finalBox.GetMin().z;	// in
			distance[5] = finalBox.GetMax().z - box->GetMin().z;	// out
			
			for ( int i = 0; i < 6; i++ )
			{
				if ( distance[i] < offsetDistance )
				{
					offsetDistance = distance[i];
					offset = direction[i] * offsetDistance;
				}
			}
			
			
			return ClipAgainstStage( start, end+offset, box_in );
		}
	}
	
	
	return end;
}

float
ctStage3D::HeightAboveGround( const vsVector3D &pos, const vsBox3D &box )
{
	vsBox3D finalBox = box + pos;
	
	float minDistance = 100000.f;
	
	for ( vsLinkedListStore<vsBox3D>::Iterator iter = m_box.Begin(); iter != m_box.End(); iter++ )
	{
		vsBox3D *box = *iter;
		
		// okay.  Let's check whether we're intersecting at all.
		
		if ( box->ContainsPoint( pos ) )
		{
			return 0.f;
		}
		if ( finalBox.Intersects( *box ) && 
            finalBox.GetMin().y > box->GetMax().y )
		{
			minDistance = vsMin( minDistance, box->GetMin().y - pos.y );
		}
	}
	
	return minDistance;
}

