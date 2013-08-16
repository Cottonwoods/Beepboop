#include "Level.h"


Box::Box( float a, float b, int width, int height ) {
	id = 0;													// Only changes with each new type
	defaultWidth = 32;
	defaultHeight = 16;

	pos = Vector2D( a, b );
	w = width;
	h = height;
	dir = 1;
	movable = false;
	solid = true;
	loadVA( );
	texture = LoadTexture( std::string( "..\\anims\\level\\tile.png" ) );
}

Box::~Box( ) {
	glDeleteTextures( 1, &texture );
}

void Box::loadVA( ) {
	Organ::loadVA( (float)w, (float)h );

	tvertices[0].x = 0.f;									// Top left
	tvertices[0].y = 0.f;
	tvertices[1].x = (1.f * w)/defaultWidth;				// Top rite
	tvertices[1].y = 0.f;
	tvertices[2].x = (1.f * w)/defaultWidth;				// Bot rite
	tvertices[2].y = (1.f * h)/defaultHeight;
	tvertices[3].x = 0.f;									// Bot left
	tvertices[3].y = (1.f * h)/defaultHeight;
}


Dropbox::Dropbox( float a, float b, int width, int height ) {
	id = 1;
	defaultWidth = 32;
	defaultHeight = 4;

	pos = Vector2D( a, b );
	w = width;
	h = height;
	dir = 1;
	movable = false;
	solid = true;
	texture = LoadTexture( std::string( "..\\anims\\level\\tile.png" ) );
	loadVA( );
}

Dropbox::~Dropbox( ) {
	glDeleteTextures( 1, &texture );
}


Invisbox::Invisbox( float a, float b, int width, int height ) {
	id = 2;
	defaultWidth = 16;
	defaultHeight = 16;

	pos = Vector2D( a, b );
	w = width;
	h = height;
	dir = 1;
	movable = false;
	solid = true;
	texture = LoadTexture( std::string( "..\\anims\\level\\invis.png" ) );
	loadVA( );
}

Invisbox::~Invisbox( ) {
	glDeleteTextures( 1, &texture );
}


Bridge::Bridge( float a, float b, int width, int height ) {
	id = 3;
	defaultWidth = 128;
	defaultHeight = 16;

	pos = Vector2D( a, b );
	w = width;
	h = height;
	dir = 1;
	movable = false;
	solid = true;
	texture = LoadTexture( std::string( "..\\anims\\level\\bridge.png" ) );
	loadVA( );
}

Bridge::~Bridge( ) {
	glDeleteTextures( 1, &texture );
}


Door::Door( float a, float b, int d, bool topper, int t, int e, int width, int height ) {
	id = 4;
	pos = Vector2D( a, b );
	w = width;
	h = height;
	to = t;
	entrance = e;
	dir = d;
	movable = false;
	solid = false;
	if( topper )
		texture = LoadTexture( std::string( "..\\anims\\level\\door_topper.png" ) );
	else
		texture = LoadTexture( std::string( "..\\anims\\level\\door.png" ) );
	defaultWidth = w;
	defaultHeight = h;
	loadVA( (float)w, (float)h );
}

Door::~Door( ) {
	glDeleteTextures( 1, &texture );
}


void Level::quit( int code ) {
	glfwCloseWindow( );
}

Level::Level( float r, int entrance, Alonebot* p, FMOD::System* fsys, FMOD::Channel* sfx ) {
	cameraX = 0.f;
	cameraY = 0.f;
	cameraV = Vector2D( );
	ratio = r;
	selection = 0;
	menuState = 0;
	paused = false;
	levelSwitch = 0;
	levelSwitchTo = 0;
	levelSwentrance = 0;

	// Initiates the FMOD system variable
	fmodSystem = fsys;
	sfxChannel = sfx;
}

Level::~Level( ) {
	typedef unsigned short sho;

	delete player;
	for( sho i=0; i<NPCs.size( ); i++ )
		delete NPCs[i];
	for( sho i=0; i<doors.size( ); i++ )
		delete doors[i];
	for( sho i=0; i<doorToppers.size( ); i++ )
		delete doorToppers[i];
	for( sho i=0; i<objects.size( ); i++ )
		delete objects[i];
	for( sho i=0; i<enemies.size( ); i++ )
		delete enemies[i];
	for( sho i=0; i<bullets.size( ); i++ )
		delete bullets[i];
	glDeleteTextures( 1, &background );
	glDeleteTextures( 1, &backbackground );
}

