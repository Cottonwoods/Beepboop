#include "Utilities.h"


Timer::Timer( ) {
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start( ) {
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = (int)( glfwGetTime( ) * 1000.0 );
}

void Timer::stop( ) {
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void Timer::pause( ) {
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) ) {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = (int)( glfwGetTime( ) * 1000.0 - startTicks );
    }
}

void Timer::unpause( ) {
    //If the timer is paused
    if( paused == true ) {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = (int)( glfwGetTime( ) * 1000.0 - pausedTicks );

        //Reset the paused ticks
        pausedTicks = 0;
	}
}

int Timer::get_ticks( ) {
	//If the timer is running
	if( started == true ) {
		//If the timer is paused
		if( paused == true ) {
			//Return the number of ticks when the timer was paused
			return pausedTicks;
		}
        else {
			//Return the current time minus the start time
			return (int)( glfwGetTime( ) * 1000.0 - startTicks );
		}
	}
	
	//If the timer isn't running
	return 0;
}

bool Timer::is_started( ) {
	return started;
}

bool Timer::is_paused( ) {
	return paused;
}


GLuint LoadTexture( std::string texPath ) {
	GLuint texture = SOIL_load_OGL_texture (
		texPath.c_str( ),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MULTIPLY_ALPHA
		| SOIL_FLAG_DDS_LOAD_DIRECT
	);

	// Bind the texture object
	glBindTexture( GL_TEXTURE_2D, texture );

	// Set the texture's stretching properties
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	return texture;
}

void FmodErrorCheck( FMOD_RESULT result ) {				// This is an error handling function
	if ( result != FMOD_OK ) {							//  for FMOD errors
		printf( "FMOD error! (%d) %s\n", result, FMOD_ErrorString( result ) );
	}
}

FMOD::Sound* LoadSound( const char* s, FMOD::System* sys, bool loop ) {
	// For loading each level's music
	FMOD::Sound* temp;
	// Uses software mixing and loops the track
	int flags = loop * FMOD_LOOP_NORMAL;
	FmodErrorCheck( sys->createStream( s, FMOD_SOFTWARE | flags, 0, &temp ) );
	return temp;
}

float min( float a, float b ) {
	return ((a<b)?a:b);
}

float max( float a, float b ) {
	return ((a>b)?a:b);
}