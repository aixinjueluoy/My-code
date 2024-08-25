#pragma once
#include"camera.h"
#include"vector2.h"
#include"animation.h"
#include"player_id.h"
#include"platform.h"
#include"bullet.h"
#include"particle.h"
#include<graphics.h>
#include<iostream>

extern bool is_debug;

extern IMAGE img_1P_cursor;//���ͼƬ
extern IMAGE img_2P_cursor;

extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;

extern Atlas atlas_run_effect;				//������Ч����ͼ��
extern Atlas atlas_jump_effect;				//��Ծ��Ч����ͼ��
extern Atlas atlas_land_effect;				//�����Ч����ͼ��
extern int k;

class Player
{
public:
	Player(bool facing_right=true):is_facing_right(facing_right)
	{

		this->current_animation = this->is_facing_right?&animation_idle_right:&animation_idle_left;	//��ʼ��

		this->timer_attack_cd.set_wait_time(this->attack_cd);
		this->timer_attack_cd.set_one_shot(true);
		this->timer_attack_cd.set_callback([&]()
			{
				this->can_attack = true;
			});
		this->timer_invulnerable.set_wait_time(750);//��ʼ���ܻ����޵�ʱ��
		this->timer_invulnerable.set_one_shot(true);
		this->timer_invulnerable.set_callback([&]()
			{
				is_invulnerable = false;//�޵�ʱ��������
			});
		this->timer_invulnerable_blink.set_wait_time(75);//75������һ��
		this->timer_invulnerable_blink.set_callback([&]()
			{
				is_showing_sketch_frame = !is_showing_sketch_frame;//��ת�Ƿ���Ҫ��ʾ��Ӱ��bool����
			});
		//����������Ч
		this->timer_run_effect_generation.set_wait_time(75);
		this->timer_run_effect_generation.set_callback([&]()
			{
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.get_image(0);
				particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
				particle_position.y = position.y + size.y - frame->getheight();
				particle_list.emplace_back(particle_position, &atlas_run_effect, 45);
			});
		//����������Ч
		this->timer_die_effect_generation.set_wait_time(35);//35���������ٶ�
		this->timer_die_effect_generation.set_callback([&]()
			{
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.get_image(0);
				particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
				particle_position.y = position.y + size.y - frame->getheight();
				particle_list.emplace_back(particle_position, &atlas_run_effect, 150);//���������������150�����Ӵ��ʱ��
			});

		this->animation_jump_effect.set_atlas(&atlas_jump_effect);
		this->animation_jump_effect.set_interval(25);
		this->animation_jump_effect.set_loop(false);
		this->animation_jump_effect.set_callback([&]()
			{
				is_jump_effect_visible = false;
			});

		this->animation_land_effect.set_atlas(&atlas_land_effect);
		this->animation_land_effect.set_interval(50);
		this->animation_land_effect.set_loop(false);
		this->animation_land_effect.set_callback([&]()
			{
				is_land_effect_visible = false;
			});

		this->timer_cursor_visibility.set_wait_time(2500);
		this->timer_cursor_visibility.set_one_shot(true);
		this->timer_cursor_visibility.set_callback([&]()
			{
				is_cursor_visible = false;
			});

	}
	~Player() = default;

