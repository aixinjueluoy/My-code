#pragma once
#include"vector2.h"
#include"util.h"
#include"atlas.h"

class Particle//���Ӷ�����Ч
{
public:
	Particle() = default;
	Particle(const Vector2& position, Atlas* atlas, int lifespan)
		:position(position), atlas(atlas), lifespan(lifespan) {}
	~Particle() = default;

	void set_atlas(Atlas* new_atlas)
	{
		this->atlas = new_atlas;
	}

	void set_position(const Vector2& new_position)
	{
		this->position = new_position;
	}

	void set_lifespan(int ms)//�������ӵ���������
	{
		this->lifespan = ms;
	}

	bool check_valid() const
	{
		return this->valid;
	}

	void on_update(int delta)
	{
		this->timer += delta;
		if (this->timer >= this->lifespan)
		{
			this->timer = 0;
			this->idx_frame++;
			if (this->idx_frame >= this->atlas->get_size())
			{
				this->idx_frame = this->atlas->get_size() - 1;
				valid = false;
			}
		}
	}

	void on_draw(const Camera& camera)	const
	{
		putimage_alpha(camera, (int)position.x, (int)position.y, atlas->get_image(idx_frame));
	}

private:
	int timer = 0;
	int lifespan = 0;	//��֡���Ӷ�������ʱ��
	int idx_frame = 0;
	Vector2 position;
	bool valid = true;
	Atlas* atlas = nullptr;	//���Ӷ�����ʹ�õ�ͼ��
};