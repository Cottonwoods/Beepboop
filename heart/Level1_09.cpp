#include "Level.h"


Level1_09::Level1_09( float r, int entrance, Alonebot* p, FMOD::System* fsys, FMOD::Channel* sfx ) :
Level( r, entrance, p, fsys, sfx ) {
	// Inititalize area and size of level
	area = 1;
	levelWidth = 640;
	levelHeight = 480;

	// Initialize backgrounds and music
	bgMusic = "..\\music\\beat171-restless.mp3";
	background = LoadTexture( std::string( "..\\anims\\bg\\bg_07.png" ) );
	backbackground = LoadTexture( std::string( "..\\anims\\bg\\bbg_04.png" ) );
	
	// Create player, placement depending upon the entrance taken
	switch( entrance ) {
	  case 0:
		player = new Alonebot( p->pos.x, p->h / -2.f, p->dir, p ); break;
	  case 1:
		player = new Alonebot( 20.f, 316.f, p->dir, p ); break;
	  case 2:
		player = new Alonebot( p->pos.x - 384.f, levelHeight - p->h / 2.f, p->dir, p ); break;
	}

	// Add static level objects to array
	objects.push_back( new Box( 0.f, 464.f, 128 ) );						// Ground floor
	objects.push_back( new Box( 256.f, 464.f, levelWidth - 256 ) );
	objects.push_back( new Box( 0.f, 0.f, levelWidth - 224 ) );				// Ceiling
	objects.push_back( new Box( 0.f, 0.f, 16, 400 ) );						// Walls
	objects.push_back( new Box( levelWidth - 16.f, 0.f, 16, 480 ) );
	for( int i=2; i<12; i++ ) {												// Staircase
		objects.push_back( new Box( levelWidth - 16.f - 24.f * i, 48.f * i - 32.f, 24 * i, 48 ) );
	}

	// Add doors
	doors.push_back( new Door( 0.f, 400.f, 1, false, 0, 0 ) );				// Left door
	doorToppers.push_back( new Door( 0.f, 400.f, 1, true ) );
	objects.push_back( new Box( 0.f, 400.f, 16, 4 ) );

	// Add enemies
	for( int i=0; i<3; i++ ) {												// Shooters
		enemies.push_back( new Shooter( Vector2D( 16.f, 176.f + 96.f * i ), 1, 50 + 400 * i ) );
	}
	enemies.push_back( new Skeleton( Vector2D( 56.f, 422.f ) ) );
	enemies.push_back( new Skeleton( Vector2D( 296.f, 422.f ) ) );

	// Start the heart
	player->init( fmodSystem );
	heartBeat.start( );
	
	// Try to center the camera
	cameraX = player->pos.x - 320;
	cameraY = player->pos.y - 240;
}

Level1_09::~Level1_09( ) {
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

void Level1_09::process_events( float dt ) {
	logic( dt );

	collision( );
	
	maintenence( dt );

	if( player->pos.y + player->h / 2 > levelHeight ) {
		levelSwitch = true;
		levelSwitchTo = 16;
		levelSwentrance = 0;
	}
	else if( player->pos.y + player->h / 2 < 0.f ) {
		levelSwitch = true;
		levelSwitchTo = 9;
		levelSwentrance = 2;
	}
}