#pragma once
#include"camera.h"
#include"player.h"
#include"pea_bullet.h"
#include<graphics.h>
#include<iostream>

extern Atlas atlas_peashooter_idle_left;		//�㶹��������Ĭ�϶���ͼ��
extern Atlas atlas_peashooter_idle_right;		//�㶹��������Ĭ�϶���ͼ��
extern Atlas atlas_peashooter_run_left;			//�㶹���������ܶ���ͼ��
extern Atlas atlas_peashooter_run_right;		//�㶹�������ұ��ܶ���ͼ��
extern Atlas atlas_peashooter_attack_ex_left;
extern Atlas atlas_peashooter_attack_ex_right;
extern Atlas atlas_peashooter_die_left;		//�㶹���ֳ������������ͼ��
extern Atlas atlas_peashooter_die_right;	//�㶹���ֳ��ҵ���������ͼ��

extern Camera main_camera;

class PeashooterPlayer:public Player
{
public:
	PeashooterPlayer(bool facing_right = true):Player(facing_right)
	{
		this->animation_idle_left.set_atlas(&atlas_peashooter_idle_left);//����ͼ��
		this->animation_idle_right.set_atlas(&atlas_peashooter_idle_right);
		this->animation_run_left.set_atlas(&atlas_peashooter_run_left);
		this->animation_run_right.set_atlas(&atlas_peashooter_run_right);
		this->animation_attack_ex_left.set_atlas(&atlas_peashooter_attack_ex_left);
		this->animation_attack_ex_right.set_atlas(&atlas_peashooter_attack_ex_right);
		this->animation_die_left.set_atlas(&atlas_peashooter_die_left);
		this->animation_die_right.set_atlas(&atlas_peashooter_die_right);

		this->animation_idle_left.set_interval(75);//����֡���
		this->animation_idle_right.set_interval(75);
		this->animation_run_left.set_interval(75);
		this->animation_run_right.set_interval(75);
		this->animation_attack_ex_left.set_interval(75);
		this->animation_attack_ex_right.set_interval(75);
		this->animation_die_left.set_interval(150);
		this->animation_die_right.set_interval(150);

		this->animation_die_left.set_loop(false);
		this->animation_die_right.set_loop(false);

		size.x = 96, size.y = 96;

		this->timer_attack_ex.set_wait_time(this->attack_ex_duration);
		this->timer_attack_ex.set_one_shot(true);//���γ���
		this->timer_attack_ex.set_callback([&]()//ʱ�䵽��ʱ�˳����⹥��
			{
				is_attacking_ex = false;
			});

		this->timer_spwan_pea_ex.set_wait_time(100);
		this->timer_spwan_pea_ex.set_callback([&]()
			{
				spawn_pea_bullet(this->speed_pea_ex);
			});
		this->attack_cd = 100;	//�չ�cd
	}
	~PeashooterPlayer() = default;

	void on_update(int delta)
	{
		Player::on_update(delta);
		
		if (this->is_attacking_ex)
		{
			main_camera.shake(5, 100);
			this->timer_attack_ex.on_update(delta);
			this->timer_spwan_pea_ex.on_update(delta);
		}
	}

	void on_attack()
	{
		this->spawn_pea_bullet(this->speed_pea);
		switch (rand()%2)//�����ӵ���Ч
		{
		case 0:
			mciSendString(_T("play pea_shoot_1 from 0"), NULL, 0, NULL);
			break;
		case 1:
			mciSendString(_T("play pea_shoot_2 from 0"), NULL, 0, NULL);
			break;
		}
	}

	void on_attack_ex()
	{
		this->is_attacking_ex = true;
		this->timer_attack_ex.restart();
		//���ݳ���ѡ�����ò�ͬ�Ľ�ɫ״̬
		this->is_facing_right ? this->animation_attack_ex_right.reset() : this->animation_attack_ex_left.reset();

		mciSendString(_T("play pea_shoot_ex from 0"), NULL, 0, NULL);
	}

private:
	//���⹥�������ݴ���
	void spawn_pea_bullet(float speed)//�����㶹���ٶ�
	{
		Bullet* bullet = new PeaBullet();	//ʵ�����㶹�ӵ�����
		Vector2 bullet_position, bullet_velocity;	//�ӵ�λ�ú��ӵ��ٶ�
		const Vector2& bullet_size = bullet->get_size();//�ӵ��ߴ�
		//�����ӵ�λ��
		bullet_position.x = this->is_facing_right
			? this->position.x + this->size.x - bullet_size.x / 2
			: this->position.x - bullet_position.x / 2;
		bullet_position.y = this->position.y;
		//�����ӵ��ٶ�
		bullet_velocity.x = this->is_facing_right ? speed : -speed;
		bullet_velocity.y = 0;
		bullet->set_position(bullet_position.x, bullet_position.y);
		bullet->set_velocity(bullet_velocity.x, bullet_velocity.y);

		bullet->set_collide_target(this->id == PlayerID::P1 ? PlayerID::P2:PlayerID::P1);	//���ù�������

		bullet->set_callback([&]() {this->mp += 25; });

		bullet_list.push_back(bullet);
	}

private:
	//�ӵ��ٶ�
	const float speed_pea = 0.75f;		//ƽa�ӵ��ٶ�
	const float speed_pea_ex = 1.5f;	//�����ӵ��ٶ�
	const int attack_ex_duration = 2500;	//���⹥��״̬�ĳ���ʱ��
private:
	Timer timer_attack_ex;		//���⹥����״̬״̬��ʱ�����������⹥�����˳�
	Timer timer_spwan_pea_ex;	//�㶹�ӵ����䶨ʱ��

};