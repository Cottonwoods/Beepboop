#include "Level.h"


Level1_03::Level1_03( float r, int entrance, Alonebot* p, FMOD::System* fsys, FMOD::Channel* sfx ) :
Level( r, entrance, p, fsys, sfx ) {
	// Inititalize area and size of level
	area = 1;
	levelWidth = 1152;
	levelHeight = 480;

	// Initialize backgrounds and music
	bgMusic = "..\\music\\beat171-restless.mp3";
	background = LoadTexture( std::string( "..\\anims\\bg\\bg_03.png" ) );
	backbackground = LoadTexture( std::string( "..\\anims\\bg\\bbg_03.png" ) );
	
	// Create player, placement depending upon the entrance taken
	switch( entrance ) {
	  case 0:
		player = new Alonebot( 20.f, 260.f, p->dir, p ); break;
	  case 1:
		player = new Alonebot( levelWidth - 38.f, 316.f, p->dir, p ); break;
	  case 2:
		player = new Alonebot( 20.f, 116.f, p->dir, p ); break;
	  case 3:
		player = new Alonebot( levelWidth - 38.f, 116.f, p->dir, p ); break;
	}

	// Add static level objects to array
	objects.push_back( new Box( 0.f, 376.f, levelWidth ) );					// Ground floor
	objects.push_back( new Box( 0.f, 0.f, levelWidth ) );					// Ceiling
	for( int i=6; i<10; i++ ) {												// Left staircase
		objects.push_back( new Box( 0.f, 224.f + 16.f * i, 24 * i ) );
	}
	for( int i=0; i<4; i++ ) {												// Middle staircase
		objects.push_back( new Box( 535.f - 24 * i, 320.f + 16.f * i, 48 * ( i + 2 ) ) );
	}
	objects.push_back( new Box( 0.f, 176.f, levelWidth / 3 + 19 ) );
	objects.push_back( new Box( levelWidth * 2.f / 3.f, 176.f, levelWidth / 3 ) );
	objects.push_back( new Dropbox( 418.f, 204.f, 64 ) );
	objects.push_back( new Dropbox( 520.f, 256.f, 128 ) );
	objects.push_back( new Dropbox( 689.f, 204.f, 64 ) );
	objects.push_back( new Box( 0.f, 0.f, 16, 112 ) );						// Walls
	objects.push_back( new Box( 0.f, 176.f, 16, 80 ) );
	objects.push_back( new Box( levelWidth - 16.f, 0.f, 16, 112 ) );
	objects.push_back( new Box( levelWidth - 16.f, 176.f, 16, 136 ) );

	// Add doors
	doors.push_back( new Door( 0.f, 112.f, 1, false, 5, 0 ) );				// Top left door
	doorToppers.push_back( new Door( 0.f, 112.f, 1, true ) );
	objects.push_back( new Box( 0.f, 112.f, 16, 4 ) );

	doors.push_back( new Door( 0.f, 256.f, 1, false, 4, 1 ) );				// Bottom left door
	doorToppers.push_back( new Door( 0.f, 256.f, 1, true ) );
	objects.push_back( new Box( 0.f, 256.f, 16, 4 ) );
	
	doors.push_back( new Door( levelWidth - 32.f, 112.f, -1, false, 7, 2 ) );// Top right door
	doorToppers.push_back( new Door( levelWidth - 32.f, 112.f, -1, true ) );
	objects.push_back( new Box( levelWidth - 16.f, 112.f, 16, 4 ) );

	doors.push_back( new Door( levelWidth - 32.f, 312.f, -1, false, 7, 0 ) );// Bottom right door
	doorToppers.push_back( new Door( levelWidth - 32.f, 312.f, -1, true ) );
	objects.push_back( new Box( levelWidth - 16.f, 312.f, 16, 4 ) );
	
	// Add enemies
	enemies.push_back( new Skeleton( Vector2D( 333.f, 334.f ), -1 ) );
	enemies.push_back( new Skeleton( Vector2D( 803.f, 334.f ), -1 ) );

	// Start the heart
	player->init( fmodSystem );
	heartBeat.start( );
	
	// Try to center the camera
	cameraX = player->pos.x - 320;
	cameraY = player->pos.y - 240;
}

Level1_03::~Level1_03( ) {
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

void Level1_03::process_events( float dt ) {
	logic( dt );

	collision( );

	maintenence( dt );
}