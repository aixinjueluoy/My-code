#pragma once
#include"vector2.h"
#include"timer.h"


class Camera
{
public:
	Camera()
	{
		this->timer_shake.set_one_shot(true);
		this->timer_shake.set_callback([&]()
			{
				is_shaking = false;
				reset();
			});
	}
	~Camera() = default;

	const Vector2& get_position()const
	{
		return position;
	}

	void reset()//重置摄像机
	{
		this->position.x = 0;
		this->position.y = 0;
	}

	void on_update(int delta)
	{
		timer_shake.on_update(delta);

		if (this->is_shaking)
		{
			//将摄像机的位置在以抖动强度为半径的圆内随机设置
			this->position.x = (-50 + rand() % 100) / 50.0f * shaking_strength;//单位元的坐标乘以抖动强度
			this->position.y = (-50 + rand() % 100) / 50.0f * shaking_strength;
		}

		//摄像机水平向左移动
		//const Vector2 speed = { -0.35f,0 };
		//this->position += speed * (float)delta;
	}

	void shake(float strength, int duration)
	{
		is_shaking = true;
		shaking_strength = strength;

		timer_shake.set_wait_time(duration);
		timer_shake.restart();
	}
private:
	Vector2 position;			//摄像机位置
	Timer timer_shake;			//摄像机抖动定时器
	bool is_shaking=false;		//摄像机是否正在抖动
	float shaking_strength = 0;	//摄像机抖动幅度
};