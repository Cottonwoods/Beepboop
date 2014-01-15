#include "Menu.h"


Menu::Menu( ) {
	// Load menu assets
	pauseMenus[0] = LoadTexture( std::string( "..\\anims\\menu\\menu.png" ) );
	pauseMenus[1] = LoadTexture( std::string( "..\\anims\\menu\\equip.png" ) );
	
	pauseSelect = new Organ( "..\\anims\\menu\\select.png" );
	longSelect = new Organ( "..\\anims\\menu\\long_select.png" );
	fullMap = new Organ( "..\\anims\\menu\\map_full.png" );
	unseen = new Organ( "..\\anims\\menu\\unseen.png" );
	here = new Organ( "..\\anims\\menu\\here.png" );

	for( int i=0; i<MAX_AREAS; i++ ) {									// Load menu area captions and maps
		std::stringstream s;
		s << i;
		areaTexts[i] = new Organ( ((std::string)"..\\anims\\menu\\area_" + s.str() + (std::string)".png").c_str() );
		areaMaps[i] = new Organ( ((std::string)"..\\anims\\menu\\map_" + s.str() + (std::string)".png").c_str() );
	}
	
	itemCards[0] = new Organ( "..\\anims\\items\\blaster_card.png" );
	itemCards[1] = new Organ( "..\\anims\\items\\sword_card.png" );

	itemSlots[0] = new Organ( "..\\anims\\items\\blaster.png" );
	itemSlots[1] = new Organ( "..\\anims\\items\\sword.png" );
}

Menu::~Menu( ) {
	
	for( int i=0; i<MAX_AREAS; i++ ) {
		delete areaTexts[i];
		delete areaMaps[i];
	}
	for( int i=0; i<4; i++ )
		glDeleteTextures( 1, &pauseMenus[i] );
	for( int i=0; i<2; i++ ) {
		delete itemCards[i];
		delete itemSlots[i];
	}
	delete pauseSelect;
	delete longSelect;
	delete fullMap;
	delete unseen;
	delete here;
}

Organ* Menu::getText( int area ) {
	return areaTexts[area];
}

Organ* Menu::getMap( int area ) {
	return areaMaps[area];
}

Organ* Menu::getFullMap( ) {
	return fullMap;
}

Organ* Menu::setUnseen( Vector2Di newPos ) {
	unseen->pos.x = (float)newPos.x;
	unseen->pos.y = (float)newPos.y;
	return unseen;
}

Organ* Menu::setHere( Vector2Di newPos ) {
	here->pos.x = (float)newPos.x;
	here->pos.y = (float)newPos.y;
	return here;
}

GLuint Menu::getMenu( int menuState ) {
	return pauseMenus[menuState];
}

Organ* Menu::getLongSelect( ) {
	return longSelect;
}

Organ* Menu::getSelect( ) {
	return pauseSelect;
}

Organ* Menu::getCard( int item ) {
	return itemCards[item];
}

Organ* Menu::getSlot( int item ) {
	return itemSlots[item];
}