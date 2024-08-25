#pragma once
#include"bullet.h"
#include"animation.h"

extern Atlas atlas_sun_ex;
extern Atlas atlas_sun_ex_explode;

extern Camera main_camera;

class SunBulletEx:public Bullet
{
public:
	SunBulletEx()
	{
		this->size.x = 288;//子弹尺寸
		this->size.y = 288;

		this->damage = 20;//子弹伤害

		this->animation_idle.set_atlas(&atlas_sun_ex);//默认动画设置
		this->animation_idle.set_interval(50);

		this->animation_explode.set_atlas(&atlas_sun_ex_explode);//爆炸动画设置
		this->animation_explode.set_interval(50);
		this->animation_explode.set_loop(false);
		this->animation_explode.set_callback([&]() {can_remove = true; });

		IMAGE* frame_idle = this->animation_idle.get_frame();//获取当前动画的帧图片
		IMAGE* frame_explode = this->animation_explode.get_frame();
		this->explode_render_offset.x = (frame_explode->getwidth() - frame_idle->getwidth()) / 2.0f;//计算两动画x轴的位移偏量
		this->explode_render_offset.y = (frame_explode->getheight() - frame_idle->getheight()) / 2.0f;//计算两动画y轴的位移偏量
	}
	~SunBulletEx() = default;

	void on_collide()
	{
		Bullet::on_collide();

		main_camera.shake(20, 350);

		mciSendString(_T("play sun_explode_ex from 0"), NULL, 0, NULL);


	}
	//尺寸较大，重写check_collide方法
	bool check_collision(const Vector2& position, const Vector2& size)//玩家坐标，和尺寸
	{
		bool is_collide_x = (max(this->position.x + this->size.x, position.x + size.x)
			- min(this->position.x, position.x) <= this->size.x + size.x);
		bool is_collide_y = (max(this->position.y + this->size.y, position.y + size.y)
			- min(this->position.y, position.y) <= this->size.y + size.y);
		return is_collide_x && is_collide_y;
	}

	void on_update(int delta)
	{
		if (this->vaild)
			this->position += this->velocity * (float)delta;
		if (!this->vaild)
			this->animation_explode.on_update(delta);
		else
			this->animation_idle.on_update(delta);
		
		if (this->check_if_exceeds_screen())
			this->can_remove = true;
	}

	void on_draw(const Camera& camera)
	{
		if (this->vaild)		//子弹有效播放子弹动画
			this->animation_idle.on_draw(camera, (int)this->position.x, (int)this->position.y);
		else
		{
			this->animation_explode.on_draw(camera,		//爆炸动画要加子弹动画与爆炸动画的位置偏移
				(int)(this->position.x - this->explode_render_offset.x),
				(int)(this->position.y - this->explode_render_offset.y));
		}
		Bullet::on_draw(camera);
	}
private:
	Animation animation_idle;		//巨型日光炸弹默认动画 
	Animation animation_explode;	//巨型日光炸弹爆炸动画
	Vector2 explode_render_offset;	//位置偏移
};