bool Level::clearBullets( ) {
	typedef unsigned short sho;
	for( sho i=0; i<bullets.size( ); i++ )
		if( !bullets[i]->movable || bullets[i]->pos.x < -2 * bullets[i]->w || bullets[i]->pos.x > levelWidth ) {
			delete bullets[i];
			bullets.erase( bullets.begin() + i );
			return true;
		}
	return false;
}

bool Level::clearEnemies( ) {
	typedef unsigned short sho;
	for( sho i=0; i<enemies.size( ); i++ )
		if( enemies[i]->flags & enemies[i]->ENEMY_DEAD ||			// If they are done dying
			enemies[i]->pos.x < -2 * enemies[i]->w ||				//  Or they are left of the level
			enemies[i]->pos.x > levelWidth ) {						//  Or right of the level
			delete enemies[i];
			enemies.erase( enemies.begin() + i );					//   Delete them
			return true;											//   Return true, they ded
		}
	return false;
}

void Level::logic( float dt ) {
	if( heartBeat.is_paused( ) )
		return;

	typedef unsigned short sho;
	// Player logic !!
	// For walking ...
	player->stand( );
	if( glfwGetKey(GLFW_KEY_RIGHT) && glfwGetKey(GLFW_KEY_LEFT) ) {
		if( player->walking ) {
			player->dir *= -1;
			player->xspeed = 0.f;
		}
		player->walking = false;
	}
	else if( glfwGetKey(GLFW_KEY_RIGHT) ) {
		player->walk( 1 );
	}
	else if( glfwGetKey(GLFW_KEY_LEFT) ) {
		player->walk( -1 );
	}
	else if( !glfwGetKey(GLFW_KEY_RIGHT) && !glfwGetKey(GLFW_KEY_LEFT) ) {
		player->walking = false;
	}
	
	// For crouching ...
	if( !player->drop && glfwGetKey(GLFW_KEY_DOWN) ) {
		player->walking = false;
		player->crouch( );
	}

	// For jumping ...
	if( player->jumping ) {
		if( glfwGetKey( 'Z' ) ) {
			if( player->jumpTime.is_paused( ) )
				player->jumpTime.unpause( );
		}
		else
			if( !player->jumpTime.is_paused( ) )
				player->jumpTime.pause( );
	}

	// For charging ...
	if( player->items.find( 1000 ) != player->items.end( ) && glfwGetKey( 'X' ) ) {
		if( !player->chargeTime.is_started( ) ) {
			player->chargeCur += player->chargeInc * dt;
			if( player->chargeCur >= 100.f ) {
				player->chargeCur = 100.f;
			}
		}
	}


	// Enemy logic !!
	// Let individual objects process
	player->walk( dt );
	for( sho i=0; i<NPCs.size( ); i++ ) {
		NPCs[i]->walk( dt );
	}
	for( sho i=0; i<enemies.size( ); i++ ) {
		enemies[i]->alert( player );
		enemies[i]->walk( dt );
	}
	for( sho i=0; i<bullets.size( ); i++ )
		bullets[i]->walk( dt, player );
	
	// Check for flags and resolve
	for( sho i=0; i<enemies.size( ); i++ ) {
		if( enemies[i]->flags & enemies[i]->ENEMY_SHOOTING ) {
			Vector2D start, vel, plr;
			plr = Vector2D( player->pos.x + player->w / 2.f, player->pos.y + player->h / 2.f );
			switch( enemies[i]->id ) {
			  case 3: {														// Shooter enemies
				if( enemies[i]->vertical == true ) {
					start = Vector2D( 3.f, (float)enemies[i]->w );
					vel = Vector2D( 0.f, enemies[i]->dir*240.f );
				}
				else {
					start = Vector2D( (float)enemies[i]->dir * enemies[i]->w, -1.f );
					vel = Vector2D( enemies[i]->dir*240.f, 0.f );
				}
				bullets.push_back( new CannonShot( false, enemies[i]->pos + start, vel ) );
				break;
			  }
			  case 4: {														// Skeleton enemies
				start = Vector2D( -enemies[i]->dir * enemies[i]->w / 2.f, enemies[i]->h / 2.f - 2.f );
				bullets.push_back( new Bone( false, enemies[i]->pos + start, plr ) );
				break;
			  }
			}
			enemies[i]->flags &= !enemies[i]->ENEMY_SHOOTING;
		}
	}
}

