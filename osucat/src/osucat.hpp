#pragma once
#ifndef OC_MAIN_HPP
#define OC_MAIN_HPP

#include "include/oppai-cpp/oppai.hpp"
#include "message.h"
#include "include/osu_api.hpp"
#include "badge.hpp"
#include "include/mysql.hpp"
#include "image.hpp"

using namespace std;
using namespace cqhttp_api;

namespace osucat {
	class CmdParser {
	public:
		static void parser(const Target tar, const GroupSender sdr) {
			if (tar.message.find("[CQ:image,file=") != string::npos) {
				string tmp = utils::GetMiddleText(tar.message, "[CQ:image,file=", ",url=http");
				cout << tmp;
				cqhttp_api::getImage(tmp);
			}
		}
	};


	class Main {
	public:
		static void getcallcount(Target tar) {
			Database db;
			db.Connect();
			send_message(tar, u8"èè��0.4�汾��ʼ����Ҫ��������һ���������� " + std::to_string(db.Getcallcount()) + u8" �Ρ�");
		}
		static void help(Target tar) {
			string cmd = tar.message.substr(4);
			utils::string_trim(cmd);
			utils::string_replace(cmd, " ", "");
			//string dir = (string)OC_ROOT_PATH + "\\", fileStr;
			if (_stricmp(cmd.substr(0, 4).c_str(), "bind") == 0) {
				send_message(tar, u8"[CQ:image,file=osucat\\help\\��.png]");
				return;
			}
			if (_stricmp(cmd.substr(0, 4).c_str(), "info") == 0) {
				send_message(tar, u8"[CQ:image,file=osucat\\help\\������Ϣ.png]");
				return;
			}
			if (_stricmp(cmd.substr(0, 5).c_str(), "score") == 0) {
				send_message(tar, u8"[CQ:image,file=osucat\\help\\�ɼ�.png]");
				return;
			}
			if (_stricmp(cmd.substr(0, 6).c_str(), "custom") == 0) {
				send_message(tar, u8"[CQ:image,file=osucat\\help\\�Զ���.png]");
				return;
			}
			if (_stricmp(cmd.substr(0, 5).c_str(), "other") == 0) {
				send_message(tar, u8"[CQ:image,file=osucat\\help\\����.png]");
				return;
			}
			send_message(tar, u8"[CQ:image,file=osucat\\help\\����.png]");
			return;
		}
		static void about(Target tar) {
			send_message(tar, u8"[CQ:image,file=osucat\\help\\about.png]");
		}
		static void contact(Target tar) {
			string cmd = tar.message.substr(7);
			utils::string_trim(cmd);
			send_message(tar, u8"[CQ:at,qq=" + to_string(tar.user_id) + u8"] ���봫��Ļ��ѳɹ������������Ŷ��");
			cqhttp_api::send_private_message(owner_userid, u8"�յ������û� " + to_string(tar.user_id) + u8" ����Ϣ��" + cmd);
		}
		static void ping(Target tar) {
			vector<string> rtnmsg;
			rtnmsg.push_back("pong");
			rtnmsg.push_back("paw");
			rtnmsg.push_back("meow");
			rtnmsg.push_back("nyan");
			rtnmsg.push_back("owo");
			rtnmsg.push_back("qwq");
			rtnmsg.push_back("��");
			send_message(tar, rtnmsg[utils::randomNum(0, rtnmsg.size() - 1)]);
		}
		static void setid(Target tar) {
			string cmd = utils::unescape(tar.message.substr(5));
			utils::string_trim(cmd);
			if (cmd.length() > 20) {
				send_message(tar, ���ṩ�Ĳ���������������);
				return;
			}
			int64_t qq, userid;
			osu_api::v1::user_info UI = { 0 };
			Database db;
			db.Connect();
			db.addcallcount();
			userid = db.osu_getuserid(tar.user_id);
			if (userid != 0) {
				if (!osu_api::v1::api::GetUser(userid, osu_api::v1::mode::std, &UI) == 0) {
					send_message(tar, u8"����˻�����osu!idΪ " + UI.username + u8" ���û��󶨹���Ŷ�����������Ķ�������Ϣ~");
				}
				else {
					send_message(tar, ���󶨵��û��ѱ�bacho���);
				}
				return;
			}
			else if (osu_api::v1::api::GetUser(cmd, osu_api::v1::mode::std, &UI) == 0) {
				send_message(tar, δ��bancho������Ҫ��ѯ���û���Ϣ);
				return;
			}
			qq = db.osu_getqq(UI.user_id);
			if (qq != 0) {
				send_message(tar,
					u8"����Ҫ�󶨵��˻�����qq��Ϊ " + to_string(qq)
					+ u8" ���û��󶨹���Ŷ���������Ϊ�����˴����������˻�������ϵ����~");
				return;
			}
			char return_message[512];
			sprintf_s(return_message,
				512,
				u8"�û� %s �ѳɹ��󶨵���qq��~\n���ڳ�ʼ�����ݣ�ʱ����ܽϳ��������ĵȴ���",
				UI.username);
			send_message(tar, return_message);
			cqhttp_api::send_group_message(management_groupid, u8"��1λ�û���������osu!id,\nqq: " + to_string(tar.user_id)
				+ "\nosu!username: " + UI.username + "\nosu!uid: " + to_string(UI.user_id));
			time_t now = time(NULL);
			tm* tm_now = localtime(&now);
			char timeStr[30] = { 0 };
			strftime(timeStr, sizeof(timeStr), "%Y-%m-%d 04:00:00", tm_now);
			db.UserSet(UI.user_id, tar.user_id);
			db.AddUserData(&UI, timeStr);
			if (osu_api::v1::api::GetUser(cmd, osu_api::v1::mode::taiko, &UI) != 0) {
				db.AddUserData(&UI, timeStr);
			}
			if (osu_api::v1::api::GetUser(cmd, osu_api::v1::mode::ctb, &UI) != 0) {
				db.AddUserData(&UI, timeStr);
			}
			if (osu_api::v1::api::GetUser(cmd, osu_api::v1::mode::mania, &UI) != 0) {
				db.AddUserData(&UI, timeStr);
			}
			info(tar);
		};
		static void unsetid(Target tar) {
			string cmd = tar.message.substr(5);
			utils::string_trim(cmd);
			Database db;
			db.Connect();
			db.addcallcount();
			int64_t uid = db.osu_getuserid(tar.user_id);
			string msg =
				u8"��ȷ��Ҫ������𣿽���󶨺��������а󶨼�¼�Լ�ÿ�����ݸ��¼�¼��\n���Ҫ����Ļ�����ʹ��!contactָ����ϵ����Ա���벻Ҫ�����󣬱������ÿ�����ݼ�¼���޷��ָ��ģ�\n���ȷ��Ҫ�����ʹ������!"
				u8"unset yes������ȷ�ϣ�\n��ظ�!unset+����������ֹ������";
			if (uid == 0) {
				send_message(tar, u8"�㻹û�а�...");
				return;
			}
			if (cmd.empty()) {
				if (db.osu_getunsetstatus(uid) == 0) {
					db.osu_changeunsetstatus(uid, true);
					send_message(tar, msg);
				}
				else {
					db.osu_changeunsetstatus(uid, false);
					send_message(tar, u8"����������ֹ��");
				}
			}
			else {
				if (cmd == "yes") {
					send_message(tar, u8"����ִ�в���...");
					db.osu_deleteuser(uid);
					DeleteFileA(("/work/v1_cover/" + to_string(uid) + "jpg").c_str());
					send_message(tar, u8"�ѳɹ����");
					osu_api::v1::user_info ui = { 0 };
					osu_api::v1::api::GetUser(uid, osu_api::v1::mode::std, &ui);
					cqhttp_api::send_group_message(management_groupid, u8"��1λ�û����������osu!id,\nqq: " + to_string(tar.user_id)
						+ "\nosu!uid: " + to_string(uid) + "\n osu!username: " + ui.username);
				}
				else {
					db.osu_changeunsetstatus(uid, false);
					send_message(tar, u8"����������ֹ��");
				}
			}
		}
		static void info(Target tar) {
			string cmd = tar.message;
			if (_stricmp(cmd.substr(0, 6).c_str(), u8"���") == 0) {
				cmd = cmd.substr(0, 6);
			}
			else if (_stricmp(tar.message.substr(0, 4).c_str(), "info") == 0) {
				cmd = cmd.substr(0, 4);
			}
			utils::unescape(cmd);
			utils::string_trim(cmd);
			utils::string_replace(cmd, u8"��", ":");
			utils::string_replace(cmd, "[CQ:", "");
			Database db;
			db.Connect();
			db.addcallcount();
			int temp, days = 0;
			float previousPP;
			osu_api::v1::mode gamemode;
			UserPanelData upd = { 0 };
			osu_api::v1::user_info previousUserInfo = { 0 };
			int64_t userid;
			string username = "";
			gamemode = osu_api::v1::mode::std;
			if (cmd.length() > 0) {
				if (cmd[0] == '#') {
					userid = db.osu_getuserid(tar.user_id);
					if (userid == 0) {
						send_message(tar, �û�û�а�osu�˺�);
						return;
					}
					gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(userid);
					try {
						days = stoi(cmd.substr(cmd.find('#') + 1));
					}
					catch (std::exception) {
						days = 0;
					}
				}
				else if (cmd[0] == ':') {
					if (cmd.find('#') == string::npos) {
						userid = db.osu_getuserid(tar.user_id);
						if (userid == 0) {
							send_message(tar, �û�û�а�osu�˺�);
							return;
						}
						else {
							try {
								temp = stoi(cmd.substr(cmd.length() - 1));
								if (temp < 4 && temp > -1) {
									gamemode = (osu_api::v1::mode)temp;
								}
								else {
									gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(userid);
								}
							}
							catch (std::exception) {
								gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(userid);
							}
						}
					}
					else {
						userid = db.osu_getuserid(tar.user_id);
						if (userid == 0) {
							send_message(tar, �û�û�а�osu�˺�);
							return;
						}
						else {
							try {
								temp = stoi(cmd.substr(cmd.find(':') + 1, 1));
								days = stoi(cmd.substr(cmd.find('#') + 1));
								if (days > 3651 || days < 0) days = 3650;
								if (temp < 4 && temp > -1) {
									gamemode = (osu_api::v1::mode)temp;
								}
								else {
									gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(userid);
								}
							}
							catch (std::exception) {
								days = 0;
								gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(userid);
							}
						}
					}
				}
				else if (cmd[0] != ':') {
					if (cmd.find(':') != string::npos) {
						if (cmd.find("at,qq=") != string::npos) {
							userid = db.osu_getuserid(stoll(utils::GetMiddleText(cmd, "=", "]")));
							if (userid == 0) {
								send_message(tar, ����ѯ���û�δ��osu�˺�);
								return;
							}
						}
						else {
							try {
								username = cmd.substr(0, cmd.find(':'));
								if (username.length() < 1) username = "%^%^%^!*(^&";
							}
							catch (std::exception) {
								username = "%^%^%^!*(^&";
							}
						}
						try {
							if (cmd.find('#') != string::npos) {
								temp = stoi(cmd.substr(cmd.find(':') + 1, 1));
								days = stoi(cmd.substr(cmd.find('#') + 1));
								if (days > 3651 || days < 0) days = 3650;
							}
							else {
								temp = stoi(cmd.substr(cmd.find(':') + 1));
							}
						}
						catch (std::exception) {
							temp = 0;
						}
						if (temp < 4 && temp > -1) {
							gamemode = (osu_api::v1::mode)temp;
						}
						else {
							days = 0;
							gamemode = osu_api::v1::mode::std;
						};
					}
					else {
						if (cmd.find("at,qq=") != string::npos) {
							userid = db.osu_getuserid(stoll(utils::GetMiddleText(cmd, "=", "]")));
							if (userid == 0) {
								send_message(tar, ����ѯ���û�δ��osu�˺�);
								return;
							}
							try {
								if (cmd.find('#') != string::npos) {
									days = stoi(cmd.substr(cmd.find('#') + 1));
									if (days > 3651 || days < 0) days = 3650;
								}
							}
							catch (std::exception) {
								days = 0;
							}
						}
						else {
							if (cmd.find('#') != string::npos) {
								username = cmd.substr(0, cmd.find('#'));
								try {
									days = stoi(cmd.substr(cmd.find('#') + 1));
									if (days > 3651 || days < 0) days = 3650;
								}
								catch (std::exception) {
									days = 0;
								}
							}
							else {
								username = cmd;
								days = 0;
							}
						}
						if (username.empty()) {
							if (osu_api::v1::api::GetUser(userid, osu_api::v1::mode::std, &upd.user_info) == 0) {
								send_message(tar, ����ѯ���û��ѱ�bancho���);
								return;
							}
							else {
								gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(upd.user_info.user_id);
							}
						}
						else {
							if (username.length() > 20) {
								send_message(tar, ���ṩ�Ĳ���������������);
								return;
							}
							utils::string_trim(username);
							if (osu_api::v1::api::GetUser(username, osu_api::v1::mode::std, &upd.user_info) == 0) {
								send_message(tar, δ��bancho������Ҫ��ѯ���û���Ϣ);;
								return;
							}
							else {
								gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(upd.user_info.user_id);
							}
						}
					}
				}
			}
			else {
				userid = db.osu_getuserid(tar.user_id);
				if (userid == 0) {
					send_message(tar, �û�û�а�osu�˺�);
					return;
				}
				else {
					gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(userid);
				}
			}
			if (username.empty()) {
				if (osu_api::v1::api::GetUser(userid, gamemode, &upd.user_info) == 0) {
					send_message(tar, ����ѯ���û��ѱ�bancho���);
					return;
				}
			}
			else {
				if (username.length() > 20) {
					send_message(tar, ���ṩ�Ĳ���������������);
					return;
				}
				utils::string_trim(username);
				if (osu_api::v1::api::GetUser(username, gamemode, &upd.user_info) == 0) {
					send_message(tar, δ��bancho������Ҫ��ѯ���û���Ϣ);
					return;
				}
			}
			// �����pp+������
			/*if (gamemode == mode::std) {
				if (db.GetUserPreviousPP(upd.user_info.user_id) != upd.user_info.pp) {
					upd.user_info.updatepplus = true;
				} else {
					upd.user_info.updatepplus = false;
					db.GetUserPreviousPPlus(upd.user_info.user_id, &upd.pplus_info);
				}
			}*/
			////
			db.osu_GetUserPreviousPPlus(upd.user_info.user_id, &upd.pplus_info);
			////
			upd.user_info.mode = gamemode;
			previousUserInfo.mode = gamemode;
			string fileStr;
			upd.badgeid = db.osu_getshownbadges(upd.user_info.user_id); //��ȡ��ʾѫ��
			if (db.osu_getqq(upd.user_info.user_id) != 0) {
				if (days > 0) {
					int dtmp = db.osu_GetUserData(upd.user_info.user_id, days, &previousUserInfo);
					if (dtmp == -1) {
						send_message(tar, u8"���������û������...");
						return;
					}
					else if (dtmp == 0) {
						previousUserInfo.days_before = days;
						fileStr = "osucat\\" + infoPic_v1(upd, previousUserInfo, true);
					}
					else {
						previousUserInfo.days_before = dtmp;
						fileStr = "osucat\\" + infoPic_v1(upd, previousUserInfo, true, false);
					}
				}
				else {
					try {
						db.osu_GetUserData(upd.user_info.user_id, 0, &previousUserInfo);
						fileStr = "osucat\\" + infoPic_v1(upd, previousUserInfo, true);
					}
					catch (osucat::database_exception) {
						fileStr = "osucat\\" + infoPic_v1(upd, upd.user_info, true);
					}
				}
			}
			else {
				fileStr = "osucat\\" + infoPic_v1(upd, upd.user_info);
			}
			send_message(tar, u8"[CQ:image,file=" + fileStr + u8"]");
			utils::_DelayDelTmpFile(to_string(OC_ROOT_PATH) + "\\data\\images\\" + fileStr);
		}
		// TODO: textinfo with data comparison
		static void recent(Target tar) {
			string cmd;
			if (_stricmp(tar.message.substr(0, 6).c_str(), "recent") == 0) {
				cmd = tar.message.substr(6);
			}
			else {
				cmd = tar.message.substr(2);
			}
			Database db;
			db.Connect();
			db.addcallcount();
			cmd = utils::unescape(cmd);
			utils::string_trim(cmd);
			utils::string_replace(cmd, u8"��", ":");
			utils::string_replace(cmd, "[CQ:", "");
			ScorePanelData sp_data = { 0 };
			int temp;
			osu_api::v1::mode gamemode;
			gamemode = osu_api::v1::mode::std;
			string username = "";
			string beatmap;
			int64_t userid;
			char beatmap_url[512];
			if (cmd.length() > 0) {
				if (cmd[0] == ':') {
					userid = db.osu_getuserid(tar.user_id);
					if (userid == 0) {
						cqhttp_api::send_message(tar, �û�û�а�osu�˺�);
						return;
					}
					else {
						try {
							if (utils::isNum(cmd.substr(cmd.length() - 1))) {
								temp = stoi(cmd.substr(cmd.length() - 1));
							}
							else {
								cqhttp_api::send_message(tar, Ӣ��ģʽ����ʾ);
								return;
							}
							if (temp < 4 && temp > -1) {
								gamemode = (osu_api::v1::mode)temp;
							}
							else {
								gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(userid);
							}
						}
						catch (std::exception) {
							gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(userid);
						}
					}
				}
				else if (cmd[0] != ':') {
					if (cmd.find(':') != string::npos) {
						if (cmd.find("at,qq=") != string::npos) {
							userid = db.osu_getuserid(stoll(utils::GetMiddleText(cmd, "=", "]")));
							if (userid == 0) {
								cqhttp_api::send_message(tar, ����ѯ���û�δ��osu�˺�);
								return;
							}
						}
						else {
							try {
								username = cmd.substr(0, cmd.find(':'));
								if (username.length() < 1) username = "%^%^%^!*(^&";
							}
							catch (std::exception) {
								username = "%^%^%^!*(^&";
							}
						}
						try {
							if (utils::isNum(cmd.substr(cmd.find(':') + 1))) {
								temp = stoi(cmd.substr(cmd.find(':') + 1));
							}
							else {
								cqhttp_api::send_message(tar, Ӣ��ģʽ����ʾ);
								return;
							}
						}
						catch (std::exception) {
							temp = 0;
						}
						if (temp < 4 && temp > -1) {
							gamemode = (osu_api::v1::mode)temp;
						}
						else {
							gamemode = osu_api::v1::mode::std;
						};
					}
					else {
						if (cmd.find("at,qq=") != string::npos) {
							userid = db.osu_getuserid(stoll(utils::GetMiddleText(cmd, "=", "]")));
							if (userid == 0) {
								cqhttp_api::send_message(tar, ����ѯ���û�δ��osu�˺�);
								return;
							}
						}
						else {
							username = cmd;
						}
						if (username.empty()) {
							if (osu_api::v1::api::GetUser(userid, osu_api::v1::mode::std, &sp_data.user_info) == 0) {
								cqhttp_api::send_message(tar, ���󶨵��û��ѱ�bacho���);
								return;
							}
							else {
								gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(sp_data.user_info.user_id);
							}
						}
						else {
							utils::string_trim(cmd);
							if (osu_api::v1::api::GetUser(cmd, osu_api::v1::mode::std, &sp_data.user_info) == 0) {
								cqhttp_api::send_message(tar, δ��bancho������Ҫ��ѯ���û���Ϣ);
								return;
							}
							else {
								gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(sp_data.user_info.user_id);
							}
						}
					}
				}
			}
			else {
				userid = db.osu_getuserid(tar.user_id);
				if (userid == 0) {
					cqhttp_api::send_message(tar, �û�û�а�osu�˺�);
					return;
				}
				else {
					gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(userid);
				}
			}
			if (username.empty()) {
				if (osu_api::v1::api::GetUser(userid, gamemode, &sp_data.user_info) == 0) {
					cqhttp_api::send_message(tar, u8"û���ҵ���¼...");
					return;
				}
				if (osu_api::v1::api::GetUserRecent(userid, gamemode, &sp_data.score_info) == 0) {
					switch (gamemode) {
					case  osu_api::v1::mode::std:
						cqhttp_api::send_message(tar, u8"�������ģʽStandard��û�������¼~");
						return;
					case  osu_api::v1::mode::taiko:
						cqhttp_api::send_message(tar, u8"�������ģʽTaiko��û�������¼~");
						return;
					case  osu_api::v1::mode::ctb:
						cqhttp_api::send_message(tar, u8"�������ģʽCatch the Beat��û�������¼~");
						return;
					case  osu_api::v1::mode::mania:
						cqhttp_api::send_message(tar, u8"�������ģʽMania��û�������¼~");
						return;
					}
				}
			}
			else {
				if (username.length() > 20) {
					cqhttp_api::send_message(tar, ���ṩ�Ĳ���������������);
					return;
				}
				utils::string_trim(username);
				if (osu_api::v1::api::GetUser(username, gamemode, &sp_data.user_info) == 0) {
					cqhttp_api::send_message(tar, u8"û����˻���TA�����������ģʽ��");
					return;
				}
				if (osu_api::v1::api::GetUserRecent(username, gamemode, &sp_data.score_info) == 0) {
					switch (gamemode) {
					case  osu_api::v1::mode::std:
						cqhttp_api::send_message(tar, u8"TA�����ģʽStandard��û�������¼~");
						return;
					case  osu_api::v1::mode::taiko:
						cqhttp_api::send_message(tar, u8"TA�����ģʽTaiko��û�������¼~");
						return;
					case  osu_api::v1::mode::ctb:
						cqhttp_api::send_message(tar, u8"TA�����ģʽCatch the Beat��û�������¼~");
						return;
					case  osu_api::v1::mode::mania:
						cqhttp_api::send_message(tar, u8"TA�����ģʽMania��û�������¼~");
						return;
					}
				}
			}
			sp_data.mode = gamemode;
			osu_api::v1::api::GetBeatmap(sp_data.score_info.beatmap_id, &sp_data.beatmap_info);
			sprintf_s(beatmap_url, OSU_FILE_URL "%lld", sp_data.score_info.beatmap_id);
			beatmap = NetConnection::HttpsGet(beatmap_url);
			if (beatmap.empty()) {
				cqhttp_api::send_message(tar, ��ȡ������Ϣ����);
				return;
			}
			if (sp_data.mode == osu_api::v1::mode::std) {
				oppai pp;
				pp.read_data(beatmap);
				pp.mods((int)sp_data.score_info.mods);
				// if fc
				pp.accuracy_percent(-1);
				pp.n300((int)sp_data.score_info.n300);
				pp.n100((int)sp_data.score_info.n100);
				pp.n50((int)sp_data.score_info.n50);
				oppai_result t = pp.calc();
				sp_data.fc = (int)t.data.total_pp.value();
				// original
				pp.end((int)sp_data.score_info.n300 + (int)sp_data.score_info.n100 + (int)sp_data.score_info.n50
					+ (int)sp_data.score_info.nmiss);
				pp.nmiss((int)sp_data.score_info.nmiss);
				pp.combo((int)sp_data.score_info.combo);
				sp_data.pp_info = pp.calc();
				sp_data.pp_info.data.total_star = t.data.total_star;
				// reset
				pp.end(-1);
				pp.n300(-1);
				pp.n100(-1);
				pp.n50(-1);
				pp.nmiss(-1);
				pp.combo(-1);
				for (int i = 0; i < 5; ++i) {
					pp.accuracy_percent((float)(i == 0 ? 95 : 96 + i));
					sp_data.confirmed_acc[i] = (int)(pp.calc().data.total_pp.value());
				}
				if (sp_data.pp_info.code == 0) {
					sp_data.pp_info.data.maxcombo = t.data.maxcombo;
					sp_data.pp_info.data.maxlength = t.data.length;
				}
			}
			else {
				if (sp_data.mode == osu_api::v1::mode::mania) {
					long n1, n2;
					n1 = sp_data.score_info.n50 * 50.0 + sp_data.score_info.n100 * 100.0
						+ sp_data.score_info.nkatu * 200.0
						+ (sp_data.score_info.n300 + sp_data.score_info.ngeki) * 300.0;
					n2 = 300
						* (sp_data.score_info.nmiss + sp_data.score_info.n50 + sp_data.score_info.nkatu
							+ sp_data.score_info.n100 + sp_data.score_info.n300 + sp_data.score_info.ngeki);
					sp_data.pp_info.data.accuracy = (double)n1 / (double)n2 * 100.0;
				}
				if (sp_data.mode == osu_api::v1::mode::taiko) {
					double n1, n2;
					n1 = ((double)sp_data.score_info.n100 + (double)sp_data.score_info.nkatu) * 0.5
						+ (double)sp_data.score_info.n300 + (double)sp_data.score_info.ngeki;
					n2 = (double)sp_data.score_info.nmiss + (double)sp_data.score_info.n100
						+ (double)sp_data.score_info.nkatu + (double)sp_data.score_info.n300
						+ (double)sp_data.score_info.ngeki;
					sp_data.pp_info.data.accuracy = (double)n1 / (double)n2 * 100.0;
				}
				if (sp_data.mode == osu_api::v1::mode::ctb) {
					long n1, n2;
					n1 = sp_data.score_info.n50 + sp_data.score_info.n100 + sp_data.score_info.n300;
					n2 = sp_data.score_info.nkatu + sp_data.score_info.nmiss + sp_data.score_info.n50
						+ sp_data.score_info.n100 + sp_data.score_info.n300;
					sp_data.pp_info.data.maxcombo = sp_data.beatmap_info.maxcombo;
					sp_data.pp_info.data.accuracy = (double)n1 / (double)n2 * 100.0;
				}
				sp_data.pp_info.data.title = sp_data.beatmap_info.title;
				sp_data.pp_info.data.artist = sp_data.beatmap_info.artist;
				sp_data.pp_info.data.creator = sp_data.beatmap_info.creator;
				sp_data.pp_info.data.total_star = sp_data.beatmap_info.stars;
				sp_data.pp_info.data.bpm = sp_data.beatmap_info.bpm;
				sp_data.pp_info.data.ar = sp_data.beatmap_info.ar;
				sp_data.pp_info.data.od = sp_data.beatmap_info.od;
				sp_data.pp_info.data.cs = sp_data.beatmap_info.cs;
				sp_data.pp_info.data.hp = sp_data.beatmap_info.hp;
				sp_data.pp_info.data.combo = sp_data.score_info.combo;
			}
			string fileStr = "osucat\\" + scorePic(sp_data);
			cqhttp_api::send_message(tar, u8"[CQ:image,file=" + fileStr + u8"]");
			db.osu_UpdatePPRecord(tar.user_id, sp_data.score_info.beatmap_id);
			utils::_DelayDelTmpFile(to_string(OC_ROOT_PATH) + "\\data\\images\\" + fileStr);
		}
		static void bp(Target tar) {
			string cmd = tar.message.substr(2);
			Database db;
			db.Connect();
			db.addcallcount();
			cmd = utils::unescape(cmd);
			utils::string_trim(cmd);
			utils::string_replace(cmd, u8"��", ":");
			utils::string_replace(cmd, "[CQ:", "");
			ScorePanelData sp_data = { 0 };
			vector<osu_api::v1::score_info> SI;
			int temp;
			osu_api::v1::mode gamemode;
			gamemode = osu_api::v1::mode::std;
			string username = "";
			string beatmap;
			int64_t userid, bpnum;
			char beatmap_url[512];
			if (cmd.length() > 0) {
				if (cmd.find('#') != string::npos) {
					if (cmd.find(':') != string::npos) {
						try {
							if (cmd.find("at,qq=") != string::npos) {
								userid = db.osu_getuserid(stoll(utils::GetMiddleText(cmd, "=", "]")));
								if (userid == 0) {
									cqhttp_api::send_message(tar, ����ѯ���û�δ��osu�˺�);
									return;
								}
							}
							else {
								username = cmd.substr(0, cmd.find('#'));
							}
						}
						catch (std::exception) {
							username = "%^%^%^!*(^&";
						}
						try {
							bpnum = stoll(cmd.substr(cmd.find('#') + 1, cmd.find(':')));
							bpnum > 100 ? bpnum = 100 : bpnum < 1 ? bpnum = 1 : bpnum = bpnum;
						}
						catch (std::exception) {
							bpnum = 1;
						}
						try {
							if (utils::isNum(cmd.substr(cmd.find(':') + 1))) {
								temp = stoi(cmd.substr(cmd.find(':') + 1));
							}
							else {
								cqhttp_api::send_message(tar, Ӣ��ģʽ����ʾ);
								return;
							}
							if (temp < 4 && temp > -1) {
								gamemode = (osu_api::v1::mode)temp;
							}
							else {
								gamemode = osu_api::v1::mode::std;
							}
						}
						catch (std::exception) {
							gamemode = osu_api::v1::mode::std;
						}
					}
					else {
						try {
							if (cmd.find("at,qq=") != string::npos) {
								userid = db.osu_getuserid(stoll(utils::GetMiddleText(cmd, "=", "]")));
								if (userid == 0) {
									cqhttp_api::send_message(tar, ����ѯ���û�δ��osu�˺�);
									return;
								}
							}
							else {
								username = cmd.substr(0, cmd.find('#'));
								if (username.length() < 1) username = "%^%^%^!*(^&";
							}
						}
						catch (std::exception) {
							username = "%^%^%^!*(^&";
						}
						try {
							bpnum = stoll(cmd.substr(cmd.find('#') + 1));
							bpnum > 100 ? bpnum = 100 : bpnum < 1 ? bpnum = 1 : bpnum = bpnum;
						}
						catch (std::exception) {
							bpnum = 1;
						}
						gamemode = osu_api::v1::mode::std;
					}
				}
				else if (cmd.find(':') != string::npos) {
					if (cmd[0] == ':') {
						bpnum = 1;
						userid = db.osu_getuserid(tar.user_id);
						if (userid == 0) {
							cqhttp_api::send_message(tar, �û�û�а�osu�˺�);
							return;
						}
						try {
							if (utils::isNum(cmd.substr(cmd.length() - 1))) {
								temp = stoi(cmd.substr(cmd.length() - 1));
							}
							else {
								cqhttp_api::send_message(tar, Ӣ��ģʽ����ʾ);
								return;
							}
							if (temp < 4 && temp > -1) {
								gamemode = (osu_api::v1::mode)temp;
							}
							else {
								gamemode = osu_api::v1::mode::std;
							}
						}
						catch (std::exception) {
							gamemode = (osu_api::v1::mode)temp;
						}
					}
					else if (cmd.find(':') != string::npos && cmd[0] != ':') {
						bpnum = 1;
						try {
							if (cmd.find("at,qq=") != string::npos) {
								userid = db.osu_getuserid(stoll(utils::GetMiddleText(cmd, "=", "]")));
								if (userid == 0) {
									cqhttp_api::send_message(tar, ����ѯ���û�δ��osu�˺�);
									return;
								}
							}
							else {
								username = cmd.substr(0, cmd.find(':'));
							}
						}
						catch (std::exception) {
							username = "%^%^%^!*(^&";
						}
						try {
							temp = stoi(cmd.substr(cmd.length() - 1));
							if (temp < 4 && temp > -1) {
								gamemode = (osu_api::v1::mode)temp;
							}
							else {
								gamemode = osu_api::v1::mode::std;
							}
						}
						catch (std::exception) {
							gamemode = (osu_api::v1::mode)temp;
						}
					}
				}
				else {
					userid = db.osu_getuserid(tar.user_id);
					if (userid == 0) {
						cqhttp_api::send_message(tar, �û�û�а�osu�˺�);
						return;
					}
					gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(userid);
					if (utils::isNum(cmd)) {
						bpnum = stoll(cmd);
						bpnum > 100 ? bpnum = 100 : bpnum < 1 ? bpnum = 1 : bpnum = bpnum;
					}
					else
						bpnum = 1;
				}
			}
			else {
				userid = db.osu_getuserid(tar.user_id);
				if (userid == 0) {
					cqhttp_api::send_message(tar, �û�û�а�osu�˺�);
					return;
				}
				gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(userid);
				bpnum = 1;
			}
			if (username.empty()) {
				if (osu_api::v1::api::GetUser(userid, gamemode, &sp_data.user_info) == 0) {
					cqhttp_api::send_message(tar, ����ѯ���û��ѱ�bancho���);
					return;
				}
				if (osu_api::v1::api::GetUserBest(userid, bpnum, gamemode, SI) == 0) {
					cqhttp_api::send_message(tar, û�в�ѯ��BP);
					return;
				}

			}
			else {
				if (username.length() > 20) {
					cqhttp_api::send_message(tar, ���ṩ�Ĳ���������������);
					return;
				}
				utils::string_trim(username);
				if (osu_api::v1::api::GetUser(username, gamemode, &sp_data.user_info) == 0) {
					cqhttp_api::send_message(tar, δ��bancho������Ҫ��ѯ���û���Ϣ);
					return;
				}
				if (osu_api::v1::api::GetUserBest(username, bpnum, gamemode, SI) == 0) {
					cqhttp_api::send_message(tar, û�в�ѯ������BP);
					return;
				}
			}
			if (bpnum != SI.size()) {
				cqhttp_api::send_message(tar, û�в�ѯ��BP);
				return;
			}
			sp_data.mode = gamemode;
			sp_data.score_info.achieved_timestamp = SI[bpnum - 1].achieved_timestamp;
			sp_data.score_info.beatmap_id = SI[bpnum - 1].beatmap_id;
			sp_data.score_info.score = SI[bpnum - 1].score;
			sp_data.score_info.combo = SI[bpnum - 1].combo;
			sp_data.score_info.n50 = SI[bpnum - 1].n50;
			sp_data.score_info.n100 = SI[bpnum - 1].n100;
			sp_data.score_info.n300 = SI[bpnum - 1].n300;
			sp_data.score_info.nkatu = SI[bpnum - 1].nkatu;
			sp_data.score_info.ngeki = SI[bpnum - 1].ngeki;
			sp_data.score_info.nmiss = SI[bpnum - 1].nmiss;
			sp_data.score_info.mods = SI[bpnum - 1].mods;
			sp_data.score_info.user_id = SI[bpnum - 1].user_id;
			sp_data.score_info.rank = SI[bpnum - 1].rank;
			sp_data.score_info.pp = SI[bpnum - 1].pp;
			sp_data.score_info.username = sp_data.user_info.username;
			osu_api::v1::api::GetBeatmap(sp_data.score_info.beatmap_id, &sp_data.beatmap_info);
			sprintf_s(beatmap_url, OSU_FILE_URL "%lld", sp_data.score_info.beatmap_id);
			beatmap = NetConnection::HttpsGet(beatmap_url);
			if (beatmap.empty()) {
				cqhttp_api::send_message(tar, ��ȡ������Ϣ����);
				return;
			}
			if (sp_data.mode == osu_api::v1::mode::std) {
				oppai pp;
				pp.read_data(beatmap);
				pp.mods((int)sp_data.score_info.mods);
				// if fc
				pp.accuracy_percent(-1);
				pp.n300(sp_data.score_info.n300);
				pp.n100(sp_data.score_info.n100);
				pp.n50(sp_data.score_info.n50);
				oppai_result t = pp.calc();
				if (t.data.total_pp.has_value()) {
					sp_data.fc = t.data.total_pp.value();
					// original
					pp.nmiss(sp_data.score_info.nmiss);
					pp.combo(sp_data.score_info.combo);
					sp_data.pp_info = pp.calc();
					sp_data.pp_info.data.total_pp = sp_data.score_info.pp;
					sp_data.pp_info.data.total_star = t.data.total_star;
					// reset
					pp.end(-1);
					pp.n300(-1);
					pp.n100(-1);
					pp.n50(-1);
					pp.nmiss(-1);
					pp.combo(-1);
					for (int i = 0; i < 5; ++i) {
						pp.accuracy_percent((float)(i == 0 ? 95 : 96 + i));
						sp_data.confirmed_acc[i] = (int)(pp.calc().data.total_pp.value());
					}
					if (sp_data.pp_info.code == 0) {
						sp_data.pp_info.data.maxcombo = t.data.maxcombo;
						sp_data.pp_info.data.maxlength = t.data.length;
					}
				}
				else {
					sp_data.fc = 0;
					sp_data.confirmed_acc[0] = 0;
					sp_data.confirmed_acc[1] = 0;
					sp_data.confirmed_acc[2] = 0;
					sp_data.confirmed_acc[3] = 0;
					sp_data.confirmed_acc[4] = 0;
					sp_data.pp_info.data.title = sp_data.beatmap_info.title;
					sp_data.pp_info.data.artist = sp_data.beatmap_info.artist;
					sp_data.pp_info.data.creator = sp_data.beatmap_info.creator;
					sp_data.pp_info.data.total_star = sp_data.beatmap_info.stars;
					sp_data.pp_info.data.bpm = sp_data.beatmap_info.bpm;
					sp_data.pp_info.data.ar = sp_data.beatmap_info.ar;
					sp_data.pp_info.data.od = sp_data.beatmap_info.od;
					sp_data.pp_info.data.cs = sp_data.beatmap_info.cs;
					sp_data.pp_info.data.hp = sp_data.beatmap_info.hp;
					sp_data.pp_info.data.combo = sp_data.score_info.combo;
					sp_data.pp_info.data.total_pp = sp_data.score_info.pp;
					sp_data.pp_info.data.maxcombo = sp_data.beatmap_info.maxcombo;
					long n1, n2;
					n1 = sp_data.score_info.n50 * 50 + sp_data.score_info.n100 * 100 + sp_data.score_info.n300 * 300;
					n2 = 300
						* (sp_data.score_info.nmiss + sp_data.score_info.n50 + sp_data.score_info.n100
							+ sp_data.score_info.n300);
					sp_data.pp_info.data.accuracy = (double)n1 / (double)n2 * 100.0;
				}

			}
			else {
				if (sp_data.mode == osu_api::v1::mode::mania) {
					long n1, n2;
					n1 = sp_data.score_info.n50 * 50.0 + sp_data.score_info.n100 * 100.0
						+ sp_data.score_info.nkatu * 200.0
						+ (sp_data.score_info.n300 + sp_data.score_info.ngeki) * 300.0;
					n2 = 300
						* (sp_data.score_info.nmiss + sp_data.score_info.n50 + sp_data.score_info.nkatu
							+ sp_data.score_info.n100 + sp_data.score_info.n300 + sp_data.score_info.ngeki);
					sp_data.pp_info.data.accuracy = (double)n1 / (double)n2 * 100.0;
				}
				if (sp_data.mode == osu_api::v1::mode::taiko) {
					double n1, n2;
					n1 = ((double)sp_data.score_info.n100 + (double)sp_data.score_info.nkatu) * 0.5
						+ (double)sp_data.score_info.n300 + (double)sp_data.score_info.ngeki;
					n2 = (double)sp_data.score_info.nmiss + (double)sp_data.score_info.n100
						+ (double)sp_data.score_info.nkatu + (double)sp_data.score_info.n300
						+ (double)sp_data.score_info.ngeki;
					sp_data.pp_info.data.accuracy = (double)n1 / (double)n2 * 100.0;
				}
				if (sp_data.mode == osu_api::v1::mode::ctb) {
					long n1, n2;
					n1 = sp_data.score_info.n50 + sp_data.score_info.n100 + sp_data.score_info.n300;
					n2 = sp_data.score_info.nkatu + sp_data.score_info.nmiss + sp_data.score_info.n50
						+ sp_data.score_info.n100 + sp_data.score_info.n300;
					sp_data.pp_info.data.maxcombo = sp_data.beatmap_info.maxcombo;
					sp_data.pp_info.data.accuracy = (double)n1 / (double)n2 * 100.0;
				}
				sp_data.pp_info.data.title = sp_data.beatmap_info.title;
				sp_data.pp_info.data.artist = sp_data.beatmap_info.artist;
				sp_data.pp_info.data.creator = sp_data.beatmap_info.creator;
				sp_data.pp_info.data.total_star = sp_data.beatmap_info.stars;
				sp_data.pp_info.data.bpm = sp_data.beatmap_info.bpm;
				sp_data.pp_info.data.ar = sp_data.beatmap_info.ar;
				sp_data.pp_info.data.od = sp_data.beatmap_info.od;
				sp_data.pp_info.data.cs = sp_data.beatmap_info.cs;
				sp_data.pp_info.data.hp = sp_data.beatmap_info.hp;
				sp_data.pp_info.data.combo = sp_data.score_info.combo;
				sp_data.pp_info.data.total_pp = sp_data.score_info.pp;
			}
			string fileStr = "osucat\\" + scorePic(sp_data);
			cqhttp_api::send_message(tar, u8"[CQ:image,file=" + fileStr + u8"]");
			db.osu_UpdatePPRecord(tar.user_id, sp_data.score_info.beatmap_id);
			utils::_DelayDelTmpFile(to_string(OC_ROOT_PATH) + "\\data\\images\\" + fileStr);
		}
		static void score(Target tar) {
			string cmd = tar.message.substr(5);
			utils::string_replace(cmd, " ", "");
			if (!utils::isNum(cmd)) {
				cqhttp_api::send_message(tar, u8"��˵����ʱ��֧�����ֲ�ѯ����������bid����ѯ~");
				return;
			}
			int64_t bid = stoll(cmd);
			Database db;
			string beatmap;
			char beatmap_url[512];
			ScorePanelData sp_data = { 0 };
			db.Connect();
			db.addcallcount();
			int64_t uid = db.osu_getuserid(tar.user_id);
			if (uid == 0) {
				cqhttp_api::send_message(tar, �û�û�а�osu�˺�);
				return;
			}
			osu_api::v1::mode gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(uid);
			if (osu_api::v1::api::GetUser(uid, gamemode, &sp_data.user_info) == 0) {
				cqhttp_api::send_message(tar, ����ѯ���û��ѱ�bancho���);
				return;
			}
			if (osu_api::v1::api::GetUserScore(sp_data.user_info.user_id, gamemode, bid, &sp_data.score_info) == 0) {
				cqhttp_api::send_message(tar, û�в�ѯ���ɼ�);
				return;
			}
			sp_data.mode = gamemode;
			sp_data.score_info.username = sp_data.user_info.username;
			osu_api::v1::api::GetBeatmap(sp_data.score_info.beatmap_id, &sp_data.beatmap_info);
			sprintf_s(beatmap_url, OSU_FILE_URL "%lld", sp_data.score_info.beatmap_id);
			beatmap = NetConnection::HttpsGet(beatmap_url);
			if (beatmap.empty()) {
				cqhttp_api::send_message(tar, ��ȡ������Ϣ����);
				return;
			}
			if (sp_data.mode == osu_api::v1::mode::std) {
				oppai pp;
				pp.read_data(beatmap);
				pp.mods((int)sp_data.score_info.mods);
				// if fc
				pp.accuracy_percent(-1);
				pp.n300((int)sp_data.score_info.n300);
				pp.n100((int)sp_data.score_info.n100);
				pp.n50((int)sp_data.score_info.n50);
				oppai_result t = pp.calc();
				sp_data.fc = (int)t.data.total_pp.value();
				// original
				pp.end((int)sp_data.score_info.n300 + (int)sp_data.score_info.n100 + (int)sp_data.score_info.n50
					+ (int)sp_data.score_info.nmiss);
				pp.nmiss((int)sp_data.score_info.nmiss);
				pp.combo((int)sp_data.score_info.combo);
				sp_data.pp_info = pp.calc();
				sp_data.pp_info.data.total_star = t.data.total_star;
				// reset
				pp.end(-1);
				pp.n300(-1);
				pp.n100(-1);
				pp.n50(-1);
				pp.nmiss(-1);
				pp.combo(-1);
				for (int i = 0; i < 5; ++i) {
					pp.accuracy_percent((float)(i == 0 ? 95 : 96 + i));
					sp_data.confirmed_acc[i] = (int)(pp.calc().data.total_pp.value());
				}
				if (sp_data.pp_info.code == 0) {
					sp_data.pp_info.data.maxcombo = t.data.maxcombo;
					sp_data.pp_info.data.maxlength = t.data.length;
				}
			}
			else {
				if (sp_data.mode == osu_api::v1::mode::mania) {
					long n1, n2;
					n1 = sp_data.score_info.n50 * 50.0 + sp_data.score_info.n100 * 100.0
						+ sp_data.score_info.nkatu * 200.0
						+ (sp_data.score_info.n300 + sp_data.score_info.ngeki) * 300.0;
					n2 = 300
						* (sp_data.score_info.nmiss + sp_data.score_info.n50 + sp_data.score_info.nkatu
							+ sp_data.score_info.n100 + sp_data.score_info.n300 + sp_data.score_info.ngeki);
					sp_data.pp_info.data.accuracy = (double)n1 / (double)n2 * 100.0;
				}
				if (sp_data.mode == osu_api::v1::mode::taiko) {
					double n1, n2;
					n1 = ((double)sp_data.score_info.n100 + (double)sp_data.score_info.nkatu) * 0.5
						+ (double)sp_data.score_info.n300 + (double)sp_data.score_info.ngeki;
					n2 = (double)sp_data.score_info.nmiss + (double)sp_data.score_info.n100
						+ (double)sp_data.score_info.nkatu + (double)sp_data.score_info.n300
						+ (double)sp_data.score_info.ngeki;
					sp_data.pp_info.data.accuracy = (double)n1 / (double)n2 * 100.0;
				}
				if (sp_data.mode == osu_api::v1::mode::ctb) {
					long n1, n2;
					n1 = sp_data.score_info.n50 + sp_data.score_info.n100 + sp_data.score_info.n300;
					n2 = sp_data.score_info.nkatu + sp_data.score_info.nmiss + sp_data.score_info.n50
						+ sp_data.score_info.n100 + sp_data.score_info.n300;
					sp_data.pp_info.data.maxcombo = sp_data.beatmap_info.maxcombo;
					sp_data.pp_info.data.accuracy = (double)n1 / (double)n2 * 100.0;
				}
				sp_data.pp_info.data.title = sp_data.beatmap_info.title;
				sp_data.pp_info.data.artist = sp_data.beatmap_info.artist;
				sp_data.pp_info.data.creator = sp_data.beatmap_info.creator;
				sp_data.pp_info.data.total_star = sp_data.beatmap_info.stars;
				sp_data.pp_info.data.bpm = sp_data.beatmap_info.bpm;
				sp_data.pp_info.data.ar = sp_data.beatmap_info.ar;
				sp_data.pp_info.data.od = sp_data.beatmap_info.od;
				sp_data.pp_info.data.cs = sp_data.beatmap_info.cs;
				sp_data.pp_info.data.hp = sp_data.beatmap_info.hp;
				sp_data.pp_info.data.combo = sp_data.score_info.combo;
			}
			string fileStr = "osucat\\" + scorePic(sp_data);
			cqhttp_api::send_message(tar, u8"[CQ:image,file=" + fileStr + u8"]");
			db.osu_UpdatePPRecord(tar.user_id, sp_data.beatmap_info.beatmap_id);
			utils::_DelayDelTmpFile(to_string(OC_ROOT_PATH) + "\\data\\images\\" + fileStr);
		}
		static void update(Target tar) {
			Database db;
			db.Connect();
			db.addcallcount();
			int64_t uid = db.osu_getuserid(tar.user_id);
			if (uid == 0) {
				cqhttp_api::send_message(tar, �û�û�а�osu�˺�);
				return;
			}
			osu_api::v1::user_info UI;
			osu_api::v1::mode mode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(uid);
			int8_t returnCode = osu_api::v1::api::GetUser(uid, mode, &UI);
			if (returnCode == 0) {
				cqhttp_api::send_message(tar, ����ѯ���û��ѱ�bancho���);
				return;
			}
			cqhttp_api::send_message(tar, u8"��Ů����...");
			DeleteFileA(("./work/avatar/" + to_string(UI.user_id) + ".png").c_str());
			vector<long> pp_plus;
			try {
				pp_plus = NetConnection::getUserPlusData(UI.user_id);
				osu_api::v1::pplus_info pi;
				pi.acc = pp_plus[0];
				pi.flow = pp_plus[1];
				pi.jump = pp_plus[2];
				pi.pre = pp_plus[3];
				pi.spd = pp_plus[4];
				pi.sta = pp_plus[5];
				db.osu_UpdatePPlus(UI.user_id, UI.pp, pi);
			}
			catch (std::exception) {
			}
			info(tar);
		}
		static void pp(Target tar) {
			string cmd = tar.message.substr(2);
			utils::string_replace(cmd, " ", "");
			if (!utils::isNum(cmd)) {
				cqhttp_api::send_message(tar, u8"�������ȷ��bid!");
				return;
			}
			string beatmap = NetConnection::HttpsGet(OSU_FILE_URL + cmd);
			if (beatmap.empty()) {
				cqhttp_api::send_message(tar, ��ȡ������Ϣ����);
				return;
			}
			Database db;
			db.Connect();
			db.addcallcount();
			db.osu_UpdatePPRecord(tar.user_id, stoll(cmd));
			oppai pp;
			vector<float> out;
			char message[5120];
			pp.read_data(beatmap);
			oppai_result t = pp.calc();
			if (t.code == -4) {
				cqhttp_api::send_message(tar, u8"�ݲ�֧�ֳ�Standardģʽ���������ģʽ��");
				return;
			}
			for (int i = 0; i < 5; ++i) {
				pp.accuracy_percent((float)(i == 0 ? 95 : 96 + i));
				oppai_result c = pp.calc();
				out.push_back((float)(c.data.total_pp.has_value() ? c.data.total_pp.value() : 0.0));
			}
			string title, artist, version;
			if (t.data.titleUnicode.length() > 2 && t.data.titleUnicode.length() < 128) {
				title = t.data.titleUnicode;
			}
			else {
				t.data.title.length() < 255 ? title = t.data.title : title = "̫����";
			}
			if (t.data.artistUnicode.length() > 2 && t.data.titleUnicode.length() < 128) {
				artist = t.data.artistUnicode;
			}
			else {
				t.data.artist.length() < 255 ? artist = t.data.artist : artist = "̫����";
			}
			t.data.map_version.length() < 255 ? version = t.data.map_version : version = "̫����";
			sprintf_s(message,
				5120,
				u8"%s - %s[%s]\n"
				u8"mapped by %s\n"
				"ar%.1f | od%.1f | cs%.1f | hp%.1f | bpm%.2f\n"
				"Stars:%.2f* | No Mod | objects:%d\n\n"
				"100%% : %.2fpp\n"
				"99%% : %.2fpp\n"
				"98%% : %.2fpp\n"
				"97%% : %.2fpp\n"
				"95%% : %.2fpp\n\n%s",
				artist.c_str(),
				title.c_str(),
				version.c_str(),
				t.data.creator.c_str(),
				t.data.ar,
				t.data.od,
				t.data.cs,
				t.data.hp,
				t.data.bpm,
				t.data.total_star,
				t.data.n300,
				out[4],
				out[3],
				out[2],
				out[1],
				out[0],
				("https://osu.ppy.sh/b/" + cmd).c_str());
			cqhttp_api::send_message(tar, message);
		}
		static void ppwith(Target tar) {
			string cmd = tar.message.substr(4);
			if (cmd.length() > 15) {
				cqhttp_api::send_message(tar, ���ṩ�Ĳ���������������);
				return;
			}
			if (cmd.find("[CQ:") != string::npos) {
				cqhttp_api::send_message(tar, ���ṩ�Ĳ���������������);
				return;
			}
			Database db;
			db.Connect();
			db.addcallcount();
			int64_t bid = db.osu_GetPPRecord(tar.user_id);
			if (bid == EOF) {
				cqhttp_api::send_message(tar, u8"�㻹û�в�ѯ���ɼ������Ȳ�ѯ�ɼ�������ʹ������ָ��");
				return;
			}
			if (!Mod(cmd).isVaild()) {
				cqhttp_api::send_message(tar, u8"��������ȷ");
				return;
			}
			string beatmap = NetConnection::HttpsGet(OSU_FILE_URL + to_string(bid));
			if (beatmap.empty()) {
				cqhttp_api::send_message(tar, ��ȡ������Ϣ����);
				return;
			}
			oppai pp;
			vector<float> out;
			char message[5120];
			pp.read_data(beatmap);
			pp.mods(Mod(cmd).GetModNumber());
			oppai_result t = pp.calc();
			for (int i = 0; i < 5; ++i) {
				pp.accuracy_percent((float)(i == 0 ? 95 : 96 + i));
				out.push_back((float)(pp.calc().data.total_pp.value()));
			}
			string modStr = Mod(cmd).GetModString();
			string title, artist, version;
			if (t.data.titleUnicode.length() > 2 && t.data.titleUnicode.length() < 128) {
				title = t.data.titleUnicode;
			}
			else {
				t.data.title.length() < 255 ? title = t.data.title : title = "̫����";
			}
			if (t.data.artistUnicode.length() > 2 && t.data.titleUnicode.length() < 128) {
				artist = t.data.artistUnicode;
			}
			else {
				t.data.artist.length() < 255 ? artist = t.data.artist : artist = "̫����";
			}
			t.data.map_version.length() < 255 ? version = t.data.map_version : version = "̫����";
			sprintf_s(message,
				5120,
				u8"%s - %s[%s]\n"
				u8"mapped by %s\n"
				"[ar%.1f | od%.1f | cs%.1f | hp%.1f | bpm%.2f]\n"
				"%s | Stars:%.2f* | objects:%d\n\n"
				"100%% : %.2fpp\n"
				"99%% : %.2fpp\n"
				"98%% : %.2fpp\n"
				"97%% : %.2fpp\n"
				"95%% : %.2fpp\n\n%s",
				artist.c_str(),
				title.c_str(),
				version.c_str(),
				t.data.creator.c_str(),
				t.data.ar,
				t.data.od,
				t.data.cs,
				t.data.hp,
				t.data.bpm,
				modStr.length() == 0 ? "No Mod" : modStr.c_str(),
				t.data.total_star,
				t.data.n300,
				out[4],
				out[3],
				out[2],
				out[1],
				out[0],
				("https://osu.ppy.sh/b/" + to_string(bid)).c_str());
			cqhttp_api::send_message(tar, message);
		}
		static void rctpp(Target tar) {
			Database db;
			db.Connect();
			db.addcallcount();
			string cmd = utils::unescape(tar.message.substr(5));
			utils::string_trim(cmd);
			utils::string_replace(cmd, u8"��", ":");
			utils::string_replace(cmd, "[CQ:", "");
			ScorePanelData sp_data = { 0 };
			int temp;
			osu_api::v1::mode gamemode;
			gamemode = osu_api::v1::mode::std;
			string username = "";
			string beatmap;
			int64_t userid;
			char beatmap_url[512];
			if (cmd.length() > 0) {
				if (cmd[0] == ':') {
					userid = db.osu_getuserid(tar.user_id);
					if (userid == 0) {
						cqhttp_api::send_message(tar, �û�û�а�osu�˺�);
						return;
					}
					else {
						try {
							if (utils::isNum(cmd.substr(cmd.length() - 1))) {
								temp = stoi(cmd.substr(cmd.length() - 1));
							}
							else {
								cqhttp_api::send_message(tar, Ӣ��ģʽ����ʾ);
								return;
							}
						}
						catch (std::exception) {
							temp = 0;
						}
						if (temp < 4 && temp > -1) {
							gamemode = (osu_api::v1::mode)temp;
						}
						else {
							gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(userid);
						}
					}
				}
				else if (cmd[0] != ':') {
					if (cmd.find(':') != string::npos) {
						if (cmd.find("at,qq=") != string::npos) {
							userid = db.osu_getuserid(stoll(utils::GetMiddleText(cmd, "=", "]")));
							if (userid == 0) {
								cqhttp_api::send_message(tar, ����ѯ���û�δ��osu�˺�);
								return;
							}
						}
						else {
							try {
								username = cmd.substr(0, cmd.find(':'));
								if (username.length() < 1) username = "%^%^%^!*(^&";
							}
							catch (std::exception) {
								username = "%^%^%^!*(^&";
							}
						}
						try {
							if (utils::isNum(cmd.substr(cmd.find(':') + 1))) {
								temp = stoi(cmd.substr(cmd.find(':') + 1));
							}
							else {
								cqhttp_api::send_message(tar, Ӣ��ģʽ����ʾ);
								return;
							}
						}
						catch (std::exception) {
							temp = 0;
						}
						if (temp < 4 && temp > -1) {
							gamemode = (osu_api::v1::mode)temp;
						}
						else {
							gamemode = osu_api::v1::mode::std;
						};
					}
					else {
						if (cmd.find("at,qq=") != string::npos) {
							userid = db.osu_getuserid(stoll(utils::GetMiddleText(cmd, "=", "]")));
							if (userid == 0) {
								cqhttp_api::send_message(tar, ����ѯ���û�δ��osu�˺�);
								return;
							}
						}
						else {
							username = cmd;
						}
						if (username.empty()) {
							if (osu_api::v1::api::GetUser(userid, osu_api::v1::mode::std, &sp_data.user_info) == 0) {
								cqhttp_api::send_message(tar, ����ѯ���û��ѱ�bancho���);
								return;
							}
							else {
								gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(sp_data.user_info.user_id);
							}
						}
						else {
							utils::string_trim(cmd);
							if (osu_api::v1::api::GetUser(cmd, osu_api::v1::mode::std, &sp_data.user_info) == 0) {
								cqhttp_api::send_message(tar, δ��bancho������Ҫ��ѯ���û���Ϣ);
								return;
							}
							else {
								gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(sp_data.user_info.user_id);
							}
						}
					}
				}
			}
			else {
				userid = db.osu_getuserid(tar.user_id);
				if (userid == 0) {
					cqhttp_api::send_message(tar, �û�û�а�osu�˺�);
					return;
				}
				else {
					gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(userid);
				}
			}
			if (username.empty()) {
				if (osu_api::v1::api::GetUser(userid, gamemode, &sp_data.user_info) == 0) {
					cqhttp_api::send_message(tar, u8"û���ҵ���¼...");
					return;
				}
				if (osu_api::v1::api::GetUserRecent(userid, gamemode, &sp_data.score_info) == 0) {
					switch (gamemode) {
					case osu_api::v1::mode::std:
						cqhttp_api::send_message(tar, u8"�������ģʽStandard��û�������¼~");
						return;
					case osu_api::v1::mode::taiko:
						cqhttp_api::send_message(tar, u8"�������ģʽTaiko��û�������¼~");
						return;
					case osu_api::v1::mode::ctb:
						cqhttp_api::send_message(tar, u8"�������ģʽCatch the Beat��û�������¼~");
						return;
					case osu_api::v1::mode::mania:
						cqhttp_api::send_message(tar, u8"�������ģʽMania��û�������¼~");
						return;
					}
				}
			}
			else {
				if (username.length() > 20) {
					"";
					cqhttp_api::send_message(tar, ���ṩ�Ĳ���������������);
					return;
				}
				utils::string_trim(username);
				if (osu_api::v1::api::GetUser(username, gamemode, &sp_data.user_info) == 0) {
					cqhttp_api::send_message(tar, u8"û����˻���TA�����������ģʽ��");
					return;
				}
				if (osu_api::v1::api::GetUserRecent(username, gamemode, &sp_data.score_info) == 0) {
					switch (gamemode) {
					case osu_api::v1::mode::std:
						cqhttp_api::send_message(tar, u8"TA�����ģʽStandard��û�������¼~");
						return;
					case osu_api::v1::mode::taiko:
						cqhttp_api::send_message(tar, u8"TA�����ģʽTaiko��û�������¼~");
						return;
					case osu_api::v1::mode::ctb:
						cqhttp_api::send_message(tar, u8"TA�����ģʽCatch the Beat��û�������¼~");
						return;
					case osu_api::v1::mode::mania:
						cqhttp_api::send_message(tar, u8"TA�����ģʽMania��û�������¼~");
						return;
					}
				}
			}
			sp_data.mode = gamemode;
			osu_api::v1::api::GetBeatmap(sp_data.score_info.beatmap_id, &sp_data.beatmap_info);
			sprintf_s(beatmap_url, OSU_FILE_URL "%lld", sp_data.score_info.beatmap_id);
			beatmap = NetConnection::HttpsGet(beatmap_url);
			if (beatmap.empty()) {
				cqhttp_api::send_message(tar, ��ȡ������Ϣ����);
				return;
			}
			int second, minute, maxsecond, maxminute;
			if (sp_data.mode == osu_api::v1::mode::std) {
				oppai pp;
				pp.read_data(beatmap);
				pp.mods((int)sp_data.score_info.mods);
				// if fc
				pp.accuracy_percent(-1);
				pp.n300((int)sp_data.score_info.n300);
				pp.n100((int)sp_data.score_info.n100);
				pp.n50((int)sp_data.score_info.n50);
				oppai_result t = pp.calc();
				sp_data.fc = (int)t.data.total_pp.value();
				// original
				pp.end((int)sp_data.score_info.n300 + (int)sp_data.score_info.n100 + (int)sp_data.score_info.n50
					+ (int)sp_data.score_info.nmiss);
				pp.nmiss((int)sp_data.score_info.nmiss);
				pp.combo((int)sp_data.score_info.combo);
				sp_data.pp_info = pp.calc();
				sp_data.pp_info.data.total_star = t.data.total_star;
				// reset
				pp.end(-1);
				pp.n300(-1);
				pp.n100(-1);
				pp.n50(-1);
				pp.nmiss(-1);
				pp.combo(-1);
				for (int i = 0; i < 5; ++i) {
					pp.accuracy_percent((float)(i == 0 ? 95 : 96 + i));
					sp_data.confirmed_acc[i] = (int)(pp.calc().data.total_pp.value());
				}
				if (sp_data.pp_info.code == 0) {
					sp_data.pp_info.data.maxcombo = t.data.maxcombo;
					sp_data.pp_info.data.maxlength = t.data.length;
				}
				second = sp_data.pp_info.data.length / 1000 % 60;
				minute = sp_data.pp_info.data.length / 1000 / 60;
				maxsecond = sp_data.pp_info.data.maxlength / 1000 % 60;
				maxminute = sp_data.pp_info.data.maxlength / 1000 / 60;
			}
			else {
				if (sp_data.mode == osu_api::v1::mode::mania) {
					long n1, n2;
					n1 = sp_data.score_info.n50 * 50.0 + sp_data.score_info.n100 * 100.0
						+ sp_data.score_info.nkatu * 200.0
						+ (sp_data.score_info.n300 + sp_data.score_info.ngeki) * 300.0;
					n2 = 300
						* (sp_data.score_info.nmiss + sp_data.score_info.n50 + sp_data.score_info.nkatu
							+ sp_data.score_info.n100 + sp_data.score_info.n300 + sp_data.score_info.ngeki);
					sp_data.pp_info.data.accuracy = (double)n1 / (double)n2 * 100.0;
				}
				if (sp_data.mode == osu_api::v1::mode::taiko) {
					double n1, n2;
					n1 = ((double)sp_data.score_info.n100 + (double)sp_data.score_info.nkatu) * 0.5
						+ (double)sp_data.score_info.n300 + (double)sp_data.score_info.ngeki;
					n2 = (double)sp_data.score_info.nmiss + (double)sp_data.score_info.n100
						+ (double)sp_data.score_info.nkatu + (double)sp_data.score_info.n300
						+ (double)sp_data.score_info.ngeki;
					sp_data.pp_info.data.accuracy = (double)n1 / (double)n2 * 100.0;
				}
				if (sp_data.mode == osu_api::v1::mode::ctb) {
					long n1, n2;
					n1 = sp_data.score_info.n50 + sp_data.score_info.n100 + sp_data.score_info.n300;
					n2 = sp_data.score_info.nkatu + sp_data.score_info.nmiss + sp_data.score_info.n50
						+ sp_data.score_info.n100 + sp_data.score_info.n300;
					sp_data.pp_info.data.maxcombo = sp_data.beatmap_info.maxcombo;
					sp_data.pp_info.data.accuracy = (double)n1 / (double)n2 * 100.0;
				}
				second = 0;
				minute = 0;
				maxsecond = 0;
				maxminute = 0;
				sp_data.pp_info.data.titleUnicode = sp_data.beatmap_info.title;
				sp_data.pp_info.data.artistUnicode = sp_data.beatmap_info.artist;
				sp_data.pp_info.data.title = sp_data.beatmap_info.title;
				sp_data.pp_info.data.artist = sp_data.beatmap_info.artist;
				sp_data.pp_info.data.creator = sp_data.beatmap_info.creator;
				sp_data.pp_info.data.total_star = sp_data.beatmap_info.stars;
				sp_data.pp_info.data.bpm = sp_data.beatmap_info.bpm;
				sp_data.pp_info.data.ar = sp_data.beatmap_info.ar;
				sp_data.pp_info.data.od = sp_data.beatmap_info.od;
				sp_data.pp_info.data.cs = sp_data.beatmap_info.cs;
				sp_data.pp_info.data.hp = sp_data.beatmap_info.hp;
				sp_data.pp_info.data.combo = sp_data.score_info.combo;
			}
			char message[5120];
			string modeStr;
			sp_data.mode == osu_api::v1::mode::std
				? modeStr = "Standard"
				: sp_data.mode == osu_api::v1::mode::taiko
				? modeStr = "Taiko"
				: sp_data.mode == osu_api::v1::mode::ctb
				? modeStr = "CtB"
				: sp_data.mode == osu_api::v1::mode::mania ? modeStr = "Mania" : modeStr = "Unknown";

			if (sp_data.mode == osu_api::v1::mode::std) {
				sprintf_s(message,
					5120,
					u8"%s's previous play - %s\n"
					u8"%s - %s[%s]\n"
					"mapped by %s\n"
					"[ar%.1f | od%.1f | cs%.1f | hp%.1f | bpm%.2f]\n"
					"Stars:%.2f* | objects:%d | %d:%02d / %d:%02d\n"
					"300��%d | 100��%d | 50��%d | miss��%d\n"
					"combo %dx/%dx | acc %.2f%% | %s | %s\n"
					"%.0fpp | if fc %dpp\n\n"
					"100%% : %dpp\n"
					"99%% : %dpp\n"
					"98%% : %dpp\n"
					"95%% : %dpp\n%s",
					sp_data.user_info.username.c_str(),
					modeStr.c_str(),
					sp_data.pp_info.data.artistUnicode.c_str(),
					sp_data.pp_info.data.titleUnicode.c_str(),
					sp_data.pp_info.data.map_version.c_str(),
					sp_data.pp_info.data.creator.c_str(),
					sp_data.pp_info.data.ar,
					sp_data.pp_info.data.od,
					sp_data.pp_info.data.cs,
					sp_data.pp_info.data.hp,
					sp_data.pp_info.data.bpm,
					sp_data.pp_info.data.total_star,
					sp_data.pp_info.data.n300,
					minute,
					second,
					maxminute,
					maxsecond,
					sp_data.score_info.n300,
					sp_data.score_info.n100,
					sp_data.score_info.n50,
					sp_data.score_info.nmiss,
					sp_data.score_info.combo,
					sp_data.beatmap_info.maxcombo,
					sp_data.pp_info.data.accuracy,
					sp_data.score_info.mods == 0 ? "No Mod"
					: osu_api::v1::api::modParser(sp_data.score_info.mods).c_str(),
					sp_data.score_info.rank.c_str(),
					sp_data.pp_info.data.total_pp.has_value() ? sp_data.pp_info.data.total_pp.value() : 0.0,
					sp_data.fc,
					sp_data.confirmed_acc[4],
					sp_data.confirmed_acc[3],
					sp_data.confirmed_acc[2],
					sp_data.confirmed_acc[0],
					("https://osu.ppy.sh/b/" + to_string(sp_data.beatmap_info.beatmap_id)).c_str());
			}
			else {
				if (sp_data.mode == osu_api::v1::mode::ctb) {
					sprintf_s(
						message,
						5120,
						u8"%s's previous play - %s\n"
						u8"%s - %s[%s]\n"
						"mapped by %s\n"
						"[ar%.1f | od%.1f | cs%.1f | hp%.1f | bpm%.2f]\n"
						"Stars:%.2f* | objects:%d | %d:%02d / %d:%02d\n"
						"combo %dx/%dx | acc %.2f%% | %s | %s\n\n%s",
						sp_data.user_info.username.c_str(),
						modeStr.c_str(),
						sp_data.pp_info.data.artistUnicode.length() > 3 ? sp_data.pp_info.data.artistUnicode.c_str()
						: sp_data.pp_info.data.artist.c_str(),
						sp_data.pp_info.data.titleUnicode.length() > 3 ? sp_data.pp_info.data.titleUnicode.c_str()
						: sp_data.pp_info.data.title.c_str(),
						sp_data.pp_info.data.map_version.c_str(),
						sp_data.pp_info.data.creator.c_str(),
						sp_data.pp_info.data.ar,
						sp_data.pp_info.data.od,
						sp_data.pp_info.data.cs,
						sp_data.pp_info.data.hp,
						sp_data.pp_info.data.bpm,
						sp_data.pp_info.data.total_star,
						sp_data.pp_info.data.n300,
						minute,
						second,
						maxminute,
						maxsecond,
						sp_data.score_info.combo,
						sp_data.beatmap_info.maxcombo,
						sp_data.pp_info.data.accuracy,
						sp_data.score_info.mods == 0 ? "No Mod"
						: osu_api::v1::api::modParser(sp_data.score_info.mods).c_str(),
						sp_data.score_info.rank.c_str(),
						("https://osu.ppy.sh/b/" + to_string(sp_data.beatmap_info.beatmap_id)).c_str());
				}
				else {
					sprintf_s(
						message,
						5120,
						u8"%s's previous play - %s\n"
						u8"%s - %s[%s]\n"
						"mapped by %s\n"
						"[ar%.1f | od%.1f | cs%.1f | hp%.1f | bpm%.2f]\n"
						"Stars:%.2f* | objects:%d | %d:%02d / %d:%02d\n"
						"combo %dx | acc %.2f%% | %s | %s\n\n%s",
						sp_data.user_info.username.c_str(),
						modeStr.c_str(),
						sp_data.pp_info.data.artistUnicode.length() > 3 ? sp_data.pp_info.data.artistUnicode.c_str()
						: sp_data.pp_info.data.artist.c_str(),
						sp_data.pp_info.data.titleUnicode.length() > 3 ? sp_data.pp_info.data.titleUnicode.c_str()
						: sp_data.pp_info.data.title.c_str(),
						sp_data.pp_info.data.map_version.c_str(),
						sp_data.pp_info.data.creator.c_str(),
						sp_data.pp_info.data.ar,
						sp_data.pp_info.data.od,
						sp_data.pp_info.data.cs,
						sp_data.pp_info.data.hp,
						sp_data.pp_info.data.bpm,
						sp_data.pp_info.data.total_star,
						sp_data.pp_info.data.n300,
						minute,
						second,
						maxminute,
						maxsecond,
						sp_data.score_info.combo,
						sp_data.pp_info.data.accuracy,
						sp_data.score_info.mods == 0 ? "No Mod"
						: osu_api::v1::api::modParser(sp_data.score_info.mods).c_str(),
						sp_data.score_info.rank.c_str(),
						("https://osu.ppy.sh/b/" + to_string(sp_data.beatmap_info.beatmap_id)).c_str());
				}
			}
			cqhttp_api::send_message(tar, message);
			db.osu_UpdatePPRecord(tar.user_id, sp_data.beatmap_info.beatmap_id);
		}
		static void setmode(Target tar) {
			string cmd = tar.message.substr(7);
			utils::string_replace(cmd, " ", "");
			if (!utils::isNum(cmd)) {
				cqhttp_api::send_message(tar, Ӣ��ģʽ����ʾ);
				return;
			}
			Database db;
			db.Connect();
			db.addcallcount();
			if (db.osu_SetUserMainMode(tar.user_id, stoi(cmd)) == 0) {
				cqhttp_api::send_message(tar, u8"��ִ�в���ʱ�����˴���..���Ժ����ԡ�");
				return;
			}
			switch (stoi(cmd)) {
			case 0:
				cqhttp_api::send_message(tar, u8"�����Ҫ��Ϸģʽ������ΪStandard.");
				break;
			case 1:
				cqhttp_api::send_message(tar, u8"�����Ҫ��Ϸģʽ������ΪTaiko.");
				break;
			case 2:
				cqhttp_api::send_message(tar, u8"�����Ҫ��Ϸģʽ������ΪCatch the Beat.");
				break;
			case 3:
				cqhttp_api::send_message(tar, u8"�����Ҫ��Ϸģʽ������ΪMania.");
				break;
			default:
				cqhttp_api::send_message(tar, u8"������δ֪����");
				break;
			}
		}
		static void bphead_tail(Target tar) {
			string cmd = tar.message.substr(4);
			utils::string_trim(cmd);
			cmd = utils::unescape(cmd);
			utils::string_replace(cmd, u8"��", ":");
			utils::string_replace(cmd, "[CQ:", "");
			int temp;
			osu_api::v1::mode gamemode;
			osu_api::v1::user_info UI = { 0 };
			vector<osu_api::v1::score_info> SI;
			int64_t userid;
			string username = "";
			Database db;
			db.Connect();
			db.addcallcount();
			if (cmd.length() > 0) {
				if (cmd[0] == ':') {
					userid = db.osu_getuserid(tar.user_id);
					if (userid == 0) {
						cqhttp_api::send_message(tar, �û�û�а�osu�˺�);
						return;
					}
					else {
						try {
							temp = stoi(cmd.substr(cmd.length() - 1));
							if (temp < 4 && temp > -1) {
								gamemode = (osu_api::v1::mode)temp;
							}
							else {
								gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(userid);
							}
						}
						catch (std::exception) {
							gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(userid);
						}
					}
				}
				else if (cmd[0] != ':') {
					if (cmd.find(':') != string::npos) {
						if (cmd.find("at,qq=") != string::npos) {
							userid = db.osu_getuserid(stoll(utils::GetMiddleText(cmd, "=", "]")));
							if (userid == 0) {
								cqhttp_api::send_message(tar, ����ѯ���û�δ��osu�˺�);
								return;
							}
						}
						else {
							try {
								username = cmd.substr(0, cmd.find(':'));
							}
							catch (std::exception) {
								username = "%^%^%^!*(^&";
							}
						}
						try {
							temp = stoi(cmd.substr(cmd.find(':') + 1));
						}
						catch (std::exception) {
							temp = 0;
						}
						if (temp < 4 && temp > -1) {
							gamemode = (osu_api::v1::mode)temp;
						}
						else {
							gamemode = osu_api::v1::mode::std;
						};
					}
					else {
						if (cmd.find("at,qq=") != string::npos) {
							userid = db.osu_getuserid(stoll(utils::GetMiddleText(cmd, "=", "]")));
							if (userid == 0) {
								cqhttp_api::send_message(tar, ����ѯ���û�δ��osu�˺�);
								return;
							}
						}
						else {
							username = cmd;
						}
						if (username.empty()) {
							if (osu_api::v1::api::GetUser(userid, osu_api::v1::mode::std, &UI) == 0) {
								cqhttp_api::send_message(tar, ����ѯ���û��ѱ�bancho���);
								return;
							}
							else {
								gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(UI.user_id);
							}
						}
						else {
							if (osu_api::v1::api::GetUser(cmd, osu_api::v1::mode::std, &UI) == 0) {
								cqhttp_api::send_message(tar, δ��bancho������Ҫ��ѯ���û���Ϣ);
								return;
							}
							else {
								gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(UI.user_id);
							}
						}
					}
				}
			}
			else {
				userid = db.osu_getuserid(tar.user_id);
				if (userid == 0) {
					cqhttp_api::send_message(tar, �û�û�а�osu�˺�);
					return;
				}
				else {
					gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(userid);
				}
			}
			if (username.empty()) {
				if (osu_api::v1::api::GetUser(userid, gamemode, &UI) == 0) {
					cqhttp_api::send_message(tar, ����ѯ���û��ѱ�bancho���);
					return;
				}
				if (osu_api::v1::api::GetUserBest(userid, 100, gamemode, SI) == 0) {
					cqhttp_api::send_message(tar, u8"���һ�����ģʽ����ʹ�ô������~\n?*??(�@?�A*)??*?");
					return;
				}
			}
			else {
				if (username.length() > 20) {
					cqhttp_api::send_message(tar, ���ṩ�Ĳ���������������);
					return;
				}
				if (osu_api::v1::api::GetUserBest(username, 100, gamemode, SI) == 0) {
					cqhttp_api::send_message(tar, u8"���һ�����ģʽ����ʹ�ô������~\n?*??(�@?�A*)??*?");
					return;
				}
			}
			if (SI.size() < 100) {
				cqhttp_api::send_message(tar, u8"���һ�����ģʽ����ʹ�ô������~\n?*??(�@?�A*)??*?");
				return;
			}
			UI.mode = gamemode;
			string modestr;
			switch (UI.mode) {
			case 0:
				modestr = "Standard";
				break;
			case 1:
				modestr = "Taiko";
				break;
			case 2:
				modestr = "Ctb";
				break;
			case 3:
				modestr = "Mania";
				break;
			default:
				modestr = "Unknown";
				break;
			}
			char message[1024];
			double totalvalue = 0.0;
			for (int i = 0; i < 100; ++i) {
				totalvalue = totalvalue + SI[i].pp;
			}
			sprintf_s(message,
				1024,
				u8"%s - ģʽ %s\n"
				u8"���bp1�� %.2f pp��\n"
				u8"���bp2�� %.2f pp��\n...\n"
				u8"���bp99�� %.2f pp��\n"
				u8"���bp100�� %.2f pp��\n"
				u8"��bp1��bp100������� %.2f pp��\n"
				u8"���bp�������гɼ���ƽ��ֵ�� %.2f pp��",
				UI.username.c_str(),
				modestr.c_str(),
				SI[0].pp,
				SI[1].pp,
				SI[98].pp,
				SI[99].pp,
				SI[0].pp - SI[99].pp,
				totalvalue / 100);
			cqhttp_api::send_message(tar, message);
		}
		static void ppvs(Target tar) {
			string cmd = tar.message.substr(4);
			utils::string_trim(cmd);
			cmd = utils::unescape(cmd);
			if (cmd.length() < 1) {
				cqhttp_api::send_message(tar, u8"���ṩҪ�Աȵ����ID");
				return;
			}
			if (cmd.length() > 20) {
				cqhttp_api::send_message(tar, ���ṩ�Ĳ���������������);
				return;
			}
			osu_api::v1::mode gamemode;
			UserPanelData UI1 = { 0 };
			UserPanelData UI2 = { 0 };
			Database db;
			db.Connect();
			db.addcallcount();
			int64_t UserID = db.osu_getuserid(tar.user_id);
			if (UserID == 0) {
				cqhttp_api::send_message(tar, �û�û�а�osu�˺�);
				return;
			}
			if (osu_api::v1::api::GetUser(UserID, osu_api::v1::mode::std, &UI1.user_info) == 0) {
				cqhttp_api::send_message(tar, ����ѯ���û��ѱ�bancho���);
				return;
			}
			if (osu_api::v1::api::GetUser(cmd, osu_api::v1::mode::std, &UI2.user_info) == 0) {
				cqhttp_api::send_message(tar, δ��bancho������Ҫ��ѯ���û���Ϣ);
				return;
			}
			float u1pp = db.osu_GetUserPreviousPP(UI1.user_info.user_id);
			float u2pp = db.osu_GetUserPreviousPP(UI2.user_info.user_id);
			UI1.user_info.updatepplus = false;
			UI2.user_info.updatepplus = false;
			//�����pp+������
			/*if (u1pp == UI1.user_info.pp) {
				UI1.user_info.updatepplus = false;
				db.GetUserPreviousPPlus(UI1.user_info.user_id, &UI1.pplus_info);
			}
			else {
				UI1.user_info.updatepplus = true;
			}
			if (u2pp == UI2.user_info.pp) {
				UI2.user_info.updatepplus = false;
				db.GetUserPreviousPPlus(UI2.user_info.user_id, &UI2.pplus_info);
			}
			else {
				UI2.user_info.updatepplus = true;
			}*/
			string fileStr = "osucat\\" + ppvsimg(UI1, UI2);
			cqhttp_api::send_message(tar, u8"[CQ:image,file=" + fileStr + u8"]");
			utils::_DelayDelTmpFile(to_string(OC_ROOT_PATH) + "\\data\\images\\" + fileStr);
		}
		static void badgelist(Target tar) {
			int64_t uid;
			Database db;
			db.Connect();
			db.addcallcount();
			uid = db.osu_getuserid(tar.user_id);
			if (uid == 0) {
				cqhttp_api::send_message(tar, �û�û�а�osu�˺�);
				return;
			}
			string message;
			int i;
			vector<int> t = db.osu_GetBadgeList(uid);
			if (t.size() == 0) {
				cqhttp_api::send_message(tar, u8"�㻹û�л�ù����¡�");
				return;
			}
			message = u8"��ӵ�� " + to_string(t.size()) + u8" ��ѫ��\n";
			for (i = 0; i < t.size(); ++i) {
				i == t.size() - 1 ? message += db.getBadgeStr(t[i]) : message += db.getBadgeStr(t[i]) + "\n";
			}
			cqhttp_api::send_message(tar, message);
		}
		static void occost(Target tar) {
			string cmd = utils::unescape(tar.message.substr(6));
			utils::string_trim(cmd);
			utils::string_replace(cmd, u8"��", ":");
			utils::string_replace(cmd, "[CQ:", "");
			Database db;
			db.Connect();
			db.addcallcount();
			string username = "";
			int64_t UserID;
			double a, c, z, p;
			UserPanelData UI = { 0 };
			if (cmd.length() > 1) {
				if (cmd.find("at,qq=") != string::npos) {
					UserID = db.osu_getuserid(stoll(utils::GetMiddleText(cmd, "=", "]")));
					if (UserID == 0) {
						cqhttp_api::send_message(tar, ����ѯ���û�δ��osu�˺�);
						return;
					}
				}
				else {
					username = cmd;
				}
			}
			else {
				UserID = db.osu_getuserid(tar.user_id);
				if (UserID == 0) {
					cqhttp_api::send_message(tar, �û�û�а�osu�˺�);
					return;
				}
			}
			if (username.empty()) {
				if (osu_api::v1::api::GetUser(UserID, osu_api::v1::mode::std, &UI.user_info) == 0) {
					cqhttp_api::send_message(tar, ����ѯ���û��ѱ�bancho���);
					return;
				}
			}
			else {
				if (username.length() > 20) {
					cqhttp_api::send_message(tar, ���ṩ�Ĳ���������������);
					return;
				}
				if (osu_api::v1::api::GetUser(username, osu_api::v1::mode::std, &UI.user_info) == 0) {
					cqhttp_api::send_message(tar, δ��bancho������Ҫ��ѯ���û���Ϣ);
					return;
				}
			}
			// ��PP+ֹͣ����֮�� ��ʹ�û����ȡ����
			//p = db.GetUserPreviousPP(UI.user_info.user_id);
			//if (UI.user_info.pp != p) {
			//    vector<long> pp_plus;
			//    try {
			//        pp_plus = NetConnection::getUserPlusData(UI.user_info.user_id);
			//        pplus_info pi;
			//        pi.acc = pp_plus[0];
			//        pi.flow = pp_plus[1];
			//        pi.jump = pp_plus[2];
			//        pi.pre = pp_plus[3];
			//        pi.spd = pp_plus[4];
			//        pi.sta = pp_plus[5];
			//        db.UpdatePPlus(UI.user_info.user_id, UI.user_info.pp, pi);
			//        p = UI.user_info.pp;
			//    } catch (std::exception) {
			//    }
			//}
			p = UI.user_info.pp;
			db.osu_GetUserPreviousPPlus(UI.user_info.user_id, &UI.pplus_info);
			z = 1.92 * pow(UI.pplus_info.jump, 0.953) + 69.7 * pow(UI.pplus_info.flow, 0.596)
				+ 0.588 * pow(UI.pplus_info.spd, 1.175) + 3.06 * pow(UI.pplus_info.sta, 0.993);
			a = pow(UI.pplus_info.acc, 1.2768) * pow(p, 0.88213);
			c = min(0.00930973 * pow(p / 1000, 2.64192) * pow(z / 4000, 1.48422), 7) + min(a / 7554280, 3);
			char message[512];
			sprintf_s(message, 512, u8"��èè��S1�У�%s ��costΪ��%.2f", UI.user_info.username.c_str(), c);
			cqhttp_api::send_message(tar, message);
		}
		static void searchuid(Target tar) {
			string cmd = utils::unescape(tar.message.substr(9));
			utils::string_trim(cmd);
			utils::string_replace(cmd, u8"��", ":");
			utils::string_replace(cmd, "[CQ:", "");
			Database db;
			db.Connect();
			db.addcallcount();
			string username = "";
			int64_t UserID;
			double a, c, z, p;
			UserPanelData UI = { 0 };
			if (cmd.length() > 1) {
				if (cmd.find("at,qq=") != string::npos) {
					UserID = db.osu_getuserid(stoll(utils::GetMiddleText(cmd, "=", "]")));
					if (UserID == 0) {
						cqhttp_api::send_message(tar, ����ѯ���û�δ��osu�˺�);
						return;
					}
				}
				else {
					username = cmd;
				}
			}
			else {
				UserID = db.osu_getuserid(tar.user_id);
				if (UserID == 0) {
					cqhttp_api::send_message(tar, ����ѯ���û�δ��osu�˺�);
					cqhttp_api::send_message(tar, �û�û�а�osu�˺�);
					return;
				}
			}
			if (username.empty()) {
				if (osu_api::v1::api::GetUser(UserID, osu_api::v1::mode::std, &UI.user_info) == 0) {
					cqhttp_api::send_message(tar, ����ѯ���û��ѱ�bancho���);
					return;
				}
			}
			else {
				if (username.length() > 20) {
					cqhttp_api::send_message(tar, ���ṩ�Ĳ���������������);
					return;
				}
				if (osu_api::v1::api::GetUser(username, osu_api::v1::mode::std, &UI.user_info) == 0) {
					cqhttp_api::send_message(tar, δ��bancho������Ҫ��ѯ���û���Ϣ);
					return;
				}
			}
			cqhttp_api::send_message(tar, UI.user_info.username + u8" ��osu!uidΪ " + to_string(UI.user_info.user_id));
		}
		static void bonuspp(Target tar) {
			Database db;
			db.Connect();
			db.addcallcount();
			string cmd = utils::unescape(tar.message.substr(7));
			utils::string_trim(cmd);
			utils::string_replace(cmd, u8"��", ":");
			utils::string_replace(cmd, "[CQ:", "");
			vector<osu_api::v1::score_info> bp;
			osu_api::v1::user_info UI = { 0 };
			int temp;
			osu_api::v1::mode gamemode;
			int64_t userid;
			string username = "";
			if (cmd.length() > 0) {
				if (cmd[0] == ':') {
					userid = db.osu_getuserid(tar.user_id);
					if (userid == 0) {
						cqhttp_api::send_message(tar, �û�û�а�osu�˺�);
						return;
					}
					else {
						try {
							temp = stoi(cmd.substr(cmd.length() - 1));
							if (temp < 4 && temp > -1) {
								gamemode = (osu_api::v1::mode)temp;
							}
							else {
								gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(userid);
							}
						}
						catch (std::exception) {
							gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(userid);
						}
					}
				}
				else if (cmd[0] != ':') {
					if (cmd.find(':') != string::npos) {
						if (cmd.find("at,qq=") != string::npos) {
							userid = db.osu_getuserid(stoll(utils::GetMiddleText(cmd, "=", "]")));
							if (userid == 0) {
								cqhttp_api::send_message(tar, ����ѯ���û�δ��osu�˺�);
								return;
							}
						}
						else {
							try {
								username = cmd.substr(0, cmd.find(':'));
								if (username.length() < 1) username = "%^%^%^!*(^&";
							}
							catch (std::exception) {
								username = "%^%^%^!*(^&";
							}
						}
						try {
							temp = stoi(cmd.substr(cmd.find(':') + 1));
						}
						catch (std::exception) {
							temp = 0;
						}
						if (temp < 4 && temp > -1) {
							gamemode = (osu_api::v1::mode)temp;
						}
						else {
							gamemode = osu_api::v1::mode::std;
						};
					}
					else {
						if (cmd.find("at,qq=") != string::npos) {
							userid = db.osu_getuserid(stoll(utils::GetMiddleText(cmd, "=", "]")));
							if (userid == 0) {
								cqhttp_api::send_message(tar, ����ѯ���û�δ��osu�˺�);
								return;
							}
						}
						else {
							username = cmd;
						}
						if (username.empty()) {
							if (osu_api::v1::api::GetUser(userid, osu_api::v1::mode::std, &UI) == 0) {
								cqhttp_api::send_message(tar, ����ѯ���û��ѱ�bancho���);
								return;
							}
							else {
								gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(UI.user_id);
							}
						}
						else {
							utils::string_trim(cmd);
							if (osu_api::v1::api::GetUser(cmd, osu_api::v1::mode::std, &UI) == 0) {
								cqhttp_api::send_message(tar, δ��bancho������Ҫ��ѯ���û���Ϣ);
								return;
							}
							else {
								gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(UI.user_id);
							}
						}
					}
				}
			}
			else {
				userid = db.osu_getuserid(tar.user_id);
				if (userid == 0) {
					cqhttp_api::send_message(tar, �û�û�а�osu�˺�);
					return;
				}
				else {
					gamemode = (osu_api::v1::mode)db.osu_GetUserDefaultGameMode(userid);
				}
			}
			if (username.empty()) {
				if (osu_api::v1::api::GetUser(userid, gamemode, &UI) == 0) {
					cqhttp_api::send_message(tar, ����ѯ���û��ѱ�bancho���);
					return;
				}
				if (osu_api::v1::api::GetUserBest(userid, 100, gamemode, bp) == 0) {
					cqhttp_api::send_message(tar, u8"���ڴ�ģʽ�ϻ�û�гɼ��ء�");
					return;
				}
			}
			else {
				if (username.length() > 20) {
					cqhttp_api::send_message(tar, ���ṩ�Ĳ���������������);
					return;
				}
				utils::string_trim(username);
				if (osu_api::v1::api::GetUser(username, gamemode, &UI) == 0) {
					cqhttp_api::send_message(tar, δ��bancho������Ҫ��ѯ���û���Ϣ);
					return;
				}
				if (osu_api::v1::api::GetUserBest(username, 100, gamemode, bp) == 0) {
					cqhttp_api::send_message(tar, u8"ta�ڴ�ģʽ�ϻ�û�гɼ��ء�");
					return;
				}
			}
			double extraPolatePP;
			if (bp.size() < 100) {
				extraPolatePP = 0.0;
			}
			double scorepp = 0.0, _bonuspp = 0.0, pp = 0.0, sumOxy = 0.0, sumOx2 = 0.0, avgX = 0.0, avgY = 0.0,
				sumX = 0.0;
			for (int i = 0; i < bp.size(); ++i) {
				scorepp += bp[i].pp * pow(0.95, i);
			}
			vector<double> ys;
			for (int i = 0; i < bp.size(); i++) {
				ys.push_back(log10(bp[i].pp * pow(0.95, i)) / log10(100));
			}
#pragma region calculateLinearRegression
			for (int n = 1; n <= ys.size(); n++) {
				double weight = log1p(n + 1.0);
				sumX += weight;
				avgX += n * weight;
				avgY += ys[n - 1] * weight;
			}
			avgX /= sumX;
			avgY /= sumX;
			for (int n = 1; n <= ys.size(); n++) {
				sumOxy += (n - avgX) * (ys[n - 1] - avgY) * log1p(n + 1.0);
				sumOx2 += pow(n - avgX, 2.0) * log1p(n + 1.0);
			}
			double Oxy = sumOxy / sumX;
			double Ox2 = sumOx2 / sumX;
#pragma endregion
			double b[] = { avgY - (Oxy / Ox2) * avgX, Oxy / Ox2 };

			for (double n = 100; n <= UI.playcount; n++) {
				double val = pow(100.0, b[0] + b[1] * n);
				if (val <= 0.0) {
					break;
				}
				pp += val;
			}
			scorepp = scorepp + pp;
			_bonuspp = UI.pp - scorepp;
			int totalscores = UI.count_a + UI.count_s + UI.count_sh + UI.count_ss + UI.count_ssh;
			bool max;
			if (totalscores >= 25397 || _bonuspp >= 416.6667) {
				max = true;
			}
			else
				max = false;
			int rankedscores =
				max ? max(totalscores, 25397) : (int)round(log10(-(_bonuspp / 416.6667) + 1.0) / log10(0.9994));
			if (_isnan(scorepp) || _isnan(_bonuspp)) {
				scorepp = 0.0;
				_bonuspp = 0.0;
				rankedscores = 0;
			}
			string gamemodeStr;
			switch (gamemode) {
			case osu_api::v1::mode::std:
				gamemodeStr = "osu!Standard";
				break;
			case osu_api::v1::mode::taiko:
				gamemodeStr = "osu!Taiko";
				break;
			case osu_api::v1::mode::ctb:
				gamemodeStr = "osu!Catch the Beat";
				break;
			case osu_api::v1::mode::mania:
				gamemodeStr = "osu!Mania";
				break;
			default:
				gamemodeStr = "error";
				break;
			};
			char rtnmessage[1024];
			sprintf_s(rtnmessage,
				1024,
				u8"%s  (%s)\n��PP��%.2f\nԭʼPP��%.2f\nBonus PP��%.2f\n������� %d ������¼��ranked����ɼ���",
				UI.username.c_str(),
				gamemodeStr.c_str(),
				UI.pp,
				scorepp,
				_bonuspp,
				rankedscores);
			cqhttp_api::send_message(tar, rtnmessage);
		}
		static void setbadge(Target tar) {
			string cmd = tar.message.substr(8);
			int64_t uid;
			Database db;
			db.Connect();
			db.addcallcount();
			uid = db.osu_getuserid(tar.user_id);
			if (uid == 0) {
				cqhttp_api::send_message(tar, �û�û�а�osu�˺�);
				return;
			}
			utils::string_trim(cmd);
			if (!utils::isNum(cmd)) {
				cqhttp_api::send_message(tar, u8"���ṩID");

				return;
			}
			int badgeID;
			try {
				badgeID = stoi(cmd);
			}
			catch (std::exception) {
				badgeID = 65536;
			}
			if (badgeID > 65535) {
				cqhttp_api::send_message(tar, u8"�ṩ��ID����");
				return;
			}
			badgeSystem::main bsm;
			vector<int> t = db.osu_GetBadgeList(uid);
			if (t.size() < 1) {
				cqhttp_api::send_message(tar, u8"�㻹û�л���κν����ء�");
				return;
			}
			for (int i = 0; i < t.size(); ++i) {
				if (badgeID == t[i]) {
					db.osu_setshownBadges(uid, badgeID);
					Badgeinfo bi = db.osu_getBadgeInfo(badgeID);
					cqhttp_api::send_message(tar, u8"������Խ�������Ϊ " + bi.name);
					return;
				}
			}
			cqhttp_api::send_message(tar, u8"��δӵ�д˽��¡�");
		}
		static void setbanner_v1(Target tar) {
			string cmd = tar.message.substr(9);
			if (cmd.find("[CQ:image,file=") == string::npos) {
				cqhttp_api::send_message(tar, �Զ���Banner����);
				return;
			}
			Database db;
			db.Connect();
			db.addcallcount();
			int64_t UserID = db.osu_getuserid(tar.user_id);
			if (UserID == 0) {
				cqhttp_api::send_message(tar, �û�û�а�osu�˺�);
				return;
			}
			int64_t QQ = db.osu_getqq(UserID);
			string picPath;
			picPath = utils::GetMiddleText(cmd, "[CQ:image,file=", ",url");
			picPath = picPath.substr(picPath.find(',') + 6);
			cqhttp_api::PictureInfo picInfo = cqhttp_api::getImage(picPath);
			picPath = ".\\data\\cache\\" + picPath + "." + picInfo.format;
			if (!utils::fileExist(picPath)) {
				cqhttp_api::send_message(tar, ����ͼƬ����);
				return;
			}
			DrawableList dl;
			Image cover(picPath);
			Image coverRoundrect(Geometry(1200, 350), Color("none"));
			cover.resize(Geometry(1200, 1000000));
			cover.crop(Geometry(1200, 350));
			dl.clear();
			dl.push_back(DrawableFillColor("white"));
			dl.push_back(DrawableRoundRectangle(0, 0, 1200, 350, 20, 20));
			coverRoundrect.draw(dl);
			coverRoundrect.composite(cover, 0, 0, InCompositeOp);
			coverRoundrect.quality(100);
			string filepath =
				".\\data\\images\\osucat\\custom\\banner_verify\\" + to_string(UserID) + ".jpg";
			coverRoundrect.write(filepath);
			//baiduaip::imageBase64(filepath);
			cqhttp_api::send_message(tar, ���ϴ��������ʾ);
			cqhttp_api::send_group_message(management_groupid, u8"��һ���µ�banner���ϴ���������UID��" + to_string(UserID) + u8"\n������QQ��" + to_string(QQ)
				+ u8" ,�뾡�����Ŷ��\r\nbanner���ݣ�\r\n"
				+ "[CQ:image,file=" + filepath.substr(14) + "]");
		}
		static void setinfopanel_v1(Target tar) {
			string cmd = tar.message.substr(12);
			if (cmd.find("[CQ:image,file=") == string::npos) {
				cqhttp_api::send_message(tar, �Զ���InfoPanel����);
				return;
			}
			Database db;
			db.Connect();
			db.addcallcount();
			int64_t UserID = db.osu_getuserid(tar.user_id);
			if (UserID == 0) {
				cqhttp_api::send_message(tar, �û�û�а�osu�˺�);
				return;
			}
			int64_t QQ = db.osu_getqq(UserID);
			string picPath;
			picPath = utils::GetMiddleText(cmd, "[CQ:image,file=", ",url");
			picPath = picPath.substr(picPath.find(',') + 6);
			cqhttp_api::PictureInfo picInfo = cqhttp_api::getImage(picPath);
			picPath = ".\\data\\cache\\" + picPath + "." + picInfo.format;
			if (!utils::fileExist(picPath)) {
				cqhttp_api::send_message(tar, ����ͼƬ����);
				return;
			}
			DrawableList dl;
			Image infoPanel(picPath);
			if (infoPanel.size().height() != 857 || infoPanel.size().width() != 1200 || infoPanel.magick() != "PNG") {
				cqhttp_api::send_message(tar, InfoPanel���ݴ�����ʾ);
				return;
			}
			if (!utils::copyFile(picPath,
				".\\data\\images\\osucat\\custom\\infopanel_verify\\"
				+ to_string(UserID) + ".png")) {
				cqhttp_api::send_message(tar, u8"������һ��δ֪����");
				return;
			}
			cqhttp_api::send_message(tar, ���ϴ��������ʾ);
			cqhttp_api::send_group_message(management_groupid, u8"��һ���µ�InfoPanel���ϴ���������UID��" + to_string(UserID) + u8"\n������QQ��" + to_string(QQ)
				+ u8" ,�뾡�����Ŷ��\r\nInfoPanel���ݣ�\r\n"
				+ "[CQ:image,file=osucat\\custom\\infopanel_verify\\" + to_string(UserID) + ".png]");
		}
		static void resetbanner_v1(Target tar) {
			Database db;
			db.Connect();
			db.addcallcount();
			string UID = to_string(db.osu_getuserid(tar.user_id));
			string picPath = "./work/v1_cover/" + UID + ".jpg";
			DeleteFileA(picPath.c_str());
			cqhttp_api::send_message(tar, u8"�ѳɹ��������banner��");
		}
		static void resetinfopanel_v1(Target tar) {
			Database db;
			db.Connect();
			db.addcallcount();
			string UID = to_string(db.osu_getuserid(tar.user_id));
			string picPath = "./work/v1_infopanel/" + UID + ".png";
			DeleteFileA(picPath.c_str());
			cqhttp_api::send_message(tar, u8"�ѳɹ��������info��塣");
		}
		static void switchfunction(Target tar, GroupSender sdr) {
			string cmd = tar.message.substr(6);
			if (tar.type != Target::Type::GROUP) {
				cqhttp_api::send_message(tar, u8"�˲���������Ⱥ����ɡ�");
				return;
			}
			if (sdr.role == GroupSender::Role::MEMBER) {
				cqhttp_api::send_message(tar, u8"�˲�����Ҫ����Աִ�С�");
				return;
			}
			cmd = utils::unescape(cmd);
			utils::string_trim(cmd);
			utils::string_replace(cmd, u8"��", ":");
			Database db;
			db.Connect();
			db.addcallcount();
			vector<string> temp = utils::string_split(cmd, ':');
			string ��������ȷ =
				u8"�����Ĳ�������ȷ\n��ȷ�Ĳ�������{function}:{on/off}\n��������bp/rctpp/recent/entertainment/repeater",
				�����ѿ��� = u8"������Ѿ�������״̬��", �����ѹر� = u8"������Ѿ����ڽ���״̬��",
				���ύ = u8"�����ѳɹ��ύ��";
			if (temp.size() != 2) {
				cqhttp_api::send_message(tar, ��������ȷ);
				return;
			}
			if (temp[0] == "bp") {
				if (temp[1] == "on") {
					if (db.isGroupEnable(tar.group_id, 1) == 1) {
						cqhttp_api::send_message(tar, �����ѿ���);
						return;
					}
					db.changeGroupSettings(tar.group_id, 1, true);
					cqhttp_api::send_message(tar, ���ύ);
					return;
				}
				if (temp[1] == "off") {
					if (db.isGroupEnable(tar.group_id, 1) == 0) {
						cqhttp_api::send_message(tar, �����ѹر�);
						return;
					}
					db.changeGroupSettings(tar.group_id, 1, false);
					cqhttp_api::send_message(tar, ���ύ);
					return;
				}
				cqhttp_api::send_message(tar, ��������ȷ);
				return;
			}
			if (temp[0] == "rctpp") {
				if (temp[1] == "on") {
					if (db.isGroupEnable(tar.group_id, 2) == 1) {
						cqhttp_api::send_message(tar, �����ѿ���);
						return;
					}
					db.changeGroupSettings(tar.group_id, 2, true);
					cqhttp_api::send_message(tar, ���ύ);
					return;
				}
				if (temp[1] == "off") {
					if (db.isGroupEnable(tar.group_id, 2) == 0) {
						cqhttp_api::send_message(tar, �����ѹر�);
						return;
					}
					db.changeGroupSettings(tar.group_id, 2, false);
					cqhttp_api::send_message(tar, ���ύ);
					return;
				}
				cqhttp_api::send_message(tar, ��������ȷ);
				return;
			}
			if (temp[0] == "recent") {
				if (temp[1] == "on") {
					if (db.isGroupEnable(tar.group_id, 3) == 1) {
						cqhttp_api::send_message(tar, �����ѿ���);
						return;
					}
					db.changeGroupSettings(tar.group_id, 3, true);
					cqhttp_api::send_message(tar, ���ύ);
					return;
				}
				if (temp[1] == "off") {
					if (db.isGroupEnable(tar.group_id, 3) == 0) {
						cqhttp_api::send_message(tar, �����ѹر�);
						return;
					}
					db.changeGroupSettings(tar.group_id, 3, false);
					cqhttp_api::send_message(tar, ���ύ);
					return;
				}
				cqhttp_api::send_message(tar, ��������ȷ);
				return;
			}
			if (temp[0] == "entertainment") {
				if (temp[1] == "on") {
					if (db.isGroupEnable(tar.group_id, 4) == 1) {
						cqhttp_api::send_message(tar, �����ѿ���);
						return;
					}
					db.changeGroupSettings(tar.group_id, 4, true);
					cqhttp_api::send_message(tar, ���ύ);
					return;
				}
				if (temp[1] == "off") {
					if (db.isGroupEnable(tar.group_id, 4) == 0) {
						cqhttp_api::send_message(tar, �����ѹر�);
						return;
					}
					db.changeGroupSettings(tar.group_id, 4, false);
					cqhttp_api::send_message(tar, ���ύ);
					return;
				}
				cqhttp_api::send_message(tar, ��������ȷ);
				return;
			}
			if (temp[0] == "repeater") {
				if (temp[1] == "on") {
					if (db.isGroupEnable(tar.group_id, 5) == 1) {
						cqhttp_api::send_message(tar, �����ѿ���);
						return;
					}
					db.changeGroupSettings(tar.group_id, 5, true);
					cqhttp_api::send_message(tar, ���ύ);
					return;
				}
				if (temp[1] == "off") {
					if (db.isGroupEnable(tar.group_id, 5) == 0) {
						cqhttp_api::send_message(tar, �����ѹر�);
						return;
					}
					db.changeGroupSettings(tar.group_id, 5, false);
					cqhttp_api::send_message(tar, ���ύ);
					return;
				}
				cqhttp_api::send_message(tar, ��������ȷ);
				return;
			}
			cqhttp_api::send_message(tar, ��������ȷ);
			return;
		}
		static void getbadgeinfo(Target tar) {
			string cmd = tar.message.substr(12);
			utils::string_trim(cmd);
			int badge;
			try {
				badge = stoi(cmd);
			}
			catch (std::exception) {
				cqhttp_api::send_message(tar, u8"û�в�ѯ���˻��µ��κ���Ϣ");
				return;
			}
			Database db;
			db.Connect();
			Badgeinfo bi = db.osu_getBadgeInfo(badge);
			if (bi.id != BADGENOTFOUND) {
				cqhttp_api::send_message(tar, "[CQ:image,file=osucat\\badges\\" + to_string(bi.id) + ".png]\nID: " + to_string(bi.id) + u8"\n����: " + bi.name_chinese + "(" + bi.name + u8")\n����: " + bi.description);
			}
			else {
				cqhttp_api::send_message(tar, u8"û�в�ѯ���˻��µ��κ���Ϣ");
			}
		}












	};


	void echo(const Target tar, const GroupSender sdr) {
		send_message(tar, tar.message);
	}

}

#endif