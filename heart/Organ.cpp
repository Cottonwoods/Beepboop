#include "Organ.h"


Organ::Organ( ) {
}

Organ::Organ( const char* path ) {
	init( path );
}

Organ::~Organ( ) {
	glDeleteTextures( 1, &texture );
}

void Organ::init( ) {
}

void Organ::init( const char* path ) {
	texture = LoadTexture( path );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
	defaultHeight = h;
	defaultWidth = w;
	loadVA( w, h );
}

GLuint Organ::getTex( ) {
	return texture;
}

void Organ::loadVA( int w, int h ) {
	vertices[0].x = 0;								// Top left
	vertices[0].y = 0;
	vertices[1].x = w;								// Top rite
	vertices[1].y = 0;
	vertices[2].x = w;								// Bot rite
	vertices[2].y = h;
	vertices[3].x = 0;								// Bot left
	vertices[3].y = h;

	tvertices[0].x = 0;								// Top left
	tvertices[0].y = 0;
	tvertices[1].x = 1;								// Top rite
	tvertices[1].y = 0;
	tvertices[2].x = 1;								// Bot rite
	tvertices[2].y = 1;
	tvertices[3].x = 0;								// Bot left
	tvertices[3].y = 1;
	
	if( vertical == true ) {
		collisionBox[0] = vertices[0];
		collisionBox[1] = Vector2Di( h, 0 );
		collisionBox[2] = Vector2Di( h, w );
		collisionBox[3] = Vector2Di( 0, w );
	}
	else {
		collisionBox[0] = vertices[0];
		collisionBox[1] = vertices[1];
		collisionBox[2] = vertices[2];
		collisionBox[3] = vertices[3];
	}
}

float Organ::dist( Organ* target, bool useX, bool useY ) {
	float x1, x2, y1, y2;
	if( useX ) {
		x1 = pos.x + w / 2.f;
		x2 = target->pos.x + target->w / 2.f;
	}
	if( useY ) {
		y1 = pos.y + h / 2.f;
		y2 = target->pos.y + target->h / 2.f;
	}

	return sqrt( pow( x2 - x1, 2.f ) + pow( y2 - y1, 2.f ) );
}

void Organ::adjustCollideBox( float mult ) {
	int fw, fh;
	fw = (int)(w * mult);
	fh = (int)(h * mult);
	if( vertical == true ) {
		collisionBox[0] += Vector2Di( -fh, -fw );
		collisionBox[1] += Vector2Di( fh, -fw );
		collisionBox[2] += Vector2Di( fh, fw );
		collisionBox[3] += Vector2Di( -fh, fw );
	}
	else {
		collisionBox[0] += Vector2Di( -fw, -fh );
		collisionBox[1] += Vector2Di( fw, -fh );
		collisionBox[2] += Vector2Di( fw, fh );
		collisionBox[3] += Vector2Di( -fw, fh );
	}
}

Vector2D Organ::collides( Organ* target ) {
	// Resolve collision depending on where the object enters
	int xDir = 0;
	int yDir = 0;
	// Difference in projections across the axes
	float dx = 0.f;
	float dy = 0.f;
	
	// Create an array of four corners of rectangle
	Vector2D self[4];
	for( int i=0; i<4; i++ ) {
		self[i].x += collisionBox[i].x;
		self[i].y += collisionBox[i].y;
		self[i] += pos;
	}
	// Same with collision target
	Vector2D targ[4];
	for( int i=0; i<4; i++ ) {
		targ[i].x += target->collisionBox[i].x;
		targ[i].y += target->collisionBox[i].y;
		targ[i] += target->pos;
	}

	// Find min and max on x axis
	float selfMin = min( min( min( self[0].x, self[1].x ), self[2].x ), self[3].x );
	float selfMax = max( max( max( self[0].x, self[1].x ), self[2].x ), self[3].x );
	// Same for target
	float targMin = min( min( min( targ[0].x, targ[1].x ), targ[2].x ), targ[3].x );
	float targMax = max( max( max( targ[0].x, targ[1].x ), targ[2].x ), targ[3].x );
	// Find dx if there is a horizontal collision
	if( selfMax <= targMax && selfMax > targMin ) {
		dx = selfMax - max( selfMin, targMin );
		xDir = -1;
	}
	else if( selfMax > targMax && selfMin < targMax ) {
		dx = targMax - max( selfMin, targMin );
		xDir = 1;
	}

	// Find min and max on y axis
	selfMin = min( min( min( self[0].y, self[1].y ), self[2].y ), self[3].y );
	selfMax = max( max( max( self[0].y, self[1].y ), self[2].y ), self[3].y );
	// Same for target
	targMin = min( min( min( targ[0].y, targ[1].y ), targ[2].y ), targ[3].y );
	targMax = max( max( max( targ[0].y, targ[1].y ), targ[2].y ), targ[3].y );
	// Find dy if there is a vertical collision
	if( selfMax <= targMax && selfMax > targMin ) {
		dy = selfMax - max( selfMin, targMin );
		yDir = -1;
	}
	else if( selfMax > targMax && selfMin < targMax ) {
		dy = targMax - max( selfMin, targMin );
		yDir = 1;
	}
	
	// SAT(Separating Axis Theorem) says if one doesn't collide then neither do
	if( !dx || !dy )
		return Vector2D( );
	
	// Collision resolution with special cases for dropboxes
	if( abs( dx ) < abs( dy ) ) {							// If there is less collision on the x plane
		if( solid && target->solid ) {
		if( movable ) {										// and this object is movable
			if( target->id != 1 ) {
				pos.x += (dx * xDir);						// resolve collision with it
				xspeed = 0.f;
			}
		}
		else if( target->movable )
			target->pos.x += dx;							// or the other, if it is movable
		}
		return Vector2D( min( dx * xDir, 0.1f ), 0.f );		// Return displacement vector x
	}
	else {													// Same thing with y coordinates
		if( solid && target->solid ) {
		if( movable ) {
			if( target->id != 1 ) {
				pos.y += (dy * yDir);
				yspeed = 0.f;
			}
		}
		else if( target->movable )
			target->pos.y -= dy;							// Caw caw.
		}
		return Vector2D( 0.f, min( dy * yDir, 0.1f ) );		// Return displacement vector y
	}
}

