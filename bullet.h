#pragma once
#include"vector2.h"
#include"player_id.h"
#include"camera.h"
#include<functional>
#include<graphics.h>

extern bool is_debug;

class Bullet
{
public:
	Bullet() = default;
	~Bullet() = default;

	void set_damage(int val)//�����ӵ��˺�
	{
		this->damage = val;
	}

	int get_damage()//��ȡ�ӵ��Ϻ�
	{
		return this->damage;
	}

	void set_position(float x, float y)//�����ӵ�λ��
	{
		this->position.x = x;
		this->position.y = y;
	}

	const Vector2& get_position() const//��ȡ�ӵ�λ��
	{
		return this->position;
	}

	const Vector2& get_size()const//��ȡ�ӵ���ײ�ߴ�
	{
		return this->size;
	}

	void set_velocity(float x, float y)//�����ӵ��ٶ�
	{
		this->velocity.x = x;
		this->velocity.y = y;
	}

	void set_collide_target(PlayerID target)//�����ӵ���ײ��Ŀ��
	{
		this->target_id = target;
	}

	PlayerID get_collide_target()//��ȡ�ӵ���ײ��Ŀ��
	{
		return this->target_id;
	}

	void set_callback(std::function<void()> callback)//���ûص������������ڶ����ڲ�
	{
		this->callback = callback;
	}

	void set_valid(bool flag)//�����ӵ��Ƿ���Ч
	{
		this->vaild = flag;
	}

	bool get_valid()const//��ȡ�ӵ��Ƿ���Ч
	{
		return this->vaild;
	}

	bool check_can_remove()const//��ȡ�ӵ��Ƿ�����Ƴ�
	{
		return this->can_remove;
	}

	virtual void on_collide()//������ײʱ����
	{
		if (callback)
			callback();
	}

	virtual bool check_collision(const Vector2& player_position,const Vector2& player_size)//���λ�ú������ײ���Ĵ�С  ����Ƿ�����ײ
	{
		return this->position.x + this->size.x / 2 >= player_position.x						//�ӵ����ĵ��Ƿ�����Ҽ���ڲ�
			&& this->position.x + this->size.x / 2 <= player_position.x + player_size.x
			&& this->position.y + this->size.y / 2 >= player_position.y
			&& this->position.y + this->size.y / 2 <= player_position.y + player_size.y;
	}

	virtual void on_update(int delta)//�ӵ�����
	{

	}

	virtual void on_draw(const Camera& camera)//�ӵ���ͼ
	{
		if (is_debug)//���
		{
			setfillcolor(RGB(255, 255, 255));
			setlinecolor(RGB(255, 255, 255));
			rectangle((int)this->position.x, (int)this->position.y,
				(int)this->position.x + (int)this->size.x, (int)this->position.y + (int)this->size.y);//�����չ�ը��
			solidcircle((int)(this->position.x + (int)this->size.x / 2), (int)(this->position.y + (int)this->size.y / 2), 5);//�㶹��С���չ�ը��
		}
	}
protected:					//ȷ��������Է���
	Vector2 size;			//�ӵ���ײ�ߴ�
	Vector2 position;		//�ӵ�λ��
	Vector2 velocity;		//�ӵ��ٶ�
	int damage = 10;		//�ӵ��˺�
	//�����ӵ���������ײ���Ϊ��Ч��Ȼ�󲥷��ӵ���ը�������������ſ�ɾ���ӵ�
	bool vaild = true;		//�ӵ��Ƿ���Ч
	bool can_remove = false;//�ӵ��Ƿ���Ա�ɾ������
	std::function<void()> callback;//�ӵ���ײ�ص�����

	PlayerID target_id = PlayerID::P1;//�ӵ���ײĿ�����ID

protected:
	bool check_if_exceeds_screen()//���ɳ���Ļ����ӵ�������ɾ��
	{
		return (this->position.x + this->size.x <= 0 || this->position.x >= getwidth()	//x����ɳ���Ļ��
			|| this->position.y + this->size.y <= 0 || this->position.y >= getheight());	//y����ɳ���Ļ��
	}

};