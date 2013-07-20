#include "Level.h"


Level1_01::Level1_01( float r, int entrance, Alonebot* p, FMOD::System* fsys, FMOD::Channel* sfx ) :
Level( r, entrance, p, fsys, sfx ) {
	// Inititalize area and size of level
	area = 1;
	levelWidth = 1152;
	levelHeight = 480;

	zombieTime = 0;

	// Initialize backgrounds and music
	bgMusic = "..\\music\\beat171-restless.mp3";
	background = LoadTexture( std::string( "..\\anims\\bg\\bg_03.png" ) );
	backbackground = LoadTexture( std::string( "..\\anims\\bg\\bbg_03.png" ) );
	
	// Create player, placement depending upon the entrance taken
	switch( entrance ) {
	  case 0:
		player = new Alonebot( 20.f, 260.f, p->dir, p ); break;
	  case 1:
		player = new Alonebot( levelWidth - 38.f, 260.f, p->dir, p ); break;
	}

	// Add static level objects to array
	objects.push_back( new Box( 0.f, 376.f, levelWidth ) );					// Ground floor
	objects.push_back( new Box( 0.f, 0.f, levelWidth ) );					// Ceiling
	for( int i=6; i<10; i++ ) {												// Left staircase
		objects.push_back( new Box( 0.f, 224.f + 16.f * i, 24 * i ) );
	}
	for( int i=6; i<10; i++ ) {												// Right staircase
		objects.push_back( new Box( levelWidth - 24.f * i, 224.f + 16.f * i, 24 * i ) );
	}
	objects.push_back( new Box( 0.f, 0.f, 16, 256 ) );						// Walls
	objects.push_back( new Box( levelWidth - 16.f, 0.f, 16, 256 ) );

	// Add doors
	doors.push_back( new Door( 0.f, 256.f, 1, false, 2, 2 ) );				// Left door
	doorToppers.push_back( new Door( 0.f, 256.f, 1, true ) );
	objects.push_back( new Box( 0.f, 256.f, 16, 4 ) );

	doors.push_back( new Door( levelWidth - 32.f, 256.f, -1, false, 4, 0 ) );// Right door
	doorToppers.push_back( new Door( levelWidth - 32.f, 256.f, -1, true ) );
	objects.push_back( new Box( levelWidth - 16.f, 256.f, 16, 4 ) );

	// Start the heart
	player->init( fmodSystem );
	heartBeat.start( );

	// Try to center the camera
	cameraX = player->pos.x - 320;
	cameraY = player->pos.y - 240;
}

Level1_01::~Level1_01( ) {
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

void Level1_01::process_events( float dt ) {
	int t = heartBeat.get_ticks( );
	if( zombieTime < t ) {
		zombieTime = t + MIN_ZOMBIE_DELAY + ( rand( ) % ( MAX_ZOMBIE_DELAY - MIN_ZOMBIE_DELAY ) );
		int x = rand( ) % 300;
		enemies.push_back( new Zombie( 350 + x, Vector2D( 590.f + x, 328.f ), -1 ) );
	}

	logic( dt );

	collision( );

	maintenence( dt );
}