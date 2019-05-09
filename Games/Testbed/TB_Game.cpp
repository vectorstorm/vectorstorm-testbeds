/*
 *  TB_Game.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/09/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#include "TB_Game.h"


#define MAP_WIDTH (20)
#define MAP_HEIGHT (20)
#define MAP_CELLS (MAP_WIDTH * MAP_HEIGHT)
#define MAP_LINES (MAP_CELLS * 2)
#define MAP_INDEX(x,y) ((x)+((y)*MAP_WIDTH))

vsMaterial *	gNormalMaterial;
vsMaterial *	gAddMaterial;
vsMaterial *	gLitMaterial;
vsMaterial *	gGlobeMaterial;

class tbMap : public vsModel
{
	typedef vsModel Parent;

	vsVector3D	m_gridPos[MAP_CELLS];
	float		m_height[MAP_CELLS];
	float		m_cellSize;

	void		BuildHeightField();
	void		BuildHeightColumns();

public:
	tbMap();

	vsVector3D		GetCenterGroundPosition();
};

tbMap::tbMap()
{
	vsPerlin heightField(4,0.4f);
	vsVector2D basePos;

	int ix = 0.5f * MAP_WIDTH;
	int iy = 0.5f * MAP_HEIGHT;

	m_cellSize = 10.f;

	for ( int x = 0; x < MAP_WIDTH; x++ )
	{
		for ( int y = 0; y < MAP_HEIGHT; y++ )
		{
			int i = MAP_INDEX(x,y);

			basePos.Set(x-ix,y-iy);

			m_height[i]	= heightField.Noise( basePos * 0.3f ) + 1.f;
			m_height[i] *= m_height[i];
			m_height[i] *= 20.f;
			m_gridPos[i].Set( basePos.x, 0.f, basePos.y );
			m_gridPos[i] *= m_cellSize;
		}
	}

	BuildHeightField();
	//BuildHeightColumns();


}

void
tbMap::BuildHeightField()
{
	int indices[MAP_CELLS];
	vsVector3D pos[MAP_CELLS];

	vsTransform3D offset = vsTransform3D::Identity;
	offset.SetTranslation( vsVector3D(0.f,0.2f,0.f) );

	for ( int i = 0; i < MAP_CELLS; i++ )
	{
		pos[i] = m_gridPos[i];
		pos[i].y = m_height[i];
	}

	vsDisplayList *list = new vsDisplayList(1024 * 500);

	list->PushTranslation( vsVector3D::Zero );

	list->VertexArray( pos, MAP_CELLS );

	list->SetMaterial( gNormalMaterial );
	list->SetColor(vsColor(0.f,0.0f,0.0f,1.0f));

	for ( int x = 0; x < MAP_WIDTH-1; x++ )
	{
		int id = 0;
		for ( int y = 0; y < MAP_HEIGHT; y++ )
		{
			indices[id++] = MAP_INDEX(x,y);
			indices[id++] = MAP_INDEX(x+1,y);
		}
		list->TriangleStripArray( indices, id );
	}
	list->SetMaterial( gAddMaterial );

	list->PushTransform(offset);

	list->SetColor(vsColor::LightGreen);
	for ( int x = 0; x < MAP_WIDTH; x++ )
	{
		for ( int y = 0; y < MAP_HEIGHT; y++ )
		{
			indices[y] = MAP_INDEX(x,y);
		}
		list->LineStripArray( indices, MAP_HEIGHT );
	}

	for ( int y = 0; y < MAP_HEIGHT; y++ )
	{
		for ( int x = 0; x < MAP_WIDTH; x++ )
		{
			indices[x] = MAP_INDEX(x,y);
		}
		list->LineStripArray( indices, MAP_HEIGHT );
	}

	list->PopTransform();
	list->PopTransform();
	SetDisplayList(list);
}

void
tbMap::BuildHeightColumns()
{
	vsDisplayList *list = new vsDisplayList(1024 * 80);

	vsVector3D vert[MAP_CELLS * 8];	// every point has a cube around it

	float hw = 0.5f * m_cellSize;

	vsVector3D offset[4] =
	{
		vsVector3D( -hw, 0.f, -hw ),
		vsVector3D(  hw, 0.f, -hw ),
		vsVector3D( -hw, 0.f,  hw ),
		vsVector3D(  hw, 0.f,  hw )
	};

	for ( int x = 0; x < MAP_WIDTH; x++ )
	{
		for ( int y = 0; y < MAP_HEIGHT; y++ )
		{
			int i = MAP_INDEX(x,y);
			int bot = i * 8;
			int top = bot + 4;

			float minHeight = m_height[i];

			if ( x > 0 )
				minHeight = vsMin( minHeight, m_height[i-1] );
			if ( x < MAP_WIDTH-1 )
				minHeight = vsMin( minHeight, m_height[i+1] );
			if ( y > 0 )
				minHeight = vsMin( minHeight, m_height[i-MAP_WIDTH] );
			if ( y < MAP_HEIGHT-1 )
				minHeight = vsMin( minHeight, m_height[i+MAP_WIDTH] );


			for ( int j = 0; j < 4; j++ )
			{
				vert[top + j] = vert[bot + j] = m_gridPos[i] + offset[j];
				vert[top + j].y = m_height[i];

				vert[bot + j].y = minHeight;
			}
		}
	}

	list->VertexArray(vert, MAP_CELLS * 8);
	int indices[4];

	list->SetMaterial( gNormalMaterial );
	list->SetColor( vsColor(0.f,0.5f,0.f,1.f) );

		// top surfaces.
	for ( int i = 0; i < MAP_CELLS; i++ )
	{
		int bot = i * 8;
		int top = bot + 4;

		for ( int j = 0; j < 4; j++ )
		{
			indices[j] = top + j;
		}

		list->TriangleStripArray(indices,4);
	}
#define PSCOLOR(r,g,b,a) (vsColor(r/255.f,g/255.f,b/255.f,a))
		// front surfaces.
	list->SetColor( PSCOLOR(100,90,60,1.f) );
	for ( int i = 0; i < MAP_CELLS; i++ )
	{
		float height = m_height[i];

		int nextRow = i-1;
		float nextRowHeight = 0.f;

		if (  nextRow > 0 )
		{
			nextRowHeight = m_height[nextRow];
		}

		if ( height > nextRowHeight )
		{
			int bot = i * 8;
			int top = bot + 4;

			indices[0] = top+0;
			indices[1] = top+2;
			indices[2] = bot+0;
			indices[3] = bot+2;

			list->TriangleStripArray(indices,4);
		}
	}

	list->SetColor( PSCOLOR(50,40,30,1.f) );
	for ( int i = 0; i < MAP_CELLS; i++ )
	{
		float height = m_height[i];

		int nextRow = i + MAP_WIDTH;
		float nextRowHeight = 0.f;

		if (  nextRow < MAP_CELLS )
		{
			nextRowHeight = m_height[nextRow];
		}

		if ( height > nextRowHeight )
		{
			int bot = i * 8;
			int top = bot + 4;

			indices[0] = top+2;
			indices[1] = top+3;
			indices[2] = bot+2;
			indices[3] = bot+3;

			list->TriangleStripArray(indices,4);
		}
	}

	list->SetColor( PSCOLOR(40,30,20,1.f) );
	for ( int i = 0; i < MAP_CELLS; i++ )
	{
		float height = m_height[i];

		int nextRow = i - MAP_WIDTH;
		float nextRowHeight = 0.f;

		if (  nextRow >= 0 )
		{
			nextRowHeight = m_height[nextRow];
		}

		if ( height > nextRowHeight )
		{
			int bot = i * 8;
			int top = bot + 4;

			indices[0] = top+1;
			indices[1] = top+0;
			indices[2] = bot+1;
			indices[3] = bot+0;

			list->TriangleStripArray(indices,4);
		}
	}

	list->SetColor( PSCOLOR(25,20,10,1.f) );
	for ( int i = 0; i < MAP_CELLS; i++ )
	{
		float height = m_height[i];

		int nextRow = i+1;
		float nextRowHeight = 0.f;

		if (  nextRow < MAP_CELLS )
		{
			nextRowHeight = m_height[nextRow];
		}

		if ( height > nextRowHeight )
		{
			int bot = i * 8;
			int top = bot + 4;

			indices[0] = top+3;
			indices[1] = top+1;
			indices[2] = bot+3;
			indices[3] = bot+1;

			list->TriangleStripArray(indices,4);
		}
	}


	SetDisplayList(list);
}

vsVector3D
tbMap::GetCenterGroundPosition()
{
	vsVector3D result = m_gridPos[MAP_INDEX(MAP_WIDTH>>1,MAP_HEIGHT>>1)];
	result.y = m_height[MAP_INDEX(MAP_WIDTH>>1,MAP_HEIGHT>>1)];
	return result;
}

class tbFlyCamera : public vsCamera3D
{
	tbGame *	m_game;
	vsVector2D	m_mousePos;
	vsEulerAngles m_angles;
	vsQuaternion	m_baseQuaternion;

public:

	tbFlyCamera( tbGame *game );

	virtual void Update( float timeStep );
};

tbFlyCamera::tbFlyCamera( tbGame *game ):
	m_baseQuaternion(vsVector3D(0.f,0.f,1.f), vsVector3D(0.f,1.f,0.f))
{
	m_game = game;
	m_transform.SetRotation(vsQuaternion(m_angles) * m_baseQuaternion);
}

void
tbFlyCamera::Update(float timeStep)
{
	vsInput *input = m_game->GetInput();

	static float speed = 60.f;
	vsTransform3D trans = GetTransform();
	vsVector3D pos = trans.GetTranslation();//l->GetCamera3D()->GetTransform().GetTranslation();
	if ( input->IsDown(CID_Right) )
	{
		pos += trans.GetMatrix().x * speed * timeStep;
	}
	if ( input->IsDown(CID_Left) )
	{
		pos -= trans.GetMatrix().x * speed * timeStep;
	}
	if ( input->IsDown(CID_Up) )
	{
		pos += trans.GetMatrix().z * speed * timeStep;
	}
	if ( input->IsDown(CID_Down) )
	{
		pos -= trans.GetMatrix().z * speed * timeStep;
	}

	if ( input->IsDown(CID_MouseRightButton) )
	{
		vsVector2D newMousePos = input->GetMousePosition(0);
		vsVector2D delta = (newMousePos - m_mousePos) * (PI / 600.f);

		m_angles.pitch += delta.y;
		m_angles.yaw += delta.x;

		/*vsQuaternion p( vsVector3D(1.f,0.f,0.f), delta.y );
		vsQuaternion y( vsVector3D(0.f,1.f,0.f), -delta.x );
		trans.SetQuaternion( trans.GetQuaternion() * y * p );*/
		trans.SetRotation(vsQuaternion(m_angles) * m_baseQuaternion);

		vsSystem::Instance()->HideCursor();
	}
	else
	{
		vsSystem::Instance()->ShowCursor(true);
	}

	m_mousePos = input->GetMousePosition(0);


	trans.SetTranslation(pos);

	// okay.  Now, we want to flatten out any bank we may have acquired.

