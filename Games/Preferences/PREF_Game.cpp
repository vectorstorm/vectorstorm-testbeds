/*
 *  Pref_Game.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 28/12/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#include "PREF_Game.h"

REGISTER_GAME("Preferences", prefGame);




prefGame::prefGame()
{
}

enum topItems
{
	Top_Screen,
	Top_Sound,
	Top_Controls,
	Top_Back
};

enum screenItems
{
	Screen_Resolution,
	Screen_Fullscreen,
	Screen_Bloom,
	Screen_Cancel,
	Screen_Accept
};

enum soundItems
{
	Sound_EffectVolume,
	Sound_MusicVolume,
	Sound_Cancel,
	Sound_Accept
};

enum controlItems
{
	Control_LLeft,
	Control_LRight,
	Control_LUp,
	Control_LDown,
	Control_AButton,
	Control_BButton,
	Control_DPadUp,
	Control_DPadDown,
	Control_DPadLeft,
	Control_DPadRight,

	Control_Cancel,
	Control_Accept,

	CONTROL_MAX,


	CONTROL_NUM_CONTROLS = Control_Cancel	// last real control to be configured.

};


static ControlID s_cid[CONTROL_NUM_CONTROLS] =
{
	CID_LLeft,		//Control_LLeft,
	CID_LRight,		//Control_LRight,
	CID_LUp,		//Control_LUp,
	CID_LDown,		//Control_LDown,
	CID_A,			//Control_AButton,
	CID_B,			//Control_BButton,
	CID_Up,			//Control_DPadUp,
	CID_Down,		//Control_DPadDown,
	CID_Left,		//Control_DPadLeft,
	CID_Right		//Control_DPadRight,
};



void
prefGame::Init()
{
	Parent::Init();

	m_state = Top;

	vsVector2D menuPos(-500,-300);

	m_topMenu = new vsSimpleMenu(4);

	m_topMenu->SetItemLabel(Top_Screen, "Screen");
	m_topMenu->SetItemLabel(Top_Sound, "Sound");
	m_topMenu->SetItemLabel(Top_Controls, "Gamepad");
	m_topMenu->SetItemLabel(Top_Back, "Back");

	m_topMenu->RegisterOnScene(0);
	m_topMenu->SetPosition(menuPos);


	m_screenMenu = new vsSimpleMenu(5);

	m_screenMenu->SetItemLabel(Screen_Resolution, "Resolution");
	m_screenMenu->SetItemLabel(Screen_Fullscreen, "Fullscreen");
	m_screenMenu->SetItemLabel(Screen_Bloom, "Bloom Filter");
	m_screenMenu->SetItemLabel(Screen_Cancel, "Cancel");
	m_screenMenu->SetItemLabel(Screen_Accept, "Accept");

	m_screenMenu->RegisterOnScene(0);
	m_screenMenu->SetPosition(menuPos);
	m_screenMenu->SetVisible(false);

	m_soundMenu = new vsSimpleMenu(4);

	m_soundMenu->SetItemLabel(Sound_EffectVolume, "Effect Volume");
	m_soundMenu->SetItemLabel(Sound_MusicVolume, "Music Volume");
	m_soundMenu->SetItemLabel(Sound_Cancel, "Cancel");
	m_soundMenu->SetItemLabel(Sound_Accept, "Accept");

	m_soundMenu->RegisterOnScene(0);
	m_soundMenu->SetPosition(menuPos);
	m_soundMenu->SetVisible(false);


	m_controlMenu = new vsSimpleMenu(CONTROL_MAX, 15.f, 25.f);
	m_controlMenu->SetItemLabel(Control_LUp, "Up");
	m_controlMenu->SetItemLabel(Control_LDown, "Down");
	m_controlMenu->SetItemLabel(Control_LLeft, "Left");
	m_controlMenu->SetItemLabel(Control_LRight, "Right");

	m_controlMenu->SetItemLabel(Control_AButton, "A Button");
	m_controlMenu->SetItemLabel(Control_BButton, "B Button");

	m_controlMenu->SetItemLabel(Control_DPadUp, "DPad Up");
	m_controlMenu->SetItemLabel(Control_DPadDown, "DPad Down");
	m_controlMenu->SetItemLabel(Control_DPadLeft, "DPad Left");
	m_controlMenu->SetItemLabel(Control_DPadRight, "DPad Right");

	m_controlMenu->SetItemLabel(Control_Cancel, "Cancel");
	m_controlMenu->SetItemLabel(Control_Accept, "Accept");

	m_controlMenu->RegisterOnScene(0);
	m_controlMenu->SetPosition(menuPos);
	m_controlMenu->SetVisible(false);


	m_controlHelpText = new vsSprite( vsBuiltInFont::CreateString("Changes will take effect after you choose Accept", 15.f, 20.f, Justification_Center) );
	m_controlHelpText->RegisterOnScene(0);
	m_controlHelpText->SetPosition( vsVector2D(0.f,400.f) );
	m_controlHelpText->SetColor( vsColor::Blue * 0.75f );

	m_deviceControl = new DeviceControl[CONTROL_NUM_CONTROLS];
	m_controlCount = CONTROL_NUM_CONTROLS;

	m_controlBeingPolled = 0;
	m_polling = false;


	ResetValues();
}

void
prefGame::Deinit()
{
	vsDeleteArray(m_deviceControl);

	vsDelete(m_topMenu);
	vsDelete(m_screenMenu);
	vsDelete(m_soundMenu);
	vsDelete(m_controlMenu);
	vsDelete(m_controlHelpText);

	Parent::Deinit();
}

void
prefGame::Update(float timeStep)
{
	vsSimpleMenuAction action;
	switch(m_state)
	{
		case Top:
			m_topMenu->SetVisible(true);
			m_screenMenu->SetVisible(false);
			m_soundMenu->SetVisible(false);
			m_controlMenu->SetVisible(false);

			action = m_topMenu->GetAction();
			if ( action.type == vsSimpleMenuAction::Type_Select )
			{
				if ( action.menuItemId == Top_Screen )
				{
					m_state = Screen;
					m_screenMenu->Enter();
				}
				if ( action.menuItemId == Top_Sound )
				{
					m_state = Sound;
					m_soundMenu->Enter();
				}
				if ( action.menuItemId == Top_Controls )
				{
					m_state = Control;
					m_controlMenu->Enter();
				}
				if ( action.menuItemId == Top_Back )
				{
					core::SetExitToMenu();
				}
			}
			break;
		case Screen:
			m_topMenu->SetVisible(false);
			m_screenMenu->SetVisible(true);

			action = m_screenMenu->GetAction();
			if ( action.type == vsSimpleMenuAction::Type_Cancel ||
				(action.type == vsSimpleMenuAction::Type_Select && action.menuItemId == Screen_Cancel) )
			{
				ResetValues();	// reset our values, if we're cancelling our changes!
				m_state = Top;
			}
			else if ( action.type == vsSimpleMenuAction::Type_Select && action.menuItemId == Screen_Accept )
			{
				UpdateValues();
				vsSystem::Instance()->UpdateVideoMode();
				m_state = Top;
			}
			else if ( action.type == vsSimpleMenuAction::Type_Left )
			{
				if ( action.menuItemId == Screen_Resolution )
					m_resolution = vsMax( m_resolution-1, 0 );
				else if ( action.menuItemId == Screen_Fullscreen )
					m_fullscreen = !m_fullscreen;
				else if ( action.menuItemId == Screen_Bloom )
					m_bloom = !m_bloom;

				SetValues();
			}
			else if ( action.type == vsSimpleMenuAction::Type_Right )
			{
				if ( action.menuItemId == Screen_Resolution )
					m_resolution = vsMin( m_resolution+1, m_resolutionCount-1 );
				else if ( action.menuItemId == Screen_Fullscreen )
					m_fullscreen = !m_fullscreen;
				else if ( action.menuItemId == Screen_Bloom )
					m_bloom = !m_bloom;

				SetValues();
			}
			break;
		case Sound:
			m_topMenu->SetVisible(false);
			m_soundMenu->SetVisible(true);

			action = m_soundMenu->GetAction();
			if ( action.type == vsSimpleMenuAction::Type_Cancel ||
				(action.type == vsSimpleMenuAction::Type_Select && action.menuItemId == Sound_Cancel) )
			{
				ResetValues();	// reset our values, if we're cancelling our changes!
				m_state = Top;
			}
			else if ( action.type == vsSimpleMenuAction::Type_Select && action.menuItemId == Sound_Accept )
			{
				UpdateValues();
				//vsSystem::Instance()->UpdateVideoMode();
				m_state = Top;
			}
			else if ( action.type == vsSimpleMenuAction::Type_Left )
			{
				if ( action.menuItemId == Sound_EffectVolume )
					m_effectVolume = vsMax( m_effectVolume-1, 0 );
				else if ( action.menuItemId == Sound_MusicVolume )
					m_musicVolume = vsMax( m_musicVolume-1, 0 );

				SetValues();
			}
			else if ( action.type == vsSimpleMenuAction::Type_Right )
			{
				if ( action.menuItemId == Sound_EffectVolume )
					m_effectVolume = vsMin( m_effectVolume+1, 10 );
				else if ( action.menuItemId == Sound_MusicVolume )
					m_musicVolume = vsMin( m_musicVolume+1, 10 );

				SetValues();
			}
			break;
		case Control:
			m_topMenu->SetVisible(false);
			m_controlMenu->SetVisible(true);

			UpdateControlScreen();

			action = m_controlMenu->GetAction();
			if ( action.type == vsSimpleMenuAction::Type_Select && action.menuItemId == Control_Cancel )
			{
				ResetValues();	// reset our values, if we're cancelling our changes!
				m_state = Top;
			}
			else if ( action.type == vsSimpleMenuAction::Type_Select && action.menuItemId == Control_Accept )
			{
				UpdateValues();
				//vsSystem::Instance()->UpdateVideoMode();
				m_state = Top;
			}
			else if ( action.type == vsSimpleMenuAction::Type_Select )
			{
				if ( action.menuItemId < m_controlCount )
				{
					m_polling = true;
					m_controlBeingPolled = action.menuItemId;
					GetInput()->StartPollingForDeviceControl();
				}
			}
			break;
		default:
			break;
	}

	Parent::Update(timeStep);
}

void
prefGame::UpdateControlScreen()
{
	if ( m_polling )
	{
		if ( !GetInput()->IsPolling() )
		{
			m_deviceControl[m_controlBeingPolled] = *GetInput()->GetPollResult();
			m_polling = false;

			SetValues();
		}
	}
}

void
prefGame::ResetValues()
{
	vsSystemPreferences *p = vsSystem::Instance()->GetPreferences();

	m_fullscreen = p->GetFullscreen();
	m_resolution = p->GetResolutionId();
	m_resolutionCount = p->GetSupportedResolutionCount();
	m_bloom = p->GetBloom();

	m_musicVolume = p->GetMusicVolume();
	m_effectVolume = p->GetEffectVolume();

	for ( int i = 0; i < m_controlCount; i++ )
	{
		// m_deviceControl[i] = *GetInput()->GetControlMapping( s_cid[i] );
	}

	SetValues();
}

vsString
prefGame::MakeVolumeString(int value)
{
	vsString result = "";
	if ( value > 0 )
		result += "<";
	else
		result += " ";

	result += vsFormatString("%d",value);

	if ( value < 10 )
		result += ">";

	return result;
}

vsString
prefGame::MakeControlString(int control)
{
	vsString result = vsEmptyString;

	// DeviceControl *dc = &m_deviceControl[control];
    //
	// if ( dc->type == CT_None )
	// 	result = "Unmapped";
	// else
	// {
	// 	vsString dirStr[CD_MAX+1] =
	// 	{
	// 		"+", //CD_Positive,
	// 		"-", //CD_Negative,
    //
	// 		"[Up]",		//CD_Hat_Up,
	// 		"[Right]",	//CD_Hat_Right,
	// 		"[Down]",	//CD_Hat_Down,
	// 		"[Left]",	//CD_Hat_Left,
	// 	};
    //
	// 	if (dc->type == CT_Axis )
	// 		result = vsFormatString("Axis %d %s", dc->cid, dirStr[dc->dir].c_str());
	// 	else if ( dc->type == CT_Button )
	// 		result = vsFormatString("Button %d", dc->cid);
	// 	else if ( dc->type == CT_Hat )
	// 		result = vsFormatString("Hat %d %s", dc->cid, dirStr[dc->dir].c_str());
	// 	else
	// 		result = "ERROR:  Unknown control type!";
	// }

	return result;
}

void
prefGame::SetValues()
{
	// vsSystemPreferences *p = vsSystem::Instance()->GetPreferences();
    //
	// vsString resolutionValue;
	// Resolution * resArray = p->GetSupportedResolutions();
    //
	// resolutionValue = vsFormatString("%dx%d", resArray[m_resolution].width, resArray[m_resolution].height);
	// if ( m_resolution > 0 )
	// 	resolutionValue = "<" + resolutionValue;
	// else
	// 	resolutionValue = " " + resolutionValue;
	// if ( m_resolution < m_resolutionCount-1 )
	// 	resolutionValue += ">";
    //
	// m_screenMenu->SetItemValue(Screen_Resolution, resolutionValue);
    //
    //
	// vsString fullscreenValue = m_fullscreen?"<ON>":"<OFF>";
	// m_screenMenu->SetItemValue(Screen_Fullscreen, fullscreenValue);
    //
    //
	// vsString bloomValue = m_bloom?"<ON>":"<OFF>";
	// m_screenMenu->SetItemValue(Screen_Bloom, bloomValue);
    //
    //
	// m_soundMenu->SetItemValue(Sound_EffectVolume, MakeVolumeString(m_effectVolume));
	// m_soundMenu->SetItemValue(Sound_MusicVolume, MakeVolumeString(m_musicVolume));
    //
    //
	// for ( int i = 0; i < m_controlCount; i++ )
	// 	m_controlMenu->SetItemValue(i, MakeControlString(i));
}

void
prefGame::UpdateValues()
{
	vsSystemPreferences *p = vsSystem::Instance()->GetPreferences();

	p->SetResolutionId(m_resolution);
	p->SetFullscreen(m_fullscreen);
	p->SetBloom(m_bloom);

	p->SetMusicVolume(m_musicVolume);
	p->SetEffectVolume(m_effectVolume);

	p->Save();

	// for ( int i = 0; i < m_controlCount; i++ )
	// 	GetInput()->SetControlMapping( s_cid[i], &m_deviceControl[i] );
}


