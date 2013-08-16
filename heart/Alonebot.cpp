#include "Alonebot.h"


Alonebot::Alonebot( float x, float y, int d, Alonebot* p ) {
	blinkMod = 0;
	jumpStart = 0;
	shotStart = 0;
	MAX_FALL_SPEED = 400.f;
	MAX_X_SPEED = 200.f;
	X_ACCEL = 1030.f;
	movable = true;
	solid = true;
	walking = false;
	jumping = false;
	crouching = false;
	canJump = true;
	canDrop = false;
	drop = false;
	canDmg = true;
	dir = 1;
	xspeed = 0.f;
	yspeed = 1.f;
	armOffset = Vector2D( 0.f, 0.f );

	if( p == 0 || p->currentHP < 1 ) {
		maxHP = 3;													// TODO: Load shit from savefile
		currentHP = 3;
		setWeap( 0 );
		offHandWeap = 0;
		chargeCur = 0.f;
		chargeInc = 0.f;
		chargeMax = 0;
		std::fill( visited, visited + MAX_ROOMS, false );
	}
	else {
		w = p->w;
		h = p->h;
		maxHP = p->maxHP;
		currentHP = p->currentHP;
		setWeap( p->weapon );
		offHandWeap = p->offHandWeap;
		for( std::set<int>::iterator i = p->items.begin(); i != p->items.end(); ++i )
			items.emplace( *i );
		for( std::set<int>::iterator i = p->equips.begin(); i != p->equips.end(); ++i )
			equips.emplace( *i );
		for( int i=0; i<MAX_ROOMS; i++ )
			visited[i] = p->visited[i];
		chargeCur = p->chargeCur;
		chargeInc = p->chargeInc;
		chargeMax = p->chargeMax;
		movable = p->movable;
		solid = p->solid;
		walking = p->walking;
		jumping = p->jumping;
		canJump = p->canJump;
		canDrop = p->canDrop;
		drop = p->drop;
		canDmg = p->canDmg;
		dir = p->dir;
		xspeed = p->xspeed;
		yspeed = p->yspeed;
		armOffset = p->armOffset;
		jumpStart = p->jumpStart;
		jumpTime = p->jumpTime;
		jumpSpeed = p->jumpSpeed;
		courage = p->courage;
	}

	dir = d;
	pos = Vector2D( x, y );
}

Alonebot::~Alonebot( ) {
	glDeleteTextures( 1, &crouchFrame );
	glDeleteTextures( 3, blstFrames );
	glDeleteTextures( 3, swrdFrames );
	glDeleteTextures( 4, jumpFrames );
	glDeleteTextures( 4, idleFrames );
	glDeleteTextures( 6, hitFrames );
	glDeleteTextures( 1, &texture );
	glDeleteTextures( 3, charge );
	glDeleteTextures( 1, arm );
	glDeleteTextures( 4, HP );
	jumpSFX->release( );
	landSFX->release( );
	hurtSFX->release( );
	doorSFX->release( );
	itemSFX->release( );
	blstSFX->release( );
	swrdSFX->release( );
}

