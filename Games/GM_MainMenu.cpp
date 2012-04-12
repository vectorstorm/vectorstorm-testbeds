/*
 *  GM_MainMenu.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 18/03/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#include "GM_MainMenu.h"

#if TARGET_OS_IPHONE
#include "Wedge.h"
#endif


REGISTER_MAINGAME("MainMenu", gmMainMenu);


#if defined(_DEBUG)
vsSprite *s_modeSprite = NULL;
#endif // _DEBUG

static void BuildModeSprite()
{
#if !TARGET_OS_IPHONE
#if defined(_DEBUG)

	if ( s_modeSprite )
		vsDelete(s_modeSprite);

	extern bool g_renderSceneTexture;
	extern bool g_renderOffscreenTexture;
	extern int g_renderOffscreenTextureId;

	if ( g_renderOffscreenTexture )
		s_modeSprite = new vsSprite( vsBuiltInFont::CreateString(vsFormatString("Blur #%d",g_renderOffscreenTextureId), 30.f, 30.f));
	else if ( g_renderSceneTexture )
	{
		vsString nobloomString = vsLocalisationTable::Instance()->GetTranslation("TEXT_NO_BLOOM");
		s_modeSprite = new vsSprite( vsBuiltInFont::CreateString(nobloomString, 30.f, 30.f) );
	}
	else
	{
		vsString bloomString = vsLocalisationTable::Instance()->GetTranslation("TEXT_BLOOM");
		s_modeSprite = new vsSprite( vsBuiltInFont::CreateString(bloomString, 30.f, 30.f) );
	}

	s_modeSprite->SetColor(vsColor::White);
	s_modeSprite->SetPosition(vsVector2D(0.f,-400.f));
	s_modeSprite->RegisterOnScene(0);

#endif //_DEBUG
#endif // TARGET_OS_IPHONE
}

/*
void
gmMainMenuEntity::Init()
{
	m_font = new vsFont;
	m_cacheString = new vsDisplayList(1024);
	m_cacheString->Clear();
	m_font->BuildDisplayListForString(m_cacheString, "VectorStorm");
}

void
gmMainMenuEntity::Deinit()
{
	delete m_font;
	delete m_cacheString;
}

void
gmMainMenuEntity::_Draw( vsDisplayList *list )
{
	list->Append( *m_cacheString );
}
*/

gmMainMenu::gmMainMenu()
{
	m_selection = 0;
}

void
gmMainMenu::Init()
{
	Parent::Init();

	vsLocalisationTable *lt = new vsLocalisationTable;
	lt->Init();

//	m_shader = new vsDrawShader(1000.f);
//	vsDisplayList *shaderList = new vsDisplayList(128);
//	shaderList->SetShader(m_shader);
//	m_shaderEntity = new vsSprite( shaderList );
	m_shaderEntity = new vsSprite( NULL );
	m_shaderEntity->RegisterOnScene(0);

	m_music = new vsMusic("TicTacTic.mp3");
//	m_music = new sndMusic("tt2.ogg");
	m_tickSound = new vsSoundSample("thump_4.wav");

	BuildTitleSprite();

	m_menuSpriteCount = coreGameRegistry::GetGameCount();
	m_menuSprite = new vsSprite *[m_menuSpriteCount];

	for ( int i = 0; i < m_menuSpriteCount; i++ )
	{
		m_menuSprite[i] = new vsSprite( vsDisplayList::Load("MenuFrame") );
		m_menuSprite[i]->RegisterOnScene(0);

		m_menuSprite[i]->m_transform.m_position.Set(0.0f, -250.0f + (90.0f * i));
		m_menuSprite[i]->m_transform.m_scale.Set(1.5f, 1.5f);
		//m_menuSprite[i]->SetShader(m_shader);

		vsSprite *buttonImage = new vsSprite( vsBuiltInFont::CreateString( coreGameRegistry::GetGameName(i), 15.0f, 25.0f ) );
		buttonImage->m_transform.m_position.Set(-190.0f, 0.0f);
		m_menuSprite[i]->AddChild(buttonImage);
	}

/*
	m_buildName = new vsSprite(
						   vsBuiltInFont::CreateString(coreVersion::GetVersionNameString(), 15.0f, 25.0f, Justification_Center) );
	m_buildName->m_transform.m_position.Set(480.0f,340.0f);
	m_buildName->m_transform.m_angle.Set(DEGREES(10.0f));
	m_buildName->SetColor(vsColor(0.0f,0.0f,1.0f));
	m_buildName->RegisterOnScene(0);
*/
	m_pulseTimer = 0.f;

	//m_music->Start();

	BuildModeSprite();
}

