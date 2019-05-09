/*
 *  Core.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 25/03/07.
 *  Copyright 2007 Trevor Powell. All rights reserved.
 *
 */

#if !TARGET_OS_IPHONE

#include "Memory/VS_Heap.h"

#include "VS/Utils/VS_Preferences.h"
#include "VS/Math/VS_Random.h"
#include "VS/Graphics/VS_Screen.h"
#include "VS/Utils/VS_System.h"

int main( int argc, char* argv[] )
{
	UNUSED(argc);
	UNUSED(argv);

	vsSystem *system = new vsSystem( "VectorStorm", "Testbeds", argc, argv, 32 * 1024 * 1024 );

	/*  Create our game  */

	core::Init( 28 * 1024 * 1024 );
	core::SetGame( coreGameRegistry::GetMainMenu() );
	core::Go();
	core::Deinit();

	delete system;


	return 0;
}

#endif