void Level::collision( ) {
	if( heartBeat.is_paused( ) )
		return;

	typedef unsigned short sho;
	// COLLISION DETECTION !!
	// With projectiles ...
	for( sho i=0; i<bullets.size( ); i++ ) {
		Vector2D displacement;
		for( sho j=0; j<objects.size( ); j++ ) {							// ... and level objects
			if( objects[j]->id != 1 && objects[j]->id != 2 && objects[j]->solid ) {
			if( bullets[i]->id == 2 || bullets[i]->solid ) {
				displacement = bullets[i]->collides( objects[j] );
				if( abs( displacement.x ) > 0.f || abs ( displacement.y ) > 0.f ) {
					bullets[i]->hit( );
				}
			}
			}
		}
		for( sho j=0; j<enemies.size( ); j++ ) {							// ... and enemies
			if( bullets[i]->playerShot && bullets[i]->solid && enemies[j]->solid ) {
				displacement = bullets[i]->collides( enemies[j] );
				if( abs( displacement.x ) > 0.f || abs ( displacement.y ) > 0.f ) {
					bullets[i]->hit( );
					enemies[j]->hurt( 0, bullets[i]->dmg );
				}
			}
		}
		if( !bullets[i]->playerShot && bullets[i]->solid ) {				// ... and the player
			displacement = player->collides( bullets[i] );
			if( abs( displacement.x ) > 0.f || abs ( displacement.y ) > 0.f ) {
				bullets[i]->hit( );
				if( player->canDmg ) {
					if( displacement.x > 0.f || abs( displacement.y ) > 0.f ) {
						player->hurt( -1, 1 );
					}
					else if( displacement.x < 0.f ) {
						bullets[i]->pos.x = player->pos.x;
						player->hurt( 1, 1 );
					}
					FmodErrorCheck( fmodSystem->playSound( FMOD_CHANNEL_FREE, player->hurtSFX, false, &sfxChannel ) );
					sfxChannel->setVolume( 0.2f );
				}
			}
		}
	}

	// With collision enabled enemies and the level
	for( sho i=0; i<enemies.size( ); i++ ) {
		if( enemies[i]->movable ) {
			for( sho j=0; j<objects.size( ); j++ )
				enemies[i]->collides( objects[j] );
			if( enemies[i]->pos.x < 0.f )
				enemies[i]->pos.x = 0.f;
			else if( enemies[i]->pos.x + enemies[i]->w > levelWidth )
				enemies[i]->pos.x = (float)levelWidth - enemies[i]->w;
		}
	}

	// With the player ...
	bool grounded = player->canJump;
	player->canJump = false;
	for( sho i=0; i<objects.size( ); i++ ) {								// ... and static tiles
		if( objects[i]->id != 2 ) {
			Vector2D displacement = player->collides( objects[i] );
			if( displacement.y < 0.f ) {									//     Pushing into a surface from the top
				player->pos.y = objects[i]->pos.y - player->h;
				if( !grounded ) {
					FmodErrorCheck( fmodSystem->playSound( FMOD_CHANNEL_FREE, player->landSFX, false, &sfxChannel ) );
					sfxChannel->setVolume( 0.2f );
				}
				player->canJump = true;
				if( objects[i]->id == 1 ) {									//     Dropping through dropboxes
					player->canDrop = true;
				}
				else {
					player->canDrop = false;
					player->jumping = false;
				}
			}
			else if( displacement.y > 0.f ) {								//     Pushing into a surface from the bottom
				if( objects[i]->id != 1 && player->yspeed < 0.f ) {
					player->jumping = false;
					player->yspeed = 0.f;
				}
			}
		}
	}
	for( sho i=0; i<NPCs.size( ); i++ ) {									// ... and NPCs
		Vector2D displacement = player->collides( NPCs[i] );
		if( abs( displacement.y ) > 0.f || abs(displacement.x ) > 0.f ) {
			NPCs[i]->talk( );
		}
	}
	for( sho i=0; i<items.size( ); i++ ) {									// ... and items
		Vector2D displacement = player->collides( items[i] );
		if( abs( displacement.y ) > 0.f || abs(displacement.x ) > 0.f ) {
			// Equip it if you can and add to inventory
			if( items[i]->equip ) {
				if( player->weapon != 0 )
					player->offHandWeap = player->weapon;
				player->setWeap( items[i]->id );
			}
			player->add( items[i] );
			FmodErrorCheck( fmodSystem->playSound( FMOD_CHANNEL_FREE, player->itemSFX, false, &sfxChannel ) );
			sfxChannel->setVolume( 0.2f );
			delete items[i];
			items.erase( items.begin() + i );
			break;
		}
	}
	for( sho i=0; i<doors.size( ); i++ ) {
		Vector2D displacement = player->collides( doors[i] );				// ... and doors
		if( abs( displacement.x ) > 16.f ) {
			levelSwitch = true;
			levelSwitchTo = doors[i]->to;
			levelSwentrance = doors[i]->entrance;
		}
	}
	for( sho i=0; i<enemies.size( ); i++ ) {
		Vector2D displacement;
		if( !( enemies[i]->flags & enemies[i]->ENEMY_DYING ) && enemies[i]->solid && player->canDmg ) {
			displacement = player->collides( enemies[i] );					// ... and enemies
			if( displacement.x > 0.f || abs( displacement.y ) > 0.f ) {
				player->hurt( -1, enemies[i]->damage );
				FmodErrorCheck( fmodSystem->playSound( FMOD_CHANNEL_FREE, player->hurtSFX, false, &sfxChannel ) );
				sfxChannel->setVolume( 0.2f );
			}
			else if( displacement.x < 0.f ) {
				player->hurt( 1, enemies[i]->damage );
				FmodErrorCheck( fmodSystem->playSound( FMOD_CHANNEL_FREE, player->hurtSFX, false, &sfxChannel ) );
				sfxChannel->setVolume( 0.2f );
			}
		}
	}
}

