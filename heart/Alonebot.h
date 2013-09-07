#include "Organ.h"


class Alonebot : public Organ {
  private:
	static const int COURAGEDURATION = 1000;
	static const int COURAGETIMEOUT = 250;

	GLuint HP[7];
	GLuint arm[1];
	GLuint charge[3];
	GLuint crouchFrame;
	GLuint hitFrames[6];
	GLuint idleFrames[4];
	GLuint jumpFrames[4];

	GLuint* weapFrames[3];
	GLuint blstFrames[3];
	GLuint swrdFrames[3];
	GLuint* shotFrames;

	float MAX_FALL_SPEED, MAX_X_SPEED, X_ACCEL;
	int blinkMod;

  public:
	static const int MAX_ROOMS = 17;

	bool visited[MAX_ROOMS];
	Vector2D armOffset;
	std::set<int> items;
	std::set<int> equips;
	int maxHP, currentHP;
	int weapon, offHandWeap, chargeMax;
	float jumpSpeed, chargeCur, chargeInc;
	Timer jumpTime, courage, idle, chargeTime;
	int jumpStart, shotStart, crouchStart, deathStart;
	bool walking, jumping, crouching, canJump, canDrop, drop, canDmg;
	FMOD::Sound *jumpSFX, *landSFX, *hurtSFX, *doorSFX, *itemSFX, *blstSFX, *swrdSFX;
	
	Alonebot( float x = 0.f, float y = 0.f, int d = 1, Alonebot* p = 0 );
	~Alonebot( );

	void init( FMOD::System* fmodSys );
	void add( Item* i );
	void jump( );
	void stand( );
	void shoot( );
	void crouch( );
	void drawHP( );
	int getDeathTime( );
	GLuint getTex( );
	GLuint getArm( );
	int getCourage( );
	void hurt( int d, int dmg );
	void walk( float dt );
	void walk( int d );
	void setWeap( int w );
};