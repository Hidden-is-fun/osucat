#pragma once
#ifndef SC_MAIN_HPP
#define SC_MAIN_HPP

namespace steamcat {

	struct UserInfo {
		int64_t steamid;
		int communityvisibilitystate;
		int profilestate;
		string personaname;
		int commentpermission;
		string profileurl;
		string avatar;
		string avatarmedium;
		string avatarfull;
		string avatarhash;
		int64_t lastlogoff;
		int personastate;
		string realname;
		string primaryclanid;
		int64_t timecreated;
		int personastateflags;
		string loccountrycode;
	};

	struct UserBanStats
	{
		int64_t SteamId;
		bool CommunityBanned;
		bool VACBanned;
		int NumberOfVACBans;
		int NumberOfGameBans;
		int DaysSinceLastBan;
		string EconomyBan;
	};

	class csgocheck {
	public:

		static string checkrtnmsg(const UserInfo us, const UserBanStats ubs) {
			string returnmsg, radstr = utils::rand_str();
			string path = to_string(OC_ROOT_PATH) + "\\data\\images\\osucat\\" + radstr + ".png";
			if (us.avatarfull != "null") {
				osucat::NetConnection::DownloadFile(us.avatarfull, path);
				returnmsg += u8"[CQ:image,file=osucat\\" + radstr + ".png]" + "\n";
			}
			if (utils::forbiddenWordsLibrary(us.personaname)) {
				returnmsg += u8"�û�����*Υ��������*";
			}
			else returnmsg += u8"�û�����" + us.personaname;
			if (us.realname != "null") {
				if (utils::forbiddenWordsLibrary(us.realname)) {
					returnmsg += u8"\n��ʵ������*Υ��������*";
				}
				else returnmsg += u8"\n��ʵ������" + us.realname;
			}
			if (us.loccountrycode != "null") {
				returnmsg += u8"\n������" + utils::getCountryNameByCode(us.loccountrycode);
			}
			if (us.lastlogoff != 0) {
				returnmsg += u8"\n����¼�ڣ�" + utils::unixTime2Str(us.lastlogoff);
			}
			returnmsg += u8"\nSteamID��" + to_string(us.steamid);
			if (ubs.CommunityBanned) {
				returnmsg += u8"\n�û���ǰ�ѱ��������";
			}
			if (ubs.EconomyBan != "none") {
				returnmsg += u8"\n�û���ǰ�ѱ���ֹ����";
			}
			if (ubs.VACBanned) {
				returnmsg += u8"\nVAC�������";
			}
			if (ubs.NumberOfGameBans > 0) {
				returnmsg += u8"\n��Ϸ�������";
			}
			if (ubs.VACBanned) {
				returnmsg += u8"\nVAC���������" + to_string(ubs.NumberOfVACBans);
			}
			if (ubs.NumberOfGameBans > 0) {
				returnmsg += u8"\n��Ϸ���������" + to_string(ubs.NumberOfGameBans);
			}
			if (ubs.VACBanned || ubs.NumberOfGameBans > 0) {
				returnmsg += u8"\n�ϴη���ڣ�" + to_string(ubs.DaysSinceLastBan) + u8" ��ǰ";
			}
			if (ubs.CommunityBanned == false && ubs.EconomyBan == "none" && ubs.NumberOfGameBans == 0 && ubs.VACBanned == false) {
				returnmsg += u8"\n���û����κβ�����¼��";
			}
			if (us.profileurl.find("/profiles/") == string::npos) {
				returnmsg += u8"\n�Զ������ӣ�" + us.profileurl;
			}
			else returnmsg += u8"\n��ҳ���ӣ�" + us.profileurl;
			thread DFH(bind(&utils::DelFileHandler, path, 120));
			DFH.detach();
			return returnmsg;
		}