/*	if ( trans.GetMatrix().y.y < 0.f || vsFabs(trans.GetMatrix().x.y) > 0.0001f )
	{
		vsQuaternion flatQuat( trans.GetMatrix().z, vsVector3D(0.f,1.f,0.f) );
		trans.SetQuaternion( flatQuat );
	}*/

	//	vsVector3D forward = -pos;
	//	vsTransform3D trans( vsQuaternion(forward, vsVector3D(0.f,1.f,0.f)), pos );
	//	trans.SetAsRotationAroundY( vsAngle(DEGREES(20.f)) );
	SetTransform(trans);
}

#define MAX_SLICES (60)

class tbSphere : public vsModel
{
	vsRenderBuffer	m_vbo;
	vsRenderBuffer	m_strip[MAX_SLICES];
public:

	tbSphere( int slices );
};

tbSphere::tbSphere( int slices )
{
	vsAssert(slices > 1, "Need more than one slice!");

	int pitchSlices = slices / 2;

	vsDisplayList *constructor = new vsDisplayList(1 * 1024);

    vsRenderBuffer::PNT pnt[ MAX_SLICES*MAX_SLICES ];

	float headingPerSlice = -TWOPI * (1.f / (slices-1));
	float pitchPerSlice = PI * (1.f / (pitchSlices-1));

	float iniHeading = 0.f;
	float iniPitch = PI * -0.5f;

	vsEulerAngles ang;
	vsMatrix3x3 matrix;

	float xIncrement = (1.f / (slices-1));
	float yIncrement = (1.f / (pitchSlices-1));

	for ( int x = 0; x < slices; x++ )
	{
		float heading = iniHeading + (x * headingPerSlice);

		for ( int y = 0; y < pitchSlices; y++ )
		{
			float pitch = iniPitch + (y * pitchPerSlice);

			ang.yaw = heading;
			ang.pitch = pitch;

			matrix.Set( vsQuaternion(ang) );

			pnt[ (y*slices)+x ].position = matrix.z * 1.f;
			pnt[ (y*slices)+x ].normal = matrix.z;
			pnt[ (y*slices)+x ].texel.Set( x * xIncrement, y * yIncrement );
		}
	}

	m_vbo.SetArray(pnt,pitchSlices*slices);

	//constructor->VertexArray(v, slices*slices);
	//constructor->NormalArray(n, slices*slices);

	constructor->BindBuffer(&m_vbo);

	uint16 indices[ MAX_SLICES*2 ];
	int i = 0;
	for ( int x = 0; x < slices-1; x++ )
	{
		i = 0;
		for ( int y = 0; y < pitchSlices; y++ )
		{
			indices[i++] = (y*slices)+x+1;
			indices[i++] = (y*slices)+x;
		}
		m_strip[x].SetArray(indices,pitchSlices*2);
		constructor->TriangleStripBuffer(&m_strip[x]);

//		constructor->TriangleStrip(indices,slices*2);
	}

	constructor->ClearArrays();

//	vsDisplayList *list = new vsDisplayList( constructor->GetSize() );
//	list->Append(constructor);

	SetDisplayList(constructor);

//	vsDelete( constructor );
}


