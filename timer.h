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

	void pause()//暂停计时器
	{
		this->paused = true;
	}

	void resume()//回复计时器
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
			//分可多次触发计时器和单次触发计时器
			if ((!one_shot || (one_shot && !shotted)) && callback)//如果不是单次触发计时器或者是单次触发计时器但还没被触发的，则这个计时器可被触发，同时回调函数要存在
				callback();
			shotted = true;
			this->pass_time = 0;
		}
	}

private:
	int pass_time = 0;		//已过时间
	int wait_time = 0;		//等待时间
	bool paused = false;	//是否暂停
	bool shotted = false;	//是否触发
	bool one_shot = false;	//单次触发
	std::function<void()> callback;//触发回调

};