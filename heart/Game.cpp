#include "Game.h"


static Level* start;												// First level that loads

// Drawing code !!
void Game::draw_box( Box* b ) {
	glPushMatrix( );
	glTranslatef( b->pos.x, b->pos.y, 0.f );
	// Bind texture
	glBindTexture( GL_TEXTURE_2D, b->getTex( ) );
	//Enable vertex arrays
	//Set vertex data
	glVertexPointer( 2, GL_INT, 0, b->vertices );
	glEnableClientState( GL_VERTEX_ARRAY );
	glTexCoordPointer(2, GL_FLOAT, 0, b->tvertices );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	//Draw quad using vertex data
	glDrawArrays( GL_QUADS, 0, 4 );
	//Disable vertex arrays
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glPopMatrix( );
};

void Game::draw_organ( Organ* o ) {
	glPushMatrix( );
	glTranslatef( o->pos.x, o->pos.y, 0.f );
	if( o->vertical == true ) {
		glTranslatef( (float)o->h, 0.f, 0.f );
		glRotatef( 90.f, 0.f, 0.f, 1.f );
	}
	if( o->dir == -1 ) {
		glTranslatef( (float)o->w, 0.f, 0.f );
		glScalef( -1.f, 1.f, 0.f );
	}
	// Bind texture
	glBindTexture( GL_TEXTURE_2D, o->getTex( ) );
	//Enable vertex arrays
	//Set vertex data
	glVertexPointer( 2, GL_INT, 0, o->vertices );
	glEnableClientState( GL_VERTEX_ARRAY );
	glTexCoordPointer(2, GL_INT, 0, o->tvertices );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	//Draw quad using vertex data
	glDrawArrays( GL_QUADS, 0, 4 );
	//Disable vertex arrays
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glPopMatrix( );
};

void Game::draw_arm( Alonebot* p ) {
	glPushMatrix( );
	int w, h;
	glTranslatef( p->pos.x, p->pos.y, 0.f );
	// Bind texture
	glBindTexture( GL_TEXTURE_2D, p->getArm() );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
	p->loadVA( w, h );
	//Enable vertex arrays
	//Set vertex data
	glVertexPointer( 2, GL_INT, 0, p->vertices );
	glEnableClientState( GL_VERTEX_ARRAY );
	glTexCoordPointer(2, GL_INT, 0, p->tvertices );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	//Draw quad using vertex data
	glDrawArrays( GL_QUADS, 0, 4 );
	//Disable vertex arrays
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glPopMatrix( );
};

void Game::draw_body( Alonebot* p, GLuint tex ) {
	int w, h;
	glPushMatrix( );
	glTranslatef( p->pos.x, p->pos.y, 0.f );

	if( !p->movable ) {
		glTranslatef( -4.f, 0.f, 0.f );
	}
	else if( p->jumping || !p->canJump ) {
		glTranslatef( -7.f, -8.f, 0.f );
		if( p->dir == -1 )
			glTranslatef( 14.f, 0.f, 0.f );
	}

	if( p->dir == -1 ) {
		glTranslatef( (float)p->w, 0.f, 0.f );
		glScalef( -1.f, 1.f, 0.f );
	}
	// Bind texture
	glBindTexture( GL_TEXTURE_2D, tex );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
	p->loadVA( w, h );
	//Enable vertex arrays
	//Set vertex data
	glVertexPointer( 2, GL_INT, 0, p->vertices );
	glEnableClientState( GL_VERTEX_ARRAY );
	glTexCoordPointer(2, GL_INT, 0, p->tvertices );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	//Draw quad using vertex data
	glDrawArrays( GL_QUADS, 0, 4 );
	//Disable vertex arrays
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glPopMatrix( );
};

void Game::draw_player( Alonebot* p ) {
	// Flashing effect for invincibility
	if( p->movable && !p->canDmg && p->getCourage( ) % 2 == 0 ) {
		return;
	}

	GLuint tex = p->getTex( );

	if( p->dir == -1 )
		draw_arm( p );

	draw_body( p, tex );

	if( p->dir == 1 )
		draw_arm( p );

	p->armOffset = Vector2D( 0, 0 );

	p->loadVA( p->defaultWidth, p->defaultHeight );
};

