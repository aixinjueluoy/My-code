#pragma once
#include"bullet.h"
#include"animation.h"

extern IMAGE img_pea;			//�㶹ͼƬ
extern Atlas atlas_pea_break;	//�㶹����ͼ��


class PeaBullet :public Bullet
{
public:
	PeaBullet()
	{
		this->size.x = 64;//�ӵ��ߴ�
		this->size.y = 64;

		this->damage = 10;//�ӵ��˺�



		this->animation_break.set_atlas(&atlas_pea_break);	//���ö���ͼ��
		this->animation_break.set_interval(100);
		this->animation_break.set_loop(false);
		this->animation_break.set_callback([&]() {can_remove = true; });	//���Ŷ��������������ӵ�״̬Ϊ��ɾ��
	}
	~PeaBullet() = default;

	void on_collide()
	{
		Bullet::on_collide();//���ø�����߼�����
		switch (rand()%3)		//�����㶹���ѵ���Ч�������
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

		if (!this->vaild)	//����ӵ��Ѿ�����ʧЧ���������Ѷ���
			this->animation_break.on_update(delta);

		if (this->check_if_exceeds_screen())//�Էɳ���Ļ����ӵ����
			this->can_remove = true;
	}

	void on_draw(const Camera& camera)
	{
		if (this->vaild)//�ӵ���Чʱ�����ӵ�����
		{
			putimage_alpha(camera,(int)this->position.x, (int)this->position.y, &img_pea);
		}
		else
			animation_break.on_draw(camera, (int)this->position.x, (int)this->position.y);//�ӵ���Ч�����ӵ�λ�ò����ӵ����鶯��
		Bullet::on_draw(camera);
	}
private:
	Animation animation_break;	//�㶹���Ѷ�������

};