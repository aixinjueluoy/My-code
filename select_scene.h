#pragma once
#include"scene.h"
#include"scene_manager.h"
#include"animation.h"
#include"peashooter_player.h"
#include"sunflower_player.h"
#include"player_id.h"

extern Player* player_1;
extern Player* player_2;

extern IMAGE* img_player_1_avatar;
extern IMAGE* img_player_2_avatar;

extern IMAGE img_VS;
extern IMAGE img_1P;
extern IMAGE img_2P;
extern IMAGE img_1P_desc;
extern IMAGE img_2P_desc;
extern IMAGE img_gravestone_left;
extern IMAGE img_gravestone_right;
extern IMAGE img_selector_tip;
extern IMAGE img_selector_background;
extern IMAGE img_1P_selector_btn_idle_left;
extern IMAGE img_1P_selector_btn_idle_right;
extern IMAGE img_1P_selector_btn_down_left;
extern IMAGE img_1P_selector_btn_down_right;
extern IMAGE img_2P_selector_btn_idle_left;
extern IMAGE img_2P_selector_btn_idle_right;
extern IMAGE img_2P_selector_btn_down_left;
extern IMAGE img_2P_selector_btn_down_right;
extern IMAGE img_peashooter_selector_background_left;
extern IMAGE img_peashooter_selector_background_right;
extern IMAGE img_sunflower_selector_background_left;
extern IMAGE img_sunflower_selector_background_right;

extern Atlas atlas_peashooter_idle_right;
extern Atlas atlas_sunflower_idle_right;

extern IMAGE img_avatar_peashooter;
extern IMAGE img_avatar_sunflower;

extern SceneManager scene_manager;

class SelectScene :public Scene
{
public:
	SelectScene() = default;
	~SelectScene() = default;