void Game::draw_bg( GLuint bg, float p, bool stretch ) {
	int w = start->levelWidth;										// Current level width
	int h = start->levelHeight;										// Current level height

	if( !stretch )
		h = 480;
	
	glPushMatrix( );
	if( w != 640 && p != 0.f )
		glTranslatef( ( w -  w / p ) * ( start->cameraX / ( w - 640 ) ), start->cameraY, 0.f );
	glBindTexture( GL_TEXTURE_2D, bg );
	Vector2Di vertices[4], tvertices[4];
	vertices[0].x = 0;													// Top left
	vertices[0].y = 0;
	vertices[1].x = (int)(w / p);									// Top rite
	vertices[1].y = 0;
	vertices[2].x = (int)(w / p);									// Bot rite
	vertices[2].y = h;
	vertices[3].x = 0;												// Bot left
	vertices[3].y = h;

	tvertices[0].x = 0;												// Top left
	tvertices[0].y = 0;
	tvertices[1].x = 1;												// Top rite
	tvertices[1].y = 0;
	tvertices[2].x = 1;												// Bot rite
	tvertices[2].y = 1;
	tvertices[3].x = 0;												// Bot left
	tvertices[3].y = 1;

	//Enable vertex arrays
	//Set vertex data
	glVertexPointer( 2, GL_INT, 0, vertices );
	glEnableClientState( GL_VERTEX_ARRAY );
	glTexCoordPointer(2, GL_INT, 0, tvertices );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	//Draw quad using vertex data
	glDrawArrays( GL_QUADS, 0, 4 );
	//Disable vertex arrays
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glPopMatrix( );
}

void Game::draw_bg( ) {
	
	if( start->levelWidth == 640 ) {
		draw_bg( start->backbackground, 1.f );						// Background texture, parallax factor
		draw_bg( start->background, 1.f );
	}
	else {
		draw_bg( start->backbackground, 1.25f );
		draw_bg( start->background, 1.125f );
	}
};

void Game::draw_map( ) {
	// Draw the area text
	glPushMatrix( );
	glTranslatef( start->cameraX + 200.f, start->cameraY + 14.f, 0.f );
	draw_organ( pauseMenu->getText( start->area ) );
	glPopMatrix( );

	// Then the full map, and the coloured area on top
	glPushMatrix( );
	glTranslatef( start->cameraX + 198.f, start->cameraY + 88.f, 0.f );
	draw_organ( pauseMenu->getFullMap( ) );
	draw_organ( pauseMenu->getMap( start->area ) );

	// Draw over the spots you have not been
	Alonebot* player = start->getPlayer();
	Vector2Di tempPos = Vector2Di( );
	for( int i=0; i<player->MAX_ROOMS; i++ ) {
		if( !player->visited[i] ) {
			switch( i ) {
			  case 0: tempPos = Vector2Di( 10, 183 ); break;
			  case 1: tempPos = Vector2Di( 25, 183 ); break;
			  case 2: tempPos = Vector2Di( 40, 183 ); break;
			  case 3: tempPos = Vector2Di( 55, 183 ); break;
			  case 4: tempPos = Vector2Di( 70, 183 ); break;
			  case 6: tempPos = Vector2Di( 85, 183 ); break;
			  case 7: tempPos = Vector2Di( 100, 183 ); break;
			  case 8: tempPos = Vector2Di( 100, 168 ); break;
			  case 9: tempPos = Vector2Di( 115, 183 ); break;
			  case 10: tempPos = Vector2Di( 85, 168 ); break;
			  case 11: tempPos = Vector2Di( 70, 168 ); break;
			  case 12: tempPos = Vector2Di( 115, 198 ); break;
			  case 13: tempPos = Vector2Di( 115, 168 ); break;
			  case 14: tempPos = Vector2Di( 130, 168 ); break;
			  case 16: tempPos = Vector2Di( 115, 213 ); break;
			}
			draw_organ( pauseMenu->setUnseen( tempPos ) );
		}
	}

	// Draw the player position
	switch( curLevel ) {
	  case 0: tempPos = Vector2Di( 10, 183 ); break;
	  case 1: tempPos = Vector2Di( 25, 183 ); break;
	  case 2: tempPos = Vector2Di( 40, 183 ); break;
	  case 3: tempPos = Vector2Di( 55, 183 ); break;
	  case 4: case 5: tempPos = Vector2Di( 70, 183 ); break;
	  case 6: tempPos = Vector2Di( 85, 183 ); break;
	  case 7: tempPos = Vector2Di( 100, 183 ); break;
	  case 8: tempPos = Vector2Di( 100, 168 ); break;
	  case 9: tempPos = Vector2Di( 115, 183 ); break;
	  case 10: tempPos = Vector2Di( 85, 168 ); break;
	  case 11: tempPos = Vector2Di( 70, 168 ); break;
	  case 12: tempPos = Vector2Di( 115, 198 ); break;
	  case 13: tempPos = Vector2Di( 115, 168 ); break;
	  case 14: case 15: tempPos = Vector2Di( 130, 168 ); break;
	  case 16: tempPos = Vector2Di( 115, 213 ); break;
	}
	draw_organ( pauseMenu->setHere( tempPos ) );

	glPopMatrix( );
};

