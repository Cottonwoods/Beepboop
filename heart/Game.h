#include "Level.h"


class Game {
  private:
	int screenWidth, screenHeight, curLevel;
	float ratio;
	bool title;

	static const int MAX_AREAS = 2;

	GLuint fadeShade,
		deathShade,
		pauseMenu;
	Organ *pauseSelect, *fullMap, *unseen, *here;
	Organ* areaTexts[MAX_AREAS];
	Organ* areaMaps[MAX_AREAS];

	float fadeAlpha;
	Timer fade;

	FMOD::System* fmodSystem;					// Variable for talking to FMOD
	FMOD::Sound* music;							// Current music being played
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