		static string banrtnmsg(const UserInfo us, const UserBanStats ubs, bool VACorGameBan) {
			string returnmsg, radstr = utils::rand_str();
			string path = to_string(OC_ROOT_PATH) + "\\data\\images\\osucat\\" + radstr + ".png";
			if (us.avatarfull != "null") {
				osucat::NetConnection::DownloadFile(us.avatarfull, path);
				returnmsg += u8"[CQ:image,file=osucat\\" + radstr + ".png]" + "\n";
			}
			if (VACorGameBan) {
				returnmsg += u8"��ѯ��һ���µ�VAC��� ��ϸ��Ϣ���£�";
			}
			else returnmsg += u8"��ѯ��һ���µ���Ϸ��� ��ϸ��Ϣ���£�";
			if (utils::forbiddenWordsLibrary(us.personaname)) {
				returnmsg += u8"\n�û�����*Υ��������*";
			}
			else returnmsg += u8"\n�û�����" + us.personaname;
			if (us.realname != "null") {
				if (utils::forbiddenWordsLibrary(us.realname)) {
					returnmsg += u8"\n��ʵ������*Υ��������*";
				}
				else returnmsg += u8"\n��ʵ������" + us.realname;
			}
			if (us.loccountrycode != "null") {
				returnmsg += u8"\n������" + utils::getCountryNameByCode(us.loccountrycode);
			}
			if (us.lastlogoff != 0) {
				returnmsg += u8"\n����¼�ڣ�" + utils::unixTime2Str(us.lastlogoff);
			}
			returnmsg += u8"\nSteamID��" + to_string(us.steamid);
			if (ubs.CommunityBanned) {
				returnmsg += u8"\n�û���ǰ�ѱ��������";
			}
			if (ubs.EconomyBan != "none") {
				returnmsg += u8"\n�û���ǰ�ѱ���ֹ����";
			}
			if (ubs.VACBanned) {
				returnmsg += u8"\nVAC�������";
			}
			if (ubs.NumberOfGameBans > 0) {
				returnmsg += u8"\n��Ϸ�������";
			}
			if (ubs.VACBanned) {
				returnmsg += u8"\nVAC���������" + to_string(ubs.NumberOfVACBans);
			}
			if (ubs.NumberOfGameBans > 0) {
				returnmsg += u8"\n��Ϸ���������" + to_string(ubs.NumberOfGameBans);
			}
			if (ubs.VACBanned || ubs.NumberOfGameBans > 0) {
				returnmsg += u8"\n�ϴη���ڣ�" + to_string(ubs.DaysSinceLastBan) + u8" ��ǰ";
			}
			if (ubs.CommunityBanned == false && ubs.EconomyBan == "none" && ubs.NumberOfGameBans == 0 && ubs.VACBanned == false) {
				returnmsg += u8"\n���û����κβ�����¼��";
			}
			if (us.profileurl.find("/profiles/") == string::npos) {
				returnmsg += u8"\n�Զ������ӣ�" + us.profileurl;
			}
			else returnmsg += u8"\n��ҳ���ӣ�" + us.profileurl;
			thread DFH(bind(&utils::DelFileHandler, path, 120));
			DFH.detach();
			return returnmsg;
		}

