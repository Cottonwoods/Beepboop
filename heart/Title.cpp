#include "Level.h"


Title::Title( float r, int entrance, Alonebot* p, FMOD::System* fsys, FMOD::Channel* sfx ) {
	area = INT_MAX;
	// Inititalize width of level, camera variable, level change
	levelWidth = 720;				// Width of level in pixels; 1.125 times the background width
	levelHeight = 480;
	cameraX = 0.f;
	cameraY = 0.f;
	cameraV = Vector2D( );
	ratio = r;
	levelSwitch = 0;
	levelSwitchTo = 0;
	levelSwentrance = 0;
	selection = entrance;
	optionMenu = false;
	
	std::fstream optionFile;
	optionFile.open( ".\\options.txt", std::fstream::in );
	for( int i=0; i<6; i++ ) {
		optionFile.ignore( 256, '\n' );
		optionFile >> optionValues[i];
		optionFile.ignore( 256, '\n' );
	}
	optionFile.close( );
	
	// Initiates the FMOD system variable
	fmodSystem = fsys;
	sfxChannel = sfx;

	// Initialize backgrounds and music
	bgMusic = "..\\music\\beat283pockettanks.mp3";
	menus[0] = LoadTexture( std::string( "..\\anims\\title\\title.png" ) );
	menus[1] = LoadTexture( std::string( "..\\anims\\title\\options\\return.png" ) );
	backbackground = menus[0];
	for( int i=0; i<MAXSELECTIONS; i++ ) {
		std::stringstream s;
		s << i;
		selections[i] = LoadTexture( std::string( "..\\anims\\title\\selections_" + s.str( ) ) + ".png" );
	}
	background = selections[0];
	for( int i=0; i<2; i++ ) {
		for( int j=0; j<2; j++ ) {
			for( int k=0; k<2; k++ ) {
				std::stringstream s;
				s << i << j << k;
				std::string t = s.str( );
				options[i][j][k] = LoadTexture( std::string( "..\\anims\\title\\options\\options_" + t ) + ".png" );
			}
		}
	}
	
	// Create player
	player = new Alonebot( 0.f, 0.f, 1, p );

	// When coming from options menu
	if( selection > 0 ) {
		selection--;
		optionMenu = true;
		backbackground = options[0][optionValues[0]][optionValues[1]];
		background = options[1][selection][optionValues[selection]];
	}

	// Start the heart
	player->init( fmodSystem );
	heartBeat.start( );
}

Title::~Title( ) {
	sfxChannel->stop( );

	delete player;
	glDeleteTextures( 2, menus );
	glDeleteTextures( 2, options[0][0] );
	glDeleteTextures( 2, options[0][1] );
	glDeleteTextures( 2, options[1][0] );
	glDeleteTextures( 2, options[1][1] );
	glDeleteTextures( MAXSELECTIONS, selections );
	glDeleteTextures( 1, &background );
	glDeleteTextures( 1, &backbackground );
}

void Title::handle_key( int key, int action ) {
	if( action == GLFW_PRESS ) {
		switch( key ) {
		  case GLFW_KEY_ESC:
			quit( 0 );
		  case GLFW_KEY_ENTER:
		  case GLFW_KEY_SPACE:
		  case 'Z':
		  case 'X': {							// Select
			if( optionMenu ) {
				if( selection == 2 ) {			// At options menu, want to return
					selection = 1;
					optionMenu = false;
					backbackground = menus[0];
					background = selections[selection];
				}
				else {							// Adjusting an option
					optionValues[selection] = ( optionValues[selection] - 1 ) * -1;
					backbackground = options[0][optionValues[0]][optionValues[1]];
					background = options[1][selection][optionValues[selection]];
					std::fstream optionFile;
					optionFile.open( ".\\options.txt", std::fstream::out | std::fstream::trunc );
					optionFile << "# Fullscreen" << std::endl << optionValues[0] << std::endl <<
						"# Scale" << std::endl << optionValues[1] << std::endl <<
						"# Jump" << std::endl << 'Z' << std::endl <<
						"# Shoot" << std::endl << 'X' << std::endl <<
						"# Change" << std::endl << 'C' << std::endl <<
						"# WASD" << std::endl << 0 << std::endl;
					optionFile.close( );
					levelSwitch = true;
					levelSwitchTo = INT_MAX;
					levelSwentrance = selection + 1;
				}
			}
			else {
				switch( selection ) {
				  case 0: {
					levelSwitch = true;
					break;
				  }
				  case 1: {
					selection = 0;
					optionMenu = true;
					background = options[1][selection][optionValues[selection]];
					backbackground = options[0][optionValues[0]][optionValues[1]];
					break;
				  }
				  case 2: {
					quit( 0 );
				  }
				}
			}
			break;
		  }
		  case GLFW_KEY_UP: {						// Up
			selection--;
			if( selection < 0 )
				selection = MAXSELECTIONS - 1;
			if( optionMenu ) {
				if( selection == 2 )
					background = menus[1];
				else {
					background = options[1][selection][optionValues[selection]];
					backbackground = options[0][optionValues[0]][optionValues[1]];
				}
			}
			else
				background = selections[selection];
			break;
		  }
		  case GLFW_KEY_DOWN: {					// Down
			selection++;
			if( selection >= MAXSELECTIONS )
				selection = 0;
			if( optionMenu ) {
				if( selection == 2 )
					background = menus[1];
				else {
					background = options[1][selection][optionValues[selection]];
					backbackground = options[0][optionValues[0]][optionValues[1]];
				}
			}
			else
				background = selections[selection];
			break;
		  }
		  case GLFW_KEY_LEFT:
		  case GLFW_KEY_RIGHT: {
			if( optionMenu ) {					// Adjusting options
				optionValues[selection] = ( optionValues[selection] - 1 ) * -1;
				backbackground = options[0][optionValues[0]][optionValues[1]];
				background = options[1][selection][optionValues[selection]];
				std::fstream optionFile;
				optionFile.open( ".\\options.txt", std::fstream::out | std::fstream::trunc );
				optionFile << "# Fullscreen" << std::endl << optionValues[0] << std::endl <<
					"# Scale" << std::endl << optionValues[1] << std::endl <<
					"# Jump" << std::endl << 'Z' << std::endl <<
					"# Shoot" << std::endl << 'X' << std::endl <<
					"# Change" << std::endl << 'C' << std::endl <<
					"# WASD" << std::endl << 0 << std::endl;
				optionFile.close( );
				levelSwitch = true;
				levelSwitchTo = INT_MAX;
				levelSwentrance = selection + 1;
			}
			break;
		  }
		  default:
			break;
		}
	}
}

void Title::process_events( float dt ) {
	logic( dt );

	maintenence( dt );
}