void Game::draw_menu( ) {
	Alonebot* p = start->getPlayer( );
	draw_bg( pauseMenu->getMenu( start->menuState ), start->levelWidth / 640.f, false );

	switch( start->menuState ) {
	  case 0: {														// Draw the pause menu and map
		glPushMatrix( );
		glTranslatef( start->cameraX + 16.f, start->cameraY + 264.f + start->selection * 47.f, 0.f );
		draw_organ( pauseMenu->getSelect( ) );
		glPopMatrix( );

		draw_map( );
		break;
	  }
	  case 1: {														// Draw the equipment menu
		if( p->equips.size( ) == 0 )
			break;
		// Draw the currently selected card
		glPushMatrix( );
		glTranslatef( start->cameraX + 445.f, start->cameraY + 65.f, 0.f );
		draw_organ( pauseMenu->getCard( start->selection ) );
		glPopMatrix( );

		// Draw the equipment in player inventory
		glPushMatrix( );
		glTranslatef( start->cameraX + 16.f, start->cameraY + 66.f, 0.f );
		draw_organ( pauseMenu->getSlot( p->weapon - 1 ) );
		glTranslatef( 0.f, 60.f, 0.f );
		for( int i=0; i<p->equips.size( ); i++ ) {
			draw_organ( pauseMenu->getSlot( i ) );
			glTranslatef( 0.f, 47.f, 0.f );
		}
		glPopMatrix( );

		// Draw the selection bar
		glPushMatrix( );
		glTranslatef( start->cameraX + 16.f, start->cameraY + 126.f + start->selection * 47.f, 0.f );
		draw_organ( pauseMenu->getLongSelect( ) );
		glPopMatrix( );
		break;
	  }
	  case 2: {														// Draw the items menu
		break;
	  }
	  case 3: {														// Draw the upgrades menu
		break;
	  }
	  case 4: {														// Draw the options menu
		break;
	  }
	}
};

void Game::draw_title( ) {
	// Draw the title screen
	draw_bg( start->backbackground, 1.f );
	draw_bg( start->background, 1.f );
};