void
gmMainMenu::Deinit()
{
#if defined(_DEBUG)
	vsDelete(s_modeSprite);
#endif // _DEBUG

	vsLocalisationTable::Instance()->Deinit();
	delete vsLocalisationTable::Instance();

	vsDelete(m_shaderEntity);

	vsDelete(m_title);

	for ( int i = 0; i < m_menuSpriteCount; i++ )
		vsDelete(m_menuSprite[i]);
	vsDeleteArray(m_menuSprite);

	vsDelete(m_music);
	vsDelete(m_tickSound);

	vsDelete(m_stripes[0]);
	vsDelete(m_stripes[1]);

//	vsDelete(m_build);
//	delete m_buildName;

	Parent::Deinit();
}

void
gmMainMenu::BuildTitleSprite()
{
	m_title = new vsSprite();

	char title[12] = "VectorStorm";
	m_title->m_transform.m_position.Set(-550.f, -350.f);

	m_title->RegisterOnScene(0);

	for ( int i = 0; i < 11; i++ )
	{
		// move objects around based on which character it is.

		if ( i == 0 )		// in 'V'
		{
			m_letter[i] = new vsSprite( vsBuiltInFont::CreateCharacter(title[i], 30.0f, 120.0f) );

			m_letter[i]->m_transform.m_position.Set(0.0f, 0.0f);
			m_letter[i]->SetColor( vsColor::Blue );
		}
		else if ( i < 6 )	// in 'ector'
		{
			m_letter[i] = new vsSprite( vsBuiltInFont::CreateCharacter(title[i], 30.0f, 80.0f)/*, i * 0.2f*/ );
			m_letter[i]->m_transform.m_position.Set(50.0f + i*35.0f,-70.0f);
			m_letter[i]->SetColor( vsColor::Blue );
		}
		else if ( i == 6 )	// in 'S'
		{
			m_letter[i] = new vsSprite( vsBuiltInFont::CreateCharacter(title[i], 30.0f, 120.0f) );
			m_letter[i]->m_transform.m_position.Set(40.0f, 40.0f);
			m_letter[i]->SetColor( vsColor::LightBlue );
		}
		else // in 'torm'
		{
			m_letter[i] = new mmStretchSprite( vsBuiltInFont::CreateCharacter(title[i], 40.0f, 80.0f), i * 0.2f );
			int StormChar = i - 6;
			m_letter[i]->m_transform.m_position.Set(60.0f + StormChar*50.0f, -30.0f);
			m_letter[i]->SetColor( vsColor::LightBlue );
		}


		m_title->AddChild( m_letter[i] );
	}
	for ( int i = 0; i < 2; i++ )
	{
		m_stripes[i] = new vsSprite( vsDisplayList::Load("Stripes") );
		m_stripes[i]->SetColor( vsColor::Yellow );
		m_stripes[i]->m_transform.m_position.Set( -250.0f + (500.0f*i), 470.0f );
		m_stripes[i]->m_transform.m_angle.Set( DEGREES(10.0f) );
		m_stripes[i]->RegisterOnScene(0);
	}
}

#define PULSE_DURATION (2.0f)

