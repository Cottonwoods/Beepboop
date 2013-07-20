#include "Level.h"


Tutorial0_0::Tutorial0_0( float r, int entrance, Alonebot* p, FMOD::System* fsys, FMOD::Channel* sfx ) :
Level( r, entrance, p, fsys, sfx ) {
	// Inititalize area and size of level
	area = 0;
	levelWidth = 1152;				// Width of level in pixels; 1.125 times the background width
	levelHeight = 480;

	// Initialize backgrounds and music
	bgMusic = "..\\music\\beat496-stag.mp3";
	background = LoadTexture( std::string( "..\\anims\\bg\\bg_00.png" ) );
	backbackground = LoadTexture( std::string( "..\\anims\\bg\\bbg_00.png" ) );
	
	// Create player, placement depending upon the entrance taken
	switch( entrance ) {
	  case 0:
		player = new Alonebot( 230.f, 316.f, -1, p ); break;
	  case 1:
		player = new Alonebot( levelWidth - 38.f, 316.f, p->dir, p ); break;
	  case 2:
		player = new Alonebot( levelWidth - 38.f, 116.f, p->dir, p ); break;
	}

	// Add static level objects to array
	objects.push_back( new Box( 0.f, 376.f, 712 ) );						// Ground floor left of bridge
	objects.push_back( new Bridge( 712.f, 376.f, 256 ) );					// Bridge
	objects.push_back( new Box( 968.f, 376.f, 184 ) );						// Ground floor right of bridge
	objects.push_back( new Box( levelWidth - 142.f, 176.f, 142 ) );			// Edge platform with door
	objects.push_back( new Box( levelWidth - 158.f, 128.f, 16, 64 ) );		// Edge platform rail

	// Add item(s)
	if( player->items.find( 1 ) == player->items.end( ) )
		items.push_back( new Blaster( Vector2D( levelWidth - 124.f, 152.f ) ) );

	// Add NPC(s)
	NPCs.push_back( new Phatbot( Vector2D( 66.f, 288.f ) ) );

	// Add doors
	doors.push_back( new Door( levelWidth - 32.f, 112.f, -1, false, 1, 1 ) );// Top right door
	doorToppers.push_back( new Door( levelWidth - 32.f, 112.f, -1, true ) );
	objects.push_back( new Box( levelWidth - 16.f, 112.f, 16, 4 ) );

	doors.push_back( new Door( levelWidth - 32.f, 312.f, -1, false, 1, 0 ) );// Bottom right door
	doorToppers.push_back( new Door( levelWidth - 32.f, 312.f, -1, true ) );
	objects.push_back( new Box( levelWidth - 16.f, 312.f, 16, 4 ) );

	// Start the heart
	player->init( fmodSystem );
	heartBeat.start( );
	
	// Try to center the camera
	cameraX = player->pos.x - 320;
	cameraY = player->pos.y - 240;
}

Tutorial0_0::~Tutorial0_0( ) {
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

void Tutorial0_0::process_events( float dt ) {
	logic( dt );

	collision( );

	maintenence( dt );
}