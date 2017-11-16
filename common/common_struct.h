#ifndef __common_struct_h__
#define __common_struct_h__

#include <string>
#include <vector>
#include <list>

#define ROOM_STATE_Empty	100		//	��
#define ROOM_STATE_Full		101		//	��
#define ROOM_STATE_idle		102		//	����
#define ROOM_STATE_Lock		103		//	����

#define DESK_STATE_Empty	100		//	����
#define DESK_STATE_Full		101		//	��
#define DESK_STATE_Idle		102		//	����
#define DESK_STATE_Lock		103		//	����

#define GAME_STATE_Start	300		//
#define GAME_STATE_End		301		//

#define USER_STATE_Playing	400		// 
#define USER_STATE_Lookon	401		//

////////////////////////////////////////////////////////////////////////////////////////////////////

class UserInfo {
	public:
		int uid;	// �û�ID
		int rid;	// ������
		int seat;	// ��λ���
		int state;	// �û�״̬

		int sex;			// �û��Ա�
		int age;			// �û�����

		std::string name;	// �û�����
		std::string addr;	// �û���ַ
		std::string ext;	// �û���չ��Ϣ

	public:
		UserInfo() {
			uid = -1;	// �û�ID
			sex = -1;	// �û��Ա�
			age = -1;	// �û�����

			rid = -1;	// ������
			seat = -1;	// ��λ���
			state = -1;	// �û�״̬
		}
};

class DeskInfo {
	public:
		int rid;		// ������
		int did;		// ��λ���
		int state;		// ����״̬
		int max_usercount;	// ����û�����

		std::string name;	// ��������
		std::string ext;	// ������չ��Ϣ

	public:
		DeskInfo() {
			rid = -1;		// ������
			did = -1;		// ��λ���
			state = -1;		// ����״̬
			max_usercount = -1;	// ����û�����
		}
};

class RoomInfo {
	public:
		int rid;	// ������
		int state;	// ����״̬
		int deskcount;	// ���Ӹ���
		int usercount;	// �û�����
		int max_usercount;	// ����û�����
		int game_type;		// ��Ϸ����
		std::string name;	// ��������
		std::string ext;	// ������չ��Ϣ

	public:
		RoomInfo() {
			rid = -1;		// ������
			state = -1;		// ����״̬
			deskcount = -1;	// ���Ӹ���

			usercount = -1;	// �û�����
			max_usercount = -1;	// ����û�����
			game_type = -1;	// ��Ϸ����
		}
};

#endif	// !__common_struct_h__