	void on_enter()
	{
		this->animation_peashooter.set_atlas(&atlas_peashooter_idle_right);
		this->animation_sunflower.set_atlas(&atlas_sunflower_idle_right);
		this->animation_peashooter.set_interval(100);
		this->animation_sunflower.set_interval(100);

		static const int OFFSET_X = 50;

		this->pos_img_VS.x = (getwidth() - img_VS.getwidth()) / 2;
		this->pos_img_VS.y= (getheight() - img_VS.getheight()) / 2;
		this->pos_img_tip.x = (getwidth() - img_selector_tip.getwidth()) / 2;
		this->pos_img_tip.y = getheight() - 125;
		this->pos_img_1P.x = (getwidth() / 2 - img_1P.getwidth()) / 2 - OFFSET_X;
		this->pos_img_1P.y = 35;
		this->pos_img_2P.x = getwidth() / 2 + (getwidth()/2-img_2P.getwidth()) / 2 + OFFSET_X;
		this->pos_img_2P.y = this->pos_img_1P.y;
		this->pos_img_1P_desc.x = (getwidth() / 2 - img_1P_desc.getwidth()) / 2 - OFFSET_X;
		this->pos_img_1P_desc.y = getwidth() - 150;
		this->pos_img_2P_desc.x = getwidth() / 2 + (getwidth() / 2 - img_2P_desc.getwidth()) / 2 + OFFSET_X;
		this->pos_img_2P_desc.y = this->pos_img_1P_desc.y;
		this->pos_img_gravestone_1P.x= (getwidth() / 2 - img_gravestone_right.getwidth()) / 2 - OFFSET_X;
		this->pos_img_gravestone_1P.y = pos_img_1P.y + img_1P.getheight() + 35;
		this->pos_img_gravestone_2P.x = getwidth() / 2 + (getwidth() / 2 - img_gravestone_left.getwidth()) / 2 + OFFSET_X;
		this->pos_img_gravestone_2P.y = this->pos_img_gravestone_1P.y;
		this->pos_animation_1P.x = (getwidth() / 2 - atlas_peashooter_idle_right.get_image(0)->getwidth()) / 2 - OFFSET_X;
		this->pos_animation_1P.y = pos_img_gravestone_1P.y + 80;
		this->pos_animation_2P.x = getwidth() / 2 + (getwidth() / 2 - atlas_peashooter_idle_right.get_image(0)->getwidth()) / 2 + OFFSET_X;
		this->pos_animation_2P.y = this->pos_animation_1P.y;
		this->pos_1P_name.y = pos_animation_1P.y + 155;
		this->pos_2P_name.y = this->pos_1P_name.y;
		this->pos_img_1P_selector_btn_idle_left.x = pos_img_gravestone_1P.x - img_1P_selector_btn_idle_left.getwidth();
		this->pos_img_1P_selector_btn_idle_left.y = pos_img_gravestone_1P.y + (img_gravestone_right.getheight() - img_1P_selector_btn_down_left.getheight()) / 2;
		this->pos_img_1P_selector_btn_idle_right.x = pos_img_gravestone_1P.x + img_gravestone_right.getwidth();
		this->pos_img_1P_selector_btn_idle_right.y = this->pos_img_1P_selector_btn_idle_left.y;
		this->pos_img_2P_selector_btn_idle_left.x = pos_img_gravestone_2P.x - img_2P_selector_btn_idle_left.getwidth();
		this->pos_img_2P_selector_btn_idle_left.y = this->pos_img_1P_selector_btn_idle_left.y;
		this->pos_img_2P_selector_btn_idle_right.x = pos_img_gravestone_2P.x + img_gravestone_left.getwidth();
		this->pos_img_2P_selector_btn_idle_right.y = this->pos_img_2P_selector_btn_idle_left.y;
	}
	void on_update(int delta)
	{
		this->animation_peashooter.on_update(delta);
		this->animation_sunflower.on_update(delta);

		this->selector_background_scorl1_offset_x += 5;
		if (this->selector_background_scorl1_offset_x >= img_peashooter_selector_background_left.getwidth())
			this->selector_background_scorl1_offset_x = 0;
	}
	void on_draw(const Camera& camera)
	{
		//背景版滚动渲染
		IMAGE* img_p1_selector_background = nullptr;
		IMAGE* img_p2_selector_background = nullptr;

		switch (this->player_type_2)
		{
		case PlayerType::Peashooter:
			img_p1_selector_background = &img_peashooter_selector_background_right;
			break;
		case PlayerType::Sunflower:
			img_p1_selector_background = &img_sunflower_selector_background_right;
			break;
		default:
			img_p1_selector_background = &img_sunflower_selector_background_right;
			break;
		}

		switch (this->player_type_1)
		{
		case PlayerType::Peashooter:
			img_p2_selector_background = &img_peashooter_selector_background_left;
			break;
		case PlayerType::Sunflower:
			img_p2_selector_background = &img_sunflower_selector_background_left;
			break;
		default:
			img_p2_selector_background = &img_peashooter_selector_background_left;
			break;
		}

		//静态图片描绘
		putimage(0, 0, &img_selector_background);

		putimage_alpha(selector_background_scorl1_offset_x - img_p1_selector_background->getwidth(), 0, img_p1_selector_background);
		putimage_alpha(selector_background_scorl1_offset_x, 0, img_p1_selector_background->getwidth() - selector_background_scorl1_offset_x, 0, img_p1_selector_background, 0, 0);
		putimage_alpha(getwidth() - img_p2_selector_background->getwidth(), 0, img_p2_selector_background->getwidth()
			- selector_background_scorl1_offset_x, 0, img_p2_selector_background, selector_background_scorl1_offset_x, 0);
		putimage_alpha(getwidth() - selector_background_scorl1_offset_x, 0, img_p2_selector_background);

		putimage_alpha(pos_img_VS.x, pos_img_VS.y, &img_VS);

		putimage_alpha(pos_img_1P.x, pos_img_1P.y, &img_1P);
		putimage_alpha(pos_img_2P.x, pos_img_2P.y, &img_2P);
		putimage_alpha(pos_img_gravestone_1P.x, pos_img_gravestone_1P.y, &img_gravestone_right);
		putimage_alpha(pos_img_gravestone_2P.x, pos_img_gravestone_2P.y, &img_gravestone_right);
		
		//渲染动画
		switch (player_type_1)
		{
		case PlayerType::Peashooter:
			animation_peashooter.on_draw(camera, pos_animation_1P.x, pos_animation_1P.y);
			pos_1P_name.x = pos_img_gravestone_1P.x + (img_gravestone_right.getwidth() - textwidth(this->str_peashooter_name)) / 2;
			this->outtextxy_shaded(this->pos_1P_name.x, this->pos_1P_name.y, this->str_peashooter_name);
			break;
		case PlayerType::Sunflower:
			animation_sunflower.on_draw(camera, pos_animation_1P.x, pos_animation_1P.y);
			pos_1P_name.x = pos_img_gravestone_1P.x + (img_gravestone_right.getwidth() - textwidth(this->str_sunflower_name)) / 2;
			this->outtextxy_shaded(this->pos_1P_name.x, this->pos_1P_name.y, this->str_sunflower_name);
			break;
		}

		switch (player_type_2)
		{
		case PlayerType::Peashooter:
			animation_peashooter.on_draw(camera, pos_animation_2P.x, pos_animation_2P.y);
			pos_2P_name.x = pos_img_gravestone_2P.x + (img_gravestone_left.getwidth() - textwidth(this->str_peashooter_name)) / 2;
			this->outtextxy_shaded(this->pos_2P_name.x, this->pos_2P_name.y, this->str_peashooter_name);
			break;
		case PlayerType::Sunflower:
			animation_sunflower.on_draw(camera, pos_animation_2P.x, pos_animation_2P.y);
			pos_2P_name.x = pos_img_gravestone_2P.x + (img_gravestone_left.getwidth() - textwidth(this->str_sunflower_name)) / 2;
			this->outtextxy_shaded(this->pos_2P_name.x, this->pos_2P_name.y, this->str_sunflower_name);
			break;
		}
		//按键的按下形态绘制
		putimage_alpha(pos_img_1P_selector_btn_idle_left.x, pos_img_1P_selector_btn_idle_left.y,
			is_btn_1P_left_down ? &img_1P_selector_btn_down_left : &img_1P_selector_btn_idle_left);
		putimage_alpha(pos_img_1P_selector_btn_idle_right.x, pos_img_1P_selector_btn_idle_right.y,
			is_btn_1P_right_down ? &img_1P_selector_btn_down_right : &img_1P_selector_btn_idle_right);
		putimage_alpha(pos_img_2P_selector_btn_idle_left.x, pos_img_2P_selector_btn_idle_left.y,
			is_btn_2P_left_down ? &img_2P_selector_btn_down_left : &img_2P_selector_btn_idle_left);
		putimage_alpha(pos_img_2P_selector_btn_idle_right.x, pos_img_2P_selector_btn_idle_right.y,
			is_btn_2P_right_down ? &img_2P_selector_btn_down_right : &img_2P_selector_btn_idle_right);

		putimage_alpha(pos_img_1P_desc.x, pos_img_1P_desc.y, &img_1P_desc);
		putimage_alpha(pos_img_2P_desc.x, pos_img_2P_desc.y, &img_2P_desc);

		putimage_alpha(pos_img_tip.x, pos_img_tip.y, &img_selector_tip);

	}
	void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN://按键按下
			switch (msg.vkcode)
			{
				// "A"
			case 0x41:
				is_btn_1P_left_down = true;
				break;
				// "D"
			case 0x44:
				is_btn_1P_right_down = true;
				break;
				// "<-"
			case VK_LEFT:
				is_btn_2P_left_down = true;
				break;
				// "->"
			case VK_RIGHT:
				is_btn_2P_right_down = true;
				break;
			default:
				break;
			}
			break;
		case WM_KEYUP://按键抬起
			switch (msg.vkcode)
			{
				// "A"
			case 0x41:
				is_btn_1P_left_down = false;
				//保证玩家类型发生变化，并且保证在[Peashooter,Invalid)之间
				player_type_1 = (PlayerType)(((int)player_type_1 - 1 + (int)PlayerType::Invalid) % (int)PlayerType::Invalid);//类似循环队列
				//mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);//按键音效
				break;
				// "D"
			case 0x44:
				is_btn_1P_right_down = false;
				player_type_1 = (PlayerType)(((int)player_type_1 + 1 + (int)PlayerType::Invalid) % (int)PlayerType::Invalid);
				//mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);//按键音效
				break;
				// "<-"
			case VK_LEFT:
				is_btn_2P_left_down = false;
				player_type_2 = (PlayerType)(((int)player_type_2 - 1 + (int)PlayerType::Invalid) % (int)PlayerType::Invalid);
				//mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);//按键音效
				break;
				// "->"
			case VK_RIGHT:
				is_btn_2P_right_down = false;
				player_type_2 = (PlayerType)(((int)player_type_2 + 1 + (int)PlayerType::Invalid) % (int)PlayerType::Invalid);
				//mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);//按键音效
				break;
			case VK_RETURN://回车键抬起
				scene_manager.switch_to(SceneManager::SceneType::Game);
				//mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);//按键确认音效
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

	}
	void on_exit()
	{
		switch (player_type_1)//根据玩家选择的角色，实例化角色子类对象
		{
		case SelectScene::PlayerType::Peashooter:
			player_1 = new PeashooterPlayer();
			img_player_1_avatar = &img_avatar_peashooter;
			break;
		case SelectScene::PlayerType::Sunflower:
			player_1 = new SunflowerPlayer();
			img_player_1_avatar = &img_avatar_sunflower;
			break;
		default:
			break;
		}
		player_1->set_id(PlayerID::P1);

		switch (player_type_2)//根据玩家选择的角色，实例化角色子类对象
		{
		case SelectScene::PlayerType::Peashooter:
			player_2 = new PeashooterPlayer(false);	//让2P朝向左侧
			img_player_2_avatar = &img_avatar_peashooter;
			break;
		case SelectScene::PlayerType::Sunflower:
			player_2 = new SunflowerPlayer(false);
			img_player_2_avatar = &img_avatar_sunflower;
			break;
		default:
			break;
		}
		player_2->set_id(PlayerID::P2);

		mciSendString(_T("stop bgm_menu"), NULL, 0, NULL);
	}

private:
	enum class PlayerType
	{
		Peashooter=0,
		Sunflower,
		Invalid		//切换角色保证值大于0
	};
private:
	POINT pos_img_VS = { 0 };
	POINT pos_img_tip = { 0 };		//提示信息位置
	POINT pos_img_1P = { 0 };		//文本图片位置
	POINT pos_img_2P = { 0 };
	POINT pos_img_1P_desc = { 0 };		//键位描述图片位置
	POINT pos_img_2P_desc = { 0 };
	POINT pos_1P_name = { 0 };
	POINT pos_2P_name = { 0 };
	POINT pos_animation_1P = { 0 };		//角色动画位置
	POINT pos_animation_2P = { 0 };
	POINT pos_img_gravestone_1P = { 0 };	//墓碑位置
	POINT pos_img_gravestone_2P = { 0 };
	POINT pos_img_1P_selector_btn_idle_left = { 0 };//左右切换按钮位置
	POINT pos_img_1P_selector_btn_idle_right = { 0 };
	POINT pos_img_2P_selector_btn_idle_left = { 0 };
	POINT pos_img_2P_selector_btn_idle_right = { 0 };

	Animation animation_peashooter;		//豌豆射手动画
	Animation animation_sunflower;

	PlayerType player_type_1 = PlayerType::Peashooter;	//1P角色类型
	PlayerType player_type_2 = PlayerType::Sunflower;	//2P角色类型

	LPCTSTR str_peashooter_name = _T("豌豆射手");	//豌豆射手角色名
	LPCTSTR str_sunflower_name = _T("龙日葵");		//龙日葵角色名

	int selector_background_scorl1_offset_x = 0;	//背景板滚动距离

	bool is_btn_1P_left_down = false;	//1P切换角色左键是否按下
	bool is_btn_1P_right_down = false;
	bool is_btn_2P_left_down = false;
	bool is_btn_2P_right_down = false;

private:
	void outtextxy_shaded(int x, int y, LPCTSTR str)//文本文字
	{
		settextcolor(RGB(45, 45, 45));//灰色文本
		outtextxy(x + 3, y + 3, str);
		settextcolor(RGB(255, 255, 255));//白色文本
		outtextxy(x, y, str);
	}

};