//#if defined(_DEBUG)
REGISTER_GAME("Testbed", tbGame);


tbGame::tbGame():
	m_networkDisplay(NULL)
{
}

void
tbGame::Init()
{
	m_sceneCount = 2;
	Parent::Init();

	gNormalMaterial = new vsMaterial("Normal");
	gAddMaterial = new vsMaterial("Add");
	gLitMaterial = new vsMaterial("Lit");
	gGlobeMaterial = new vsMaterial("Globe");

	//	m_e = new vsPhysicsEmitter("Texture", 1.0f, 500);

	/*m_title = new vsSprite( vsBuiltInFont::CreateString("VectorStorm Textures", 30.f, 40.f, Justification_Center) );
	m_title->SetPosition( vsVector2D(0.f, -100.f) );
	m_title->RegisterOnScene(0);

	m_texture = vsSprite::Load("Texture");
	m_texture->SetPosition(vsVector2D(-100.f,0.f));
	m_texture->RegisterOnScene(0);

	m_texture2 = vsSprite::Load("Texture2");
	m_texture2->SetPosition(vsVector2D(100.f,0.f));
	m_texture2->RegisterOnScene(0);
	*/
/*	vsVector3D array[3] = {
		vsVector3D(0.f,100.f,100.f),
		vsVector3D(0.f,10.f,100.f),
		vsVector3D(10.f,0.f,100.f)
	};
	int indices[4] = { 0, 1, 2, 0 };

	vsDisplayList *list = new vsDisplayList(512);
	list->VertexArray(array,3);
	list->SetColor(vsColor::Red);
	list->TriangleStrip(indices,3);
	list->SetColor(vsColor::White);
	list->LineStrip(indices,4);
	vsSprite *s = new vsSprite(list);
	s->RegisterOnScene(0);*/


	m_cube = new vsModel();
	m_cube->RegisterOnScene(1);
	m_cube->SetPosition( vsVector3D(-60.f, 0.f, 0.f) );
    m_cube->AddFragment( vsMakeSolidBox3D(vsBox3D(20.f * -vsVector3D::One, 20.f * vsVector3D::One), "Lit") );

	//m_sphere->SetScale(50.f);

	m_map = new tbMap;
	//m_map->SetColor(vsColor::LightGreen);
	m_map->SetPosition( vsVector3D(0.f,0.f,0.f) );
	m_map->RegisterOnScene(1);

	m_sphere = new tbSphere(60);
	m_sphere->RegisterOnScene(1);
	m_sphere->SetPosition( m_map->GetCenterGroundPosition() + vsVector3D(0.f,1.f,0.f) );
	m_sphere->SetMaterial( "Globe" );



	vsScene *l = vsSystem::GetScreen()->GetScene(1);
	l->Set3D(true);
	/*vsVector4D pos(0.f,0.f,-100.f,1.f);
	vsVector3D forward = -pos;
	vsTransform3D trans( vsQuaternion(forward, vsVector3D(0.f,1.f,0.f)), pos );

	l->GetCamera3D()->SetTransform( trans );*/
	m_camera = new tbFlyCamera(this);
	m_camera->SetPosition( m_sphere->GetPosition() + vsVector3D(0.f, 0.f, -4.f) );
	l->SetCamera3D( m_camera );


	vsMatrix3x3 ini( vsVector3D(1.f,0.f,0.f), vsVector3D(0.f,1.f,0.f) );
	vsMatrix3x3 mat;
	vsQuaternion quat( vsVector3D(0.f,1.f,0.f), 0.f );
	quat.Set(ini);
	mat.Set(quat);


	m_background = new vsSprite( vsDisplayList::Load("Background") );
	m_background->RegisterOnScene(0);
	m_background->SetColor(vsColor::PureWhite);


	m_light = new vsLight;
	m_light->SetType( vsLight::Type_Point );
	m_lightAngle = 0.f;
	m_light->SetPosition( m_lightAngle.GetForwardVector() * 30.f );

	m_lightB = new vsLight;
	m_lightB->SetType( vsLight::Type_Point );
	m_lightAngleB = PI*0.5f;
	m_lightB->SetPosition( m_lightAngle.GetForwardVector() * 30.f );
	m_lightB->SetColor( vsColor(0.9f,0.0f,0.1f,1.f) );

	m_lightC = new vsLight;
	m_lightC->SetType( vsLight::Type_Ambient );
	m_lightC->SetColor( vsColor(0.1f,0.1f,0.1f,1.f) );

	l->AddLight(m_light);
	l->AddLight(m_lightB);
	l->AddLight(m_lightC);

	m_lightSphere = new tbSphere(10);
	m_lightSphere->RegisterOnScene(1);
	m_lightSphere->SetScale(1.0f);
//	m_lightSphere->GetMaterial()->m_color = m_light->GetColor();
//	m_lightSphere->GetMaterial()->m_color.a = 1.f;

	m_lightSphereB = new tbSphere(10);
	m_lightSphereB->RegisterOnScene(1);
	m_lightSphereB->SetScale(1.0f);
//	m_lightSphereB->GetMaterial()->m_color = m_lightB->GetColor();
//	m_lightSphereB->GetMaterial()->m_color.a = 1.f;


	//m_networkDisplay = new tbNetworkDisplay();
	//m_networkDisplay->RegisterOnScene(0);
}

