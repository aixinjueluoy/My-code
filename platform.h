#pragma once
#include"util.h"
#include"camera.h"

extern bool is_debug;

class Platform
{
public:
	struct CollisonShape//碰撞平台	玩家视觉位置
	{
		float y;	//平台y坐标
		float left, right;	//平台左边缘和右边缘坐标
	};

public:
	Platform() = default;
	~Platform() = default;
	void on_draw(const Camera& camera)const
	{
		putimage_alpha(this->render_poition.x, this->render_poition.y, this->img);

		if (is_debug)
		{
			setlinecolor(RGB(255, 0, 0));
			line(camera, (int)shape.left, (int)shape.y, (int)shape.right, (int)shape.y);
		}
	}

public:
	CollisonShape shape;	//平台的碰撞外形
	IMAGE* img = nullptr;	//平台的图片
	POINT render_poition = { 0 };	//图片实际位置
};