void Alonebot::init( FMOD::System* fmodSys ) {
	// Load textures, set width and height
	texture = LoadTexture( std::string( "..\\anims\\chars\\alonebot.png" ) );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
	loadVA( (float)w, (float)h );
	defaultHeight = h;
	defaultWidth = w;
	arm[0] = LoadTexture( std::string( "..\\anims\\weapons\\arm.png" ) );
	for( int i=0; i<5; i++ ) {
		if( i != 2 ) {
			int j = i;
			if( j > 2 )
				j--;
			std::stringstream s;
			s << i;
			idleFrames[j] = LoadTexture( std::string( "..\\anims\\idle\\alonebot_idle_" + s.str() ) + ".png" );
		}
	}
	for( int i=0; i<4; i++ ) {
		std::stringstream s;
		s << i;
		jumpFrames[i] = LoadTexture( std::string( "..\\anims\\jump\\alonebot_jump_flame_" + s.str() ) + ".png" );
	}
	for( int i=0; i<3; i++ ) {
		std::stringstream s;
		s << i;
		blstFrames[i] = LoadTexture( std::string( "..\\anims\\weapons\\blaster\\blaster_" + s.str() ) + ".png" );
		swrdFrames[i] = LoadTexture( std::string( "..\\anims\\weapons\\sword\\sword_" + s.str() ) + ".png" );
	}
	crouchFrame = LoadTexture( std::string( "..\\anims\\crouch\\alonebot_crouch.png" ) );
	for( int i=0; i<4; i++ ) {
		std::stringstream s;
		s << i;
		HP[i] = LoadTexture( std::string( "..\\anims\\hp\\hp_" + s.str() + ".png" ) );
	}
	for( int i=0; i<3; i++ ) {
		std::stringstream s;
		s << i;
		charge[i] = LoadTexture( std::string( "..\\anims\\charge\\charge_" + s.str() + ".png" ) );
	}
	for( int i=0; i<6; i++ ) {
		std::stringstream s;
		s << i;
		hitFrames[i] = LoadTexture( std::string( "..\\anims\\hit\\alonebot_hit_" + s.str() + ".png" ) );
	}

	// Load the player sounds
	jumpSFX = LoadSound( "..\\sounds\\jump.wav", fmodSys );
	landSFX = LoadSound( "..\\sounds\\land.wav", fmodSys );
	hurtSFX = LoadSound( "..\\sounds\\hurt.wav", fmodSys );
	doorSFX = LoadSound( "..\\sounds\\door.wav", fmodSys );
	itemSFX = LoadSound( "..\\sounds\\item.wav", fmodSys );
	swrdSFX = LoadSound( "..\\sounds\\swrd.wav", fmodSys );
	blstSFX = LoadSound( "..\\sounds\\blst.wav", fmodSys );

	// Weapons
	weapFrames[0] = arm;
	weapFrames[1] = blstFrames;
	weapFrames[2] = swrdFrames;
	
	heartBeat.start( );
}

void Alonebot::add( Item* i ) {
	// Add equipment to equipment inventory
	if( i->equip )
		equips.emplace( i->id );
	// Or an item to item inventory
	else {
		items.emplace( i->id );
	// If it's a heart container, increase maxHP and restore to full
		if( i->id >= 100 )
			currentHP = maxHP = maxHP + 1;
	}
}

GLuint Alonebot::getTex( ) {
	int ticks = heartBeat.get_ticks( );
	int temp = 0;

	// Animation
	if( dir == 1 )
		armOffset += Vector2D( 5.f, 22.f );							// Arm offset facing right

	if( !movable ) {												// You dead!
		int dj = heartBeat.get_ticks( ) - deathStart;
		armOffset += Vector2D( -2.f * dir, 0.f );
		if( dj > 125 ) {
			temp++;
			if( dj > 250 ) {
				armOffset += Vector2D( 0.f, 1.f );
				temp++;
				if( dj > 350 ) {
					temp++;
					if( dj > 450 ) {
						armOffset += Vector2D( 1.f * dir, 1.f );
						temp++;
						if( dj > 550 ) {
							temp++;
							if( dj > 650 ) {
								dj -= 650;
								dj /= 100;
								dj %= 2;
								temp -= dj;
							}
						}
					}
				}
			}
		}
		return hitFrames[temp];
	}

	if( jumping ) {													// Jumping arm offset at different frames
		int dj = heartBeat.get_ticks( ) - jumpStart;
		armOffset += Vector2D( 0.f, -1.f );							// 1
		if( dj > 60 ) {
			temp++;													// 2
			if( dj > 120 ) {
				temp++;												// 3
				armOffset += Vector2D( 0.f, -1.f );
				if( dj > 800 ) {
					temp++;											// 4
					armOffset += Vector2D( 0.f, 1.f );
				}
			}
		}
		return jumpFrames[temp];
	}

	if( crouching ) {												// Crouching
		armOffset += Vector2D( 0.f, -1.f );
		return crouchFrame;
	}

	if( !jumping && !canJump ) {									// Falling
		return jumpFrames[3];
	}

	// Blinking controls, only affect idle animations at the moment
	if( ticks > ( blinkMod + ( rand( ) % 4700 ) + 5700 ) )
		blinkMod = ( ticks + ( rand( ) % 30 ) + 120 );
	if( blinkMod > ticks )
		temp += 2;

	// Idle
	int idleTicks = idle.get_ticks( );
	if( idleTicks > 1500 ) {
		temp += ( ( idleTicks - 1500 ) % ( 500 * 2 ) ) / 500;
		if( temp == 1 || temp == 3 )
			armOffset += Vector2D( 0.f, 2.f );
		return idleFrames[temp];
	}

	return idleFrames[temp];										// Default
}

