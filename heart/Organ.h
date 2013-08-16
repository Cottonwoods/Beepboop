#include "Utilities.h"
#include "Vector2D.h"


class Organ {
  protected:
	GLuint texture;
	Timer heartBeat;

  public:
	int w, h, dir, id;
	float xspeed, yspeed;
	bool movable, solid, vertical;
	int defaultWidth, defaultHeight;
	Vector2D pos, vertices[4], tvertices[4];
	Vector2D collisionBox[4];

	Organ( );
	Organ( const char* path );
	virtual ~Organ( );

	virtual void init( );
	void init( const char* path );
	virtual GLuint getTex( );

	void loadVA( float w, float h );
	float dist( Organ* target, bool useX = true, bool useY = true );
	void adjustCollideBox( float mult = 0.f );
	Vector2D collides( Organ* target );

	void pause( );
	void unpause( );
};


class NPC : public Organ {
  public:
	NPC( Vector2D p = Vector2D( ) );
	virtual ~NPC( );

	virtual void walk( float dt );
	virtual void talk( );
};

class Phatbot : public NPC {
  private:
	GLuint anim[2];
  
  public:
	Phatbot( Vector2D p = Vector2D( ) );
	~Phatbot( );

	void walk( float dt );
	void talk( );
};


class Projectile : public Organ {
  protected:
	int hitTime;
	GLuint* hitFrames;

  public:
	int dmg;
	bool playerShot;

	Projectile( bool play = false, Vector2D p = Vector2D( ), Vector2D v = Vector2D( ) );
	virtual ~Projectile( );

	virtual void walk( float dt, Organ* pl = 0 );
	virtual void hit( );
};

class BlasterShot : public Projectile {
  private:
	GLuint hitFrames[4];

  public:
	BlasterShot( bool play = false, Vector2D p = Vector2D( ), Vector2D v = Vector2D( ) );
	~BlasterShot( );

	void walk( float dt, Organ* pl = 0 );
	void hit( );
};

class SwordSlash : public Projectile {
  private:
	GLuint hitFrames[4];

  public:
	SwordSlash( bool play = false, int d = 1, Vector2D p = Vector2D( ), Vector2D v = Vector2D( ) );
	~SwordSlash( );

	void walk( float dt, Organ* pl = 0 );
	void hit( );
};

class CannonShot : public Projectile {
  private:
	GLuint hitFrames[4];

  public:
	CannonShot( bool play = false, Vector2D p = Vector2D( ), Vector2D v = Vector2D( ) );
	~CannonShot( );

	void walk( float dt, Organ* pl = 0 );
	void hit( );
};

class Bone : public Projectile {
  private:
	GLuint hitFrames[4];
	Vector2D start, dest;

  public:
	Bone( bool play = false, Vector2D p = Vector2D( ), Vector2D d = Vector2D( ) );
	~Bone( );

	void walk( float dt, Organ* pl = 0 );
	void hit( );
};


class Enemy : public Organ {
  protected:
	GLuint* walkFrames;
	GLuint* hitFrames;
	int hitTime;

  public:
	static const int ENEMY_DYING = 1;
	static const int ENEMY_DEAD = 2;
	static const int ENEMY_SHOOTING = 4;
	
	int damage, maxHP, currentHP, flags;

	Enemy( );
	virtual ~Enemy( );

	virtual void hurt( int d, int dmg );
	virtual void walk( float dt );
	virtual void alert( Organ* pl );
};

class Medusa : public Enemy {
  public:
	GLuint walkFrames[3];
	GLuint hitFrames[2];

	Medusa( Vector2D p = Vector2D( ), int d = 1 );
	~Medusa( );

	void walk( float dt );
};

class Runner : public Enemy {
  private:
	static const int X_ACCEL = 220;
	static const int MAX_X_SPEED = 270;
	static const int MAX_WALK_SPEED = 70;
	static const int ALERT_RANGE_X = 260;

	bool alerted;
	int alert_range_y;
	float waitTime, walkTime;

  public:
	GLuint walkFrames[12];
	GLuint hitFrames[3];

	Runner( Vector2D p = Vector2D( ), int d = 1 );
	~Runner( );

	void walk( float dt );
	void alert( Organ* pl );
};

class Shooter : public Enemy {							// Can be used as a base for other shooters
  private:
	static const int COOLDOWN = 1800;

  public:
	int shotStart, delay;

	GLuint walkFrames[1];
	GLuint hitFrames[2];

	Shooter( Vector2D p = Vector2D( ), int d = 1, int delay = 0, bool vert = false );
	~Shooter( );

	void walk( float dt );
};

class Sawblade : public Enemy {
  public:
	bool vertical;
	int distance, start;
	
	GLuint walkFrames[4];
	GLuint hitFrames[1];

	Sawblade( int dist = 0, Vector2D p = Vector2D( ), int d = 1, bool vert = false );
	~Sawblade( );

	void hurt( int d, int dmg );
	void walk( float dt );
};

class Skeleton : public Enemy {
  private:
	static const int X_ACCEL = 140;
	static const int COOLDOWN = 1500;
	static const int MAX_WALK_SPEED = 40;
	static const int ALERT_RANGE_X = 180;
	static const int ALERT_RANGE_Y = 100;

	bool alerted;
	float waitTime, walkTime;

  public:
	int shotStart;

	GLuint walkFrames[1];
	GLuint hitFrames[1];

	Skeleton( Vector2D p = Vector2D( ), int d = 1 );
	~Skeleton( );

	void walk( float dt );
	void alert( Organ* pl );
};

class Zombie : public Enemy {
  public:
	int distance, start, end;

	GLuint riseFrames[7];
	GLuint walkFrames[4];
	GLuint hitFrames[2];

	Zombie( int dist = 0, Vector2D p = Vector2D( ), int d = 1 );
	~Zombie( );

	void walk( float dt );
};


class Item : public Organ {
  public:
	int id;
	bool equip;

	Item( Vector2D p = Vector2D( ) );
	virtual ~Item( );
};

class Blaster : public Item {
  public:
	Blaster( Vector2D p = Vector2D( ) );
	~Blaster( );
};

class Sword : public Item {
  public:
	Sword( Vector2D p = Vector2D( ) );
	~Sword( );
};

class HeartUp : public Item {
  public:
	HeartUp( Vector2D p = Vector2D( ), int i = 100 );
	~HeartUp( );
};