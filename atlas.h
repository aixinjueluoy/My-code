#pragma once
#include<graphics.h>
#include<vector>


class Atlas//����ͼ��
{
public:
	Atlas() = default;
	~Atlas() = default;

	void load_from_file(LPCTSTR path_template, int num)
	{
		img_list.clear();//�������
		this->img_list.resize(num);
		TCHAR path_file[256];
		
		for (size_t i = 0; i < num; i++)//����ͼƬ��img_list������
		{
			_stprintf_s(path_file, path_template, i + 1);
			loadimage(&this->img_list[i], path_file);
		}
	}

	void clear()//���ͼ��
	{
		this->img_list.clear();
	}

	int get_size()//��ȡͼ������
	{
		return (int)this->img_list.size();
	}

	IMAGE* get_image(int idx)//��ȡָ��ͼ��
	{
		if (idx < 0 || idx >= this->img_list.size())
			return nullptr;//����Խ�緵�ؿ�ָ��
		return &this->img_list[idx];
	}

	void add_amage(const IMAGE& img)//���ͼ��
	{
		this->img_list.push_back(img);
	}

private:
	std::vector<IMAGE> img_list;
};