GLuint Alonebot::getArm( ) {
	if( dir == -1 ) {
		armOffset += Vector2D( -13.f, 22.f );						// Adjust offset when switching sides
		glScalef( -1.0, 1.0, 0.f );									// Switch arm direction
	}
	
	int temp = 0;
	glTranslatef( armOffset.x, armOffset.y, 0.f );					// Move arm sprite by offset

	// Shooting animations
	if( weapon == 1 ) {												// For weapon 1 ( Blaster ) ...
		glTranslatef( 0.f, -2.f, 0.f );								//  Slightly move arm to compensate
		if( shotStart != 0 ) {										//  If shooting ...
			if( heartBeat.get_ticks( ) - shotStart < 80 ) {			//   Animate the frames
				temp++;
				if( heartBeat.get_ticks( ) - shotStart > 40 )
					temp++;
			}
			else
				shotStart = 0;										//   Animation is over at 80ms
		}
	}
	else if( weapon == 2 ) {										// For weapon 2 ( Sword ) ...
		glTranslatef( 0.f, -22.f, 0.f );							//  Slightly move arm to compensate
		if( shotStart != 0 ) {										//  If shooting ...
			if( heartBeat.get_ticks( ) - shotStart < 160 ) {		//   Animate the frames
				temp++;
				glTranslatef( 0.f, 20.f, 0.f );
				if( heartBeat.get_ticks( ) - shotStart > 80 ) {
					temp++;
					glTranslatef( 0.f, 0.f, 0.f );
				}
			}
			else
				shotStart = 0;										//   Animation is over at 160ms
		}
	}
	shotFrames = weapFrames[weapon];

	return shotFrames[temp];
}

int Alonebot::getDeathTime( ) {
	if( !movable )
		return heartBeat.get_ticks( ) - deathStart;
	return 0;
}

int Alonebot::getCourage( ) {
	return courage.get_ticks( );									// Return duration of invincibility
}

