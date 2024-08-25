#pragma once
#include<iostream>
#include"scene.h"
#include"scene_manager.h"
#include"animation.h"
#include"timer.h"

//extern Atlas atlas_peashooter_run_right;
extern SceneManager scene_manager;
extern IMAGE img_menu_background;

class MenuScene :public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;
	void on_enter()
	{
		mciSendString(_T("play bgm_menu repeat from 0"), NULL,0,NULL);

		//²âÊÔ
		/*this->animation_peashooter_run_right.set_atlas(&atlas_peashooter_run_right);
		this->animation_peashooter_run_right.set_interval(75);
		this->animation_peashooter_run_right.set_loop(true);*/


		/*this->timer.set_wait_time(1000);
		this->timer.set_one_shot(false);
		this->timer.set_callback([]()
			{
				std::cout << "shot" << std::endl;
			});*/
	}
	void on_update(int delta)
	{

		//²âÊÔ
		/*this->timer.on_update(delta);
		this->camera.on_update(delta);
		this->animation_peashooter_run_right.on_update(delta);*/
	}
	void on_draw(const Camera& camera)
	{
		putimage(0, 0, &img_menu_background);
		//²âÊÔ
		////this->animation_peashooter_run_right.on_draw(100, 100);
		//const Vector2& pos_camera = camera.get_position();
		//this->animation_peashooter_run_right.on_draw((int)(100 - pos_camera.x), (int)(100 - pos_camera.y));
	}
	void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYUP)//°´¼üÌ§ÆğºóÌø×ª
		{
			mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);
			scene_manager.switch_to(SceneManager::SceneType::Select);
		}
		//²âÊÔ
		/*if (msg.message == WM_KEYDOWN)
		{
			camera.shake(10, 350);
		}*/
	}
	void on_exit()
	{


		//²âÊÔ
		//std::cout << "ÍË³ö²Ëµ¥" << std::endl;
	}


private:



	//²âÊÔ
	/*Timer timer;
	Camera camera;
	Animation animation_peashooter_run_right;*/
};