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
		this->size.x = 96;//�ӵ��ߴ�
		this->size.y = 96;

		this->damage = 20;//�ӵ��˺�

		this->animation_idle.set_atlas(&atlas_sun);//Ĭ�϶�������
		this->animation_idle.set_interval(50);

		this->animation_explode.set_atlas(&atlas_sun_explode);//��ը��������
		this->animation_explode.set_interval(50);
		this->animation_explode.set_loop(false);
		this->animation_explode.set_callback([&]() {can_remove = true; });

		IMAGE* frame_idle = this->animation_idle.get_frame();//��ȡ��ǰ������֡ͼƬ
		IMAGE* frame_explode = this->animation_explode.get_frame();
		this->explode_render_offset.x = (frame_explode->getwidth() - frame_idle->getwidth()) / 2.0f;//����������x���λ��ƫ��
		this->explode_render_offset.y = (frame_explode->getheight() - frame_idle->getheight()) / 2.0f;//����������y���λ��ƫ��
	}
	~SunBullet() = default;


	void on_collide()
	{
		Bullet::on_collide();//���ø�����߼�����

		main_camera.shake(5, 250);//��ըʱ���������
		
		mciSendString(_T("play sun_explode from 0"), NULL, 0, NULL);
	}

	void on_update(int delta)
	{
		if (this->vaild)//���ж��ӵ��Ƿ���Ч���ڸ����ӵ�λ��
		{
			this->velocity.y += this->gravity * delta;
			this->position += this->velocity*(float)delta;
		}

		if (!this->vaild)	//�ӵ�ʧЧ�����¶���
			this->animation_explode.on_update(delta);
		else				//���ûʧЧ����Ĭ�϶���
			this->animation_idle.on_update(delta);
		
		if (this->check_if_exceeds_screen())//�Էɳ���Ļ����ӵ����
			this->can_remove = true;
	}

	void on_draw(const Camera& camera)
	{
		if (this->vaild)		//�ӵ���Ч�����ӵ�����
			this->animation_idle.on_draw(camera, (int)this->position.x, (int)this->position.y);
		else
		{
			this->animation_explode.on_draw(camera,		//��ը����Ҫ���ӵ������뱬ը������λ��ƫ��
				(int)(this->position.x-this->explode_render_offset.x),
				(int)(this->position.y-this->explode_render_offset.y));
		}

		Bullet::on_draw(camera);
	}
private:
	const float gravity = 1e-3f;	//�չ�ը������

private:
	Animation animation_idle;		//�չ�ը��Ĭ�϶���  ��Ϊ�㶹���ֵ��ӵ�Ĭ���������ͼƬ�������տ��ӵ�Ĭ���������ը��
	Animation animation_explode;	//�չ�ը����ը����
	Vector2 explode_render_offset;	//��ը������Ⱦƫ��  ��Ϊ��ը�����Դ��������ӵ�����  ȷ���������������ĵ���ͬ
};