		static void check(const Target tar) {
			string tmp = tar.message.substr(7);
			utils::string_trim(tmp);
			if (tmp.empty()) { send_message(tar, u8"���ṩҪ��ѯ����Ϣ��URL/SteamId/VanityURL/VantiyURLName����"); return; }
			if (tmp.find("steamcommunity.com/id/") != string::npos) {
				string t = tmp.substr(tmp.find("/id/") + 4);
				utils::string_replace(t, "/", "");
				string source = osucat::NetConnection::HttpGet("http://api.steampowered.com/ISteamUser/ResolveVanityURL/v0001/?key=" + to_string(SC_STEAM_API_KEY) + "&vanityurl=" + t);
				json j = json::parse(source)["response"];
				if (j["success"].get<int>() == 1) {
					UserBanStats ubs = { 0 };
					if (ban_check(stoll(j["steamid"].get<string>()), &ubs)) {
						UserInfo us = get_user_status(stoll(j["steamid"].get<string>()));
						send_message(tar, checkrtnmsg(us, ubs));
						return;
					}
				}
				else { send_message(tar, u8"δ�ҵ����û�"); return; }
			}
			if (tmp.find("steamcommunity.com/profiles/") != string::npos) {
				string t = tmp.substr(tmp.find("/profiles/") + 10);
				utils::string_replace(t, "/", "");
				UserBanStats ubs = { 0 };
				if (ban_check(stoll(t), &ubs)) {
					UserInfo us = get_user_status(stoll(t));
					send_message(tar, checkrtnmsg(us, ubs));
					return;
				}
				else { send_message(tar, u8"δ�ҵ����û�"); return; }
			}
			if (utils::isNum(tmp) && tmp.length() > 13) {
				UserBanStats ubs = { 0 };
				if (ban_check(stoll(tmp), &ubs)) {
					UserInfo us = get_user_status(stoll(tmp));
					send_message(tar, checkrtnmsg(us, ubs));
					return;
				}
				else { send_message(tar, u8"δ�ҵ����û�"); return; }
			}
			else {
				json j = json::parse(osucat::NetConnection::HttpGet("http://api.steampowered.com/ISteamUser/ResolveVanityURL/v0001/?key=" + to_string(SC_STEAM_API_KEY) + "&vanityurl=" + tmp))["response"];
				if (j["success"].get<int>() == 1) {
					UserBanStats ubs = { 0 };
					if (ban_check(stoll(j["steamid"].get<string>()), &ubs)) {
						UserInfo us = get_user_status(stoll(j["steamid"].get<string>()));
						send_message(tar, checkrtnmsg(us, ubs));
						return;
					}
				}
				else { send_message(tar, u8"δ�ҵ����û�"); return; }
			}
		}

		static void listen(const Target tar) {
			string tmp = tar.message.substr(8);
			utils::string_trim(tmp);
			if (tmp.empty()) { send_message(tar, u8"���ṩҪ��ѯ����Ϣ��URL/SteamId/VanityURL/VantiyURLName����"); return; }
			int64_t steamid;
			UserBanStats ubs = { 0 };
			if (tmp.find("steamcommunity.com/id/") != string::npos) {
				string t = tmp.substr(tmp.find("/id/") + 4);
				utils::string_replace(t, "/", "");
				string source = osucat::NetConnection::HttpGet("http://api.steampowered.com/ISteamUser/ResolveVanityURL/v0001/?key=" + to_string(SC_STEAM_API_KEY) + "&vanityurl=" + t);
				json j = json::parse(source)["response"];
				if (j["success"].get<int>() == 1) {
					steamid = stoll(j["steamid"].get<string>());
				}
				else { send_message(tar, u8"δ�ҵ����û�"); return; }
			}
			else if (tmp.find("steamcommunity.com/profiles/") != string::npos) {
				string t = tmp.substr(tmp.find("/profiles/") + 10);
				utils::string_replace(t, "/", "");
				if (ban_check(stoll(t), &ubs)) {
					steamid = stoll(t);
				}
				else { send_message(tar, u8"δ�ҵ����û�"); return; }
			}
			else if (utils::isNum(tmp) && tmp.length() > 13) {
				UserBanStats ubs = { 0 };
				if (ban_check(stoll(tmp), &ubs)) {
					steamid = stoll(tmp);
				}
				else { send_message(tar, u8"δ�ҵ����û�"); return; }
			}
			else {
				json j = json::parse(osucat::NetConnection::HttpGet("http://api.steampowered.com/ISteamUser/ResolveVanityURL/v0001/?key=" + to_string(SC_STEAM_API_KEY) + "&vanityurl=" + tmp))["response"];
				if (j["success"].get<int>() == 1) {
					steamid = stoll(j["steamid"].get<string>());
				}
				else { send_message(tar, u8"δ�ҵ����û�"); return; }
			}

			osucat::Database db;
			db.Connect();
			if (tar.type == Target::Type::GROUP) {
				if (db.steam_add_ban_listening(steamid, ubs.NumberOfVACBans, ubs.NumberOfGameBans, tar.user_id, tar.group_id)) send_message(tar, u8"�û��ѳɹ���ӵ������б����������ܵ��������������Ⱥ");
				else {
					if (db.steam_change_ban_listening_receive_groupid(steamid, tar.group_id)) send_message(tar, u8"�û��ѳɹ���ӵ������б����������ܵ��������������Ⱥ");
					else {
						if (db.steam_change_ban_listening_receive_userid(steamid, tar.user_id))send_message(tar, u8"���û��Ѵ�����Ⱥ�����б��У��ѽ����������˽�������б�");
						else send_message(tar, u8"���û��Ѵ��������������б���");
					}
				}
				return;
			}
			else {
				if (db.steam_add_ban_listening(steamid, ubs.NumberOfVACBans, ubs.NumberOfGameBans, tar.user_id)) send_message(tar, u8"�û��ѳɹ���ӵ������б����������ܵ������ͨ��˽������");
				else {
					if (db.steam_change_ban_listening_receive_userid(steamid, tar.user_id)) send_message(tar, u8"���û��Ѵ�����Ⱥ�����б��У��ѽ����������˽�������б�");
					else send_message(tar, u8"���û��Ѵ��������������б���");
				}
				return;
			}
		}

