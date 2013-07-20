#include "Level.h"


Level1_07::Level1_07( float r, int entrance, Alonebot* p, FMOD::System* fsys, FMOD::Channel* sfx ) :
Level( r, entrance, p, fsys, sfx ) {
	// Inititalize area and size of level
	area = 1;
	levelWidth = 810;
	levelHeight = 960;

	medusaeTime = 0;

	// Initialize backgrounds and music
	bgMusic = "..\\music\\beat171-restless.mp3";
	background = LoadTexture( std::string( "..\\anims\\bg\\bg_05.png" ) );
	backbackground = LoadTexture( std::string( "..\\anims\\bg\\bbg_03.png" ) );
	
	// Create player, placement depending upon the entrance taken
	switch( entrance ) {
	  case 0:
		player = new Alonebot( 20.f, 116.f, p->dir, p ); break;
	  case 1:
		player = new Alonebot( levelWidth - 38.f, 116.f, p->dir, p ); break;
	}

	// Add static level objects to array
	objects.push_back( new Box( 0.f, 0.f, 16, 112 ) );						// Walls
	objects.push_back( new Box( 0.f, 176.f, 16, levelHeight - 176 ) );
	objects.push_back( new Box( levelWidth - 16.f, 0.f, 16, 112 ) );
	objects.push_back( new Box( levelWidth - 16.f, 176.f, 16, levelHeight - 176 ) );
	objects.push_back( new Box( 0.f, levelHeight - 16.f, levelWidth ) );	// Ground floors
	objects.push_back( new Box( 0.f, 0.f, levelWidth ) );					// Ceiling
	objects.push_back( new Box( 0.f, 176.f, 128 ) );						// Top floors
	objects.push_back( new Box( levelWidth - 128.f, 176.f, 128 ) );
	for( int i=0; i<7; i++ ) {												// Inner floors
		objects.push_back( new Dropbox( 204.f, 220.f + 110.f * i, 128 ) );
		if( i != 5 && i != 6 )
			objects.push_back( new Dropbox( levelWidth - 332.f, 220.f + 110.f * i, 128 ) );
	}
	for( int i=0; i<6; i++ ) {												// Outer floors
		objects.push_back( new Dropbox( 44.f, 275.f + 110.f * i, 128 ) );
		if( i != 6 && i != 5 && i != 4 )
			objects.push_back( new Dropbox( levelWidth - 172.f, 275.f + 110.f * i, 128 ) );
	}

	// Add doors
	doors.push_back( new Door( 0.f, 112.f, 1, false, 11, 0 ) );				// Top left door
	doorToppers.push_back( new Door( 0.f, 112.f, 1, true ) );
	objects.push_back( new Box( 0.f, 112.f, 16, 4 ) );

	doors.push_back( new Door( levelWidth - 32.f, 112.f, -1, false, 8, 0 ) );// Top right door
	doorToppers.push_back( new Door(levelWidth - 32.f, 112.f, -1, true ) );
	objects.push_back( new Box( levelWidth - 16.f, 112.f, 16, 4 ) );

	// Add enemies
	for( int i=0; i<2; i++ ) {												// Runners
		enemies.push_back( new Runner( Vector2D( 220.f + 200 * i, levelHeight - 40.f ), 1 ) );
	}
	enemies.push_back( new Runner( Vector2D( levelWidth - 56.f, levelHeight - 40.f ), -1 ) );
	for( int i=0; i<10; i++ ) {												// Sawblades
		enemies.push_back( new Sawblade( 100 + 25 * i, Vector2D( 400, 16.f + 52.f * i ), 1, true ) );
		enemies.push_back( new Sawblade( 100 + 25 * i, Vector2D( 400, 840.f - 52.f * i ), -1, true ) );
	}

	// Start the heart
	player->init( fmodSystem );
	heartBeat.start( );
	
	// Try to center the camera
	cameraX = player->pos.x - 320;
	cameraY = player->pos.y - 240;
}

Level1_07::~Level1_07( ) {
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

void Level1_07::process_events( float dt ) {
	// Add enemies
	if( medusaeTime < heartBeat.get_ticks( ) ) {							// Medusae
		int r = rand( ) % ( MAX_MEDUSAE_DELAY - MIN_MEDUSAE_DELAY );
		medusaeTime = heartBeat.get_ticks( ) + MIN_MEDUSAE_DELAY + r;
		float yy = rand( ) % ( levelHeight - 320 ) + 220.f;
		r = rand( ) % 2;
		if( r == 1 )
			enemies.push_back( new Medusa( Vector2D( -27.f, yy ), 1 ) );
		else
			enemies.push_back( new Medusa( Vector2D( levelWidth - 1.f, yy ), -1 ) );
	}

	logic( dt );

	collision( );

	maintenence( dt );
}