void Alonebot::drawHP( ) {
	if( weapon != 0 ) {												// Draw the charge bar
		glPushMatrix( );
		glTranslatef( 8.f, 40.f, 0.f );								// Offset
		glBindTexture( GL_TEXTURE_2D, charge[0] );
		float w, h;
		glGetTexLevelParameterfv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
		glGetTexLevelParameterfv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
		glBegin( GL_QUADS );
			glTexCoord2i( 0, 0 );
			glVertex3f( 0.f, 0.f, 0.f );
			glTexCoord2i( 1, 0 );
			glVertex3f( w, 0.f, 0.f );
			glTexCoord2i( 1, 1 );
			glVertex3f( w, h, 0.f );
			glTexCoord2i( 0, 1 );
			glVertex3f( 0.f, h, 0.f );
		glEnd( );
		glTranslatef( 2.f, 2.f, 0.f );
		glBindTexture( GL_TEXTURE_2D, charge[1] );
		if( chargeCur / 100.f == 1 ) {								// Flash on full
			if( !chargeTime.is_started( ) )
				chargeTime.start( );
			if( chargeTime.get_ticks( ) < 50 )
				glBindTexture( GL_TEXTURE_2D, charge[2] );
			else if( chargeTime.get_ticks( ) >= 100 )
				chargeTime.stop( );
		}
		glGetTexLevelParameterfv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
		glGetTexLevelParameterfv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
		if( chargeCur == 0 && chargeTime.is_started( ) ) {			// Cooldown for shooting
			w *= ( (float)chargeMax - chargeTime.get_ticks( ) ) / chargeMax;
			if( weapon == 1 )										//  Hidden for blaster
				w = 0;
		}
		else														// And for charging
			w *= chargeCur / 100.f;
		if( w < 0 )
			w = 0;
		glBegin( GL_QUADS );
			glTexCoord2i( 0, 0 );
			glVertex3f( 0.f, 0.f, 0.f );
			glTexCoord2i( 1, 0 );
			glVertex3f( w, 0.f, 0.f );
			glTexCoord2i( 1, 1 );
			glVertex3f( w, h, 0.f );
			glTexCoord2i( 0, 1 );
			glVertex3f( 0.f, h, 0.f );
		glEnd( );
		glPopMatrix( );
	}
	for( int i=0; i<3; i++ ) {										// Draw HP bar
		if( i == 1 ) {												// Middle portions
			for( int j=0; j<maxHP-2; j++ ) {						// For loop drawing middle portions of HP bar
				glPushMatrix( );
				// Offset
				glTranslatef( 34.f + 20.f*j, 8.f, 0.f );
				glBindTexture( GL_TEXTURE_2D, HP[i] );
				float w, h;
				glGetTexLevelParameterfv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
				glGetTexLevelParameterfv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
				glBegin( GL_QUADS );
					glTexCoord2i( 0, 0 );
					glVertex3f( 0.f, 0.f, 0.f );
					glTexCoord2i( 1, 0 );
					glVertex3f( w, 0.f, 0.f );
					glTexCoord2i( 1, 1 );
					glVertex3f( w, h, 0.f );
					glTexCoord2i( 0, 1 );
					glVertex3f( 0.f, h, 0.f );
				glEnd( );
				glPopMatrix( );
			}
		}
		else {
			glPushMatrix( );
			glTranslatef( 8.f, 8.f, 0.f );							// Offset
			if( i > 0 )
				glTranslatef( 26.f + 20.f*(maxHP-2), 0.f, 0.f );	// Alter transformations for last piece
			glBindTexture( GL_TEXTURE_2D, HP[i] );
			float w, h;
			glGetTexLevelParameterfv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
			glGetTexLevelParameterfv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
			glBegin( GL_QUADS );
				glTexCoord2i( 0, 0 );
				glVertex3f( 0.f, 0.f, 0.f );
				glTexCoord2i( 1, 0 );
				glVertex3f( w, 0.f, 0.f );
				glTexCoord2i( 1, 1 );
				glVertex3f( w, h, 0.f );
				glTexCoord2i( 0, 1 );
				glVertex3f( 0.f, h, 0.f );
			glEnd( );
			glPopMatrix( );
		}
	}

	for( int i=0; i<currentHP; i++ ) {								// Draw hearts
		glPushMatrix( );
		glTranslatef( 14.f + 20.f * i, 8.f, 0.f );					// Offset
		glBindTexture( GL_TEXTURE_2D, HP[3] );
		float w, h;
		glGetTexLevelParameterfv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
		glGetTexLevelParameterfv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );
		glBegin( GL_QUADS );
			glTexCoord2i( 0, 0 );
			glVertex3f( 0.f, 0.f, 0.f );
			glTexCoord2i( 1, 0 );
			glVertex3f( w, 0.f, 0.f );
			glTexCoord2i( 1, 1 );
			glVertex3f( w, h, 0.f );
			glTexCoord2i( 0, 1 );
			glVertex3f( 0.f, h, 0.f );
		glEnd( );
		glPopMatrix( );
	}
}

void Alonebot::jump( ) {
	if( canJump ) {
		jumpSpeed = 280.f;
		yspeed = -jumpSpeed;
		jumping = true;
		jumpTime.start( );
		canJump = false;
		jumpStart = heartBeat.get_ticks( );
		stand( );
	}
}

void Alonebot::crouch( ) {
	if( !crouching && !walking && !jumping ) {						// Can only crouch if you are standing still
		crouching = true;
		h -= 18;
		pos.y += 18.f;
		defaultHeight = h;
		loadVA( (float)w, (float)h );
	}
}

void Alonebot::stand( ) {
	if( crouching ) {												// Can only stand if you are crouching
		crouching = false;
		h += 18;
		pos.y -= 18.f;
		defaultHeight = h;
		loadVA( (float)w, (float)h );
		idle.stop( );
	}
}

void Alonebot::shoot( ) {
	shotStart = heartBeat.get_ticks( );								// Initialize shooting animations
	chargeTime.start( );
}

