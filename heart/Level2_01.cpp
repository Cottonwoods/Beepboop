#include "Level.h"


Level2_01::Level2_01( float r, int entrance, Alonebot* p, FMOD::System* fsys, FMOD::Channel* sfx ) :
Level( r, entrance, p, fsys, sfx ) {
	// Inititalize area and size of level
	area = 2;
	levelWidth = 640;
	levelHeight = 480;

	// Initialize backgrounds and music
	bgMusic = "..\\music\\beat311-creep.mp3";
	background = LoadTexture( std::string( "..\\anims\\bg\\bg_10.png" ) );
	backbackground = LoadTexture( std::string( "..\\anims\\bg\\bbg_04.png" ) );
	
	// Create player, placement depending upon the entrance taken
	switch( entrance ) {
	  case 0:
		player = new Alonebot( levelWidth - 38.f, 316.f, p->dir, p ); break;
	  case 1:
		player = new Alonebot( 20.f, 316.f, p->dir, p ); break;
	}

	// Add static level objects to array
	objects.push_back( new Box( 0.f, 376.f, levelWidth ) );					// Ground floor
	objects.push_back( new Box( 0.f, 0.f, levelWidth ) );					// Ceiling
	objects.push_back( new Box( 0.f, 0.f, 16, 316 ) );						// Walls
	objects.push_back( new Box( levelWidth - 16.f, 0.f, 16, 316 ) );

	// Add doors
	doors.push_back( new Door( 0.f, 312.f, 1, false, 0, 0 ) );				// Left door
	doorToppers.push_back( new Door( 0.f, 312.f, 1, true ) );
	objects.push_back( new Box( 0.f, 312.f, 16, 4 ) );

	doors.push_back( new Door( levelWidth - 32.f, 312.f, -1, false, 16, 1 ) );// Right door
	doorToppers.push_back( new Door( levelWidth - 32.f, 312.f, -1, true ) );
	objects.push_back( new Box( levelWidth - 16.f, 312.f, 16, 4 ) );

	// Start the heart
	player->init( fmodSystem );
	heartBeat.start( );

	// Try to center the camera
	cameraX = player->pos.x - 320;
	cameraY = player->pos.y - 240;
}

Level2_01::~Level2_01( ) {
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

void Level2_01::process_events( float dt ) {
	logic( dt );

	collision( );

	maintenence( dt );
}