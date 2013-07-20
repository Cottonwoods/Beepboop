#include "Level.h"


Level1_02_1::Level1_02_1( float r, int entrance, Alonebot* p, FMOD::System* fsys, FMOD::Channel* sfx ) :
Level( r, entrance, p, fsys, sfx ) {
	// Inititalize area and size of level
	area = 1;
	levelWidth = 640;
	levelHeight = 480;

	// Initialize backgrounds and music
	bgMusic = "..\\music\\beat171-restless.mp3";
	background = LoadTexture( std::string( "..\\anims\\bg\\bg_04.png" ) );
	backbackground = LoadTexture( std::string( "..\\anims\\bg\\bbg_03.png" ) );
	
	// Create player, placement depending upon the entrance taken
	switch( entrance ) {
	  case 0:
		player = new Alonebot( levelWidth - 38.f, 116.f, p->dir, p ); break;
	}

	// Add static level objects to array
	objects.push_back( new Box( levelWidth - 310.f, 176.f, 310  ) );		// Ground floor
	objects.push_back( new Box( levelWidth - 310.f, 0.f, 310 ) );			// Ceiling
	objects.push_back( new Box( levelWidth - 310.f, 0.f, 16, 184 ) );		// Walls
	objects.push_back( new Box( levelWidth - 16.f, 0.f, 16, 112 ) );
	
	// Add item(s)
	if( player->items.find( 2 ) == player->items.end( ) )
		items.push_back( new Sword( Vector2D( levelWidth - 164.f, 132.f ) ) );

	// Add doors
	doors.push_back( new Door( levelWidth - 32.f, 112.f, -1, false, 6, 2 ) );// Right door
	doorToppers.push_back( new Door( levelWidth - 32.f, 112.f, -1, true ) );
	objects.push_back( new Box( levelWidth - 16.f, 112.f, 16, 4 ) );

	// Start the heart
	player->init( fmodSystem );
	heartBeat.start( );
	
	// Try to center the camera
	cameraX = player->pos.x - 320;
	cameraY = player->pos.y - 240;
}

Level1_02_1::~Level1_02_1( ) {
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

void Level1_02_1::process_events( float dt ) {
	logic( dt );

	collision( );

	maintenence( dt );
}