void Organ::pause( ) {
	heartBeat.pause( );
}

void Organ::unpause( ) {
	heartBeat.unpause( );
}


// NPCs !!
NPC::NPC( Vector2D p ) {
	pos = p;
	w = 0;
	h = 0;
	vertical = false;
	movable = false;
	solid = true;
	loadVA( 0, 0 );
	heartBeat.start( );
}

NPC::~NPC( ) {
	glDeleteTextures( 1, &texture );
}

void NPC::walk( float dt ) {
}

void NPC::talk( ) {
}


Phatbot::Phatbot( Vector2D p ) {
	pos = p;
	for( int i=0; i<2; i++ ) {
		std::stringstream s;
		s << i;
		anim[i] = LoadTexture( std::string( "..\\anims\\chars\\phatbot\\phatbot_" + s.str() ) + ".png" );
	}
	texture = anim[0];
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
	defaultHeight = h;
	defaultWidth = w;
	vertical = false;
	movable = false;
	solid = false;
	dir = 1;
	loadVA( w, h );
	adjustCollideBox( 0.2f );
	heartBeat.start( );
}

Phatbot::~Phatbot( ) {
	glDeleteTextures( 1, &texture );
	glDeleteTextures( 2, anim );
}

void Phatbot::walk( float dt ) {
	texture = anim[0];
}

void Phatbot::talk( ) {
	texture = anim[1];
}


// Projectiles !!
Projectile::Projectile( bool play, Vector2D p, Vector2D v ) {
	id = 0;
	dmg = 0;
	playerShot = play;
	w = 0;
	h = 0;
	pos = p;
	xspeed = v.x;
	yspeed = v.y;
	vertical = false;
	movable = true;
	solid = true;
	hitTime = 0;
	loadVA( 0, 0 );
	heartBeat.start( );
}

Projectile::~Projectile( ) {
	glDeleteTextures( 1, &texture );
}

void Projectile::walk( float dt, Organ* pl ) {
	// If still active, move
	if( solid ) {
		pos.x += xspeed * dir * dt;
		pos.y += yspeed * dt;
	}
}

void Projectile::hit( ) {
	if( hitTime == 0 )
		hitTime = heartBeat.get_ticks( );
	solid = false;
}


BlasterShot::BlasterShot( bool play, Vector2D p, Vector2D v ) {
	id = 1;
	dmg = 1;
	playerShot = play;
	pos = p;
	xspeed = v.x;
	yspeed = v.y;
	texture = LoadTexture( std::string( "..\\anims\\projectiles\\blaster\\blaster_shot.png" ) );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
	defaultHeight = h;
	defaultWidth = w;
	vertical = false;
	movable = true;
	solid = true;
	hitTime = 0;
	for( int i=0; i<4; i++ ) {
		std::stringstream s;
		s << i;
		hitFrames[i] = LoadTexture( std::string( "..\\anims\\projectiles\\blaster\\blaster_hit_" + s.str() ) + ".png" );
	}
	if( abs( yspeed ) > abs( xspeed ) ) {
		dir = (yspeed >= 0)?1:-1;
		vertical = true;
	}
	else
		dir = (xspeed >= 0)?1:-1;
	loadVA( w, h );
	if( !playerShot )
		adjustCollideBox( -0.2f );
	heartBeat.start( );
}

BlasterShot::~BlasterShot( ) {
	glDeleteTextures( 1, &texture );
	glDeleteTextures( 4, hitFrames );
}