	virtual void on_update(int delta)
	{
		int direction = this->is_right_key_down - this->is_left_key_down;//-1,0,1    ����������Ҽ��������߶����������Ҽ�


		if (direction != 0)
		{

			if(!this->is_attacking_ex)//�����⼼��ʱ����ת��
				this->is_facing_right = direction > 0;	//0��1   ���󣬳���
			//
			this->current_animation = this->is_facing_right ? &this->animation_run_right : &this->animation_run_left;
			float distance = this->run_velocity * delta * direction;	//�ƶ�����
			this->on_run(distance);
		}
		else
		{
			this->current_animation = this->is_facing_right ? &this->animation_idle_right : &this->animation_idle_left;
			this->timer_run_effect_generation.pause();//���ܶ�ʱ�������Ӷ�ʱ���ݶ�
		}

		if (this->is_attacking_ex)
			this->current_animation = this->is_facing_right ? &animation_attack_ex_right : &animation_attack_ex_left;

		if (this->hp<=0)
		{
			current_animation = last_hurt_direction.x < 0 ? &animation_die_left : &animation_die_right;

		}
		this->current_animation->on_update(delta);
		this->animation_jump_effect.on_update(delta);
		this->animation_land_effect.on_update(delta);

		this->timer_attack_cd.on_update(delta);
		this->timer_invulnerable.on_update(delta);
		this->timer_invulnerable_blink.on_update(delta);
		this->timer_run_effect_generation.on_update(delta);
		this->timer_cursor_visibility.on_update(delta);

		if (this->hp <= 0)
			this->timer_die_effect_generation.on_update(delta);

		particle_list.erase(std::remove_if(				//�Ƴ�ʧЧ������
			particle_list.begin(), particle_list.end(),
			[](const Particle& particle)
			{
				return !particle.check_valid();
			}),
			particle_list.end());
		for (Particle& particle : particle_list)		//���ε�����Ч���ӵ�on_update����
			particle.on_update(delta);

		if (this->is_showing_sketch_frame)
			sketch_img(current_animation->get_frame(), &img_sketch);//�ú�������ӰͼƬ�洢��img_sketch��


		this->move_and_collide(delta);

	}


	virtual void on_draw(const Camera& camera)
	{
		if (this->is_jump_effect_visible)
			this->animation_jump_effect.on_draw(camera, (int)this->position_jump_effect.x, (int)this->position_jump_effect.y);
		if (this->is_land_effect_visible)
			this->animation_land_effect.on_draw(camera, (int)this->position_land_effect.x, (int)this->position_land_effect.y);

		for (Particle& particle : particle_list)
			particle.on_draw(camera);

		if (this->is_showing_sketch_frame && this->hp > 0 && this->is_invulnerable)//��Ҫ��ʾ��Ӱ��Ѫ������0�������޵�
			putimage_alpha(camera, (int)this->position.x, (int)this->position.y,&this->img_sketch);
		else
			this->current_animation->on_draw(camera, (int)this->position.x, (int)this->position.y);

		if (this->is_cursor_visible)
		{
			switch (id)
			{
			case PlayerID::P1:
				putimage_alpha(camera, (int)(this->position.x + (this->size.x - img_1P_cursor.getwidth()) / 2)
					, (int)(this->position.y - img_1P_cursor.getheight()), &img_1P_cursor);
				break;
			case PlayerID::P2:
				putimage_alpha(camera, (int)(this->position.x + (this->size.x - img_2P_cursor.getwidth()) / 2)
					, (int)(this->position.y - img_2P_cursor.getheight()), &img_2P_cursor);
				break;
			default:
				break;
			}
		}
		if (is_debug)
		{
			setlinecolor(RGB(0, 125, 255));
			rectangle((int)this->position.x, (int)this->position.y, (int)(this->position.x + this->size.x), (int)(this->position.y + this->size.y));	//�ĸ��������
		}
	}

