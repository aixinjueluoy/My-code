#pragma once
#include"util.h"
#include"camera.h"

extern bool is_debug;

class Platform
{
public:
	struct CollisonShape//��ײƽ̨	����Ӿ�λ��
	{
		float y;	//ƽ̨y����
		float left, right;	//ƽ̨���Ե���ұ�Ե����
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
	CollisonShape shape;	//ƽ̨����ײ����
	IMAGE* img = nullptr;	//ƽ̨��ͼƬ
	POINT render_poition = { 0 };	//ͼƬʵ��λ��
};