void
gmMainMenu::Update(float timeStep)
{
#if TARGET_OS_IPHONE
	switch( GetDeviceOrientation() )
	{
		case Orientation_Portrait:
			vsSystem::Instance()->SetOrientation( Orientation_Normal );
			break;
		case Orientation_PortraitUpsideDown:
			vsSystem::Instance()->SetOrientation( Orientation_Six );
			break;
		case Orientation_LandscapeLeft:
			vsSystem::Instance()->SetOrientation( Orientation_Three );
			break;
		case Orientation_LandscapeRight:
			vsSystem::Instance()->SetOrientation( Orientation_Nine );
			break;
	}
#endif // TARGET_OS_IPHONE

	vsInput *input = GetInput();

	//m_shader->Update(timeStep);

	if ( input->WasPressed( CID_Down ) || input->WasPressed( CID_LDown ) )
	{
		coreGame::GetSound()->PlaySound(m_tickSound);
		m_selection++;
		if ( m_selection >= m_menuSpriteCount )
			m_selection = 0;
	}
	if ( input->WasPressed( CID_Up ) || input->WasPressed( CID_LUp ) )
	{
		coreGame::GetSound()->PlaySound(m_tickSound);
		m_selection--;
		if ( m_selection < 0 )
			m_selection = m_menuSpriteCount-1;
	}
	if ( input->WasPressed( CID_A ) )
		core::SetGame( coreGameRegistry::GetGame(m_selection) );

#if !TARGET_OS_IPHONE
#if defined(_DEBUG)
	if ( input->WasPressed( CID_B ) )
	{
		extern bool g_renderSceneTexture;
		extern bool g_renderOffscreenTexture;
		extern int g_renderOffscreenTextureId;

		if ( g_renderOffscreenTexture )
		{
			g_renderOffscreenTextureId++;
			if ( g_renderOffscreenTextureId >= 3 )
			{
				g_renderSceneTexture = false;
				g_renderOffscreenTexture = false;
				g_renderOffscreenTextureId = 0;
			}
		}
		else if ( g_renderSceneTexture )
		{
			g_renderSceneTexture = false;
			g_renderOffscreenTexture = true;
			g_renderOffscreenTextureId = 0;
		}
		else
		{
			g_renderSceneTexture = true;
		}
		BuildModeSprite();
	}
#endif // _DEBUG
#endif // TARGET_OS_IPHONE

	m_pulseTimer += timeStep;
	if ( m_pulseTimer > PULSE_DURATION )
		m_pulseTimer -= PULSE_DURATION;

	float frac = m_pulseTimer / PULSE_DURATION;

	float pulseAmt = vsCos(TWOPI * frac);	// [ -1..1 ]
	pulseAmt = (pulseAmt * 0.5f) + 0.5f;	// [ 0..1 ]

	for ( int i = 0; i < m_menuSpriteCount; i++ )
	{
		if ( i == m_selection )
		{
			vsColor lightBlue(0.5f,0.5f,1.0f,0.8f);
			vsColor pulseColor = vsInterpolate( pulseAmt, lightBlue, vsColor::White );
			m_menuSprite[i]->SetColor( pulseColor );
		}
		else
			m_menuSprite[i]->SetColor( vsColor::Blue );
	}
}

mmStretchSprite::mmStretchSprite( vsDisplayList *list, float delay ): vsSprite(list), m_timer(-delay)
{
}

void
mmStretchSprite::Update( float timeStep )
{
	m_timer += timeStep;

	if ( m_timer > 2.0f )
		m_timer -= 2.0f;

	float scaleVal = 0.5f * (vsCos( m_timer * PI ) + 1.0f);	// [0..1]
	scaleVal += 1.5f;										// [1.5 .. 2.5]
	scaleVal *= 0.5f;										// [0.75 .. 1.25]

	m_transform.m_scale.Set(1.0f,scaleVal);
}