	virtual void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN:
			switch (this->id)
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
					// "A"
				case 0x41:
					this->is_left_key_down = true;
					break;
					// "D"
				case 0x44:
					this->is_right_key_down = true;
					break;
					// "w"
				case 0x57:
					this->on_jump();
					break;
					// "F"
				case 0x46:
					if (this->can_attack)
					{
						this->on_attack();
						this->can_attack = false;
						this->timer_attack_cd.restart();
					}
					break;
					//"G"
				case 0x47:
					if (mp >= 100)//�������˲����ͷż���
					{
						this->on_attack_ex();
						this->mp = 0;
					}
					break;
				}
				break;
			case PlayerID::P2:
				switch (msg.vkcode)
				{
					// "<-"
				case VK_LEFT:
					this->is_left_key_down = true;
					break;
					// "->"
				case VK_RIGHT:
					this->is_right_key_down = true;
					break;
				case VK_UP:
					this->on_jump();
					break;
					// "1"
				case VK_NUMPAD1:
					if (this->can_attack)//VK_OEM_PERIOD
					{
						this->on_attack();
						this->can_attack = false;
						this->timer_attack_cd.restart();
					}
					break;
					//"2"
				case VK_NUMPAD2:
					if (mp >= 100)//�������˲����ͷż���
					{
						this->on_attack_ex();
						this->mp = 0;
					}
					break;
				}
				break;
			}
			break;
		case WM_KEYUP:
			switch (this->id)
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
					// "A"
				case 0x41:
					this->is_left_key_down = false;
					break;
					// "D"
				case 0x44:
					this->is_right_key_down = false;
					break;
				}
				break;
			case PlayerID::P2:
				switch (msg.vkcode)
				{
					// "<-"
				case VK_LEFT:
					this->is_left_key_down = false;
					break;
					// "->"
				case VK_RIGHT:
					this->is_right_key_down = false;
					break;
					//"/"
				case VK_OEM_2:
					if (mp >= 100)//�������˲����ͷż���
					{
						this->on_attack_ex();
						this->mp = 0;
					}
					break;
				}
				break;
			}
		break;
		}
	}

	void set_id(PlayerID id)
	{
		this->id = id;
	}

	void set_position(float x, float y)
	{
		this->position.x = x;
		this->position.y = y;
	}

	const Vector2& get_position()
	{
		return this->position;
	}

	const Vector2& get_size()
	{
		return this->size;
	}

	void set_hp(int val)

	{
		this->hp = val;
	}

	virtual void on_run(float distance)
	{
		if (this->is_attacking_ex)
			return;
		this->position.x += distance;
		this->timer_run_effect_generation.resume();//���ܶ���ʱ���ָ�����
	}


	virtual void on_jump()
	{
		if (this->velocity.y != 0||this->is_attacking_ex)//���y���ٶ�Ϊ�㲢��û���ͷż���ʱ��������
			return;
		this->velocity.y += this->jump_velocity;

		this->is_jump_effect_visible = true;
		this->animation_jump_effect.reset();//���ö���״̬

		IMAGE* effect_frame = animation_jump_effect.get_frame();
		this->position_jump_effect.x = this->position.x + (this->size.x - effect_frame->getwidth()) / 2;//�ڽ�ɫ�����·��м�
		this->position_jump_effect.y = this->position.y + this->size.y - effect_frame->getheight();
	}

	virtual void on_land()
	{
		this->is_land_effect_visible = true;
		this->animation_land_effect.reset();

		IMAGE* effect_frame = this->animation_land_effect.get_frame();
		this->position_land_effect.x = this->position.x + (this->size.x - effect_frame->getwidth()) / 2;
		this->position_land_effect.y = this->position.y + this->size.y - effect_frame->getheight();

	}

	void make_invulnerable()
	{
		this->is_invulnerable = true;
		this->timer_invulnerable.restart();
	}

	int get_hp()	const
	{
		return this->hp;
	}

	int get_mp()	const
	{
		return this->mp;
	}
	virtual void on_attack(){}

	virtual void on_attack_ex(){}
protected:
	void move_and_collide(int delta)//�ƶ�����ײ
	{
		float last_velocity_y = this->velocity.y;//���ǰһ֡���ٶ�
		//�ƶ��߼�
		this->velocity.y += gravity * delta;
		this->position += velocity * (float)delta;//x��y�����ϵ�λ�ü���x��y�����ϵ��ٶ�	�������λ��

		if (this->hp <= 0)
			return;
		//��ײ�߼�
		if (this->velocity.y > 0)//����ٶ�����ʱ���
		{
			for (const Platform& platform : platform_list)
			{
				const Platform::CollisonShape& shape = platform.shape;
				bool is_collide_x = (max(this->position.x + this->size.x, shape.right) - min(this->position.x, shape.left)//�жϣ����������ұ�����-��С����������Ƿ����ƽ̨���Ⱥ���ҳ���
					<= this->size.x + (shape.right - shape.left));
				bool is_collide_y = (shape.y >= this->position.y && shape.y <= this->position.y + this->size.y);

				if (is_collide_x && is_collide_y)//���x�����y���������ײ�߼�		������ײ
				{
					//�ж�ǰһ֡��ҵĽŵ��Ƿ񴩹�ƽ̨
					float delta_pos_y = velocity.y * delta;	//��һ֡y���λ��
					float last_tick_foot_pos_y = this->position.y + this->size.y - delta_pos_y;//�˻���һ֡��y��λ��
					if (last_tick_foot_pos_y <= shape.y)//��һ֡��ҵĽ���ƽ̨����
					{
						//�������λ�ú�y���ٶ�
						this->position.y = shape.y - this->size.y;
						this->velocity.y = 0;
						if (last_velocity_y != 0)//ǰһ֡�ٶȲ�Ϊ�㣬��ײ���ٶ�Ϊ�㣬˵�����
							this->on_land();

						break;
					}
				}
			}
		}
		if (!this->is_invulnerable)//��ɫ�������޵в��ܷ�����ײ
		{
			for (Bullet* bullet : bullet_list)
			{
				if (!bullet->get_valid() || bullet->get_collide_target() != id)
					continue;
				if (bullet->check_collision(this->position, this->size))
				{
					this->make_invulnerable();
					bullet->on_collide();
					bullet->set_valid(false);
					this->hp -= bullet->get_damage();
					this->last_hurt_direction = bullet->get_position() - this->position;
					if (this->hp <= 0)
					{
						this->velocity.x = this->last_hurt_direction.x < 0 ? 0.35f : -0.35f;
						velocity.y = -1.0f;
					}
				}
			}
		}
	}