void BlasterShot::walk( float dt, Organ* pl ) {
	// If still active, move
	if( solid ) {
		pos.x += xspeed * dt;
		pos.y += yspeed * dt;
	}
	// Else, perform hit animation logic
	else {
		int d = heartBeat.get_ticks( ) - hitTime;
		int temp = 0;
		if( d > 50 )
			temp++;
			if( d > 100 )
				temp++;
				if( d > 150 )
					temp++;
					if( d > 200 )
						movable = false;
		texture = hitFrames[temp];
	}
}

void BlasterShot::hit( ) {
	if( hitTime == 0 )
		hitTime = heartBeat.get_ticks( );
	solid = false;
	pos.x -= 3;
	pos.y -= 5;
	texture = hitFrames[0];
	glBindTexture( GL_TEXTURE_2D, texture );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
	loadVA( w, h );
	if( vertical )
		pos.y += dir * h / 2;
	else
		pos.x += dir * w / 2;
}


SwordSlash::SwordSlash( bool play, int d, Vector2D p, Vector2D v ) {
	id = 2;
	dmg = 2;
	playerShot = play;
	pos = p;
	xspeed = v.x;
	yspeed = v.y;
	texture = LoadTexture( std::string( "..\\anims\\projectiles\\sword\\sword_shot.png" ) );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
	defaultHeight = h;
	defaultWidth = w;
	vertical = false;
	movable = true;
	solid = true;
	hitTime = 0;
	dir = d;
	xspeed *= dir;
	for( int i=0; i<4; i++ ) {
		std::stringstream s;
		s << i;
		hitFrames[i] = LoadTexture( std::string( "..\\anims\\projectiles\\sword\\sword_hit_" + s.str() ) + ".png" );
	}
	loadVA( w, h );
	if( !playerShot )
		adjustCollideBox( -0.2f );
	heartBeat.start( );
}

SwordSlash::~SwordSlash( ) {
	glDeleteTextures( 1, &texture );
	glDeleteTextures( 4, hitFrames );
}

void SwordSlash::walk( float dt, Organ* pl ) {
	if( pl != 0 ) {
		dir = pl->dir;
		pos = pl->pos;
		pos += Vector2D( pl->dir*65.f, 20.f );
		if( pl->dir == 1 )
			pos.x += pl->w - 52;
	}
	// If still active, move
	if( solid ) {
		if( heartBeat.get_ticks( ) > 175 )
			movable = false;
	}
	// Else, perform hit animation logic
	else {
		int d = heartBeat.get_ticks( ) - hitTime;
		int temp = 0;
		if( d > 20 )
			temp++;
			if( d > 40 )
				temp++;
				if( d > 60 )
					temp++;
					if( d > 80 )
						movable = false;
		texture = hitFrames[temp];
	}
}

void SwordSlash::hit( ) {
	if( hitTime == 0 )
		hitTime = heartBeat.get_ticks( );
	solid = false;
	texture = hitFrames[0];
	glBindTexture( GL_TEXTURE_2D, texture );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
	loadVA( w, h );
}


CannonShot::CannonShot( bool play, Vector2D p, Vector2D v ) {
	id = 3;
	dmg = 1;
	playerShot = play;
	pos = p;
	xspeed = v.x;
	yspeed = v.y;
	texture = LoadTexture( std::string( "..\\anims\\projectiles\\cannon\\cannon_shot.png" ) );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
	defaultHeight = h;
	defaultWidth = w;
	vertical = false;
	movable = true;
	solid = true;
	hitTime = 0;
	for( int i=0; i<4; i++ ) {
		std::stringstream s;
		s << i;
		hitFrames[i] = LoadTexture( std::string( "..\\anims\\projectiles\\cannon\\cannon_hit_" + s.str() ) + ".png" );
	}
	if( abs( yspeed ) > abs( xspeed ) ) {
		dir = (yspeed >= 0)?1:-1;
		vertical = true;
	}
	else
		dir = (xspeed >= 0)?1:-1;
	loadVA( w, h );
	if( !playerShot )
		adjustCollideBox( -0.2f );
	heartBeat.start( );
}

CannonShot::~CannonShot( ) {
	glDeleteTextures( 1, &texture );
	glDeleteTextures( 4, hitFrames );
}

void CannonShot::walk( float dt, Organ* pl ) {
	// If still active, move
	if( solid ) {
		pos.x += xspeed * dt;
		pos.y += yspeed * dt;
	}
	// Else, perform hit animation logic
	else {
		int d = heartBeat.get_ticks( ) - hitTime;
		int temp = 0;
		if( d > 50 )
			temp++;
			if( d > 100 )
				temp++;
				if( d > 150 )
					temp++;
					if( d > 200 )
						movable = false;
		texture = hitFrames[temp];
	}
}

