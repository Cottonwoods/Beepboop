#include "Level.h"


Level1_11_1::Level1_11_1( float r, int entrance, Alonebot* p, FMOD::System* fsys, FMOD::Channel* sfx ) :
Level( r, entrance, p, fsys, sfx ) {
	// Inititalize area and size of level
	area = 1;
	levelWidth = 640;
	levelHeight = 480;

	// Initialize backgrounds and music
	bgMusic = "..\\music\\beat171-restless.mp3";
	background = LoadTexture( std::string( "..\\anims\\bg\\bg_09.png" ) );
	backbackground = LoadTexture( std::string( "..\\anims\\bg\\bbg_03.png" ) );
	
	// Create player, placement depending upon the entrance taken
	switch( entrance ) {
	  case 0:
		player = new Alonebot( 20.f, 404.f, p->dir, p ); break;
	}

	// Add static level objects to array
	objects.push_back( new Box( 0.f, 464.f, 310  ) );			// Ground floor
	objects.push_back( new Box( 0.f, 288.f, 310 ) );			// Ceiling
	objects.push_back( new Box( 294.f, 288.f, 16, 184 ) );		// Walls
	objects.push_back( new Box( 0.f, 288.f, 16, 112 ) );
	
	// Add item(s)
	if( player->items.find( 101 ) == player->items.end( ) )
		items.push_back( new HeartUp( Vector2D( 164.f, 420.f ), 101 ) );

	// Add doors
	doors.push_back( new Door( 0.f, 400.f, 1, false, 14, 1 ) );// Left door
	doorToppers.push_back( new Door( 0.f, 400.f, 1, true ) );
	objects.push_back( new Box( 0.f, 400.f, 16, 4 ) );

	// Start the heart
	player->init( fmodSystem );
	heartBeat.start( );
	
	// Try to center the camera
	cameraX = player->pos.x - 320;
	cameraY = player->pos.y - 240;
}

Level1_11_1::~Level1_11_1( ) {
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

void Level1_11_1::process_events( float dt ) {
	logic( dt );

	collision( );

	maintenence( dt );
}