		static void _AutoCheck() {
			thread csgocheck_autocheck(bind(&autocheck));
			csgocheck_autocheck.detach();
		}

		static void autocheck() {
			Sleep(4000);
			cout << u8"�Ѵ���steam ban�Զ�����߳�" << endl;
			while (true) {
				osucat::Database db;
				db.Connect();
				vector<CSGOUserInfo> cui;
				cui = db.steam_get_csgo_listen_list();
				if (cui[0].SteamId != -1) {
					for (int i = 0; i < cui.size(); ++i) {
						thread autobancheckt(bind(&checkthread, cui[i]));
						autobancheckt.detach();
						Sleep(2000);
					}
				}
				Sleep(1000 * 60 * 10);
			}
		}

		static void checkthread(CSGOUserInfo cui) {
			UserBanStats ubs = { 0 };
			UserInfo us;
			us = get_user_status(cui.SteamId);
			if (ban_check(cui.SteamId, &ubs)) {
				if (ubs.NumberOfVACBans != cui.VACBanCount) {
					if (cui.ReceiveGroupId.find(';') == string::npos) {
						if (cui.ReceiveGroupId != "null") {
							send_group_message(stoll(cui.ReceiveGroupId), banrtnmsg(us, ubs, true));
							Sleep(1000);
						}
					}
					else {
						vector<string> b = utils::string_split(cui.ReceiveGroupId, ';');
						for (int i = 0; i < b.size(); ++i) {
							send_group_message(stoll(b[i]), banrtnmsg(us, ubs, true));
							Sleep(1000);
						}
					}
					if (cui.ReceiveUserId.find(';') == string::npos) {
						if (cui.ReceiveUserId != "null") {
							send_private_message(stoll(cui.ReceiveUserId), banrtnmsg(us, ubs, true));
						}
					}
					else {
						vector<string> b = utils::string_split(cui.ReceiveUserId, ';');
						for (int i = 0; i < b.size(); ++i) {
							send_private_message(stoll(b[i]), banrtnmsg(us, ubs, true));
							Sleep(1000);
						}
					}
					osucat::Database db;
					db.Connect();
					db.steam_change_ban_stats(ubs.SteamId);
				}
				if (ubs.NumberOfGameBans != cui.GameBanCount) {
					if (cui.ReceiveGroupId.find(';') != string::npos) {
						if (cui.ReceiveGroupId != "null") {
							send_group_message(stoll(cui.ReceiveGroupId), banrtnmsg(us, ubs, false));
							Sleep(1000);
						}
					}
					else {
						vector<string> b = utils::string_split(cui.ReceiveGroupId, ';');
						for (int i = 0; i < b.size(); ++i) {
							send_group_message(stoll(b[i]), banrtnmsg(us, ubs, false));
							Sleep(1000);
						}
					}
					if (cui.ReceiveUserId.find(';') != string::npos) {
						if (cui.ReceiveUserId != "null") {
							send_private_message(stoll(cui.ReceiveUserId), banrtnmsg(us, ubs, false));
						}
					}
					else {
						vector<string> b = utils::string_split(cui.ReceiveUserId, ';');
						for (int i = 0; i < b.size(); ++i) {
							send_private_message(stoll(b[i]), banrtnmsg(us, ubs, false));
							Sleep(1000);
						}
					}
					osucat::Database db;
					db.Connect();
					db.steam_change_ban_stats(ubs.SteamId);
				}
			}
		}

