#ifndef __command_H__
#define __command_H__

namespace LW 
{
	enum NET_TCP_CMD
	{
		cmd_base = 0,

		cmd_heart_beat = cmd_base + 100,
		cmd_connected = cmd_base + 101,

		/*
			平台消息
		*/
		p_cs_login_request = cmd_base + 200,
		p_sc_login_reply = cmd_base + 1200,

		p_cs_logout_request = cmd_base + 201,
		p_sc_logout_reply = cmd_base + 1201,

		/*
			平台框架消息
		*/
		p_frame_join_room_request = cmd_base + 202,
		p_frame_join_room_reply = cmd_base + 1202,

		p_frame_leave_room_request = cmd_base + 203,
		p_frame_leave_room_reply = cmd_base + 1203,
	};

	enum NET_GAME_MSG_CMD
	{
		/*
		游戏框架消息
		*/
		cmd_game_base = 10000,
		
		g_frame_cs_game_start = cmd_game_base + 100,
		g_frame_sc_game_end = cmd_game_base + 1100,

		g_frame_cs_sit_up = cmd_game_base + 101,
		g_frame_sc_sit_up = cmd_game_base + 1101,

		g_frame_cs_sit_down = cmd_game_base + 102,
		g_frame_sc_sit_down = cmd_game_base + 1102,
	};

	enum NET_CHAT_MSG_CMD
	{
		cmd_chat_base = 20000,

		cs_chat_request = cmd_chat_base + 100,
		sc_chat_reply = cmd_chat_base + 1100,

		sc_chat_broadcast = cmd_chat_base + 1101,
	};
}

#endif // !__command_H__