void Game::draw_screen( ) {
	glPushMatrix( );
	glClear( GL_COLOR_BUFFER_BIT );
	glScalef( ratio, ratio, 0.f );
	
	float w = (float)start->levelWidth;
	float h = (float)start->levelHeight;

	if( title )
		draw_title( );
	else {
		// Get a pointer to the player
		Alonebot* player = start->getPlayer();

		// Draw the pause menu
		if( start->paused ) {
			draw_menu( );
		}
		// Draw the game
		else {
			// Draw the background
			draw_bg( );
	
			// Get the static foreground from the level
			std::vector<Box*> objects = start->getScene( );
			// Draw each static object
			for( unsigned short i=0; i<objects.size( ); i++ )
				draw_box( objects[i] );

			// Break on through to the other side
			std::vector<Door*> doors = start->getDoors( );
			for( unsigned short i=0; i<doors.size( ); i++ )
				draw_organ( doors[i] );
	
			// Get all the NPCs and draw them
			std::vector<NPC*> NPCs = start->getNPCs( );
			for( unsigned short i=0; i<NPCs.size( ); i++ )
				draw_organ( NPCs[i] );
			
			// Draw the player
			draw_player( player );

			// Get all the enemies and draw them
			std::vector<Enemy*> enemies = start->getEnemies( );
			for( unsigned short i=0; i<enemies.size( ); i++ )
				draw_organ( enemies[i] );

			// Get all the items and draw them
			std::vector<Item*> items = start->getItems( );
			for( unsigned short i=0; i<items.size( ); i++ )
				draw_organ( items[i] );

			// Get all the projectiles and draw them
			std::vector<Projectile*> bullets = start->getBullets( );
			for( unsigned short i=0; i<bullets.size( ); i++ )
				draw_organ( bullets[i] );

			// Draw parts of the door that the player goes under
			doors = start->getDoorToppers( );
			for( unsigned short i=0; i<doors.size( ); i++ )
				draw_organ( doors[i] );
			
			// Draw the death screen when appropriate
			if( player->getDeathTime( ) >= 650 ) {
				fadeAlpha = 1.f - ( 1650 - player->getDeathTime( ) ) / 1000.f;
				glColor4f( 1.f, 1.f, 1.f, fadeAlpha );
				draw_bg( deathShade, w / 640.f, false );
				glColor4f( 1.f, 1.f, 1.f, 1.f );
			}
		}

		// Draw the health and charge bars always
		glPushMatrix( );
		glTranslatef( start->cameraX, start->cameraY, 0.f );
		player->drawHP( );
		glPopMatrix( );
	}
	
	// Fade in the screen when appropriate
	if( fade.is_started( ) ) {
		glColor4f( 0.f, 0.f, 0.f, fadeAlpha );
		draw_bg( fadeShade, w / 640.f, false );
		glColor4f( 1.f, 1.f, 1.f, 1.f );
	}

	glPopMatrix( );

	// Swap the buffers
	glfwSwapBuffers( );
};


// Loading code!!
void Game::pre_screen( ) {
	// First, initialize GLFW's video subsystem
	glfwInit( );

	// Initialize glew
	glewInit( );
};

void Game::post_screen( ) {
	// Enable textures on 2D surfaces
	glEnable( GL_TEXTURE_2D );

	// Enable blending
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	// Set shading model
	glShadeModel( GL_FLAT );

	// Set the clear color
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	// Change to the projection matrix and set our viewing volume.
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	glOrtho( 0.f, screenWidth, screenHeight, 0.f, 1.f, -1.f );
	
	// Save default modelview matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	glPushMatrix( );
};