	private:

		static UserInfo get_user_status(int64_t SteamId) {
			char tmp[1024];
			sprintf_s(tmp, "http://api.steampowered.com/ISteamUser/GetPlayerSummaries/v0002/?key=%s&steamids=%lld", SC_STEAM_API_KEY, SteamId);
			json j = json::parse(osucat::NetConnection::HttpGet(tmp))["response"]["players"][0];
			UserInfo us;
			us.steamid = stoll(j["steamid"].get<string>());
			try { us.communityvisibilitystate = j["communityvisibilitystate"].get<int>(); }
			catch (std::exception) { us.communityvisibilitystate = 0; }
			try { us.profilestate = j["profilestate"].get<int>(); }
			catch (std::exception) { us.profilestate = 0; }
			try { us.personaname = j["personaname"].get<string>(); }
			catch (std::exception) { us.personaname = "null"; }
			try { us.commentpermission = j["commentpermission"].get<int>(); }
			catch (std::exception) { us.commentpermission = 0; }
			try { us.profileurl = j["profileurl"].get<string>(); }
			catch (std::exception) { us.profileurl = "null"; }
			try { us.avatar = j["avatar"].get<string>(); }
			catch (std::exception) { us.avatar = "null"; }
			try { us.avatarmedium = j["avatarmedium"].get<string>(); }
			catch (std::exception) { us.avatarmedium = "null"; }
			try { us.avatarfull = j["avatarfull"].get<string>(); }
			catch (std::exception) { us.avatarfull = "null"; }
			try { us.avatarhash = j["avatarhash"].get<string>(); }
			catch (std::exception) { us.avatarhash = "null"; }
			try { us.lastlogoff = j["lastlogoff"].get<int64_t>(); }
			catch (std::exception) { us.lastlogoff = 0; }
			try { us.personastate = j["personastate"].get<int>(); }
			catch (std::exception) { us.personastate = 0; }
			try { us.realname = j["realname"].get<string>(); }
			catch (std::exception) { us.realname = "null"; }
			try { us.primaryclanid = j["primaryclanid"].get<string>(); }
			catch (std::exception) { us.primaryclanid = "null"; }
			try { us.timecreated = j["timecreated"].get<int64_t>(); }
			catch (std::exception) { us.timecreated = 0; }
			try { us.personastateflags = j["personastateflags"].get<int>(); }
			catch (std::exception) { us.personastateflags = 0; }
			try { us.loccountrycode = j["loccountrycode"].get<string>(); }
			catch (std::exception) { us.loccountrycode = "null"; }
			return us;
		}

		static bool ban_check(int64_t SteamId, UserBanStats* rtn) {
			try {
				string source = osucat::NetConnection::HttpGet("http://api.steampowered.com/ISteamUser/GetPlayerBans/v1/?key=" + to_string(SC_STEAM_API_KEY) + "&steamids=" + to_string(SteamId));
				json j = json::parse(source)["players"][0];
				UserBanStats ubs;
				ubs.SteamId = stoll(j["SteamId"].get<string>());
				ubs.CommunityBanned = j["CommunityBanned"].get<bool>();
				ubs.VACBanned = j["VACBanned"].get<bool>();
				ubs.NumberOfGameBans = j["NumberOfGameBans"].get<int>();
				ubs.NumberOfVACBans = j["NumberOfVACBans"].get<int>();
				ubs.DaysSinceLastBan = j["DaysSinceLastBan"].get<int>();
				ubs.EconomyBan = j["EconomyBan"].get<string>();
				*rtn = ubs;
			}
			catch (osucat::NetWork_Exception) {
				return false;
			}
			catch (std::exception& e) {
				return false;
			}
			return true;
		}



	};

	class cmdParser {
	public:
		static void parse(const Target tar, const GroupSender sdr) {

			if (_stricmp(tar.message.substr(0, 7).c_str(), "s check") == 0) {
				csgocheck::check(tar); return;
			}

			if (_stricmp(tar.message.substr(0, 8).c_str(), "s listen") == 0) {
				csgocheck::listen(tar); return;
			}
		}
	};

}
#endif // !SC_MAIN_HPP
