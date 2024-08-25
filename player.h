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

extern IMAGE img_1P_cursor;//光标图片
extern IMAGE img_2P_cursor;

extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;

extern Atlas atlas_run_effect;				//奔跑特效动画图集
extern Atlas atlas_jump_effect;				//跳跃特效动画图集
extern Atlas atlas_land_effect;				//落地特效动画图集
extern int k;

class Player
{
public:
	Player(bool facing_right=true):is_facing_right(facing_right)
	{

		this->current_animation = this->is_facing_right?&animation_idle_right:&animation_idle_left;	//初始化

		this->timer_attack_cd.set_wait_time(this->attack_cd);
		this->timer_attack_cd.set_one_shot(true);
		this->timer_attack_cd.set_callback([&]()
			{
				this->can_attack = true;
			});
		this->timer_invulnerable.set_wait_time(750);//初始化受击后无敌时间
		this->timer_invulnerable.set_one_shot(true);
		this->timer_invulnerable.set_callback([&]()
			{
				is_invulnerable = false;//无敌时间过后调用
			});
		this->timer_invulnerable_blink.set_wait_time(75);//75毫秒闪一下
		this->timer_invulnerable_blink.set_callback([&]()
			{
				is_showing_sketch_frame = !is_showing_sketch_frame;//翻转是否需要显示剪影的bool变量
			});
		//奔跑粒子特效
		this->timer_run_effect_generation.set_wait_time(75);
		this->timer_run_effect_generation.set_callback([&]()
			{
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.get_image(0);
				particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
				particle_position.y = position.y + size.y - frame->getheight();
				particle_list.emplace_back(particle_position, &atlas_run_effect, 45);
			});
		//死亡粒子特效
		this->timer_die_effect_generation.set_wait_time(35);//35粒子生成速度
		this->timer_die_effect_generation.set_callback([&]()
			{
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.get_image(0);
				particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
				particle_position.y = position.y + size.y - frame->getheight();
				particle_list.emplace_back(particle_position, &atlas_run_effect, 150);//插入的是匿名对象，150是粒子存活时间
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
		int direction = this->is_right_key_down - this->is_left_key_down;//-1,0,1    按左键，左右键都按或者都不按，按右键


		if (direction != 0)
		{

			if(!this->is_attacking_ex)//放特殊技能时不能转向
				this->is_facing_right = direction > 0;	//0，1   朝左，朝右
			//
			this->current_animation = this->is_facing_right ? &this->animation_run_right : &this->animation_run_left;
			float distance = this->run_velocity * delta * direction;	//移动距离
			this->on_run(distance);
		}
		else
		{
			this->current_animation = this->is_facing_right ? &this->animation_idle_right : &this->animation_idle_left;
			this->timer_run_effect_generation.pause();//不跑动时，是粒子定时器暂定
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

		particle_list.erase(std::remove_if(				//移除失效的粒子
			particle_list.begin(), particle_list.end(),
			[](const Particle& particle)
			{
				return !particle.check_valid();
			}),
			particle_list.end());
		for (Particle& particle : particle_list)		//依次调用有效粒子的on_update方法
			particle.on_update(delta);

		if (this->is_showing_sketch_frame)
			sketch_img(current_animation->get_frame(), &img_sketch);//用函数将剪影图片存储到img_sketch中


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

		if (this->is_showing_sketch_frame && this->hp > 0 && this->is_invulnerable)//需要显示剪影，血量大于0，处于无敌
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
			rectangle((int)this->position.x, (int)this->position.y, (int)(this->position.x + this->size.x), (int)(this->position.y + this->size.y));	//四个点的坐标
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
					if (mp >= 100)//能量满了才能释放技能
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
					if (mp >= 100)//能量满了才能释放技能
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
					if (mp >= 100)//能量满了才能释放技能
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
		this->timer_run_effect_generation.resume();//让跑动定时器恢复运行
	}


	virtual void on_jump()
	{
		if (this->velocity.y != 0||this->is_attacking_ex)//玩家y轴速度为零并且没有释放技能时才能起跳
			return;
		this->velocity.y += this->jump_velocity;

		this->is_jump_effect_visible = true;
		this->animation_jump_effect.reset();//重置动画状态

		IMAGE* effect_frame = animation_jump_effect.get_frame();
		this->position_jump_effect.x = this->position.x + (this->size.x - effect_frame->getwidth()) / 2;//在角色的正下方中间
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
	void move_and_collide(int delta)//移动和碰撞
	{
		float last_velocity_y = this->velocity.y;//玩家前一帧的速度
		//移动逻辑
		this->velocity.y += gravity * delta;
		this->position += velocity * (float)delta;//x，y方向上的位置加上x，y方向上的速度	更新玩家位置

		if (this->hp <= 0)
			return;
		//碰撞逻辑
		if (this->velocity.y > 0)//玩家速度向下时检测
		{
			for (const Platform& platform : platform_list)
			{
				const Platform::CollisonShape& shape = platform.shape;
				bool is_collide_x = (max(this->position.x + this->size.x, shape.right) - min(this->position.x, shape.left)//判断：两者最大的右边坐标-最小的左边坐标是否等于平台长度和玩家长度
					<= this->size.x + (shape.right - shape.left));
				bool is_collide_y = (shape.y >= this->position.y && shape.y <= this->position.y + this->size.y);

				if (is_collide_x && is_collide_y)//如果x方向和y方向符合碰撞逻辑		发生碰撞
				{
					//判断前一帧玩家的脚底是否穿过平台
					float delta_pos_y = velocity.y * delta;	//这一帧y轴的位移
					float last_tick_foot_pos_y = this->position.y + this->size.y - delta_pos_y;//退还这一帧的y轴位移
					if (last_tick_foot_pos_y <= shape.y)//上一帧玩家的脚在平台上面
					{
						//重置玩家位置和y轴速度
						this->position.y = shape.y - this->size.y;
						this->velocity.y = 0;
						if (last_velocity_y != 0)//前一帧速度不为零，碰撞后速度为零，说明落地
							this->on_land();

						break;
					}
				}
			}
		}
		if (!this->is_invulnerable)//角色不处于无敌才能发生碰撞
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
	const float gravity = 1.6e-3f;		//玩家重力
	const float run_velocity = 0.55f;	//奔跑速度
	const float jump_velocity = -0.85f;	//跳跃速度  向上跳跃所以为负值


protected://确保子类可以访问
	int mp = 0;			//玩家能量
	int hp = 100;		//玩家生命值

	Vector2 size;			//玩家的碰撞外形
	Vector2 position;		//二维向量记录玩家位置
	Vector2 velocity;		//玩家速度

	Animation animation_idle_left;	//默认朝左动画
	Animation animation_idle_right;
	Animation animation_run_left;	//向左奔跑动画
	Animation animation_run_right;
	Animation animation_attack_ex_left;	//向左特殊攻击动画
	Animation animation_attack_ex_right;	//向右特殊攻击动画
	Animation animation_jump_effect;		//跳跃动画特效
	Animation animation_land_effect;		//落地特效动画
	Animation animation_die_left;			//朝左的死亡动画
	Animation animation_die_right;			//朝右的死亡动画


	bool is_jump_effect_visible = false;	//动画是否可见
	bool is_land_effect_visible = false;

	Vector2 position_jump_effect;			//特效位置
	Vector2 position_land_effect;

	Animation* current_animation = nullptr;	//当前正在播放的动画对象

	PlayerID id = PlayerID::P1;	//玩家序号id

	bool is_left_key_down = false;//向左按键是否按下
	bool is_right_key_down = false;//向右按键是否按下

	bool is_facing_right = true;	//玩家是否朝向右

	int attack_cd = 500;		//普通攻击冷却时间
	bool can_attack = true;		//角色是否可以释放普通攻击
	Timer timer_attack_cd;		//普通攻击冷却定时器

	bool is_attacking_ex = false;	//当前是否正在释放特殊攻击

	IMAGE img_sketch;				//动画帧剪影图片
	bool is_invulnerable = false;	//角色是否处于无敌状态
	bool is_showing_sketch_frame = false;	//当前帧是否应该显示剪影
	Timer timer_invulnerable;	//无敌状态定时器
	Timer timer_invulnerable_blink;		//无敌状态闪烁定时器

	std::vector<Particle> particle_list;	//跑动特效粒子发射定时器

	Timer timer_run_effect_generation;		//跑动特效粒子发射定时器
	Timer timer_die_effect_generation;		//死亡特效粒子发射定时器

	bool is_cursor_visible = true;		//玩家光标指示器是否可见
	Timer timer_cursor_visibility;		//玩家光标指示器可见性定时器

	Vector2 last_hurt_direction;		//最后一次受击的方向
};