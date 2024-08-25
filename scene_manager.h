#pragma once
#include"scene.h"
#include"menu_scene.h"
#include"camera.h"
//#include"game_scene.h"

extern Scene* menu_scene;//��ȡmain�е�ȫ��ָ�������
extern Scene* game_scene;
extern Scene* select_scene;

class SceneManager
{
public:
	enum class SceneType
	{
		Menu,
		Game,
		Select
	};
public:
	SceneManager() = default;
	~SceneManager() = default;

	void set_current_scene(Scene* scene)//���õ�ǰ����
	{
		current_scene = scene;
		scene->on_enter();
	}
	void switch_to(SceneType type)//��ת����
	{
		current_scene->on_exit();//�˳���ǰ����
		switch (type)//ѡ���³���
		{
		case SceneManager::SceneType::Menu:
			current_scene = menu_scene;
			break;
		case SceneManager::SceneType::Game:
			current_scene = game_scene;
			break;
		case SceneManager::SceneType::Select:
			current_scene = select_scene;
			break;
		default:
			break;
		}
		current_scene->on_enter();//�����³���
	}

	void on_update(int delta)
	{
		current_scene->on_update(delta);
	}
	void on_draw(const Camera& camera)
	{
		current_scene->on_draw(camera);
	}
	void on_input(const ExMessage& msg)
	{
		current_scene->on_input(msg);
	}


private:
	Scene* current_scene=nullptr;

};