void Level::maintenence( float dt ) {
	// Maintenence !!
	// Update sound system
	fmodSystem->update();

	// Stop player at level boundaries
	if( player->pos.x < 0.f )
		player->pos.x = 0.f;
	else if( player->pos.x + player->w > (float)levelWidth )
		player->pos.x = (float)levelWidth - player->w;

	// Clear enemies
	while( clearEnemies( ) ) { }
	
	// Clear projectiles
	while( clearBullets( ) ) { }

	// Move camera
	glPopMatrix( );
    glLoadIdentity();
	
	float center = player->pos.x - 320;										// Move it on X
	if( player->dir == 1 )
		center += player->w / 2;
	const float c0 = dt * 3.75f / 0.25f;
	if(c0 >= 1.f) {
		// Here, constant is too small, spring too stiff.  so go the whole way to prevent oscillation.
		cameraX = center;
		cameraV.x = 0.f;
	}
	else {
		const float delta = center - cameraX;
		const float force = delta - 2.0f * cameraV.x;
		if( abs(delta) >= 320.f ) {											// Reset when too far
			cameraX = center;
			cameraV.x = 0.f;
		}
		else {																// Accelerate and move camera on x
			cameraX += cameraV.x * c0;
			cameraV.x += force * c0;
		}
	}
	if( cameraX + 640 > levelWidth ) {										// Reset at edges
		cameraX = (float)levelWidth - 640.f;
		cameraV.x = 0.f;
	}
	else if( cameraX < 0 ) {
		cameraX = 0;
		cameraV.x = 0.f;
	}

	center = player->pos.y - 240;											// Move it on Y
	center += player->h / 2;
	if(c0 >= 1.f) {
		// Here, constant is too small, spring too stiff.  so go the whole way to prevent oscillation.
		cameraY = center;
		cameraV.y = 0.f;
	}
	else {
		const float delta = center - cameraY;
		const float force = delta - 2.0f * cameraV.y;
		if( abs(delta) >= 240.f ) {											// Reset when too far
			cameraY = center;
			cameraV.y = 0.f;
		}
		else {																// Accelerate and move camera on x
			cameraY += cameraV.y * c0;
			cameraV.y += force * c0;
		}
	}
	if( cameraY + 480 > levelHeight ) {										// Reset at edges
		cameraY = (float)levelHeight - 480.f;
		cameraV.y = 0.f;
	}
	else if( cameraY < 0 ) {
		cameraY = 0;
		cameraV.y = 0.f;
	}

	glTranslatef( -cameraX * ratio, -cameraY * ratio, 0.f );				// Move the camera
	glPushMatrix( );
}

