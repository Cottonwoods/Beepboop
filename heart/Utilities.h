#include "GL\glew.h"
#include "GL\glfw.h"
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "SOIL.h"

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>

#define GRAVITY 6.32f


class Timer {
  private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

  public:
    //Initializes variables
    Timer( );

    //The various clock actions
    void start( );
    void stop( );
    void pause( );
    void unpause( );

    //Gets the timer's time
    int get_ticks( );

    //Checks the status of the timer
    bool is_started( );
    bool is_paused( );
};


GLuint LoadTexture( std::string texPath );

FMOD::Sound* LoadSound( const char* s, FMOD::System* sys, bool loop = false );

void FmodErrorCheck( FMOD_RESULT result );

float min( float a, float b );

float max( float a, float b );