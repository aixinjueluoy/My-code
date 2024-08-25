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

	void reset()//���������
	{
		this->position.x = 0;
		this->position.y = 0;
	}

	void on_update(int delta)
	{
		timer_shake.on_update(delta);

		if (this->is_shaking)
		{
			//���������λ�����Զ���ǿ��Ϊ�뾶��Բ���������
			this->position.x = (-50 + rand() % 100) / 50.0f * shaking_strength;//��λԪ��������Զ���ǿ��
			this->position.y = (-50 + rand() % 100) / 50.0f * shaking_strength;
		}

		//�����ˮƽ�����ƶ�
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
	Vector2 position;			//�����λ��
	Timer timer_shake;			//�����������ʱ��
	bool is_shaking=false;		//������Ƿ����ڶ���
	float shaking_strength = 0;	//�������������
};