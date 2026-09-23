#pragma once

#include <math.h>
class Vector3
{
	//教科書P64参照

public:

	float x, y, z;
	//コンストラクタ
	//デフォルトコンストラクタではベクトルは未決定な状態

	Vector3() {}

	//引数つきコンストラクタ
	Vector3(const Vector3& a) :x(a.x), y(a.y), z(a.z) {}
	Vector3(float nx, float ny, float nz) :x(nx), y(ny), z(nz) {}


	//引数で貰ってきたベクトルを自分に代入して返す
	//operatorは新しいアドレスなどにコピーした値をぶち込む時などに使用
	Vector3& operator=(const Vector3& a)
	{
		x = a.x;
		y = a.y;
		z = a.z;
		return *this;
	}

	bool operator==(const Vector3& a) const
	{
		return (x == a.x) && (y == a.y) && (z == a.z);
	}

	bool operator!=(const Vector3& a) const
	{
		return x != a.x || y != a.y || z != a.z;
	}

	void zero() { x = y = z = 0.0f; }

	Vector3 operator-() const
	{
		return Vector3(-x, -y, -z);
	}

	Vector3 operator+(const Vector3& a) const
	{
		return Vector3(x + a.x, y + a.y, z + a.z);
	}

	Vector3 operator-(const Vector3& a) const
	{
		return Vector3(x - a.x, y - a.y, z - a.z);
	}

	Vector3 operator*(float s) const
	{
		return Vector3(x * s, y * s, z * s);
	}

	Vector3 operator/(float s) const
	{
		float oneOverA = 1.0f / s;
		return Vector3(x * oneOverA, y * oneOverA, z * oneOverA);

	}
	Vector3& operator+=(const Vector3& a)
	{
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}

	Vector3& operator-=(const Vector3& a)
	{
		x -= a.x;
		y -= a.y;
		z -= a.z;
		return *this;
	}

	Vector3& operator*=(float s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	Vector3& operator/=(float s)
	{
		float oneOverA = 1.0f / s;
		x *= oneOverA;
		y *= oneOverA;
		z *= oneOverA;
		return *this;
	}

	void normalize()
	{
		float magSq = x * x + y * y + z * z;
		if (magSq > 0.0f)
		{
			float oneOverMag = 1.0f / sqrtf(magSq);
			x *= oneOverMag;
			y *= oneOverMag;
			z *= oneOverMag;
		}
	}

	float operator*(const Vector3& a) const
	{
		return x * a.x + y * a.y + z * a.z;
	}
	static Vector3 cross(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x);
	}

	float length() const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	static float dot(Vector3& a, Vector3& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

};