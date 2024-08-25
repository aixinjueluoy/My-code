#pragma once
#include"camera.h"
#include"player.h"
#include"pea_bullet.h"
#include<graphics.h>
#include<iostream>

extern Atlas atlas_peashooter_idle_left;		//豌豆射手向左默认动画图集
extern Atlas atlas_peashooter_idle_right;		//豌豆射手向右默认动画图集
extern Atlas atlas_peashooter_run_left;			//豌豆射手向左奔跑动画图集
extern Atlas atlas_peashooter_run_right;		//豌豆射手向右奔跑动画图集
extern Atlas atlas_peashooter_attack_ex_left;
extern Atlas atlas_peashooter_attack_ex_right;
extern Atlas atlas_peashooter_die_left;		//豌豆射手朝左的死亡动画图集
extern Atlas atlas_peashooter_die_right;	//豌豆射手朝右的死亡动画图集

extern Camera main_camera;

class PeashooterPlayer:public Player
{
public:
	PeashooterPlayer(bool facing_right = true):Player(facing_right)
	{
		this->animation_idle_left.set_atlas(&atlas_peashooter_idle_left);//设置图集
		this->animation_idle_right.set_atlas(&atlas_peashooter_idle_right);
		this->animation_run_left.set_atlas(&atlas_peashooter_run_left);
		this->animation_run_right.set_atlas(&atlas_peashooter_run_right);
		this->animation_attack_ex_left.set_atlas(&atlas_peashooter_attack_ex_left);
		this->animation_attack_ex_right.set_atlas(&atlas_peashooter_attack_ex_right);
		this->animation_die_left.set_atlas(&atlas_peashooter_die_left);
		this->animation_die_right.set_atlas(&atlas_peashooter_die_right);

		this->animation_idle_left.set_interval(75);//设置帧间隔
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
		this->timer_attack_ex.set_one_shot(true);//单次出发
		this->timer_attack_ex.set_callback([&]()//时间到达时退出特殊攻击
			{
				is_attacking_ex = false;
			});

		this->timer_spwan_pea_ex.set_wait_time(100);
		this->timer_spwan_pea_ex.set_callback([&]()
			{
				spawn_pea_bullet(this->speed_pea_ex);
			});
		this->attack_cd = 100;	//普工cd
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
		switch (rand()%2)//发射子弹音效
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
		//根据朝向，选择重置不同的角色状态
		this->is_facing_right ? this->animation_attack_ex_right.reset() : this->animation_attack_ex_left.reset();

		mciSendString(_T("play pea_shoot_ex from 0"), NULL, 0, NULL);
	}

private:
	//特殊攻击的数据处理
	void spawn_pea_bullet(float speed)//生成豌豆的速度
	{
		Bullet* bullet = new PeaBullet();	//实例化豌豆子弹对象
		Vector2 bullet_position, bullet_velocity;	//子弹位置和子弹速度
		const Vector2& bullet_size = bullet->get_size();//子弹尺寸
		//设置子弹位置
		bullet_position.x = this->is_facing_right
			? this->position.x + this->size.x - bullet_size.x / 2
			: this->position.x - bullet_position.x / 2;
		bullet_position.y = this->position.y;
		//设置子弹速度
		bullet_velocity.x = this->is_facing_right ? speed : -speed;
		bullet_velocity.y = 0;
		bullet->set_position(bullet_position.x, bullet_position.y);
		bullet->set_velocity(bullet_velocity.x, bullet_velocity.y);

		bullet->set_collide_target(this->id == PlayerID::P1 ? PlayerID::P2:PlayerID::P1);	//设置攻击对象

		bullet->set_callback([&]() {this->mp += 25; });

		bullet_list.push_back(bullet);
	}

private:
	//子弹速度
	const float speed_pea = 0.75f;		//平a子弹速度
	const float speed_pea_ex = 1.5f;	//技能子弹速度
	const int attack_ex_duration = 2500;	//特殊攻击状态的持续时间
private:
	Timer timer_attack_ex;		//特殊攻击的状态状态计时器，控制特殊攻击的退出
	Timer timer_spwan_pea_ex;	//豌豆子弹发射定时器

};