void CannonShot::hit( ) {
	if( hitTime == 0 )
		hitTime = heartBeat.get_ticks( );
	solid = false;
	pos.x -= 1.5f;
	pos.y -= 1.5f;
	texture = hitFrames[0];
	glBindTexture( GL_TEXTURE_2D, texture );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
	loadVA( w, h );
	if( vertical == true )
		pos.y += dir * h / 2;
	else
		pos.x += dir * w / 2;
}


Bone::Bone( bool play, Vector2D p, Vector2D d ) {
	id = 4;
	dmg = 1;
	playerShot = play;
	pos = p;
	xspeed = 0.f;
	yspeed = 0.f;
	texture = LoadTexture( std::string( "..\\anims\\projectiles\\bone\\bone_shot.png" ) );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
	defaultHeight = h;
	defaultWidth = w;
	vertical = false;
	movable = true;
	solid = true;
	hitTime = 0;
	for( int i=0; i<4; i++ ) {
		std::stringstream s;
		s << i;
		hitFrames[i] = LoadTexture( std::string( "..\\anims\\projectiles\\bone\\bone_hit_" + s.str() ) + ".png" );
	}
	dir = 0;
	loadVA( w, h );
	if( !playerShot )
		adjustCollideBox( -0.2f );
	dest = d;
	start = p;
	dest.y -= h;
	start.y -= h /2.f;
	xspeed = dest.x - start.x;
	dir = (xspeed >= 0)?1:-1;
	xspeed = dir * 180.f;
	if( dir == 1 ) {
		start.x += w;
		pos.x += w;
	}
	heartBeat.start( );
}

Bone::~Bone( ) {
	glDeleteTextures( 1, &texture );
	glDeleteTextures( 4, hitFrames );
}

void Bone::walk( float dt, Organ* pl ) {
	// If still active, move
	if( solid ) {
		pos.x += xspeed * dt;

		float mid = ( start.x + dest.x ) / 2.f;
		if( mid > start.x )
			yspeed = pos.x - mid;
		else
			yspeed = mid - pos.x;
		pos.y += yspeed * dt * 2.5f;
	}
	// Else, perform hit animation logic
	else {
		int d = heartBeat.get_ticks( ) - hitTime;
		int temp = 0;
		if( d > 50 )
			temp++;
			if( d > 100 )
				temp++;
				if( d > 150 )
					temp++;
					if( d > 200 )
						movable = false;
		texture = hitFrames[temp];
	}
}

void Bone::hit( ) {
	if( hitTime == 0 )
		hitTime = heartBeat.get_ticks( );
	solid = false;
	texture = hitFrames[0];
	glBindTexture( GL_TEXTURE_2D, texture );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
	loadVA( w, h );
}


// Enemies !!
Enemy::Enemy( ) {
	w = 0;
	h = 0;
	pos = Vector2D( );
	vertical = false;
	movable = false;
	solid = false;
	damage = 0;
	maxHP = 0;
	currentHP = 0;
	flags = 0;
	loadVA( 0, 0 );
	heartBeat.start( );
}

Enemy::~Enemy( ) {
}

void Enemy::hurt( int d = 0, int dmg = 0 ) {
	currentHP -= dmg;
	if( currentHP <= 0 && ( flags & ENEMY_DYING ) == false ) {
		flags |= ENEMY_DYING;
		hitTime = heartBeat.get_ticks( );
	}
}

void Enemy::walk( float dt ) {
}

void Enemy::alert( Organ* pl ) {
}


Medusa::Medusa( Vector2D p, int d ) {
	id = 1;
	pos = p;
	dir = d;
	xspeed = 115;
	yspeed = 115;
	vertical = false;
	movable = false;
	solid = true;
	damage = 1;
	maxHP = 1;
	currentHP = 1;
	flags = 0;
	
	for( int i=0; i<3; i++ ) {					// Load walking animations
		std::stringstream s;
		s << i;
		walkFrames[i] = LoadTexture( std::string( "..\\anims\\enemies\\medusa\\medusa_" + s.str() ) + ".png" );
	}
	for( int i=0; i<2; i++ ) {					// And dying animations
		std::stringstream s;
		s << i;
		hitFrames[i] = LoadTexture( std::string( "..\\anims\\enemies\\medusa\\medusa_hit_" + s.str() ) + ".png" );
	}
	texture = walkFrames[0];
	glBindTexture( GL_TEXTURE_2D, texture );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
	defaultHeight = h;
	defaultWidth = w;
	loadVA( w, h );
	adjustCollideBox( -0.2f );
	heartBeat.start( );
}

Medusa::~Medusa( ) {
	glDeleteTextures( 1, &texture );
	glDeleteTextures( 3, walkFrames );
	glDeleteTextures( 2, hitFrames );
}

