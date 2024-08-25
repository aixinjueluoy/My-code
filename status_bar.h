#pragma once
#include<graphics.h>
#include"util.h"

class StatusBar
{
public:
	StatusBar() = default;
	~StatusBar() = default;

	void set_avatar(IMAGE* img)
	{
		this->img_avatar = img;
	}

	void set_hp(int val)
	{
		this->hp = val;
	}

	void set_mp(int val)
	{
		this->mp = val;
	}

	void set_position(int x, int y)
	{
		this->position.x = x;
		this->position.y = y;
	}

	void on_draw()//����ͼƬ����Ҫ�����
	{
		putimage_alpha(this->position.x, this->position.y, this->img_avatar);	//ͷ����Ⱦ

		setfillcolor(RGB(5, 5, 5));
		solidroundrect(this->position.x + 100, this->position.y + 10, this->position.x + 100 + this->width + 3 * 2, this->position.y + 36, 8, 8);//���Բ�Ǿ���,���Ͻ����꣬���½����꣬Բ�ǵ�ˮƽ�뾶�ʹ�ֱ�뾶
		solidroundrect(this->position.x + 100, this->position.y + 45, this->position.x + 100 + this->width + 3 * 2, this->position.y + 71, 8, 8);
		//��������ʵ����ӰЧ��  ƫ��Ϊ3
		setfillcolor(RGB(67, 47, 47));
		solidroundrect(this->position.x + 100, this->position.y + 10, this->position.x + 100 + this->width + 3, this->position.y + 33, 8, 8);
		solidroundrect(this->position.x + 100, this->position.y + 45, this->position.x + 100 + this->width + 3, this->position.y + 68, 8, 8);

		float hp_bar_width = this->width * max(0, this->hp) / 100.0f;//���ݵ�ǰ״̬������Ҫ������
		float mp_bar_width = this->width * min(100, this->mp) / 100.0f;
		setfillcolor(RGB(197, 61, 67));
		solidroundrect(this->position.x + 100, this->position.y + 10, this->position.x + 100 + (int)hp_bar_width+3, this->position.y + 33, 8, 8);
		setfillcolor(RGB(83, 131, 195));
		solidroundrect(this->position.x + 100, this->position.y + 45, this->position.x + 100 + (int)mp_bar_width+3, this->position.y + 68, 8, 8);
	}

private:
	const int width = 275;	//״̬�����
	int hp = 0;			//Ѫ��
	int mp = 0;				//����
	POINT position = { 0 };	//�ڴ�������ʾ��λ��
	IMAGE* img_avatar = nullptr;//��ɫͷ��
};