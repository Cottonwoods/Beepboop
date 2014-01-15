#include "Menu.h"


class Game {
  private:
	int screenWidth, screenHeight, curLevel;
	float ratio;									// Scaling ratio
	bool title;										// Whether you are at the title screen

	Menu* pauseMenu;								// Holds logic and assets for the menu

	GLuint fadeShade, deathShade;
	float fadeAlpha;
	Timer fade;

	FMOD::System* fmodSystem;						// Variable for talking to FMOD
	FMOD::Sound* music;								// Current music being played
	FMOD::Channel* musicChannel;
	FMOD::Channel* sfxChannel;

	void draw_box( Box* b );
	void draw_organ( Organ* o );
	void draw_arm( Alonebot* p );
	void draw_body( Alonebot* p, GLuint tex );
	void draw_player( Alonebot* p );
	void draw_bg( GLuint bg, float p = 1.f, bool stretch = true );
	void draw_bg( );
	void draw_map( );
	void draw_menu( );
	void draw_title( );
	void draw_screen( );

	void pre_screen( );
	void post_screen( );
	void load_screen( bool post = false );
	void load_level( int lev = INT_MAX, int entrance = 0 );
	
  public:
	Game( );
	~Game( );
};