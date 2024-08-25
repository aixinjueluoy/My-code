#pragma once
#include"camera.h"
#include"player.h"
#include"sun_bullet.h"
#include"sun_bullet_ex.h"
#include<graphics.h>
#include<iostream>


extern Atlas atlas_sunflower_idle_left;		//���տ�����Ĭ�϶���ͼ��
extern Atlas atlas_sunflower_idle_right;		//���տ�����Ĭ�϶���ͼ��
extern Atlas atlas_sunflower_run_left;			//���տ������ܶ���ͼ��
extern Atlas atlas_sunflower_run_right;		//���տ����ұ��ܶ���ͼ��
extern Atlas atlas_sunflower_attack_ex_left;
extern Atlas atlas_sunflower_attack_ex_right;//���⹥��
extern Atlas atlas_sun_text;
extern Atlas atlas_sunflower_die_left;		//���տ��������������ͼ��
extern Atlas atlas_sunflower_die_right;		//���տ����ҵ���������ͼ��

extern Player* player_1;//��ҵ�λ��
extern Player* player_2;

class SunflowerPlayer :public Player
{
public:
	SunflowerPlayer(bool facing_right = true):Player(facing_right)
	{
		this->animation_idle_left.set_atlas(&atlas_sunflower_idle_left);//����ͼ��
		this->animation_idle_right.set_atlas(&atlas_sunflower_idle_right);
		this->animation_run_left.set_atlas(&atlas_sunflower_run_left);
		this->animation_run_right.set_atlas(&atlas_sunflower_run_right);
		this->animation_attack_ex_left.set_atlas(&atlas_sunflower_attack_ex_left);
		this->animation_attack_ex_left.set_atlas(&atlas_sunflower_attack_ex_right);
		this->animation_sun_text.set_atlas(&atlas_sun_text);
		this->animation_die_left.set_atlas(&atlas_sunflower_die_left);
		this->animation_die_right.set_atlas(&atlas_sunflower_die_right);

		this->animation_idle_left.set_interval(75);//����֡���
		this->animation_idle_right.set_interval(75);
		this->animation_run_left.set_interval(75);
		this->animation_run_right.set_interval(75);
		this->animation_attack_ex_left.set_interval(100);
		this->animation_attack_ex_right.set_interval(100);
		this->animation_sun_text.set_interval(100);
		this->animation_die_left.set_interval(150);
		this->animation_die_right.set_interval(150);

		this->animation_attack_ex_left.set_loop(false);
		this->animation_attack_ex_right.set_loop(false);
		this->animation_sun_text.set_loop(false);
		this->animation_die_left.set_loop(false);
		this->animation_die_right.set_loop(false);

		//���⹥�����Ž�������߼���ȡ�����⹥��״̬���ı��Ŀɼ���
		this->animation_attack_ex_left.set_callback([&]()
			{
				is_attacking_ex = false;
				is_sun_text_visible = false;
			});

		this->animation_attack_ex_right.set_callback([&]()
			{
				is_attacking_ex = false;
				is_sun_text_visible = false;
			});

		size.x = 96, size.y = 96;

		this->attack_cd = 250;//��ͨ������ȴ
	}
	~SunflowerPlayer() = default;

	void on_update(int delta)
	{
		Player::on_update(delta);

		if (this->is_sun_text_visible)
			this->animation_sun_text.on_update(delta);
	}

	void on_draw(const Camera& camera)
	{
		Player::on_draw(camera);

		if (this->is_sun_text_visible)
		{
			Vector2 text_position;
			IMAGE* frame = this->animation_sun_text.get_frame();
			text_position.x = this->position.x - (this->size.x - frame->getwidth()) / 2;
			text_position.y = this->position.y - frame->getheight();
			this->animation_sun_text.on_draw(camera, (int)text_position.x, (int)text_position.y);
		}
	}

	void on_attack()
	{
		Bullet* bullet = new SunBullet();

		Vector2 bullet_position;
		const Vector2& bullet_size = bullet->get_size();
		bullet_position.x = this->position.x + (this->size.x - bullet_size.x) / 2;
		bullet_position.y = this->position.y;

		bullet->set_position(bullet_position.x, bullet_position.y);
		bullet->set_velocity(is_facing_right ? velocity_sun.x : -velocity_sun.x, velocity_sun.y);

		bullet->set_collide_target(this->id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);	//���ù�������

		bullet->set_callback([&]() {this->mp += 35; });

		bullet_list.push_back(bullet);
	}

	void on_attack_ex()
	{
		this->is_attacking_ex = true;
		this->is_sun_text_visible = true;

		this->animation_sun_text.reset();
		this->is_facing_right ? animation_attack_ex_right.reset() : animation_attack_ex_left.reset();

		Bullet* bullet = new SunBulletEx();
		Player* target_player = (this->id == PlayerID::P1 ? player_2 : player_1);

		Vector2 bullet_position, bullet_velocity;
		const Vector2& bullet_size = bullet->get_size();
		const Vector2& target_size = target_player->get_size();
		const Vector2& target_position = target_player->get_position();
		bullet_position.x = target_position.x + (target_size.x - bullet_size.x) / 2;
		bullet_position.y = -bullet_size.y;///////////////////////////////-size.y
		bullet_velocity.x = 0;
		bullet_velocity.y = this->speed_sun_ex;

		bullet->set_position(bullet_position.x, bullet_position.y);
		bullet->set_velocity(bullet_velocity.x, bullet_velocity.y);

		bullet->set_collide_target(id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);

		bullet->set_callback([&]() {mp += 50; });

		bullet_list.push_back(bullet);

		mciSendString(_T("play sun_text from 0"), NULL, 0, NULL);
	}
private:
	const float speed_sun_ex = 0.15f;	//�����չ�ը�������ٶ�
	const Vector2 velocity_sun = { 0.25f,-0.5f };	//С���չ�ը�������ٶ�

private:
	Animation animation_sun_text;	//ͷ���ı�����
	bool is_sun_text_visible = false;//�Ƿ���ʾͷ���ı�
};