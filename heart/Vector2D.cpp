#include "Vector2D.h"


//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
Vector2D::Vector2D( float x, float y ) {
	this->x = x;
	this->y = y;
}

const float Vector2D::_x( ) {
	return x;
}

const float Vector2D::_y( ) {
	return y;
}

//-----------------------------------------------------------------------------
// Purpose: Rotate a vector
//-----------------------------------------------------------------------------
void Vector2D::Rotate( const float angle ) {
	float xt = (x * cosf(angle)) - (y * sinf(angle));
	float yt = (y * cosf(angle)) + (x * sinf(angle));
	x = xt;
	y = yt;
}

//-----------------------------------------------------------------------------
// Purpose: Get vector magnitude
//-----------------------------------------------------------------------------
float Vector2D::Magnitude() const {
	return sqrtf(x * x + y * y);
}

//-----------------------------------------------------------------------------
// Purpose: Convert vector to a unit vector and return previous magnitude
//-----------------------------------------------------------------------------
float Vector2D::Normalize( ) {
	float mag = Magnitude( );
	
	if( mag != 0.0 ) {
		x /= mag;
		y /= mag;
	}

	return mag;
}

//-----------------------------------------------------------------------------
// Purpose: Dot Product
//-----------------------------------------------------------------------------
float Vector2D::DotProduct( const Vector2D& v2 ) const {
	return (x * v2.x) + (y * v2.y);
}

//-----------------------------------------------------------------------------
// Purpose: Cross Product
//-----------------------------------------------------------------------------
float Vector2D::CrossProduct( const Vector2D& v2 ) const {
	return (x * v2.y) - (y * v2.x);
}

//-----------------------------------------------------------------------------
// Purpose: Return an empty vector
//-----------------------------------------------------------------------------
Vector2D Vector2D::Zero( ) {
	return Vector2D(0, 0);
}

//-----------------------------------------------------------------------------
// Purpose: Get distance between two vectors
//-----------------------------------------------------------------------------
float Vector2D::Distance( const Vector2D& v1, const Vector2D& v2 ) {
	return sqrtf( pow((v2.x - v1.x), 2 ) + pow((v2.y - v1.y), 2) );
}

Vector2D& Vector2D::operator= ( const Vector2D& v2 ) {
	if (this == &v2)
		return *this;
	
	x = v2.x;
	y = v2.y;
	
	return *this;
}

Vector2D& Vector2D::operator+= ( const Vector2D& v2 ) {
	x += v2.x;
	y += v2.y;
	
	return *this;
}

Vector2D& Vector2D::operator-= ( const Vector2D& v2 ) {
	x -= v2.x;
	y -= v2.y;
	
	return *this;
}

Vector2D& Vector2D::operator*= ( const float scalar ) {
	x *= scalar;
	y *= scalar;
	
	return *this;
}

Vector2D& Vector2D::operator/= ( const float scalar ) {
	x /= scalar;
	y /= scalar;
	
	return *this;
}

const Vector2D Vector2D::operator+ ( const Vector2D& v2 ) const {
	return Vector2D(*this) += v2;
}

const Vector2D Vector2D::operator- ( const Vector2D& v2 ) const {
	return Vector2D(*this) -= v2;
}

const Vector2D Vector2D::operator* ( const float scalar ) const {
	return Vector2D(*this) *= scalar;
}

const Vector2D Vector2D::operator/ ( const float scalar ) const {
	return Vector2D(*this) /= scalar;
}

bool Vector2D::operator== ( const Vector2D& v2 ) const {
	return ((x == v2.x) && (y == v2.y));
}

bool Vector2D::operator!= ( const Vector2D& v2 ) const {
	return !((x == v2.x) && (y == v2.y));
}


// Vector2Di
//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
Vector2Di::Vector2Di( int x, int y ) {
	this->x = x;
	this->y = y;
}

const int Vector2Di::_x( ) {
	return x;
}

const int Vector2Di::_y( ) {
	return y;
}

//-----------------------------------------------------------------------------
// Purpose: Rotate a vector
//-----------------------------------------------------------------------------
void Vector2Di::Rotate( const float angle ) {
	float xt = (x * cosf(angle)) - (y * sinf(angle));
	float yt = (y * cosf(angle)) + (x * sinf(angle));
	x = (int)xt;
	y = (int)yt;
}

//-----------------------------------------------------------------------------
// Purpose: Get vector magnitude
//-----------------------------------------------------------------------------
int Vector2Di::Magnitude() const {
	return (int)sqrtf((float)x * x + (float)y * y);
}

//-----------------------------------------------------------------------------
// Purpose: Convert vector to a unit vector and return previous magnitude
//-----------------------------------------------------------------------------
int Vector2Di::Normalize( ) {
	int mag = Magnitude( );
	
	if( mag != 0 ) {
		x /= mag;
		y /= mag;
	}

	return mag;
}

//-----------------------------------------------------------------------------
// Purpose: Dot Product
//-----------------------------------------------------------------------------
int Vector2Di::DotProduct( const Vector2Di& v2 ) const {
	return (x * v2.x) + (y * v2.y);
}

//-----------------------------------------------------------------------------
// Purpose: Cross Product
//-----------------------------------------------------------------------------
int Vector2Di::CrossProduct( const Vector2Di& v2 ) const {
	return (x * v2.y) - (y * v2.x);
}

//-----------------------------------------------------------------------------
// Purpose: Return an empty vector
//-----------------------------------------------------------------------------
Vector2Di Vector2Di::Zero( ) {
	return Vector2Di(0, 0);
}

//-----------------------------------------------------------------------------
// Purpose: Get distance between two vectors
//-----------------------------------------------------------------------------
int Vector2Di::Distance( const Vector2Di& v1, const Vector2Di& v2 ) {
	return (int)sqrtf( pow(((float)v2.x - (float)v1.x), 2.f ) + pow(((float)v2.y - (float)v1.y), 2.f) );
}

Vector2Di& Vector2Di::operator= ( const Vector2Di& v2 ) {
	if (this == &v2)
		return *this;
	
	x = v2.x;
	y = v2.y;
	
	return *this;
}

Vector2Di& Vector2Di::operator+= ( const Vector2Di& v2 ) {
	x += v2.x;
	y += v2.y;
	
	return *this;
}

Vector2Di& Vector2Di::operator-= ( const Vector2Di& v2 ) {
	x -= v2.x;
	y -= v2.y;
	
	return *this;
}

Vector2Di& Vector2Di::operator*= ( const int scalar ) {
	x *= scalar;
	y *= scalar;
	
	return *this;
}

Vector2Di& Vector2Di::operator/= ( const int scalar ) {
	x /= scalar;
	y /= scalar;
	
	return *this;
}

const Vector2Di Vector2Di::operator+ ( const Vector2Di& v2 ) const {
	return Vector2Di(*this) += v2;
}

const Vector2Di Vector2Di::operator- ( const Vector2Di& v2 ) const {
	return Vector2Di(*this) -= v2;
}

const Vector2Di Vector2Di::operator* ( const int scalar ) const {
	return Vector2Di(*this) *= scalar;
}

const Vector2Di Vector2Di::operator/ ( const int scalar ) const {
	return Vector2Di(*this) /= scalar;
}

bool Vector2Di::operator== ( const Vector2Di& v2 ) const {
	return ((x == v2.x) && (y == v2.y));
}

bool Vector2Di::operator!= ( const Vector2Di& v2 ) const {
	return !((x == v2.x) && (y == v2.y));
}