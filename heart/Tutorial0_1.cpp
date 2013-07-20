#include "Level.h"


Tutorial0_1::Tutorial0_1( float r, int entrance, Alonebot* p, FMOD::System* fsys, FMOD::Channel* sfx ) :
Level( r, entrance, p, fsys, sfx ) {
	// Inititalize area and size of level
	area = 0;
	levelWidth = 1440;
	levelHeight = 480;

	// Initialize backgrounds and music
	bgMusic = "..\\music\\beat496-stag.mp3";
	background = LoadTexture( std::string( "..\\anims\\bg\\bg_01.png" ) );
	backbackground = LoadTexture( std::string( "..\\anims\\bg\\bbg_01.png" ) );
	
	// Create player, placement depending upon the entrance taken
	switch( entrance ) {
	  case 0:
		player = new Alonebot( 20.f, 316.f, 1, p ); break;
	  case 1:
		player = new Alonebot( 20.f, 116.f, 1, p ); break;
	  case 2:
		player = new Alonebot( levelWidth - 38.f, 316.f, -1, p ); break;
	  case 3:
		player = new Alonebot( levelWidth - 38.f, 116.f, -1, p ); break;
	}

	// Add static level objects to array
	objects.push_back( new Box( 0.f, 376.f, levelWidth ) );					// Ground floor
	objects.push_back( new Box( 0.f, 176.f, 92 ) );							// Left door platform
	objects.push_back( new Dropbox( 110.f, 234.f, 48 ) );
	objects.push_back( new Dropbox( 188.f, 292.f, 48 ) );					// Staircase platforms
	objects.push_back( new Box( 270.f, 344.f, 48 ) );
	objects.push_back( new Box( 218.f, 180.f, 52 ) );						// Floating platform at start
	objects.push_back( new Box( 378.f, 156.f, 52 ) );						// Floating platforms leading to edge
	objects.push_back( new Box( 538.f, 132.f, 52 ) );
	objects.push_back( new Box( 698.f, 108.f, 52 ) );						// Highest platform, slightly longer
	objects.push_back( new Box( 874.f, 156.f, 128 ) );						// Two long platforms
	objects.push_back( new Box( 1126.f, 166.f, 128 ) );						//  before the edge
	objects.push_back( new Box( 1368.f, 176.f, 72 ) );						// Edge platform with door
	objects.push_back( new Box( 452.f, 360.f, 50 ) );
	objects.push_back( new Box( 652.f, 360.f, 50 ) );
	objects.push_back( new Box( 852.f, 360.f, 50 ) );
	objects.push_back( new Box( 1052.f, 360.f, 50 ) );

	// Add doors
	doors.push_back( new Door( 0.f, 112.f, 1, false, 0, 2 ) );				// Top left door
	doorToppers.push_back( new Door( 0.f, 112.f, 1, true ) );
	objects.push_back( new Box( 0.f, 112.f, 16, 4 ) );

	doors.push_back( new Door( 0.f, 312.f, 1, false, 0, 1 ) );				// Bottom left door
	doorToppers.push_back( new Door( 0.f, 312.f, 1, true ) );
	objects.push_back( new Box( 0.f, 312.f, 16, 4 ) );
	
	doors.push_back( new Door( levelWidth - 32.f, 112.f, -1, false, 2, 1 ) );// Top right door
	doorToppers.push_back( new Door( levelWidth - 32.f, 112.f, -1, true ) );
	objects.push_back( new Box( levelWidth - 16.f, 112.f, 16, 4 ) );

	doors.push_back( new Door( levelWidth - 32.f, 312.f, -1, false, 2, 0 ) );// Bottom right door
	doorToppers.push_back( new Door( levelWidth - 32.f, 312.f, -1, true ) );
	objects.push_back( new Box( levelWidth - 16.f, 312.f, 16, 4 ) );

	// Add enemies
	enemies.push_back( new Sawblade( 150, Vector2D( 302.f, 360.f ) ) );
	enemies.push_back( new Sawblade( 150, Vector2D( 502.f, 360.f ) ) );
	enemies.push_back( new Sawblade( 150, Vector2D( 702.f, 360.f ) ) );
	enemies.push_back( new Sawblade( 150, Vector2D( 902.f, 360.f ) ) );

	// Start the heart
	player->init( fmodSystem );
	heartBeat.start( );
	
	// Try to center the camera
	cameraX = player->pos.x - 320;
	cameraY = player->pos.y - 240;
}

Tutorial0_1::~Tutorial0_1( ) {
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

void Tutorial0_1::process_events( float dt ) {
	logic( dt );

	collision( );

	maintenence( dt );
}