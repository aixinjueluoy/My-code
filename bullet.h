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

	void set_damage(int val)//设置子弹伤害
	{
		this->damage = val;
	}

	int get_damage()//获取子弹上害
	{
		return this->damage;
	}

	void set_position(float x, float y)//设置子弹位置
	{
		this->position.x = x;
		this->position.y = y;
	}

	const Vector2& get_position() const//获取子弹位置
	{
		return this->position;
	}

	const Vector2& get_size()const//获取子弹碰撞尺寸
	{
		return this->size;
	}

	void set_velocity(float x, float y)//设置子弹速度
	{
		this->velocity.x = x;
		this->velocity.y = y;
	}

	void set_collide_target(PlayerID target)//设置子弹碰撞的目标
	{
		this->target_id = target;
	}

	PlayerID get_collide_target()//获取子弹碰撞的目标
	{
		return this->target_id;
	}

	void set_callback(std::function<void()> callback)//设置回调函数，保存在对象内部
	{
		this->callback = callback;
	}

	void set_valid(bool flag)//设置子弹是否有效
	{
		this->vaild = flag;
	}

	bool get_valid()const//获取子弹是否有效
	{
		return this->vaild;
	}

	bool check_can_remove()const//获取子弹是否可以移除
	{
		return this->can_remove;
	}

	virtual void on_collide()//发生碰撞时调用
	{
		if (callback)
			callback();
	}

	virtual bool check_collision(const Vector2& player_position,const Vector2& player_size)//玩家位置和玩家碰撞检测的大小  检查是否发生碰撞
	{
		return this->position.x + this->size.x / 2 >= player_position.x						//子弹中心点是否在玩家检测内部
			&& this->position.x + this->size.x / 2 <= player_position.x + player_size.x
			&& this->position.y + this->size.y / 2 >= player_position.y
			&& this->position.y + this->size.y / 2 <= player_position.y + player_size.y;
	}

	virtual void on_update(int delta)//子弹更新
	{

	}

	virtual void on_draw(const Camera& camera)//子弹绘图
	{
		if (is_debug)//检测
		{
			setfillcolor(RGB(255, 255, 255));
			setlinecolor(RGB(255, 255, 255));
			rectangle((int)this->position.x, (int)this->position.y,
				(int)this->position.x + (int)this->size.x, (int)this->position.y + (int)this->size.y);//大型日光炸弹
			solidcircle((int)(this->position.x + (int)this->size.x / 2), (int)(this->position.y + (int)this->size.y / 2), 5);//豌豆和小型日光炸弹
		}
	}
protected:					//确保子类可以访问
	Vector2 size;			//子弹碰撞尺寸
	Vector2 position;		//子弹位置
	Vector2 velocity;		//子弹速度
	int damage = 10;		//子弹伤害
	//播放子弹动画，碰撞后变为无效，然后播放子弹爆炸动画，播放完后才可删除子弹
	bool vaild = true;		//子弹是否有效
	bool can_remove = false;//子弹是否可以被删除可以
	std::function<void()> callback;//子弹碰撞回调函数

	PlayerID target_id = PlayerID::P1;//子弹碰撞目标玩家ID

protected:
	bool check_if_exceeds_screen()//检测飞出屏幕外的子弹，进行删除
	{
		return (this->position.x + this->size.x <= 0 || this->position.x >= getwidth()	//x坐标飞出屏幕外
			|| this->position.y + this->size.y <= 0 || this->position.y >= getheight());	//y坐标飞出屏幕外
	}

};