#include "Level.h"


Tutorial0_2::Tutorial0_2( float r, int entrance, Alonebot* p, FMOD::System* fsys, FMOD::Channel* sfx ) :
Level( r, entrance, p, fsys, sfx ) {
	// Inititalize area and size of level
	area = 0;
	levelWidth = 1152;
	levelHeight = 480;

	// Initialize backgrounds and music
	bgMusic = "..\\music\\beat496-stag.mp3";
	background = LoadTexture( std::string( "..\\anims\\bg\\bg_02.png" ) );
	backbackground = LoadTexture( std::string( "..\\anims\\bg\\bbg_02.png" ) );
	
	// Create player, placement depending upon the entrance taken
	switch( entrance ) {
	  case 0:
		player = new Alonebot( 20.f, 316.f, p->dir, p ); break;
	  case 1:
		player = new Alonebot( 20.f, 116.f, p->dir, p ); break;
	  case 2:
		player = new Alonebot( levelWidth - 38.f, 260.f, p->dir, p ); break;
	}

	// Add static level objects to array
	objects.push_back( new Box( 0.f, 376.f, levelWidth ) );					// Ground floor
	objects.push_back( new Box( 0.f, 176.f, 132 ) );						// Left door platform
	for( int i=6; i<10; i++ ) {
		objects.push_back( new Box( levelWidth - 24.f * i, 224.f + 16.f * i, 24 * i ) );
	}
	objects.push_back( new Box( levelWidth - 16.f, 0.f, 16, 256 ) );
	
	// Add item(s)
	if( player->items.find( 100 ) == player->items.end( ) )
		items.push_back( new HeartUp( Vector2D( 100.f, 140.f ) ) );			// Heart Up #0

	// Add doors
	doors.push_back( new Door( 0.f, 112.f, 1, false, 1, 3 ) );				// Top left door
	doorToppers.push_back( new Door( 0.f, 112.f, 1, true ) );
	objects.push_back( new Box( 0.f, 112.f, 16, 4 ) );

	doors.push_back( new Door( 0.f, 312.f, 1, false, 1, 2 ) );				// Bottom left door
	doorToppers.push_back( new Door( 0.f, 312.f, 1, true ) );
	objects.push_back( new Box( 0.f, 312.f, 16, 4 ) );
	
	doors.push_back( new Door( levelWidth - 32.f, 256.f, -1, false, 3, 0 ) );// Right door
	doorToppers.push_back( new Door( levelWidth - 32.f, 256.f, -1, true ) );
	objects.push_back( new Box( levelWidth - 16.f, 256.f, 16, 4 ) );

	// Start the heart
	player->init( fmodSystem );
	heartBeat.start( );
	
	// Try to center the camera
	cameraX = player->pos.x - 320;
	cameraY = player->pos.y - 240;
}

Tutorial0_2::~Tutorial0_2( ) {
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

void Tutorial0_2::process_events( float dt ) {
	logic( dt );

	collision( );

	maintenence( dt );
}