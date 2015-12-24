#ifndef _Vector3_H
#define _Vector3_H

#include <math.h>

class Vector3 {
	
public:
	
	// vars
	float x, y, z;
	
	// constructors
	Vector3() {}
	
	Vector3( float x1, float y1, float z1 ) {
		
		x = x1; y = y1; z = z1;
		
	}
	
	// vector ops
	void normalize() {
		
		float fTemp = 1 / Length();
		
		x *= fTemp;
		y *= fTemp;
		z *= fTemp;
		
	}

  void set(float x1, float y1, float z1) {
		x = x1; y = y1; z = z1;
  }

	void set(Vector3 v) {
	  x = v.x;
		y = v.y;
		z = v.z;
  }

	Vector3 cross( const Vector3 & rhs ) {

		return( Vector3( y * rhs.z - z * rhs.y, 
		               - x * rhs.z - z * rhs.x,
					     x * rhs.y - y * rhs.x ) );


	}

	float dot( const Vector3 & rhs ) const {
		
		// dot product
		return( (x * rhs.x) + (y * rhs.y) + (z * rhs.z) );
		
	}

	
	inline double Length() const {
		
		return( sqrt( (x * x) + (y * y) + (z * z) ) );
		
	}
	
	//  operators
	Vector3 operator + ( const Vector3 & rhs ) const {
		
		return( Vector3( x + rhs.x, y + rhs.y, z + rhs.z ) );
		
	}
	
	Vector3 operator - ( const Vector3 & rhs ) const {
		
		return( Vector3( x - rhs.x, y - rhs.y, z - rhs.z ) );
		
	}
	
	Vector3 operator / ( float k ) const {
		
		return( Vector3( x / k, y / k, z / k ) );
		
	}
	
	Vector3 operator * ( const float k ) const {

		// scalar multiply
		return( Vector3( x * k, y * k, z * k ) );

	}

	Vector3 operator *= ( const float k ) {

		x *= k;
		y *= k;
		z *= k;

		return( *this );

	}

	Vector3 operator += ( const Vector3 & rhs ) {

		x += rhs.x;
		y += rhs.y;
		z += rhs.z;

		return( *this );

	}
	
	float & operator [] ( int n ) {
		
		// access like an array
		switch( n ) {
			
		case 0: return( x );
		case 1: return( y );
		case 2: return( z );
		default: /* bug out */;
			
		}
	}
	
}; // end class


#endif
