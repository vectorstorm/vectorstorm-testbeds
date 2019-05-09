/*
 *  FM_FontMaker.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 28/12/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#if 0
#if defined(_DEBUG)

#include "FM_FontMaker.h"


//#include "VS_FontMaker.h"

//REGISTER_GAME("FontMaker", fmFontMaker);


fmFontMaker::fmFontMaker()
{
}

fmFontMaker::~fmFontMaker()
{
}

void
fmFontMaker::Init()
{
	Parent::Init();
#if 0
	vsFontMaker::MakeFont("FreeSans", 64);
	vsFontMaker::MakeFont("FreeSerif", 64);
	vsFontMaker::MakeFont("CircleOfDust", 64);
	vsFontMaker::MakeFont("FreeMonoOblique", 64);
	vsFontMaker::MakeFont("Bitmap04B_03", 64);
#endif // 0
	m_font = new vsFont("FreeSerif.txt");
	vsFontRenderer fr(m_font, 64.f);
	fr.SetJustificationType(Justification_Center);
	fr.SetMaxWidth(800.f);
	m_sprite = new vsSprite( fr.DisplayList2D("Behold!  A very long wrapping string!") );
	m_sprite->RegisterOnScene(0);
	m_sprite->SetPosition( vsVector2D(0.f,0.f) );
	m_sprite->SetColor( vsColor::LightGreen );

	vsDisplayList *list = new vsDisplayList(1024);
	list->MoveTo(vsVector2D(-400.f,-64.f));
	list->LineTo(vsVector2D(400.f,-64.f));
	list->LineTo(vsVector2D(400.f,64.f));
	list->LineTo(vsVector2D(-400.f,64.f));
	list->LineTo(vsVector2D(-400.f,-64.f));
	m_guide = new vsSprite(list);
	m_guide->RegisterOnScene(0);

	m_fontB = new vsFont("CircleOfDust.txt");
	m_other = new vsSprite( m_fontB->CreateString2D("Bitmap fonts", 40.f, Justification_Center ) );
	m_other->RegisterOnScene(0);
	m_other->SetPosition( vsVector2D(0.f,100.f) );
	m_other->SetColor( vsColor::LightBlue );

	m_fontC = new vsFont("FreeMonoOblique.txt");
	m_final = new vsSprite( m_fontC->CreateString2D("in VectorStorm!", 60.f, Justification_Center ) );
	m_final->RegisterOnScene(0);
	m_final->SetAngle( DEGREES(-10.f) );
	m_final->SetPosition( vsVector2D(0.f,120.f) );
	m_final->SetColor( vsColor::Red );


	m_muncher = new vsSprite( vsDisplayList::Load("Muncher") );
	m_muncher->SetPosition( vsVector2D(200.f,20.f) );
	m_muncher->RegisterOnScene(0);
}

void
fmFontMaker::Deinit()
{
	vsDelete( m_guide );
	vsDelete( m_sprite );
	vsDelete( m_other );
	vsDelete( m_final );
	vsDelete( m_font );
	vsDelete( m_fontB );
	vsDelete( m_fontC );
	vsDelete( m_muncher );
	Parent::Deinit();
}

void
fmFontMaker::Update( float timeStep )
{
	Parent::Update(timeStep);
}

#endif
#endif
