#include "Level.h"


Level1_11::Level1_11( float r, int entrance, Alonebot* p, FMOD::System* fsys, FMOD::Channel* sfx ) :
Level( r, entrance, p, fsys, sfx ) {
	// Inititalize area and size of level
	area = 1;
	levelWidth = 1152;
	levelHeight = 480;

	// Initialize backgrounds and music
	bgMusic = "..\\music\\beat171-restless.mp3";
	background = LoadTexture( std::string( "..\\anims\\bg\\bg_08.png" ) );
	backbackground = LoadTexture( std::string( "..\\anims\\bg\\bbg_03.png" ) );
	
	// Create player, placement depending upon the entrance taken
	switch( entrance ) {
	  case 0:
		player = new Alonebot( 20.f, 404.f, p->dir, p ); break;
	  case 1:
		player = new Alonebot( levelWidth - 38.f, 404.f, p->dir, p ); break;
	  case 2:
		player = new Alonebot( 20.f, 116.f, p->dir, p ); break;
	  case 3:
		player = new Alonebot( levelWidth - 38.f, 116.f, p->dir, p ); break;
	}

	// Add static level objects to array
	objects.push_back( new Box( 0.f, 464.f, levelWidth ) );					// Ground floor
	objects.push_back( new Box( 0.f, 176.f, levelWidth ) );					// Mid floor
	objects.push_back( new Box( 0.f, 0.f, 480 ) );							// Ceiling
	objects.push_back( new Box( 692.f, 0.f, levelWidth - 692 ) );
	objects.push_back( new Box( 0.f, 176.f, levelWidth / 3 + 60 ) );		// Top floors
	objects.push_back( new Box( levelWidth * 2.f / 3.f - 42, 176.f, levelWidth / 3 + 42 ) );
	objects.push_back( new Dropbox( 504.f, 90.f, 164 ) );					// Platform leading up
	objects.push_back( new Box( 0.f, 0.f, 16, 112 ) );						// Walls
	objects.push_back( new Box( 0.f, 176.f, 16, 224 ) );
	objects.push_back( new Box( levelWidth - 16.f, 0.f, 16, 112 ) );
	objects.push_back( new Box( levelWidth - 16.f, 176.f, 16, 224 ) );

	// Add doors
	doors.push_back( new Door( 0.f, 112.f, 1, false, 13, 3 ) );				// Top left door
	doorToppers.push_back( new Door( 0.f, 112.f, 1, true ) );
	objects.push_back( new Box( 0.f, 112.f, 16, 4 ) );

	doors.push_back( new Door( 0.f, 400.f, 1, false, 13, 1 ) );				// Bottom left door
	doorToppers.push_back( new Door( 0.f, 400.f, 1, true ) );
	objects.push_back( new Box( 0.f, 400.f, 16, 4 ) );
	
	doors.push_back( new Door( levelWidth - 32.f, 112.f, -1, false, 0, 0 ) );// Top right door
	doorToppers.push_back( new Door( levelWidth - 32.f, 112.f, -1, true ) );
	objects.push_back( new Box( levelWidth - 16.f, 112.f, 16, 4 ) );

	doors.push_back( new Door( levelWidth - 32.f, 400.f, -1, false, 15, 0 ) );// Bottom right door
	doorToppers.push_back( new Door( levelWidth - 32.f, 400.f, -1, true ) );
	objects.push_back( new Box( levelWidth - 16.f, 400.f, 16, 4 ) );

	// Start the heart
	player->init( fmodSystem );
	heartBeat.start( );
	
	// Try to center the camera
	cameraX = player->pos.x - 320;
	cameraY = player->pos.y - 240;
}

Level1_11::~Level1_11( ) {
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

void Level1_11::process_events( float dt ) {
	logic( dt );

	collision( );
	
	maintenence( dt );

	if( player->pos.y + player->h / 2 < 0.f ) {
		levelSwitch = true;
		levelSwitchTo = 8;
		levelSwentrance = 1;
	}
}