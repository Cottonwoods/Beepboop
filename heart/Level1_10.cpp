#include "Level.h"


Level1_10::Level1_10( float r, int entrance, Alonebot* p, FMOD::System* fsys, FMOD::Channel* sfx ) :
Level( r, entrance, p, fsys, sfx ) {
	// Inititalize area and size of level
	area = 1;
	levelWidth = 1152;
	levelHeight = 480;

	// Initialize backgrounds and music
	bgMusic = "..\\music\\beat171-restless.mp3";
	background = LoadTexture( std::string( "..\\anims\\bg\\bg_08.png" ) );
	backbackground = LoadTexture( std::string( "..\\anims\\bg\\bbg_03.png" ) );
	
	// Create player, placement depending upon the entrance taken
	switch( entrance ) {
	  case 0:
		player = new Alonebot( p->pos.x, levelHeight - p->h / 2.f, p->dir, p ); break;
	  case 1:
		player = new Alonebot( levelWidth - 38.f, 404.f, p->dir, p ); break;
	  case 2:
		player = new Alonebot( 20.f, 116.f, p->dir, p ); break;
	  case 3:
		player = new Alonebot( levelWidth - 38.f, 116.f, p->dir, p ); break;
	}

	// Add static level objects to array
	objects.push_back( new Box( 0.f, 464.f, 400 ) );						// Floor, left
	objects.push_back( new Box( 624.f, 464.f, levelWidth - 624 ) );			//		  right
	objects.push_back( new Box( 0.f, 0.f, levelWidth ) );					// Ceiling
	objects.push_back( new Box( 0.f, 0.f, 16, 480 ) );						// Walls
	objects.push_back( new Box( levelWidth - 16.f, 0.f, 16, 112 ) );
	objects.push_back( new Box( levelWidth - 16.f, 176.f, 16, 224 ) );
	objects.push_back( new Box( levelWidth - 129.f, 176.f, 128 ) );			// Door floor
	objects.push_back( new Dropbox( 852.f, 268.f, 128 ) );					// Pathway
	objects.push_back( new Dropbox( 680.f, 358.f, 128 ) );

	// Add doors
	doors.push_back( new Door( levelWidth - 32.f, 112.f, -1, false, 14, 2 ) );// Top right door
	doorToppers.push_back( new Door( levelWidth - 32.f, 112.f, -1, true ) );
	objects.push_back( new Box( levelWidth - 16.f, 112.f, 16, 4 ) );

	doors.push_back( new Door( levelWidth - 32.f, 400.f, -1, false, 14, 0 ) );// Bottom right door
	doorToppers.push_back( new Door( levelWidth - 32.f, 400.f, -1, true ) );
	objects.push_back( new Box( levelWidth - 16.f, 400.f, 16, 4 ) );

	// Start the heart
	player->init( fmodSystem );
	heartBeat.start( );
	
	// Try to center the camera
	cameraX = player->pos.x - 320;
	cameraY = player->pos.y - 240;
}

Level1_10::~Level1_10( ) {
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

void Level1_10::process_events( float dt ) {
	logic( dt );

	collision( );

	maintenence( dt );

	if( player->pos.y + player->h / 2 > levelHeight ) {
		levelSwitch = true;
		levelSwitchTo = 9;
		levelSwentrance = 3;
	}
}