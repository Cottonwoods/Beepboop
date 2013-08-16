#include "Alonebot.h"


class Box : public Organ {
  public:
	Box( float a = 0.f, float b = 0.f, int width = 32, int height = 16 );
	~Box( );

	void loadVA( );
};


class Dropbox : public Box {
  public:
	Vector2D vertices[4], tvertices[4];
	Dropbox( float a = 0.f, float b = 0.f, int width = 32, int height = 4 );
	~Dropbox( );
};


class Invisbox : public Box {
  public:
	Invisbox( float a = 0.f, float b = 0.f, int width = 16, int height = 16 );
	~Invisbox( );
};


class Bridge : public Box {
  public:
	Bridge( float a = 0.f, float b = 0.f, int width = 128, int height = 16 );
	~Bridge( );
};


class Door : public Organ {
  public:
	int to, entrance;
	Door( float a = 0.f, float b = 0.f, int d = 1, bool topper = false,
		int t = 0, int e = 0, int width = 32, int height = 64 );
	~Door( );
};


class Level {									// Generic level template
  protected:
	Timer heartBeat;							// Heartbeat of the level, used to handle timed events
	
	Alonebot* player;
	std::vector<NPC*> NPCs;
	std::vector<Item*> items;
	std::vector<Door*> doors;
	std::vector<Door*> doorToppers;
	std::vector<Box*> objects;
	std::vector<Enemy*> enemies;
	std::vector<Projectile*> bullets;

	FMOD::System* fmodSystem;					// Handle for talking to FMOD from Game
	FMOD::Channel* sfxChannel;					// A handle to the game's channel

	void quit( int code );

  public:
	int area;
	bool paused;
	int selection, menuState;
	int optionValues[6];
	GLuint background, backbackground;
	float cameraX, cameraY;						// Camera position, might change to Vector2D ?
	Vector2D cameraV;							// Camera velocity
	float ratio;								// Scaling ratio used to translate movements of the camera
	int levelWidth;								// In pixels
	int levelHeight;
	bool levelSwitch;							// Used for the main program to control level changes
	int levelSwitchTo;							// Controls the level of arrival
	int levelSwentrance;						// Level switching to's entrance
	const char* bgMusic;						// Location of the bg music

	Level( float r = 1.f, int entrance = 0, Alonebot* p = 0, FMOD::System* fsys = 0, FMOD::Channel* sfx = 0 );
	~Level( );

	bool clearBullets( );
	bool clearEnemies( );
	void logic( float dt );
	void collision( );
	void maintenence( float dt );
	
	virtual void pause( );
	virtual void unpause( );

	virtual void process_events( float dt );	// Think
	virtual void handle_key( int key, int action );
	
	Alonebot* getPlayer( );
	std::vector<NPC*> getNPCs( );
	std::vector<Item*> getItems( );
	std::vector<Door*> getDoors( );
	std::vector<Door*> getDoorToppers( );
	std::vector<Box*> getScene( );
	std::vector<Enemy*> getEnemies( );
	std::vector<Projectile*> getBullets( );

	void playDoorSFX( );
};


// All the levels
class Title : public Level {
  public:
	bool optionMenu;
	static const int MAXSELECTIONS = 3;
	GLuint menus[2];
	GLuint selections[MAXSELECTIONS];
	GLuint options[2][2][2];

	Title( float r = 1.f, int entrance = 0, Alonebot* p = 0, FMOD::System* fsys = 0, FMOD::Channel* sfx = 0 );
	~Title( );
	void handle_key( int key, int action );
	void process_events( float dt );
};

class Tutorial0_0 : public Level {
  public:
	Tutorial0_0( float r = 1.f, int entrance = 0, Alonebot* p = 0, FMOD::System* fsys = 0, FMOD::Channel* sfx = 0 );
	~Tutorial0_0( );
	void process_events( float dt );
};

class Tutorial0_1 : public Level {
  public:
	Tutorial0_1( float r = 1.f, int entrance = 0, Alonebot* p = 0, FMOD::System* fsys = 0, FMOD::Channel* sfx = 0 );
	~Tutorial0_1( );
	void process_events( float dt );
};

class Tutorial0_2 : public Level {
  public:
	Tutorial0_2( float r = 1.f, int entrance = 0, Alonebot* p = 0, FMOD::System* fsys = 0, FMOD::Channel* sfx = 0 );
	~Tutorial0_2( );
	void process_events( float dt );
};

class Level1_01 : public Level {
  private:
	static const int MIN_ZOMBIE_DELAY = 1500;	// Time in ms between spawns
	static const int MAX_ZOMBIE_DELAY = 2000;
	int zombieTime;								// Used to measure pauses between zombie spawn

