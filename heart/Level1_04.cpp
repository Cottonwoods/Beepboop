#include "Level.h"


Level1_04::Level1_04( float r, int entrance, Alonebot* p, FMOD::System* fsys, FMOD::Channel* sfx ) :
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
		player = new Alonebot( 20.f, 316.f, p->dir, p ); break;
	  case 1:
		player = new Alonebot( levelWidth - 38.f, 316.f, p->dir, p ); break;
	  case 2:
		player = new Alonebot( 20.f, 116.f, p->dir, p ); break;
	  case 3:
		player = new Alonebot( levelWidth - 38.f, 116.f, p->dir, p ); break;
	  case 4:
		player = new Alonebot( 178.f + p->pos.x, p->h / -2.f, p->dir, p ); break;
	}

	// Add static level objects to array
	objects.push_back( new Box( 0.f, 376.f, levelWidth ) );					// Ground floor
	objects.push_back( new Box( 0.f, 0.f, 480 ) );							// Ceiling
	objects.push_back( new Box( 692.f, 0.f, levelWidth - 692 ) );
	for( int i=4; i<7; i++ ) {												// Left staircase
		objects.push_back( new Box( 444.f - 24 * i, 64.f + 16.f * i, 24 * i ) );
	}
	for( int i=4; i<7; i++ ) {												// Left staircase
		objects.push_back( new Box( 726.f, 64.f + 16.f * i, 24 * i ) );
	}
	objects.push_back( new Box( 0.f, 176.f, levelWidth / 3 + 60 ) );		// Top floors
	objects.push_back( new Box( levelWidth * 2.f / 3.f - 42, 176.f, levelWidth / 3 + 42 ) );
	objects.push_back( new Dropbox( 504.f, 100.f, 164 ) );					// Platforms leading up
	objects.push_back( new Dropbox( 504.f, 36.f, 164 ) );
	objects.push_back( new Box( 212.f, 360.f, 50 ) );						// Short stops for sawblades first
	objects.push_back( new Box( 462.f, 360.f, 50 ) );
	objects.push_back( new Box( 656.f, 360.f, 50 ) );						// Second
	objects.push_back( new Box( 906.f, 360.f, 50 ) );
	objects.push_back( new Box( 0.f, 0.f, 16, 112 ) );						// Walls
	objects.push_back( new Box( 0.f, 176.f, 16, 136 ) );
	objects.push_back( new Box( levelWidth - 16.f, 0.f, 16, 112 ) );
	objects.push_back( new Box( levelWidth - 16.f, 176.f, 16, 136 ) );

	// Add doors
	doors.push_back( new Door( 0.f, 112.f, 1, false, 6, 3 ) );				// Top left door
	doorToppers.push_back( new Door( 0.f, 112.f, 1, true ) );
	objects.push_back( new Box( 0.f, 112.f, 16, 4 ) );

	doors.push_back( new Door( 0.f, 312.f, 1, false, 6, 1 ) );				// Bottom left door
	doorToppers.push_back( new Door( 0.f, 312.f, 1, true ) );
	objects.push_back( new Box( 0.f, 312.f, 16, 4 ) );
	
	doors.push_back( new Door( levelWidth - 32.f, 112.f, -1, false, 9, 1 ) );// Top right door
	doorToppers.push_back( new Door( levelWidth - 32.f, 112.f, -1, true ) );
	objects.push_back( new Box( levelWidth - 16.f, 112.f, 16, 4 ) );

	doors.push_back( new Door( levelWidth - 32.f, 312.f, -1, false, 9, 0 ) );// Bottom right door
	doorToppers.push_back( new Door( levelWidth - 32.f, 312.f, -1, true ) );
	objects.push_back( new Box( levelWidth - 16.f, 312.f, 16, 4 ) );

	// Add enemies
	enemies.push_back( new Runner( Vector2D( 260.f, 152.f ), -1 ) );		// Left runners
	enemies.push_back( new Runner( Vector2D( 164.f, 352.f ), -1 ) );
	enemies.push_back( new Runner( Vector2D( 1020.f, 152.f ), -1 ) );		// And right
	enemies.push_back( new Skeleton( Vector2D( 569.f, 334.f ), -1 ) );
	enemies.push_back( new Sawblade( 200, Vector2D( 262.f, 360.f ) ) );
	enemies.push_back( new Sawblade( 200, Vector2D( 706.f, 360.f ) ) );

	// Start the heart
	player->init( fmodSystem );
	heartBeat.start( );
	
	// Try to center the camera
	cameraX = player->pos.x - 320;
	cameraY = player->pos.y - 240;
}

Level1_04::~Level1_04( ) {
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

void Level1_04::process_events( float dt ) {
	logic( dt );

	collision( );
	
	maintenence( dt );

	if( player->pos.y + player->h / 2 < 0.f ) {
		levelSwitch = true;
		levelSwitchTo = 8;
		levelSwentrance = 1;
	}
}