#include "Vector2D.h"


//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
Vector2D::Vector2D( float x, float y ) {
	this->x = x;
	this->y = y;
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
float Vector2D::DotProduct( const Vector2D &v2 ) const {
	return (x * v2.x) + (y * v2.y);
}


//-----------------------------------------------------------------------------
// Purpose: Cross Product
//-----------------------------------------------------------------------------
float Vector2D::CrossProduct( const Vector2D &v2 ) const {
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


const Vector2D Vector2D::operator+ ( const Vector2D &v2 ) const {
	return Vector2D(*this) += v2;
}


const Vector2D Vector2D::operator- ( const Vector2D &v2 ) const {
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