  public:
	Level1_01( float r = 1.f, int entrance = 0, Alonebot* p = 0, FMOD::System* fsys = 0, FMOD::Channel* sfx = 0 );
	~Level1_01( );
	void process_events( float dt );
};

class Level1_02 : public Level {
  public:
	Level1_02( float r = 1.f, int entrance = 0, Alonebot* p = 0, FMOD::System* fsys = 0, FMOD::Channel* sfx = 0 );
	~Level1_02( );
	void process_events( float dt );
};

class Level1_02_1 : public Level {
  public:
	Level1_02_1( float r = 1.f, int entrance = 0, Alonebot* p = 0, FMOD::System* fsys = 0, FMOD::Channel* sfx = 0 );
	~Level1_02_1( );
	void process_events( float dt );
};

class Level1_03 : public Level {
  public:
	  Level1_03( float r = 1.f, int entrance = 0, Alonebot* p = 0, FMOD::System* fsys = 0, FMOD::Channel* sfx = 0 );
	  ~Level1_03( );
	  void process_events( float dt );
};

class Level1_04 : public Level {
  public:
	  Level1_04( float r = 1.f, int entrance = 0, Alonebot* p = 0, FMOD::System* fsys = 0, FMOD::Channel* sfx = 0 );
	  ~Level1_04( );
	  void process_events( float dt );
};

class Level1_05 : public Level {
  private:
	static const int MIN_MEDUSAE_DELAY = 1500;	// Time in ms between spawns
	static const int MAX_MEDUSAE_DELAY = 2000;
	int medusaeTime;							// Used to measure pauses between medusae spawn

  public:
	  Level1_05( float r = 1.f, int entrance = 0, Alonebot* p = 0, FMOD::System* fsys = 0, FMOD::Channel* sfx = 0 );
	  ~Level1_05( );
	  void process_events( float dt );
};

class Level1_06 : public Level {
  public:
	  Level1_06( float r = 1.f, int entrance = 0, Alonebot* p = 0, FMOD::System* fsys = 0, FMOD::Channel* sfx = 0 );
	  ~Level1_06( );
	  void process_events( float dt );
};

class Level1_07 : public Level {
  private:
	static const int MIN_MEDUSAE_DELAY = 1500;	// Time in ms between spawns
	static const int MAX_MEDUSAE_DELAY = 2000;
	int medusaeTime;							// Used to measure pauses between medusae spawn

  public:
	  Level1_07( float r = 1.f, int entrance = 0, Alonebot* p = 0, FMOD::System* fsys = 0, FMOD::Channel* sfx = 0 );
	  ~Level1_07( );
	  void process_events( float dt );
};

class Level1_08 : public Level {
  private:
	static const int MIN_MEDUSAE_DELAY = 1500;	// Time in ms between spawns
	static const int MAX_MEDUSAE_DELAY = 2000;
	int medusaeTime;							// Used to measure pauses between medusae spawn

  public:
	  Level1_08( float r = 1.f, int entrance = 0, Alonebot* p = 0, FMOD::System* fsys = 0, FMOD::Channel* sfx = 0 );
	  ~Level1_08( );
	  void process_events( float dt );
};

class Level1_09 : public Level {
  public:
	  Level1_09( float r = 1.f, int entrance = 0, Alonebot* p = 0, FMOD::System* fsys = 0, FMOD::Channel* sfx = 0 );
	  ~Level1_09( );
	  void process_events( float dt );
};

class Level1_10 : public Level {
  public:
	  Level1_10( float r = 1.f, int entrance = 0, Alonebot* p = 0, FMOD::System* fsys = 0, FMOD::Channel* sfx = 0 );
	  ~Level1_10( );
	  void process_events( float dt );
};

class Level1_11 : public Level {
  public:
	  Level1_11( float r = 1.f, int entrance = 0, Alonebot* p = 0, FMOD::System* fsys = 0, FMOD::Channel* sfx = 0 );
	  ~Level1_11( );
	  void process_events( float dt );
};

class Level1_11_1 : public Level {
  public:
	  Level1_11_1( float r = 1.f, int entrance = 0, Alonebot* p = 0, FMOD::System* fsys = 0, FMOD::Channel* sfx = 0 );
	  ~Level1_11_1( );
	  void process_events( float dt );
};

class Level1_12 : public Level {
  public:
	  Level1_12( float r = 1.f, int entrance = 0, Alonebot* p = 0, FMOD::System* fsys = 0, FMOD::Channel* sfx = 0 );
	  ~Level1_12( );
	  void process_events( float dt );
};