void Game::load_screen( bool post ) {
	// Information about the current video settings
	int full = GLFW_WINDOW;
	int s = 0;
	
	std::fstream optionFile;
	optionFile.open( ".\\options.txt", std::fstream::in | std::fstream::out );
	// Create default option file if none is found
	if( !optionFile.good( ) ) {
		optionFile.clear( );
		optionFile.open( ".\\options.txt", std::fstream::out | std::fstream::trunc );
		optionFile << "# Fullscreen" << std::endl << 0 << std::endl <<
			"# Scale" << std::endl << 0 << std::endl <<
			"# Jump" << std::endl << 'Z' << std::endl <<
			"# Shoot" << std::endl << 'X' << std::endl <<
			"# Change" << std::endl << 'C' << std::endl <<
			"# WASD" << std::endl << 0 << std::endl;
		optionFile.close( );
		optionFile.open( ".\\options.txt", std::fstream::in | std::fstream::out );
	}
	optionFile.ignore( 256, '\n' );
	optionFile >> full;
	optionFile.ignore( 256, '\n' );
	optionFile.ignore( 256, '\n' );
	optionFile >> s;
	screenWidth = 640 * ( s + 1 );
	screenHeight = 480 * ( s + 1 );
	optionFile.close( );
	if( full != 0 )
		full = GLFW_FULLSCREEN;
	else
		full = GLFW_WINDOW;

	// Set the video mode or resize
	if( post )
		glfwCloseWindow( );
	glfwOpenWindowHint( GLFW_WINDOW_NO_RESIZE, GL_TRUE );
//	printf( "%ix%i\n", screenWidth, screenHeight );
	if( !glfwOpenWindow( screenWidth, screenHeight, 0, 0, 0, 1, 0, 0, full ) ) {
		printf( "Error opening window via GLFW\n" );
		exit( -1 );
	}
	if( full == GLFW_FULLSCREEN )
		glfwGetWindowSize( &screenWidth, &screenHeight);
	
	// Set window title and disable mouse
	glfwSetWindowTitle( "Alonebot: Nightmare in Machineland" );
//	glfwDisable( GLFW_MOUSE_CURSOR );

	// Setup our viewport
	glViewport( 0, 0, screenWidth, screenHeight );

	// Enable screen culling
	glScissor( 0, 0, screenWidth, screenHeight );
	glEnable( GL_SCISSOR_TEST );

	// Set scaling ratio based on resolution
	ratio = screenWidth / 640.f;
	
	// Enable v-sync
	glfwSwapInterval( 1 );

	if( post ) {
		post_screen( );
	}

	// Load screen assets
	fadeShade = LoadTexture( std::string( "..\\anims\\bg\\fade.png" ) );
	deathShade = LoadTexture( std::string( "..\\anims\\bg\\death.png" ) );

	pauseMenu = new Menu;
};

void Game::load_level( int lev, int entrance ) {
	Level* s;
	curLevel = lev;
	int curArea = 0;
	Alonebot* p = 0;
	Alonebot* temp = 0;

	if( start != 0 ) {													// Create the first player object
		temp = start->getPlayer( );
		p = new Alonebot( temp->pos.x, temp->pos.y, temp->dir, temp );
		curArea = start->area;
		delete start;
	}

	// If you haven't been here before, add it to your map
	if( lev != INT_MAX && !p->visited[lev] )
		p->visited[lev] = true;

	switch( lev ) {
	  case 0:
		s = new Tutorial0_0( ratio, entrance, p, fmodSystem, sfxChannel ); title = false; break;
	  case 1:
		s = new Tutorial0_1( ratio, entrance, p, fmodSystem, sfxChannel ); title = false; break;
	  case 2:
		s = new Tutorial0_2( ratio, entrance, p, fmodSystem, sfxChannel ); title = false; break;
	  case 3:
		s = new Level1_01( ratio, entrance, p, fmodSystem, sfxChannel ); title = false; break;
	  case 4:
		s = new Level1_02( ratio, entrance, p, fmodSystem, sfxChannel ); title = false; break;
	  case 5:
		s = new Level1_02_1( ratio, entrance, p, fmodSystem, sfxChannel ); title = false; break;
	  case 6:
		s = new Level1_03( ratio, entrance, p, fmodSystem, sfxChannel ); title = false; break;
	  case 7:
		s = new Level1_04( ratio, entrance, p, fmodSystem, sfxChannel ); title = false; break;
	  case 8:
		s = new Level1_05( ratio, entrance, p, fmodSystem, sfxChannel ); title = false; break;
	  case 9:
		s = new Level1_06( ratio, entrance, p, fmodSystem, sfxChannel ); title = false; break;
	  case 10:
		s = new Level1_07( ratio, entrance, p, fmodSystem, sfxChannel ); title = false; break;
	  case 11:
		s = new Level1_08( ratio, entrance, p, fmodSystem, sfxChannel ); title = false; break;
	  case 12:
		s = new Level1_09( ratio, entrance, p, fmodSystem, sfxChannel ); title = false; break;
	  case 13:
		s = new Level1_10( ratio, entrance, p, fmodSystem, sfxChannel ); title = false; break;
	  case 14:
		s = new Level1_11( ratio, entrance, p, fmodSystem, sfxChannel ); title = false; break;
	  case 15:
		s = new Level1_11_1( ratio, entrance, p, fmodSystem, sfxChannel ); title = false; break;
	  case 16:
		s = new Level1_12( ratio, entrance, p, fmodSystem, sfxChannel ); title = false; break;
	  case 17:
		s = new Level2_01( ratio, entrance, p, fmodSystem, sfxChannel ); title = false; break;
	  case INT_MAX: {
		load_screen( true ); s = new Title( ratio, entrance, p, fmodSystem, sfxChannel ); title = true; break;
	  }
	}

	start = s;
	
	if( !title ) {
		start->playDoorSFX( );
	}

	if( start->area != curArea ) {										// Change the music!
		if( musicChannel != 0 )
			FmodErrorCheck( musicChannel->stop( ) );
		if( music != 0 )
			FmodErrorCheck( music->release( ) );
		music = LoadSound( start->bgMusic, fmodSystem, true );
		FmodErrorCheck( fmodSystem->playSound( FMOD_CHANNEL_FREE, music, false, &musicChannel ) );
		musicChannel->setVolume( 0.4f );
	}
};

