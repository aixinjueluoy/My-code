#pragma once
#include"scene.h"
#include"scene_manager.h"
#include"util.h"
#include"platform.h"
#include"peashooter_player.h"
#include"sunflower_player.h"
#include"status_bar.h"
#include<vector>
#include<iostream>

extern Player* player_1;		//玩家角色对象
extern Player* player_2;

extern IMAGE* img_player_1_avatar;	//玩家头像
extern IMAGE* img_player_2_avatar;

extern IMAGE img_sky;							//天空图片
extern IMAGE img_hills;							//山脉图片
extern IMAGE img_platform_large;				//大型平台图片
extern IMAGE img_platform_small;				//小型平台

extern IMAGE img_1P_winner;					//1P获胜文本图片
extern IMAGE img_2P_winner;					//2P获胜文本图片
extern IMAGE img_winner_bar;				//获胜玩家文本背景图片

extern Camera main_camera;
extern std::vector<Platform> platform_list;

extern SceneManager scene_manager;


class GameScene :public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;
	void on_enter()
	{
		this->is_game_over = false;
		this->is_slide_out_started = false;

		this->pos_img_winner_bar.x = -img_winner_bar.getwidth();//结算条的初始位置
		this->pos_img_winner_bar.y = (getheight() - img_winner_bar.getheight()) / 2;
		this->pos_x_img_winner_bar_dst = (getwidth() - img_winner_bar.getwidth()) / 2;//结算条应滑到的位置

		this->pos_img_winner_text.x = pos_img_winner_bar.x;//结算条的初始位置
		this->pos_img_winner_text.y = (getheight() - img_1P_winner.getheight()) / 2;
		this->pos_x_img_winner_text_dst = (getwidth() - img_1P_winner.getwidth()) / 2;//结算条应滑到的位置

		this->timer_winner_slide_in.restart();
		this->timer_winner_slide_in.set_wait_time(2500);//滑入2.5秒后开始滑出
		this->timer_winner_slide_in.set_one_shot(true);
		this->timer_winner_slide_in.set_callback([&]()
			{
				is_slide_out_started = true;
			});

		this->timer_winner_slide_out.restart();
		this->timer_winner_slide_out.set_wait_time(1000);//滑出1秒后进入菜单
		this->timer_winner_slide_out.set_one_shot(true);
		this->timer_winner_slide_out.set_callback([&]()
			{
				scene_manager.switch_to(SceneManager::SceneType::Menu);
			});


		this->status_bar_1P.set_avatar(img_player_1_avatar);
		this->status_bar_2P.set_avatar(img_player_2_avatar);

		this->status_bar_1P.set_position(235, 625);
		this->status_bar_2P.set_position(675, 625);

		player_1->set_position(200, 50);//角色初始位置
		player_2->set_position(975, 50);
		this->pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
		this->pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;

		this->pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
		this->pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;

		platform_list.resize(4);

		//大平台
		Platform& large_platform = platform_list[0];	//用引用将第一个平台取名为大平台
		large_platform.img = &img_platform_large;
		large_platform.render_poition.x = 122;
		large_platform.render_poition.y = 455;
		large_platform.shape.left = (float)large_platform.render_poition.x + 30;
		large_platform.shape.right = (float)large_platform.render_poition.x + img_platform_large.getwidth() - 30;
		large_platform.shape.y = (float)large_platform.render_poition.y + 60;
		
		//小平台
		Platform& small_platform_1 = platform_list[1];
		small_platform_1.img = &img_platform_small;
		small_platform_1.render_poition.x = 175;
		small_platform_1.render_poition.y = 360;
		small_platform_1.shape.left = (float)small_platform_1.render_poition.x + 40;
		small_platform_1.shape.right = (float)small_platform_1.render_poition.x + img_platform_small.getwidth() - 40;
		small_platform_1.shape.y = (float)small_platform_1.render_poition.y + img_platform_small.getheight() / 2;

		Platform& small_platform_2 = platform_list[2];
		small_platform_2.img = &img_platform_small;
		small_platform_2.render_poition.x = 855;
		small_platform_2.render_poition.y = 360;
		small_platform_2.shape.left = (float)small_platform_2.render_poition.x + 40;
		small_platform_2.shape.right = (float)small_platform_2.render_poition.x + img_platform_small.getwidth() - 40;
		small_platform_2.shape.y = (float)small_platform_2.render_poition.y + img_platform_small.getheight() / 2;

		Platform& small_platform_3 = platform_list[3];
		small_platform_3.img = &img_platform_small;
		small_platform_3.render_poition.x = 515;
		small_platform_3.render_poition.y = 225;
		small_platform_3.shape.left = (float)small_platform_3.render_poition.x + 40;
		small_platform_3.shape.right = (float)small_platform_3.render_poition.x + img_platform_small.getwidth() - 40;
		small_platform_3.shape.y = (float)small_platform_3.render_poition.y + img_platform_small.getheight() / 2;

		mciSendString(_T("play bgm_game repeat from 0"), NULL, 0, NULL);

	}
	void on_update(int delta)
	{
		player_1->on_update(delta);
		player_2->on_update(delta);
		main_camera.on_update(delta);

		bullet_list.erase(std::remove_if(
			bullet_list.begin(),bullet_list.end(),
			[](const Bullet* bullet) {
				bool deletable = bullet->check_can_remove();
				if (deletable) delete bullet;
				return deletable;
			}), bullet_list.end());

		for (Bullet* bullet : bullet_list)
			bullet->on_update(delta);

		const Vector2& position_player_1 = player_1->get_position();
		const Vector2& position_player_2 = player_2->get_position();
		if (position_player_1.y >= getheight())
			player_1->set_hp(0);
		if (position_player_2.y >= getheight())
			player_2->set_hp(0);
		if (player_1->get_hp() <= 0 || player_2->get_hp() <= 0)
		{
			if (!this->is_game_over)
			{
				//mciSendString(_T("stop bgm_game"), NULL, 0, NULL);
				//mciSendString(_T("play ui_win from 0"), NULL, 0, NULL);
			}
			this->is_game_over = true;
		}

		//更新血条和蓝条
		this->status_bar_1P.set_hp(player_1->get_hp());
		this->status_bar_1P.set_mp(player_1->get_mp());
		this->status_bar_2P.set_hp(player_2->get_hp());
		this->status_bar_2P.set_mp(player_2->get_mp());

		if (this->is_game_over)
		{	//结算条开始移动
			pos_img_winner_bar.x += (int)(speed_winner_bar * delta);
			pos_img_winner_text.x += (int)(speed_winner_text * delta);

			if (!this->is_slide_out_started)
			{	//还没到滑出时间时，继续滑入
				timer_winner_slide_in.on_update(delta);	//滑入到达目的坐标后停住
				if (pos_img_winner_bar.x > pos_x_img_winner_bar_dst)
					pos_img_winner_bar.x = pos_x_img_winner_bar_dst;
				if (pos_img_winner_text.x > pos_x_img_winner_text_dst)
					pos_img_winner_text.x = pos_x_img_winner_text_dst;
			}
			else
				timer_winner_slide_out.on_update(delta);	//滑出时间到，开始滑出
		}
	}
	void on_draw(const Camera& camera)
	{
		putimage_alpha(main_camera,this->pos_img_sky.x, this->pos_img_sky.y, &img_sky);
		putimage_alpha(main_camera,this->pos_img_hills.x, this->pos_img_hills.y, &img_hills);

		for (const Platform& platform : platform_list)
			platform.on_draw(camera);
		if (is_debug)
		{
			settextcolor(RGB(255, 0, 0));
			outtextxy(15, 15, _T("已开启调试模式，按“Q”关闭"));
		}
		player_1->on_draw(camera);
		player_2->on_draw(camera);

		for (Bullet* bullet : bullet_list)
			bullet->on_draw(camera);

		if (this->is_game_over)
		{
			putimage_alpha(pos_img_winner_bar.x, pos_img_winner_bar.y, &img_winner_bar);
			putimage_alpha(pos_img_winner_text.x, pos_img_winner_text.y,
				player_1->get_hp() > 0 ? &img_1P_winner : &img_2P_winner);
		}
		else
		{
			this->status_bar_1P.on_draw();
			this->status_bar_2P.on_draw();
		}
	}
	void on_input(const ExMessage& msg)
	{
		player_1->on_input(msg);
		player_2->on_input(msg);
		switch (msg.message)
		{
		case WM_KEYUP:
			//"Q"
			if (msg.vkcode == 0x51)
				is_debug = !is_debug;
			break;
		default:
			break;
		}
	}
	void on_exit()
	{
		std::cout << player_1->get_hp() << std::endl;
		std::cout << player_2->get_hp() << std::endl;
		delete player_1; player_1 = nullptr;
		delete player_2; player_2 = nullptr;

		is_debug = false;

		bullet_list.clear();
		main_camera.reset();
	}
private:
	const float speed_winner_bar = 3.0f;	//结算动效背景移动速度
	const float speed_winner_text = 1.5f;	//结算动效文本移动速度

private:
	POINT pos_img_sky = { 0 };	//天空背景图位置
	POINT pos_img_hills = { 0 };//山脉背景图位置
	StatusBar status_bar_1P;	//玩家状态条
	StatusBar status_bar_2P;

	bool is_game_over = false;	//游戏是否结束

	POINT pos_img_winner_bar = { 0 };	//结算动效背景位置
	POINT pos_img_winner_text = { 0 };	//结算动效文本位置
	int pos_x_img_winner_bar_dst = 0;	//结算动效背景移动的目标位置
	int pos_x_img_winner_text_dst = 0;	//结算动效文本移动的目标位置
	Timer timer_winner_slide_in;		//结算动效滑入定时器
	Timer timer_winner_slide_out;		//结算动效滑出定时器
	bool is_slide_out_started = false;	//结算动效是否开始滑出
};