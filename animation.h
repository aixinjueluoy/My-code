#pragma once
#include"util.h"
#include"atlas.h"
#include"camera.h"
#include<functional>

class Animation//������
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

	int get_idx_frame()//��ȡ��ǰ֡����
	{
		return this->idx_frame;
	}

	IMAGE* get_frame()//��ȡ��ǰ֡ͼƬ
	{
		return this->atlas->get_image(this->idx_frame);
	}

	bool check_finished()
	{
		if (this->is_loop) return false;
		return (this->idx_frame == this->atlas->get_size() - 1);//���Ŷ��������Ƿ��Ƕ����������һ������
	}

	void on_update(int delta)
	{
		this->timer+=delta;
		if (this->timer >= this->interval)
		{
			this->timer = 0;
			this->idx_frame++;
			if (this->idx_frame >= this->atlas->get_size())//��ǰ�������Ƿ񲥷���
			{
				//���ѭ�����ţ���������Ϊ��һ�Ŷ���������ͣ�����һ֡����
				this->idx_frame = (this->is_loop ? 0 : this->atlas->get_size() - 1);
				if (!this->is_loop && this->callback)//����������Ž�������callback���ھ͵���callback��������
					callback();
			}
		}
	}

	void on_draw(const Camera& camera,int x, int y)//��ȾͼƬ
	{
		putimage_alpha(camera,x, y, this->atlas->get_image(this->idx_frame));
	}

	void set_callback(std::function<void()> callback)
	{
		this->callback = callback;
	}
private:
	int timer = 0;		//��ʱ��
	int interval = 0;	//֡���
	int idx_frame = 0;	//֡����
	bool is_loop = true;//�Ƿ�ѭ��
	Atlas* atlas = nullptr;//���ŵ�֡����ָ��

	std::function<void()> callback;//function ��һ��ģ����,������һ����������callback,<>����Ƿ���ֵ���ͺͲ���


};