protected:
	const float gravity = 1.6e-3f;		//�������
	const float run_velocity = 0.55f;	//�����ٶ�
	const float jump_velocity = -0.85f;	//��Ծ�ٶ�  ������Ծ����Ϊ��ֵ


protected://ȷ��������Է���
	int mp = 0;			//�������
	int hp = 100;		//�������ֵ

	Vector2 size;			//��ҵ���ײ����
	Vector2 position;		//��ά������¼���λ��
	Vector2 velocity;		//����ٶ�

	Animation animation_idle_left;	//Ĭ�ϳ��󶯻�
	Animation animation_idle_right;
	Animation animation_run_left;	//�����ܶ���
	Animation animation_run_right;
	Animation animation_attack_ex_left;	//�������⹥������
	Animation animation_attack_ex_right;	//�������⹥������
	Animation animation_jump_effect;		//��Ծ������Ч
	Animation animation_land_effect;		//�����Ч����
	Animation animation_die_left;			//�������������
	Animation animation_die_right;			//���ҵ���������


	bool is_jump_effect_visible = false;	//�����Ƿ�ɼ�
	bool is_land_effect_visible = false;

	Vector2 position_jump_effect;			//��Чλ��
	Vector2 position_land_effect;

	Animation* current_animation = nullptr;	//��ǰ���ڲ��ŵĶ�������

	PlayerID id = PlayerID::P1;	//������id

	bool is_left_key_down = false;//���󰴼��Ƿ���
	bool is_right_key_down = false;//���Ұ����Ƿ���

	bool is_facing_right = true;	//����Ƿ�����

	int attack_cd = 500;		//��ͨ������ȴʱ��
	bool can_attack = true;		//��ɫ�Ƿ�����ͷ���ͨ����
	Timer timer_attack_cd;		//��ͨ������ȴ��ʱ��

	bool is_attacking_ex = false;	//��ǰ�Ƿ������ͷ����⹥��

	IMAGE img_sketch;				//����֡��ӰͼƬ
	bool is_invulnerable = false;	//��ɫ�Ƿ����޵�״̬
	bool is_showing_sketch_frame = false;	//��ǰ֡�Ƿ�Ӧ����ʾ��Ӱ
	Timer timer_invulnerable;	//�޵�״̬��ʱ��
	Timer timer_invulnerable_blink;		//�޵�״̬��˸��ʱ��

	std::vector<Particle> particle_list;	//�ܶ���Ч���ӷ��䶨ʱ��

	Timer timer_run_effect_generation;		//�ܶ���Ч���ӷ��䶨ʱ��
	Timer timer_die_effect_generation;		//������Ч���ӷ��䶨ʱ��

	bool is_cursor_visible = true;		//��ҹ��ָʾ���Ƿ�ɼ�
	Timer timer_cursor_visibility;		//��ҹ��ָʾ���ɼ��Զ�ʱ��

	Vector2 last_hurt_direction;		//���һ���ܻ��ķ���
};