void Level::pause( ) {
	typedef unsigned short sho;

	menuState = 0;
	selection = 0;

	player->chargeTime.pause( );
	heartBeat.pause( );
	for( sho i=0; i<NPCs.size( ); i++ )
		NPCs[i]->pause( );
	for( sho i=0; i<items.size( ); i++ )
		items[i]->pause( );
	for( sho i=0; i<objects.size( ); i++ )
		objects[i]->pause( );
	for( sho i=0; i<enemies.size( ); i++ )
		enemies[i]->pause( );
	for( sho i=0; i<bullets.size( ); i++ )
		bullets[i]->pause( );
}

void Level::unpause( ) {
	typedef unsigned short sho;

	menuState = 0;
	
	player->chargeTime.unpause( );
	heartBeat.unpause( );
	for( sho i=0; i<NPCs.size( ); i++ )
		NPCs[i]->unpause( );
	for( sho i=0; i<items.size( ); i++ )
		items[i]->unpause( );
	for( sho i=0; i<objects.size( ); i++ )
		objects[i]->unpause( );
	for( sho i=0; i<enemies.size( ); i++ )
		enemies[i]->unpause( );
	for( sho i=0; i<bullets.size( ); i++ )
		bullets[i]->unpause( );
}

void Level::process_events( float dt ) {
}