void Medusa::walk( float dt ) {
	// If still active, move
	if( ( flags & ENEMY_DYING ) == false ) {
		pos.x += xspeed * dir * dt;
		pos.y += sin( pos.x / 16.f ) * yspeed * dt;								// Number is amplitude

		texture = walkFrames[(heartBeat.get_ticks( ) % ( 300 * 3 ) ) / 300];	// Cycle through every 300ms
	}
	// Else, perform hit animation logic
	else {
		solid = false;
		pos.y += 60.f * dt;
		int d = heartBeat.get_ticks( ) - hitTime;
		int temp = 0;
		if( d > 150 )
			temp++;
			if( d > 300 )
				flags |= ENEMY_DEAD;
		texture = hitFrames[temp];
	}
}


Runner::Runner( Vector2D p, int d ) {
	id = 2;
	pos = p;
	dir = d;
	xspeed = 0;
	yspeed = 0;
	vertical = false;
	movable = true;
	solid = true;
	damage = 2;
	maxHP = 2;
	currentHP = 2;
	flags = 0;
	alerted = false;
	walkTime = 0.01f;
	waitTime = 0.f;
	alert_range_y = 30;
	
	for( int i=0; i<12; i++ ) {									// Load walking animations
		std::stringstream s;
		s << i;
		walkFrames[i] = LoadTexture( std::string( "..\\anims\\enemies\\runner\\runner_" + s.str() ) + ".png" );
	}
	for( int i=0; i<3; i++ ) {									// And dying animations
		std::stringstream s;
		s << i;
		hitFrames[i] = LoadTexture( std::string( "..\\anims\\enemies\\runner\\runner_hit_" + s.str() ) + ".png" );
	}
	texture = walkFrames[0];
	glBindTexture( GL_TEXTURE_2D, texture );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
	defaultHeight = h;
	defaultWidth = w;
	loadVA( w, h );
	adjustCollideBox( -0.2f );
}

Runner::~Runner( ) {
	glDeleteTextures( 1, &texture );
	glDeleteTextures( 12, walkFrames );
	glDeleteTextures( 3, hitFrames );
}

void Runner::walk( float dt ) {
	// If still active, move
	if( ( flags & ENEMY_DYING ) == false ) {
		if( alerted ) {											// Alerted to player
			if( xspeed < MAX_X_SPEED ) {
				xspeed += X_ACCEL * dt;
			}
			if( xspeed > MAX_X_SPEED )
				xspeed = MAX_X_SPEED;
		}
		else {													// Roaming AI
			if( waitTime > 0.f ) {								// Wait if there is time left
				waitTime = waitTime - dt;
				
				if( waitTime <= 0.f ) {							// Switch to walk when you run out
					walkTime = ( rand( ) % 150 + 50.f ) * dt;	// 50 - 200 ms
					waitTime = 0.f;
					if( !heartBeat.is_started( ) )
						heartBeat.start( );
					dir *= -1;
					xspeed = MAX_WALK_SPEED / 2.f;
				}
			}

			if( walkTime > 0.f ) {								// Walk if there is time left
				if( xspeed < MAX_WALK_SPEED )
					xspeed += X_ACCEL * 0.5f * dt;
				if( xspeed > MAX_WALK_SPEED )
					xspeed = MAX_WALK_SPEED;
				walkTime = walkTime - dt;

				if( walkTime <= 0.f ) {							// Switch to wait when you run out
					waitTime = ( rand( ) % 250 + 50.f ) * dt;	// 50 - 300 ms
					walkTime = 0.f;
					if( heartBeat.is_started( ) )
						heartBeat.stop( );
				}
			}
			else {
				float fr = 0.56f;								// Friction between movement cycles
				xspeed *= 1.0f - min( dt * X_ACCEL * fr, fr );
			}
		}
		pos.x += xspeed * dir * dt;

		// Cycle through every Xms, where X is maxFrameTime - maxFrameTime * 2 based off movespeed
		int maxFrameTime = 40;
		float t = 1000.f * maxFrameTime;
		if( alerted )
			t = t * ( (float)MAX_X_SPEED / xspeed );
		else
			t = t * ( (float)MAX_WALK_SPEED / xspeed ) * 2.f;

		if( xspeed != 0 )
			texture = walkFrames[ (int)( ( (int)(heartBeat.get_ticks( ) * 1000.f ) ) % ( (int)( t * 12 ) ) / t ) ];
		else
			texture = walkFrames[0];
	}
	// Else, perform hit animation logic
	else {
		if( !heartBeat.is_started( ) ) {
			heartBeat.start( );
			hitTime = 0;
		}
		solid = false;
		int d = heartBeat.get_ticks( ) - hitTime;
		int temp = 0;
		if( d > 100 )
			temp++;
			if( d > 200 )
				temp++;
				if( d > 300 )
					flags |= ENEMY_DEAD;
		texture = hitFrames[temp];
	}
}

