#pragma once
#include<cmath>

class Vector2	//世界坐标
{
public:
	float x = 0;
	float y = 0;

public:
	Vector2() = default;
	~Vector2() = default;
	Vector2(float x, float y) 
		:x(x), y(y) {}
	Vector2 operator+(const Vector2& vec)const//重载+号
	{
		return Vector2(this->x + vec.x, this->y + vec.y);
	}

	Vector2 operator-(const Vector2& vec)const
	{
		return Vector2(this->x - vec.x, this->y - vec.y);
	}

	Vector2 operator+=(const Vector2& vec)
	{
		return Vector2(this->x += vec.x, this->y += vec.y);
	}

	Vector2 operator-=(const Vector2& vec)
	{
		return Vector2(this->x -= vec.x, this->y -= vec.y);
	}

	float operator*(const Vector2& vec)const
	{
		return x * vec.x + y * vec.y;
	}

	Vector2 operator*(float val)const
	{
		return Vector2(this->x * val, this->y * val);
	}

	void operator*=(float val)
	{
		this->x *= val,this->y *= val;
	}

	float length()//获取向量长度
	{
		return sqrt(x * x + y * y);
	}

	Vector2 normalize()//向量长度标准化
	{
		float len = length();
		if (len == 0)
			return Vector2(0, 0);
		return Vector2(x / len, y / len);
	}
};