void Level::handle_key( int key, int action ) {
	// Death controls
	if( player->getDeathTime( ) != 0 ) {						// You dead
		if( action == GLFW_PRESS && player->getDeathTime( ) >= 1650 ) {
			if( key == 'X' ) {
				levelSwitchTo = 0;
				levelSwitch = true;
				levelSwentrance = 0;
			}
			else if( key == GLFW_KEY_ESC )
				quit( 0 );
		}
	}

	// Pause menu controls
	else if( paused ) {										// Paused
		if( action == GLFW_PRESS ) {
			switch( key ) {
			  case 'Z': {									// Back button on pause menu
				switch( menuState ) {
				  case 0: paused = !paused; unpause( ); break;
				  case 1: menuState = 0; break;
				  default: break;
				}
				break;
			  }
			  case 'X': {									// Selection button on pause menu
				switch( menuState ) {
				  case 0: menuState = selection + 1; break;
				  case 1: if( player->weapon == 1 || player->chargeTime.get_ticks( ) == 0 ) {
					player->chargeTime.stop( ); player->weapon = selection + 1; } break;
				  default: break;
				}
				break;
			  }
			  case GLFW_KEY_UP: {
				selection--;
				switch( menuState ) {
				  case 0: {
					if( selection < 0 )
						selection = 3;
					break;
				  }
				  case 1: {
					if( selection < 0 )
						selection = player->equips.size( ) - 1;
					break;
				  }
				  case 2: {
					if( selection < 0 )
						selection = player->items.size( ) - 1;
					break;
				  }
				  default: break;
				}
				break;
			  }
			  case GLFW_KEY_DOWN: {
				selection++;
				switch( menuState ) {
				  case 0: {
					if( selection > 3 )
						selection = 0;
					break;
				  }
				  case 1: {
					if( selection > player->equips.size( ) - 1 )
						selection = 0;
					break;
				  }
				  case 2: {
					if( selection > player->items.size( ) - 1 )
						selection = 0;
					break;
				  }
				  default: break;
				}
				break;
			  }
			  case GLFW_KEY_SPACE: {
				paused = !paused;
				unpause( );
				break;
			  }
			  case GLFW_KEY_ESC: {
				quit( 0 );
			  }
			}
		}
	}

	// Gameplay controls
	else {
		if( action == GLFW_PRESS ) {						// Pressing keys
			switch( key ) {
			  case 'Z': {									// Jump!
				if( player->crouching && player->canDrop ) {
					player->stand( );
					player->pos.y += 18.f;					// Crouch coefficient
					player->canDrop = false;
				}
				else if( player->canJump ) {
					player->jump( );
					FmodErrorCheck( fmodSystem->playSound( FMOD_CHANNEL_FREE, player->jumpSFX, false, &sfxChannel ) );
					sfxChannel->setVolume( 0.2f );
				}
				break;
			  }
			  case 'X': {									// Shoot!
				if( !player->chargeTime.is_started( ) ) {
				switch( player->weapon ) {					// Different shot depending on the weapon you use
				  case 1: {										// Blaster
					Vector2D start = Vector2D( player->dir*15.f, 21.f - (player->crouching?1.f:0.f) );
					if( player->dir == 1 )
						start.x += player->w - 11;
					bullets.push_back( new BlasterShot( true, player->pos + start, Vector2D( player->dir*650.f ) ) );
					player->shoot( );
					FmodErrorCheck( fmodSystem->playSound( FMOD_CHANNEL_FREE, player->blstSFX, false, &sfxChannel ) );
					sfxChannel->setVolume( 0.2f );
					break;
				  }
				  case 2: {										// Sword
					Vector2D start = Vector2D( player->dir*65.f, 20.f - (player->crouching?1.f:0.f) );
					if( player->dir == 1 )
						start.x += player->w - 52;
					bullets.push_back( new SwordSlash( true, player->dir, player->pos + start ) );
					player->shoot( );
					FmodErrorCheck( fmodSystem->playSound( FMOD_CHANNEL_FREE, player->swrdSFX, false, &sfxChannel ) );
					sfxChannel->setVolume( 0.2f );
					break;
				  }
				}
				}
				break;
			  }
			  case 'C': {									// Switch weapons
				if( !player->chargeTime.is_started( ) ) {
					if( player->offHandWeap != 0 ) {
						int temp = player->weapon;
						player->setWeap( player->offHandWeap );
						player->offHandWeap = temp;
					}
				}
				break;
			  }
			  case GLFW_KEY_DOWN: {							// Crouch!
				player->crouch( );
				break;
			  }
			  case GLFW_KEY_SPACE: {						// Pause
				paused = !paused;
				pause( );
				break;
			  }
			  case GLFW_KEY_ESC: {
				quit( 0 );
			  }
			}
		}
		else {												// Releasing keys
			switch( key ) {
			  case 'X': {									// Charge shot!
				if( player->chargeCur != 0.f ) {
				float charge = player->chargeCur / 100.f;
				player->chargeCur = 0.f;
				switch( player->weapon ) {					// Different shot depending on the weapon you use
				  case 1: {										// Blaster
					if( charge == 1.f ) {
						Vector2D start = Vector2D( player->dir*15.f, 21.f - (player->crouching?1.f:0.f) );
						if( player->dir == 1 )
							start.x += player->w - 11;
						bullets.push_back( new BlasterShot( true, player->pos + start, Vector2D( player->dir*650.f ) ) );
						player->shoot( );
						FmodErrorCheck( fmodSystem->playSound( FMOD_CHANNEL_FREE, player->blstSFX, false, &sfxChannel ) );
						sfxChannel->setVolume( 0.2f );
					}
					break;
				  }
				  case 2: {										// Sword
					if( charge == 1.f ) {
						Vector2D start = Vector2D( player->dir*65.f, 20.f - (player->crouching?1.f:0.f) );
						if( player->dir == 1 )
							start.x += player->w - 52;
						bullets.push_back( new SwordSlash( true, player->dir, player->pos + start ) );
						player->shoot( );
						FmodErrorCheck( fmodSystem->playSound( FMOD_CHANNEL_FREE, player->swrdSFX, false, &sfxChannel ) );
						sfxChannel->setVolume( 0.2f );
					}
					break;
				  }
				}
				}
				break;
				}
			  case GLFW_KEY_DOWN: {
				player->stand( );
				break;
			  }
			}
		}
	}
}

Alonebot* Level::getPlayer( ) {
	return player;
}

std::vector<NPC*> Level::getNPCs( ) {
	return NPCs;
}

std::vector<Item*> Level::getItems( ) {
	return items;
}

std::vector<Door*> Level::getDoors( ) {
	return doors;
}

std::vector<Door*> Level::getDoorToppers( ) {
	return doorToppers;
}

std::vector<Box*> Level::getScene( ) {
	return objects;
}

std::vector<Enemy*> Level::getEnemies( ) {
	return enemies;
}

std::vector<Projectile*> Level::getBullets( ) {
	return bullets;
}

void Level::playDoorSFX( ) {
	FmodErrorCheck( fmodSystem->playSound( FMOD_CHANNEL_FREE, player->doorSFX, false, &sfxChannel ) );
	sfxChannel->setVolume( 0.2f );
}