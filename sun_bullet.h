#pragma once
#include"bullet.h"
#include"animation.h"

extern Atlas atlas_sun;
extern Atlas atlas_sun_explode;

extern Camera main_camera;

class SunBullet :public Bullet
{
public:
	SunBullet()
	{
		this->size.x = 96;//子弹尺寸
		this->size.y = 96;

		this->damage = 20;//子弹伤害

		this->animation_idle.set_atlas(&atlas_sun);//默认动画设置
		this->animation_idle.set_interval(50);

		this->animation_explode.set_atlas(&atlas_sun_explode);//爆炸动画设置
		this->animation_explode.set_interval(50);
		this->animation_explode.set_loop(false);
		this->animation_explode.set_callback([&]() {can_remove = true; });

		IMAGE* frame_idle = this->animation_idle.get_frame();//获取当前动画的帧图片
		IMAGE* frame_explode = this->animation_explode.get_frame();
		this->explode_render_offset.x = (frame_explode->getwidth() - frame_idle->getwidth()) / 2.0f;//计算两动画x轴的位移偏量
		this->explode_render_offset.y = (frame_explode->getheight() - frame_idle->getheight()) / 2.0f;//计算两动画y轴的位移偏量
	}
	~SunBullet() = default;


	void on_collide()
	{
		Bullet::on_collide();//调用父类的逻辑方法

		main_camera.shake(5, 250);//爆炸时摄像机抖动
		
		mciSendString(_T("play sun_explode from 0"), NULL, 0, NULL);
	}

	void on_update(int delta)
	{
		if (this->vaild)//先判断子弹是否有效后在更新子弹位置
		{
			this->velocity.y += this->gravity * delta;
			this->position += this->velocity*(float)delta;
		}

		if (!this->vaild)	//子弹失效，更新动画
			this->animation_explode.on_update(delta);
		else				//如果没失效更新默认动画
			this->animation_idle.on_update(delta);
		
		if (this->check_if_exceeds_screen())//对飞出屏幕外的子弹检查
			this->can_remove = true;
	}

	void on_draw(const Camera& camera)
	{
		if (this->vaild)		//子弹有效播放子弹动画
			this->animation_idle.on_draw(camera, (int)this->position.x, (int)this->position.y);
		else
		{
			this->animation_explode.on_draw(camera,		//爆炸动画要加子弹动画与爆炸动画的位置偏移
				(int)(this->position.x-this->explode_render_offset.x),
				(int)(this->position.y-this->explode_render_offset.y));
		}

		Bullet::on_draw(camera);
	}
private:
	const float gravity = 1e-3f;	//日光炸弹重力

private:
	Animation animation_idle;		//日光炸弹默认动画  因为豌豆射手的子弹默认情况下是图片，而龙日葵子弹默认情况就是炸弹
	Animation animation_explode;	//日光炸弹爆炸动画
	Vector2 explode_render_offset;	//爆炸动画渲染偏移  因为爆炸动画略大于阳光子弹动画  确保两个动画的中心点相同
};