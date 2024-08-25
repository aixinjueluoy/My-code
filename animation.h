#pragma once
#include"util.h"
#include"atlas.h"
#include"camera.h"
#include<functional>

class Animation//播放器
{
public:
	Animation() = default;
	~Animation() = default;


	void reset()
	{
		this->timer = 0;
		this->idx_frame=0;
	}

	void set_atlas(Atlas* new_atlas)
	{
		this->reset();
		this->atlas = new_atlas;
	}

	void set_loop(bool flag)
	{
		this->is_loop = flag;
	}

	void set_interval(int ms)
	{
		this->interval = ms;
	}

	int get_idx_frame()//获取当前帧索引
	{
		return this->idx_frame;
	}

	IMAGE* get_frame()//获取当前帧图片
	{
		return this->atlas->get_image(this->idx_frame);
	}

	bool check_finished()
	{
		if (this->is_loop) return false;
		return (this->idx_frame == this->atlas->get_size() - 1);//播放动画索引是否是动画集的最后一个动画
	}

	void on_update(int delta)
	{
		this->timer+=delta;
		if (this->timer >= this->interval)
		{
			this->timer = 0;
			this->idx_frame++;
			if (this->idx_frame >= this->atlas->get_size())//当前动画集是否播放完
			{
				//如果循环播放，设置索引为第一张动画，否则，停在最后一帧动画
				this->idx_frame = (this->is_loop ? 0 : this->atlas->get_size() - 1);
				if (!this->is_loop && this->callback)//如果动画播放结束并且callback存在就调用callback函数对象
					callback();
			}
		}
	}

	void on_draw(const Camera& camera,int x, int y)//渲染图片
	{
		putimage_alpha(camera,x, y, this->atlas->get_image(this->idx_frame));
	}

	void set_callback(std::function<void()> callback)
	{
		this->callback = callback;
	}
private:
	int timer = 0;		//计时器
	int interval = 0;	//帧间隔
	int idx_frame = 0;	//帧索引
	bool is_loop = true;//是否循环
	Atlas* atlas = nullptr;//播放的帧动画指针

	std::function<void()> callback;//function 是一个模板类,创建了一个函数对象callback,<>里的是返回值类型和参数


};