void Runner::alert( Organ* pl ) {
	float side = ( pos.x + w / 2.f ) - ( pl->pos.x + pl->w / 2.f );
	side = side + abs( side );									// Math magic determines if it is facing player
	side = ( side == 0.f )?1.f:-1.f;
	
	if( dist( pl, true, false ) < ALERT_RANGE_X ) {				// <260 units away on x axis
		if( dist( pl, false, true ) < alert_range_y ) {			// <30 units away on y axis, 80 when alerted
			if( (int)side == dir ) {
				alerted = true;
				alert_range_y = 80;
				if( !heartBeat.is_started( ) )
					heartBeat.start( );
				return;
			}
		}
	}
	if( alerted ) {
		alerted = false;
		waitTime = 0.f;
		walkTime = 0.01f;
		alert_range_y = 30;
		if( heartBeat.is_started( ) )
			heartBeat.stop( );
	}
}


Shooter::Shooter( Vector2D p, int d, int del, bool vert ) {
	id = 3;
	pos = p;
	dir = d;
	xspeed = 0.f;
	yspeed = 0.f;
	vertical = vert;
	movable = false;
	solid = true;
	damage = 1;
	maxHP = 4;
	currentHP = 4;
	flags = 0;
	delay = del;
	shotStart = 0;

	for( int i=0; i<1; i++ ) {									// Load walking animations
		std::stringstream s;
		s << i;
		walkFrames[i] = LoadTexture( std::string( "..\\anims\\enemies\\shooter\\shooter_" + s.str() ) + ".png" );
	}
	for( int i=0; i<2; i++ ) {									// And dying animations
		std::stringstream s;
		s << i;
		hitFrames[i] = LoadTexture( std::string( "..\\anims\\enemies\\shooter\\shooter_hit_" + s.str() ) + ".png" );
	}
	texture = walkFrames[0];
	glBindTexture( GL_TEXTURE_2D, texture );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
	defaultHeight = h;
	defaultWidth = w;
	loadVA( w, h );
	adjustCollideBox( -0.2f );
	heartBeat.start( );
}

Shooter::~Shooter( ) {
	glDeleteTextures( 1, &texture );
	glDeleteTextures( 1, walkFrames );
	glDeleteTextures( 2, hitFrames );
}

void Shooter::walk( float dt ) {
	if( ( flags & ENEMY_DYING ) == false ) {					// If still active ...
		if( delay <= heartBeat.get_ticks( ) ) {					//  And you can begin shooting ...
		if( shotStart <= heartBeat.get_ticks( ) ) {				//   And you aren't on cooldown ...
			shotStart = heartBeat.get_ticks( ) + COOLDOWN;		//    Shoot
			flags |= ENEMY_SHOOTING;
		} }
	}
	else {														// Else, perform hit animation logic
		solid = false;
		int d = heartBeat.get_ticks( ) - hitTime;
		int temp = 0;
		if( d > 150 )
			temp++;
			if( d > 300 )
				flags |= ENEMY_DEAD;
		texture = hitFrames[temp];
	}
}


Sawblade::Sawblade( int dist, Vector2D p, int d, bool vert ) {
	id = 0;
	pos = p;
	dir = d;
	xspeed = 190.f;
	yspeed = 190.f;
	vertical = vert;
	movable = false;
	solid = true;
	damage = 1;
	maxHP = 1;
	currentHP = 1;
	flags = 0;
	
	for( int i=0; i<4; i++ ) {									// Load walking animations
		std::stringstream s;
		s << i;
		walkFrames[i] = LoadTexture( std::string( "..\\anims\\enemies\\sawblade\\sawblade_" + s.str() ) + ".png" );
	}
	texture = walkFrames[0];
	glBindTexture( GL_TEXTURE_2D, texture );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
	defaultHeight = h;
	defaultWidth = w;
	if( vertical ) {
		start = (int)pos.y;
		distance = dist - h;
		if( dir == -1 ) {
			pos.y -= h;
			start -= ( distance + h );
		}
	}
	else {
		start = (int)pos.x;
		distance = dist - w;
		if( dir == -1 ) {
			pos.x -= w;
			start -= ( distance + w );
		}
	}
	loadVA( w, h );
	adjustCollideBox( -0.2f );
	heartBeat.start( );
}

Sawblade::~Sawblade( ) {
	glDeleteTextures( 1, &texture );
	glDeleteTextures( 4, walkFrames );
}

void Sawblade::hurt( int d, int dmg ) {
	return;
}

