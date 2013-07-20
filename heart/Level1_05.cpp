#include "Level.h"


Level1_05::Level1_05( float r, int entrance, Alonebot* p, FMOD::System* fsys, FMOD::Channel* sfx ) :
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
		player = new Alonebot( p->pos.x - 178.f, levelHeight - p->h / 2.f, p->dir, p ); break;
	}

	// Add static level objects to array
	objects.push_back( new Box( 0.f, 0.f, 16, 112 ) );						// Walls
	objects.push_back( new Box( 0.f, 176.f, 16, levelHeight - 176 ) );
	objects.push_back( new Box( levelWidth - 16.f, 0.f, 16, levelHeight ) );
	objects.push_back( new Box( 0.f, levelHeight - 4.f, 302 ) );			// Ground floors
	objects.push_back( new Dropbox( 302.f, levelHeight - 4.f, 212 ) );
	objects.push_back( new Box( 514.f, levelHeight - 4.f, levelWidth - 514 ) );
	objects.push_back( new Box( 0.f, 0.f, levelWidth ) );					// Ceiling
	for( int i=0; i<4; i++ ) {												// Left floors
		objects.push_back( new Box( 0.f, 176.f + 200.f * i, 256 ) );
		objects.push_back( new Invisbox( 256.f, 160.f + 200.f * i ) );
	}
	for( int i=0; i<4; i++ ) {												// Right floors
		objects.push_back( new Box( levelWidth - 256.f, 266.f + 200.f * i, 256 ) );
		objects.push_back( new Invisbox( levelWidth - 256.f - 16.f, 250.f + 200.f * i ) );
	}
	for( int i=0; i<8; i++ ) {												// Middle platforms
		objects.push_back( new Dropbox( 335.f, 216.f + 100.f * i, 140 ) );
	}

	// Add doors
	doors.push_back( new Door( 0.f, 112.f, 1, false, 10, 1 ) );				// Top left door
	doorToppers.push_back( new Door( 0.f, 112.f, 1, true ) );
	objects.push_back( new Box( 0.f, 112.f, 16, 4 ) );

	// Add enemies
	for( int i=0; i<3; i++ ) {												// Left skeles
		enemies.push_back( new Skeleton( Vector2D( 58.f, 334.f + 200 * i ), 1 ) );
	}
	for( int i=0; i<3; i++ ) {												// And right
		enemies.push_back( new Skeleton( Vector2D( levelWidth - 88.f, 224.f + 200 * i ), -1 ) );
	}

	// Start the heart
	player->init( fmodSystem );
	heartBeat.start( );
	
	// Try to center the camera
	cameraX = player->pos.x - 320;
	cameraY = player->pos.y - 240;
}

Level1_05::~Level1_05( ) {
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

void Level1_05::process_events( float dt ) {
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

	if( player->pos.y + player->h / 2 > levelHeight ) {
		levelSwitch = true;
		levelSwitchTo = 7;
		levelSwentrance = 4;
	}
}