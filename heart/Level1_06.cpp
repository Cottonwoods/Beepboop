#include "Level.h"


Level1_06::Level1_06( float r, int entrance, Alonebot* p, FMOD::System* fsys, FMOD::Channel* sfx ) :
Level( r, entrance, p, fsys, sfx ) {
	// Inititalize area and size of level
	area = 1;
	levelWidth = 640;
	levelHeight = 480;

	// Initialize backgrounds and music
	bgMusic = "..\\music\\beat171-restless.mp3";
	background = LoadTexture( std::string( "..\\anims\\bg\\bg_06.png" ) );
	backbackground = LoadTexture( std::string( "..\\anims\\bg\\bbg_03.png" ) );
	
	// Create player, placement depending upon the entrance taken
	switch( entrance ) {
	  case 0:
		player = new Alonebot( 20.f, 316.f, p->dir, p ); break;
	  case 1:
		player = new Alonebot( 20.f, 116.f, p->dir, p ); break;
	  case 2:
		player = new Alonebot( p->pos.x, levelHeight - p->h / 2.f, p->dir, p ); break;
	  case 3:
		player = new Alonebot( p->pos.x, p->h / -2.f, p->dir, p ); break;
	}

	// Add static level objects to array
	objects.push_back( new Box( 0.f, 176.f, levelWidth ) );					// Ground floor
	objects.push_back( new Box( 0.f, 376.f, 316 ) );
	objects.push_back( new Box( 0.f, 0.f, 400 ) );							// Ceiling
	objects.push_back( new Box( levelWidth - 16.f, 0.f, 16, levelHeight ) );// Right edge
	for( int i=2; i<10; i++ ) {												// Top staircase
		objects.push_back( new Box( 648 - 24.f * i, 16.f + 16.f * i, 24 * i ) );
	}
	for( int i=2; i<10; i++ ) {												// Bottom staircase
		objects.push_back( new Box( 648 - 24.f * i, 336.f - 16.f * i, 24 * i ) );
	}
	for( int i=2; i<8; i++ ) {
		objects.push_back( new Box( 256 + 16.f * i, 360.f + 16.f * i, 48 ) );
	}
	objects.push_back( new Box( 0.f, 0.f, 16, 112 ) );						// Walls
	objects.push_back( new Box( 0.f, 176.f, 16, 136 ) );

	// Add doors
	doors.push_back( new Door( 0.f, 112.f, 1, false, 7, 3 ) );				// Top left door
	doorToppers.push_back( new Door( 0.f, 112.f, 1, true ) );
	objects.push_back( new Box( 0.f, 112.f, 16, 4 ) );

	doors.push_back( new Door( 0.f, 312.f, 1, false, 7, 1 ) );				// Bottom left door
	doorToppers.push_back( new Door( 0.f, 312.f, 1, true ) );
	objects.push_back( new Box( 0.f, 312.f, 16, 4 ) );

	// Start the heart
	player->init( fmodSystem );
	heartBeat.start( );
	
	// Try to center the camera
	cameraX = player->pos.x - 320;
	cameraY = player->pos.y - 240;
}

Level1_06::~Level1_06( ) {
	typedef unsigned short sho;
	
	sfxChannel->stop( );
	delete player;
	for( sho i=0; i<NPCs.size( ); i++ )
		delete NPCs[i];
	for( sho i=0; i<items.size( ); i++ )
		delete items[i];
	for( sho i=0; i<doors.size( ); i++ )
		delete doors[i];
	for( sho i=0; i<doorToppers.size( ); i++ )
		delete doorToppers[i];
	for( sho i=0; i<objects.size( ); i++ )
		delete objects[i];
	for( sho i=0; i<enemies.size( ); i++ )
		delete enemies[i];
	for( sho i=0; i<bullets.size( ); i++ )
		delete bullets[i];
	glDeleteTextures( 1, &background );
	glDeleteTextures( 1, &backbackground );
}

void Level1_06::process_events( float dt ) {
	logic( dt );

	collision( );
	
	maintenence( dt );

	if( player->pos.y + player->h / 2 > levelHeight ) {
		levelSwitch = true;
		levelSwitchTo = 12;
		levelSwentrance = 0;
	}
	else if( player->pos.y + player->h / 2 < 0.f ) {
		levelSwitch = true;
		levelSwitchTo = 13;
		levelSwentrance = 0;
	}
}