void Sawblade::walk( float dt ) {
	if( solid ) {												// If active ...
		texture = walkFrames[(heartBeat.get_ticks( ) % ( 100 * 4 ) ) / 100];
		int test = dir * start;
		if( dir == 1 )
			test += distance;

		if( vertical ) {
			if( dir * pos.y < test ) {
				pos.y += yspeed * dt * dir;
			}
			else {
				pos.y = (float)start;
				if( dir == 1 )
					pos.y += distance;
				dir *= -1;
			}
		}
		else {
			if( dir * pos.x < test ) {
				pos.x += xspeed * dt * dir;
			}
			else {
				pos.x = (float)start;
				if( dir == 1 )
					pos.x += distance;
				dir *= -1;
			}
		}
	}
}


Skeleton::Skeleton( Vector2D p, int d ) {
	id = 4;
	pos = p;
	dir = d;
	xspeed = 0;
	yspeed = 0;
	vertical = false;
	movable = true;
	solid = true;
	damage = 1;
	maxHP = 2;
	currentHP = 2;
	flags = 0;
	alerted = false;
	walkTime = 0.01f;
	waitTime = 0.f;
	
	for( int i=0; i<1; i++ ) {									// Load walking animations
		std::stringstream s;
		s << i;
		walkFrames[i] = LoadTexture( std::string( "..\\anims\\enemies\\skeleton\\skeleton_" + s.str() ) + ".png" );
	}
	for( int i=0; i<1; i++ ) {									// And dying animations
		std::stringstream s;
		s << i;
		hitFrames[i] = LoadTexture( std::string( "..\\anims\\enemies\\skeleton\\skeleton_" + s.str() ) + ".png" );
	}
	texture = walkFrames[0];
	glBindTexture( GL_TEXTURE_2D, texture );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
	defaultHeight = h;
	defaultWidth = w;
	loadVA( w, h );
	adjustCollideBox( -0.1f );
}

Skeleton::~Skeleton( ) {
	glDeleteTextures( 1, &texture );
	glDeleteTextures( 1, walkFrames );
	glDeleteTextures( 1, hitFrames );
}

void Skeleton::walk( float dt ) {
	// If still active, move
	if( ( flags & ENEMY_DYING ) == false ) {
		if( alerted ) {											// Alerted to player
			if( shotStart <= heartBeat.get_ticks( ) ) {			//  And you aren't on cooldown ...
				shotStart = heartBeat.get_ticks( ) + COOLDOWN;	//   Throw bows
				flags |= ENEMY_SHOOTING;
			}
			xspeed = 0.f;
		}
		else {													// Roaming AI
			if( waitTime > 0.f ) {								// Wait if there is time left
				waitTime = waitTime - dt;
				
				if( waitTime <= 0.f ) {							// Switch to walk when you run out
					walkTime = ( rand( ) % 150 + 50.f ) * dt;	// 50 - 200 ms
					waitTime = 0.f;
					if( !heartBeat.is_started( ) )
						heartBeat.start( );
					dir *= -1;
					xspeed = MAX_WALK_SPEED / 2.f;
				}
			}

			if( walkTime > 0.f ) {								// Walk if there is time left
				if( xspeed < MAX_WALK_SPEED )
					xspeed += X_ACCEL * 0.5f * dt;
				if( xspeed > MAX_WALK_SPEED )
					xspeed = MAX_WALK_SPEED;
				walkTime = walkTime - dt;

				if( walkTime <= 0.f ) {							// Switch to wait when you run out
					waitTime = ( rand( ) % 250 + 50.f ) * dt;	// 50 - 300 ms
					walkTime = 0.f;
					if( heartBeat.is_started( ) ) {
						shotStart -= heartBeat.get_ticks( );
						heartBeat.stop( );
					}
				}
			}
			else {
				float fr = 0.56f;							// Friction between movement cycles
				xspeed *= 1.0f - min( dt * X_ACCEL * fr, fr );
			}
		}
		pos.x += xspeed * dir * dt;
	}
	// Else, perform hit animation logic
	else {
		if( !heartBeat.is_started( ) ) {
			heartBeat.start( );
			hitTime = 0;
		}
		solid = false;
		int d = heartBeat.get_ticks( ) - hitTime;
		int temp = 0;
//		if( d > 100 )
//			temp++;
//			if( d > 200 )
//				temp++;
//				if( d > 300 )
					flags |= ENEMY_DEAD;
		texture = hitFrames[temp];
	}
}

void Skeleton::alert( Organ* pl ) {
	float side = ( pos.x + w / 2.f ) - ( pl->pos.x + pl->w / 2.f );
	side = side + abs( side );									// Math magic determines if it is facing player
	side = ( side == 0.f )?1.f:-1.f;
	
	if( dist( pl, true, false ) < ALERT_RANGE_X ) {				// <100 units away on x axis
		if( dist( pl, false, true ) < ALERT_RANGE_Y ) {			// <80 units away on y axis
			if( (int)side == dir ) {
				alerted = true;
				if( !heartBeat.is_started( ) ) {
					heartBeat.start( );
				}
				return;
			}
		}
	}
	if( alerted ) {
		alerted = false;
		waitTime = 0.f;
		walkTime = 0.01f;
		if( heartBeat.is_started( ) ) {
			shotStart -= heartBeat.get_ticks( );
			heartBeat.stop( );
		}
	}
}


