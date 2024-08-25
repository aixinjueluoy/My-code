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

	void on_draw()//绘制图片不需要摄像机
	{
		putimage_alpha(this->position.x, this->position.y, this->img_avatar);	//头像渲染

		setfillcolor(RGB(5, 5, 5));
		solidroundrect(this->position.x + 100, this->position.y + 10, this->position.x + 100 + this->width + 3 * 2, this->position.y + 36, 8, 8);//填充圆角矩形,左上角坐标，右下角坐标，圆角的水平半径和垂直半径
		solidroundrect(this->position.x + 100, this->position.y + 45, this->position.x + 100 + this->width + 3 * 2, this->position.y + 71, 8, 8);
		//绘制两次实现阴影效果  偏量为3
		setfillcolor(RGB(67, 47, 47));
		solidroundrect(this->position.x + 100, this->position.y + 10, this->position.x + 100 + this->width + 3, this->position.y + 33, 8, 8);
		solidroundrect(this->position.x + 100, this->position.y + 45, this->position.x + 100 + this->width + 3, this->position.y + 68, 8, 8);

		float hp_bar_width = this->width * max(0, this->hp) / 100.0f;//根据当前状态计算需要填充多少
		float mp_bar_width = this->width * min(100, this->mp) / 100.0f;
		setfillcolor(RGB(197, 61, 67));
		solidroundrect(this->position.x + 100, this->position.y + 10, this->position.x + 100 + (int)hp_bar_width+3, this->position.y + 33, 8, 8);
		setfillcolor(RGB(83, 131, 195));
		solidroundrect(this->position.x + 100, this->position.y + 45, this->position.x + 100 + (int)mp_bar_width+3, this->position.y + 68, 8, 8);
	}

private:
	const int width = 275;	//状态条宽度
	int hp = 0;			//血条
	int mp = 0;				//蓝条
	POINT position = { 0 };	//在窗口中显示的位置
	IMAGE* img_avatar = nullptr;//角色头像
};