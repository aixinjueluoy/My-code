#pragma once
#include"bullet.h"
#include"animation.h"

extern IMAGE img_pea;			//豌豆图片
extern Atlas atlas_pea_break;	//豌豆破碎图集


class PeaBullet :public Bullet
{
public:
	PeaBullet()
	{
		this->size.x = 64;//子弹尺寸
		this->size.y = 64;

		this->damage = 10;//子弹伤害



		this->animation_break.set_atlas(&atlas_pea_break);	//设置动画图集
		this->animation_break.set_interval(100);
		this->animation_break.set_loop(false);
		this->animation_break.set_callback([&]() {can_remove = true; });	//播放动画结束后设置子弹状态为可删除
	}
	~PeaBullet() = default;

	void on_collide()
	{
		Bullet::on_collide();//调用父类的逻辑方法
		switch (rand()%3)		//三种豌豆破裂的音效随机播放
		{
		case 0:
			mciSendString(_T("play pea_break_1 from 0"), NULL, 0, NULL);
			break;
		case 1:
			mciSendString(_T("play pea_break_2 from 0"), NULL, 0, NULL);
			break;
		case 2:
			mciSendString(_T("play pea_break_3 from 0"), NULL, 0, NULL);
			break;
		default:
			break;
		}
	}

	void on_update(int delta)
	{
		this->position += this->velocity * (float)delta;

		if (!this->vaild)	//如果子弹已经破碎失效，更新破裂动画
			this->animation_break.on_update(delta);

		if (this->check_if_exceeds_screen())//对飞出屏幕外的子弹检查
			this->can_remove = true;
	}

	void on_draw(const Camera& camera)
	{
		if (this->vaild)//子弹有效时播放子弹动画
		{
			putimage_alpha(camera,(int)this->position.x, (int)this->position.y, &img_pea);
		}
		else
			animation_break.on_draw(camera, (int)this->position.x, (int)this->position.y);//子弹无效后在子弹位置播放子弹破碎动画
		Bullet::on_draw(camera);
	}
private:
	Animation animation_break;	//豌豆破裂动画对象

};