#include "Level.h"


class Menu {
  public:
	static const int MAX_AREAS = 2;					// Areas are composed of levels

	GLuint pauseMenus[4];							// Textures for pause menu
	Organ *fullMap, *unseen, *here;					// Main menu and map textures
	Organ *pauseSelect, *longSelect;
	Organ* areaTexts[MAX_AREAS];					// For describing current area
	Organ* areaMaps[MAX_AREAS];						// Full area maps

	Organ* itemCards[2];							// Holds item details
	Organ* itemSlots[2];

	Menu( );
	~Menu( );

	Organ* getText( int area );
	Organ* getMap( int area );
	Organ* getFullMap( );

	Organ* setUnseen( Vector2Di newPos );
	Organ* setHere( Vector2Di newPos );

	GLuint getMenu( int menuState );
	Organ* getLongSelect( );
	Organ* getSelect( );
	
	Organ* getCard( int item );
	Organ* getSlot( int item );
};