#pragma once
#include<graphics.h>
#include<vector>


class Atlas//动画图集
{
public:
	Atlas() = default;
	~Atlas() = default;

	void load_from_file(LPCTSTR path_template, int num)
	{
		img_list.clear();//首先清空
		this->img_list.resize(num);
		TCHAR path_file[256];
		
		for (size_t i = 0; i < num; i++)//加载图片到img_list容器中
		{
			_stprintf_s(path_file, path_template, i + 1);
			loadimage(&this->img_list[i], path_file);
		}
	}

	void clear()//清空图集
	{
		this->img_list.clear();
	}

	int get_size()//获取图集数量
	{
		return (int)this->img_list.size();
	}

	IMAGE* get_image(int idx)//获取指定图集
	{
		if (idx < 0 || idx >= this->img_list.size())
			return nullptr;//索引越界返回空指针
		return &this->img_list[idx];
	}

	void add_amage(const IMAGE& img)//添加图集
	{
		this->img_list.push_back(img);
	}

private:
	std::vector<IMAGE> img_list;
};