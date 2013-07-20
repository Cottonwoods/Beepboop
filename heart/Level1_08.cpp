#include "Level.h"


Level1_08::Level1_08( float r, int entrance, Alonebot* p, FMOD::System* fsys, FMOD::Channel* sfx ) :
Level( r, entrance, p, fsys, sfx ) {
	// Inititalize area and size of level
	area = 1;
	levelWidth = 810;
	levelHeight = 720;

	medusaeTime = 0;

	// Initialize backgrounds and music
	bgMusic = "..\\music\\beat171-restless.mp3";
	background = LoadTexture( std::string( "..\\anims\\bg\\bg_05.png" ) );
	backbackground = LoadTexture( std::string( "..\\anims\\bg\\bbg_03.png" ) );
	
	// Create player, placement depending upon the entrance taken
	switch( entrance ) {
	  case 0:
		player = new Alonebot( levelWidth - 38.f, 116.f, p->dir, p ); break;
	  case 1:
		player = new Alonebot( p->pos.x - 171.f, levelHeight - p->h / 2.f, p->dir, p ); break;
	  case 2:
		player = new Alonebot( p->pos.x, p->h / -2.f, p->dir, p ); break;
	}

	// Add static level objects to array
	objects.push_back( new Box( 0.f, 0.f, 16, levelHeight ) );				// Walls
	objects.push_back( new Box( levelWidth - 16.f, 0.f, 16, 112 ) );
	objects.push_back( new Box( levelWidth - 16.f, 176.f, 16, levelHeight - 176 ) );
	objects.push_back( new Box( 0.f, levelHeight - 16.f, ( levelWidth - 180 ) / 2 ) ); // Ground floor
	objects.push_back( new Box( ( levelWidth + 180 ) / 2.f, levelHeight - 16.f, ( levelWidth - 180 ) / 2 ) );
	objects.push_back( new Box( 0.f, 0.f, ( levelWidth - 180 ) / 2 ) );		// Ceiling
	objects.push_back( new Box( ( levelWidth + 180 ) / 2.f, 0.f, ( levelWidth - 180 ) / 2 ) );
	objects.push_back( new Box( 0.f, 144.f, 264 ) );						// Top floors
	objects.push_back( new Box( levelWidth - 256.f, 176.f, 256 ) );
	for( int i=0; i<6; i++ ) {												// Floating platforms
		if( i < 3 )
			objects.push_back( new Dropbox( 341.f, 44.f + 66.f * i, 128 ) );
		else
			objects.push_back( new Dropbox( 341.f, 308.f + 132.f * ( i - 3 ), 128 ) );

		if( i < 4 )
			objects.push_back( new Dropbox( 181.f, 242.f + 132.f * i, 128 ) );
	}

	// Add doors
	doors.push_back( new Door( levelWidth - 32.f, 112.f, -1, false, 10, 0 ) );// Top right door
	doorToppers.push_back( new Door(levelWidth - 32.f, 112.f, -1, true ) );
	objects.push_back( new Box( levelWidth - 16.f, 112.f, 16, 4 ) );

	// Start the heart
	player->init( fmodSystem );
	heartBeat.start( );
	
	// Try to center the camera
	cameraX = player->pos.x - 320;
	cameraY = player->pos.y - 240;
}

Level1_08::~Level1_08( ) {
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

void Level1_08::process_events( float dt ) {
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
	
	if( player->pos.y + player->h / 2 < 0.f ) {
		levelSwitch = true;
		levelSwitchTo = 0;
		levelSwentrance = 0;
	}
	else if( player->pos.y + player->h / 2 > levelHeight ) {
		levelSwitch = true;
		levelSwitchTo = 4;
		levelSwentrance = 2;
	}
}