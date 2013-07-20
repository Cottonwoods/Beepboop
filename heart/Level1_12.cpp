#include "Level.h"


Level1_12::Level1_12( float r, int entrance, Alonebot* p, FMOD::System* fsys, FMOD::Channel* sfx ) :
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
		player = new Alonebot( 384 + p->pos.x, p->h / -2.f, p->dir, p ); break;
	  case 1:
		player = new Alonebot( 20.f, 316.f, p->dir, p ); break;
	  case 2:
		player = new Alonebot( 20.f, 316.f, p->dir, p ); break;
	}

	// Add static level objects to array
	objects.push_back( new Box( 0.f, 376.f, levelWidth ) );					// Ground floor
	objects.push_back( new Box( 0.f, 0.f, 512 ) );							// Ceiling
	objects.push_back( new Box( 640.f, 0.f, 512 ) );
	for( int i=0; i<6; i++ ) {												// Staircase
		objects.push_back( new Box( 492.f - 40 * i, 280.f + 16.f * i, 80 * ( i + 2 ) ) );
	}
	objects.push_back( new Dropbox( 512.f, 80.f, 128 ) );					// Floating platforms
	objects.push_back( new Dropbox( 512.f, 180.f, 128 ) );
	objects.push_back( new Box( 0.f, 0.f, 16, 312 ) );						// Walls
	objects.push_back( new Box( levelWidth - 16.f, 0.f, 16, 312 ) );

	// Add doors
	doors.push_back( new Door( 0.f, 312.f, 1, false, 0, 0 ) );				// Left door
	doorToppers.push_back( new Door( 0.f, 312.f, 1, true ) );
	objects.push_back( new Box( 0.f, 312.f, 16, 4 ) );

	doors.push_back( new Door( levelWidth - 32.f, 312.f, -1, false, 0, 0 ) );// Right door
	doorToppers.push_back( new Door( levelWidth - 32.f, 312.f, -1, true ) );
	objects.push_back( new Box( levelWidth - 16.f, 312.f, 16, 4 ) );
	
	// Add enemies
	for( int i=0; i<2; i++ ) {												// Shooters
		enemies.push_back( new Shooter( Vector2D( 388.f + 348.f * i, 16.f ), 1, 50, true ) );
	}
	enemies.push_back( new Skeleton( Vector2D( 562.f, 238.f ) ) );
	enemies.push_back( new Runner( Vector2D( 100.f, 352.f ), 1 ) );			// Runners
	enemies.push_back( new Runner( Vector2D( levelWidth - 148.f, 352.f ), -1 ) );

	// Start the heart
	player->init( fmodSystem );
	heartBeat.start( );
	
	// Try to center the camera
	cameraX = player->pos.x - 320;
	cameraY = player->pos.y - 240;
}

Level1_12::~Level1_12( ) {
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

void Level1_12::process_events( float dt ) {
	logic( dt );

	collision( );

	maintenence( dt );

	if( player->pos.y + player->h / 2 < 0.f ) {
		levelSwitch = true;
		levelSwitchTo = 12;
		levelSwentrance = 2;
	}
}