void Alonebot::hurt( int d, int dmg ) {
	if( canDmg ) {
		dir = d;
		stand( );
		xspeed = 666.f;
		yspeed = -120.f;
		currentHP -= dmg;
		canJump = false;
		canDmg = false;

		if( currentHP <= 0 ) {										// You died
			deathStart = heartBeat.get_ticks( );
			movable = false;
			pos.x += 4.5f;
			if( dir == -1 )
				pos.x -= 1.5f;
		}
		else
			courage.start( );
	}
}

void Alonebot::walk( float dt ) {
	if( !movable ) {
		// Fall
		stand( );
		if( !canJump ){
			yspeed += GRAVITY;
		}
		else
			yspeed = 1.f;
		pos.y += yspeed * dt;

		return;
	}

	if( courage.is_started( ) ) {									// Duration of invincibility
		int t = courage.get_ticks( );
		if( t > COURAGEDURATION ) {
			courage.stop( );
			canDmg = true;
		}
	}

	if( !walking && !jumping && canJump ) {							// Idle
		if( !idle.is_started( ) )
			idle.start( );
		if( shotStart != 0 || courage.is_started( ) )
			idle.stop( );
	}
	else
		idle.stop( );

	// Walking logic
	if( courage.is_started( ) && courage.get_ticks( ) < 250 ) {		// Duration of control loss
		pos.x += xspeed * dir * dt * -1;							// Knockback
		xspeed = xspeed * 90 * dt;									// Deceleration
	}
	else if( walking ) {											// Maximum and minimum walking speed
		if( xspeed < MAX_X_SPEED ) {								// Acceleration is different when you're ...
			if( canJump )											// ... On the ground
				xspeed += X_ACCEL * dt;
			else													// ... And in the air
				xspeed += X_ACCEL * dt * 0.55f;
			if( xspeed > MAX_X_SPEED )								// Cap speed
				xspeed = MAX_X_SPEED;
		}
		pos.x += xspeed * dir * dt;
	}
	else {															// Friction is different when you're ...
		float fr = 0.56f;
		if( canJump ) {												// ... On the ground,
			if( crouching )
				fr *= 0.0927f;										// ... Crouching,
			xspeed *= 1.0f - min( dt * X_ACCEL * fr, fr );
		}
		else {														// ... And in the air
			fr *= 0.0927f;
			xspeed *= 1.0f - min( dt * X_ACCEL * fr, fr );
		}
		pos.x += xspeed * dir * dt;
	}

	// Falling logic
	if( jumping ) {													// Jumping
		if( yspeed >= 0.f ) {
			jumping = false;
		}
		else {
			if( !jumpTime.is_paused( ) && jumpTime.get_ticks( ) > 0 && jumpTime.get_ticks( ) < 190 ) {
				jumpSpeed -= GRAVITY * 0.7f;
			}
			else
				jumpSpeed -= GRAVITY * 1.4f;
		
			yspeed = -jumpSpeed;
		}
	}
	else {
		jumpTime.stop( );
		if( !canJump ){												// In the air, but not jumping
			yspeed += GRAVITY;
			stand( );
		}
		else														// Not jumping, on the ground
			yspeed = 1.f;
	}
	if( yspeed > MAX_FALL_SPEED ) {									// Limit fall speed
		yspeed = MAX_FALL_SPEED;
	}
	pos.y += yspeed * dt;

	// Shooting logic
	if( chargeTime.get_ticks( ) >= chargeMax )
		chargeTime.stop( );
}

void Alonebot::walk( int d ) {
	if( movable ) {
		if( courage.is_started( ) && courage.get_ticks( ) < COURAGETIMEOUT )
			return;
		if( d != dir )												// Lower speed on direction change
			xspeed = 0.f;
		walking = true;												// Flag that you're walking
		dir = d;													// Set direction
	}
}

void Alonebot::setWeap( int w ) {
	if( movable ) {
		switch( w ) {
		  case 0: {
			weapon = 0;
			chargeCur = 0.f;
			chargeInc = 0.f;
			chargeMax = 1;
			break;
		  }
		  case 1: {													// Blaster
			weapon = 1;
			chargeCur = 0.f;
			chargeInc = 100.f;
			chargeMax = 80;
			break;
		  }
		  case 2: {													// Sword
			weapon = 2;
			chargeCur = 0.f;
			chargeInc = 70.f;
			chargeMax = 240;
			break;
		  }
		}
	}
}