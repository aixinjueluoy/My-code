#pragma once
#include<functional>

class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	void restart()
	{
		this->pass_time = 0;
		this->shotted = false;
	}

	void set_wait_time(int val)
	{
		this->wait_time = val;
	}

	void set_one_shot(bool flag)
	{
		this->one_shot = flag;
	}

	void set_callback(std::function<void()> callback)
	{
		this->callback = callback;
	}

	void pause()//��ͣ��ʱ��
	{
		this->paused = true;
	}

	void resume()//�ظ���ʱ��
	{
		this->paused = false;
	}

	void on_update(int delta)
	{
		if (this->paused)
			return;
		this->pass_time += delta;
		if (this->pass_time >= this->wait_time)
		{
			//�ֿɶ�δ�����ʱ���͵��δ�����ʱ��
			if ((!one_shot || (one_shot && !shotted)) && callback)//������ǵ��δ�����ʱ�������ǵ��δ�����ʱ������û�������ģ��������ʱ���ɱ�������ͬʱ�ص�����Ҫ����
				callback();
			shotted = true;
			this->pass_time = 0;
		}
	}

private:
	int pass_time = 0;		//�ѹ�ʱ��
	int wait_time = 0;		//�ȴ�ʱ��
	bool paused = false;	//�Ƿ���ͣ
	bool shotted = false;	//�Ƿ񴥷�
	bool one_shot = false;	//���δ���
	std::function<void()> callback;//�����ص�

};