void GLFWCALL handle_key_call( int key, int action ) {
	start->handle_key( key, action );
};


// Main hook!!
Game::Game( ) {
	// Initialize variables for time, fading, and sound
	Timer fps;
	float t = 0.f;
	int frame = 0;
	int lastFPS = 0;
	int currentFPS = 0;
	int currentTime = 0;
	const float dt = 0.01f;
	float accumulator = 0.f;
	float timeScale = 1.0f;
	musicChannel = 0;
	fadeAlpha = 1.f;
	fade.start( );
	start = 0;
	music = 0;

	// Initiate sound and windowing subsystems, create new game and load the title screen
	FmodErrorCheck( FMOD::System_Create( &fmodSystem ) );				// Initiates the FMOD system variable
	FmodErrorCheck( fmodSystem->init(32, FMOD_INIT_NORMAL, 0) );		// Max of 32 sounds at once
	pre_screen( );
	load_level( );

	/*
		* Now we want to begin our normal app process--
		* an event loop with a lot of redrawing.
		*/
	fps.start( );
	lastFPS = fps.get_ticks( );
	currentTime = fps.get_ticks( );
	while( glfwGetWindowParam( GLFW_OPENED ) ) {
		int frameTime = fps.get_ticks( ) - currentTime;
		currentTime = fps.get_ticks( );
		accumulator += frameTime / 1000.f;
		accumulator *= timeScale;

		// LOGIC!!
		while( accumulator >= dt ) {
			if( !fade.is_started( ) ) {
				if( start->levelSwitch ) {								// If the levelSwitch is activated ...
					glfwSetKeyCallback( NULL );							// Unbind keys
					fadeAlpha = 1.f;									// Black the screen!
					fade.start( );										// Begin fade and draw the screen
					draw_screen( );
					load_level( start->levelSwitchTo, start->levelSwentrance );	// ... Switch the level
					if( title ) {
						glfwSetKeyCallback( &handle_key_call );
						fadeAlpha = 0.f;
						fade.stop( );
					}
					else
						start->pause( );
				}
				start->process_events( dt );							// Let the level process
			}
			else {
				if( fadeAlpha > 0.f ) {
					if( frameTime != 0 )
						fadeAlpha -= dt / 0.7f;							// Number of seconds to fade
				}
				else {
					glfwSetKeyCallback( &handle_key_call );
					start->unpause( );
					fade.stop( );
				}
			}
			accumulator -= dt;											// Remove time used in dt chunks
			t += dt;													// Increment time passed
		}

		// Draw the screen
		draw_screen( );

		// Record FPS
		frame++;
		if( lastFPS < currentTime - 1000.0 ) {
			currentFPS = frame;
			frame = 0;
			lastFPS = currentTime;
		}
		printf( "\rFPS %i", currentFPS );
	}
	
	fmodSystem->release( );
	glfwTerminate( );
};

Game::~Game( ) {
	glDeleteTextures( 1, &fadeShade );
};