Zombie::Zombie( int dist, Vector2D p, int d ) {
	pos = p;
	dir = d;
	xspeed = 120.f;
	yspeed = 0.f;
	vertical = false;
	movable = false;
	solid = true;
	damage = 1;
	maxHP = 2;
	currentHP = 2;
	flags = 0;
	
	for( int i=0; i<7; i++ ) {									// Load rising animations
		std::stringstream s;
		s << i;
		riseFrames[i] = LoadTexture( std::string( "..\\anims\\enemies\\zombie\\zombie_" + s.str() ) + ".png" );
	}
	for( int i=0; i<4; i++ ) {									// Load walking animations
		std::stringstream s;
		s << ( i + 6 );
		walkFrames[i] = LoadTexture( std::string( "..\\anims\\enemies\\zombie\\zombie_" + s.str() ) + ".png" );
	}
	for( int i=0; i<2; i++ ) {									// Load dying animations
		std::stringstream s;
		s << i;
		hitFrames[i] = LoadTexture( std::string( "..\\anims\\enemies\\zombie\\zombie_hit_" + s.str() ) + ".png" );
	}
	texture = riseFrames[0];
	glBindTexture( GL_TEXTURE_2D, texture );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
	w *= 2;
	h *= 2;
	defaultHeight = h;
	defaultWidth = w;
	start = (int)(pos.x + w );
	end = 0;
	distance = dist - w * 2;
	if( distance < 0 )
		distance = 0;
	loadVA( w, h );
	adjustCollideBox( -0.2f );
	heartBeat.start( );
}

Zombie::~Zombie( ) {
	glDeleteTextures( 1, &texture );
	glDeleteTextures( 7, riseFrames );
	glDeleteTextures( 4, walkFrames );
	glDeleteTextures( 2, hitFrames );
}

void Zombie::walk( float dt ) {
	int t = heartBeat.get_ticks( );
	if( t < 700 ) {												// If still rising ...
		int f = t / 100;
		texture = riseFrames[f];
		solid = false;
	}
	else if( ( flags & ENEMY_DYING ) == false ) {				// If still active ...
		solid = true;
		if( dir * pos.x < dir * start + distance ) {
			pos.x += xspeed * dt * dir;
			texture = walkFrames[(heartBeat.get_ticks( ) % ( 90 * 4 ) ) / 90];
		}
		else {
			solid = false;
			if( end == 0 )
				end = t;
			if( t - end < 700 ) {
				texture = riseFrames[7 - (t - end ) / 100];
			}
			else
				flags |= ENEMY_DEAD;
		}
	}
	else {														// Else, perform hit animation logic
		int d = heartBeat.get_ticks( ) - hitTime;
		int temp = 0;
		if( d > 100 )
			temp++;
			if( d > 200 )
				flags |= ENEMY_DEAD;
		texture = hitFrames[temp];
	}
}


// Items !!
Item::Item( Vector2D p ) {
	w = 0;
	h = 0;
	id = 0;
	pos = p;
	vertical = false;
	movable = false;
	equip = false;
	solid = true;
	loadVA( 0, 0 );
}

Item::~Item( ) {
}


Blaster::Blaster( Vector2D p ) {
	pos = p;
	vertical = false;
	movable = false;
	solid = true;
	equip = true;
	id = 1;
	dir = 1;
	
	texture = LoadTexture( std::string( "..\\anims\\weapons\\blaster\\blaster_ground.png" ) );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
	defaultHeight = h;
	defaultWidth = w;
	loadVA( w, h );
	adjustCollideBox( 0.2f );
}

Blaster::~Blaster( ) {
	glDeleteTextures( 1, &texture );
}


Sword::Sword( Vector2D p ) {
	pos = p;
	vertical = false;
	movable = false;
	solid = true;
	equip = true;
	id = 2;
	dir = 1;
	
	texture = LoadTexture( std::string( "..\\anims\\weapons\\sword\\sword_ground.png" ) );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
	defaultHeight = h;
	defaultWidth = w;
	loadVA( w, h );
	adjustCollideBox( 0.2f );
}

Sword::~Sword( ) {
	glDeleteTextures( 1, &texture );
}


HeartUp::HeartUp( Vector2D p, int i ) {
	pos = p;
	vertical = false;
	movable = false;
	solid = true;
	equip = false;
	id = i;
	dir = 1;
	
	texture = LoadTexture( std::string( "..\\anims\\hp\\hp_up.png" ) );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
	defaultHeight = h;
	defaultWidth = w;
	loadVA( w, h );
	adjustCollideBox( 0.2f );
}

HeartUp::~HeartUp( ) {
	glDeleteTextures( 1, &texture );
}