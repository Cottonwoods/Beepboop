#include <math.h>


class Vector2D {
  public:
	float x, y;
	Vector2D( float x = 0.f, float y = 0.f );
	~Vector2D( ) {};

	const float _x( );
	const float _y( );

	void Rotate( const float angle );
	float Magnitude( ) const;
	float Normalize( );

	Vector2D& operator= ( const Vector2D& v2 );

	Vector2D& operator+= ( const Vector2D& v2 );
	Vector2D& operator-= ( const Vector2D& v2 );
	Vector2D& operator*= ( const float scalar );
	Vector2D& operator/= ( const float scalar );

	const Vector2D operator+( const Vector2D& v2 ) const;
	float DotProduct( const Vector2D& v2 ) const;
	float CrossProduct( const Vector2D& v2 ) const;

	static Vector2D Zero();
	static float Distance( const Vector2D& v1, const Vector2D& v2 );
	const Vector2D operator-( const Vector2D& v2 ) const;
	const Vector2D operator*( const float scalar ) const;
	const Vector2D operator/( const float scalar ) const;

	bool operator== ( const Vector2D& v2 ) const;
	bool operator!= ( const Vector2D& v2 ) const;
};


class Vector2Di {
  public:
	int x, y;
	Vector2Di( int x = 0, int y = 0 );
	~Vector2Di() {};

	const int _x( );
	const int _y( );

	void Rotate( const float angle );
	int Magnitude() const;
	int Normalize();

	Vector2Di& operator= ( const Vector2Di& v2 );

	Vector2Di& operator+= ( const Vector2Di& v2 );
	Vector2Di& operator-= ( const Vector2Di& v2 );
	Vector2Di& operator*= ( const int scalar );
	Vector2Di& operator/= ( const int scalar );

	const Vector2Di operator+( const Vector2Di& v2 ) const;
	int DotProduct( const Vector2Di& v2 ) const;
	int CrossProduct( const Vector2Di& v2 ) const;

	static Vector2Di Zero();
	static int Distance( const Vector2Di& v1, const Vector2Di& v2 );
	const Vector2Di operator-( const Vector2Di& v2 ) const;
	const Vector2Di operator*( const int scalar ) const;
	const Vector2Di operator/( const int scalar ) const;

	bool operator== ( const Vector2Di& v2 ) const;
	bool operator!= ( const Vector2Di& v2 ) const;
};