void
tbGame::Deinit()
{
	vsSystem::GetScreen()->GetScene(1)->RemoveLight(m_light);
	vsSystem::GetScreen()->GetScene(1)->RemoveLight(m_lightB);
	vsSystem::GetScreen()->GetScene(1)->RemoveLight(m_lightC);
	vsDelete( m_light );
	vsDelete( m_lightB );
	vsDelete( m_lightC );
	vsDelete( m_lightSphere );
	vsDelete( m_lightSphereB );
	vsDelete( m_camera );
	vsDelete( m_map );
	vsDelete( m_cube );
	vsDelete( m_sphere );
	vsDelete( m_background );

	vsDelete( gNormalMaterial );
	vsDelete( gAddMaterial );
	vsDelete( gLitMaterial );
	vsDelete( gGlobeMaterial );

	//vsDelete(m_child);
	//vsDelete(m_networkDisplay);

//	vsDelete(m_title);
//	vsDelete(m_texture);
//	vsDelete(m_texture2);
//	vsDelete(m_e);

	Parent::Deinit();
}

static vsVector2D mousePos;

void
tbGame::Update( float timeStep )
{
//	UNUSED(timeStep);

//	m_e->SetSpawnVelocity( vsVector2D::Zero, 1000.f );
//	m_e->SetSpawnRate(10.f);
//	m_e->Update( timeStep );

	m_lightAngle += timeStep * PI;
	vsVector2D pos =  m_lightAngle.GetForwardVector() * 60.f;
	m_lightSphere->SetPosition( vsVector3D(pos.x, 60.f, pos.y) );
	m_light->m_position.Set( pos.x, 60.f, pos.y );

	m_lightAngleB += 1.4f * timeStep * PI;
	pos =  m_lightAngleB.GetForwardVector() * 100.f;
	m_lightSphereB->SetPosition( vsVector3D(pos.x, -10.f, pos.y) );
	m_lightB->m_position.Set( pos.x, -10.f, pos.y );

	Parent::Update(timeStep);
}

//#endif // _DEBUG
