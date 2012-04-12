#include "LT_PlayerControl.h"
#include "LT_Scenes.h"

ltPlayerControl::ltPlayerControl():
	ltLerpable("Player Control"),
	m_player(NULL),
	m_target(NULL)
{
	vsBox2D box( vsVector2D::One * -10.f, vsVector2D::One * 10.f );
	vsBox2D targetBox( vsVector2D::One * -50.f, vsVector2D::One * 50.f );
	vsBox2D playerBox( vsVector2D::One * -20.f, vsVector2D::One * 20.f );

	m_player = new vsSprite;
	m_player->AddFragment( vsMakeOutlineBox2D(playerBox, "White", &vsColor::White ) );
	m_player->AddFragment( vsMakeSolidBox2D(playerBox, "White", &vsColor::Blue ) );
	m_player->BuildBoundingBox();
	m_target = new vsSprite;
	m_target->AddFragment( vsMakeOutlineBox2D(targetBox, "White", &vsColor::White ) );
	m_target->AddFragment( vsMakeSolidBox2D(targetBox, "White", &vsColor::Green ) );
	m_target->BuildBoundingBox();

	for ( int i = 0; i < ENEMY_COUNT; i++ )
	{
		m_enemy[i] = new vsSprite;
		m_enemy[i]->AddFragment( vsMakeOutlineBox2D(box, "White", &vsColor::Red) );
		m_enemy[i]->BuildBoundingBox();
	}

	m_text = new vsSprite;
}

ltPlayerControl::~ltPlayerControl()
{
	vsDelete( m_player );
	vsDelete( m_target );

	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		vsDelete( m_enemy[i] );
	}
	vsDelete( m_text );
}

void
ltPlayerControl::Init()
{
	Parent::Init();

	m_player->RegisterOnScene( Scene_UI );
	m_target->RegisterOnScene( Scene_UI );
	m_text->RegisterOnScene( Scene_UI );

	vsVector2D bottomRight = vsSystem::GetScreen()->GetScene( Scene_UI )->GetBottomRightCorner();
	m_text->SetPosition( bottomRight + vsVector2D( -30.f, -60.f ) );

	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		m_enemy[i]->RegisterOnScene( Scene_UI );
	}

	PlaceGamePieces();

}

void
ltPlayerControl::Deinit()
{
	m_player->Extract();
	m_target->Extract();
	m_text->Extract();
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		m_enemy[i]->Extract();
	}

	Parent::Deinit();
}

void
ltPlayerControl::Update( float timeStep )
{
	vsVector2D playerInput = vsInput::Instance()->GetLeftStick();
	float magnitude;

	if ( playerInput.SqLength() == 0.f )
	{
		magnitude = 0.f;
	}
	else
	{
		magnitude = vsClamp(playerInput.Length(), 0.f, 1.f);
		magnitude = m_curve->GetCurveValue( magnitude );

		playerInput.Normalise();
		playerInput *= magnitude;
	}

	if ( magnitude >= 0.4f )
	{
		m_ok = false;
	}
	else
	{
		// travel back to the bottom of the screen to try again.
		if ( m_player->GetPosition().y >= 400.f )
		{
			m_ok = true;
		}
	}

	SetText(magnitude);



	const float c_playerSpeed = 300.f;
	m_player->SetPosition( m_player->GetPosition() + playerInput * c_playerSpeed * timeStep );

	if ( Intersects( m_player, m_target ) )
	{
		PlaceGamePieces();
		return;
	}

	for ( int i = 0; i < ENEMY_COUNT; i++ )
	{
		if ( Intersects( m_player, m_enemy[i] ) )
		{
			m_player->SetPosition( vsVector2D( vsRandom::GetFloat(-400.f,400.f), 450.f ) );
		}
	}
}

void
ltPlayerControl::SetText(float value)
{
	vsString string = vsFormatString("%d%%", (int)(value*100));
	vsColor color = vsColor::Red;

	if ( m_ok )
	{
		color = vsColor::Green;
	}

	m_text->ClearFragments();
	m_text->AddFragment( m_font->CreateString_Fragment( FontContext_2D, string, 50.f, Justification_Right, -1.f, color ) );
}

void
ltPlayerControl::PlaceGamePieces()
{
	m_ok = true;

	// player gets placed somewhere at the bottom end of the screen.
	m_player->SetPosition( vsVector2D( vsRandom::GetFloat(-400.f,400.f), 450.f ) );
	// target is somewhere at the top
	m_target->SetPosition( vsVector2D( vsRandom::GetFloat(-400.f,400.f), -450.f ) );

	// enemies go in between
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		m_enemy[i]->SetPosition( vsRandom::GetVector2D( -400.f, 400.f ) );
	}
}

bool
ltPlayerControl::Intersects( vsSprite *a, vsSprite *b )
{
	vsBox2D ab = a->GetBoundingBox();
	ab += a->GetPosition();

	vsBox2D bb = b->GetBoundingBox();
	bb += b->GetPosition();

	return ab.Intersects( bb );
}

