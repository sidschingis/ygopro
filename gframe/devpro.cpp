#include "game.h"
#include "devpro.h"
#include "network.h"
#include "duelclient.h"

namespace ygo {
	irr::gui::IGUIButton* DevPro::btnAiMode;

	void DevPro::Init()
	{
		irr::gui::IGUIWindow* wMainMenu = ygo::mainGame->wMainMenu;
		irr::gui::IGUIEnvironment* env = mainGame->env;


		// Add Ai mode after Lan mode
		wMainMenu->setRelativePosition(rect<s32>(370, 200, 650, 415 + 35));


		int y = 65;
		int y2 = 95;

		btnAiMode				= env->addButton(rect<s32>(10, y, 270, y2), wMainMenu, BUTTON_AI_MODE, dataManager.GetSysString(2017));
		y += 35;
		y2 += 35;
		mainGame->btnServerMode->setRelativePosition(rect<s32>(10, y, 270, y2));
		y += 35;
		y2 += 35;
		mainGame->btnReplayMode->setRelativePosition(rect<s32>(10, y, 270, y2));
		//y += 35;
		//y2 += 35;
		//mainGame->btnTestMode->setRelativePosition(rect<s32>(10, y, 270, y2));
		y += 35;
		y2 += 35;
		mainGame->btnDeckEdit->setRelativePosition(rect<s32>(10, y, 270, y2));
		y += 35;
		y2 += 35;
		mainGame->btnModeExit->setRelativePosition(rect<s32>(10, y, 270, y2));
		
	}

	void DevPro::SetForceMode(bool value) {
		if (mainGame->dInfo.isSingleMode || mainGame->dInfo.isReplay)
			return;

		CTOS_HandResult cshr;
		cshr.res = value;
		DuelClient::SendPacketToServer(CTOS_FORCE_CHAIN, cshr);
	}

	bool DevPro::ParseArg(char** argv , int i)
	{
		if (!strcmp(argv[i], "-f"))
		{ // Force Mode
			//OnJoinHost();
			DevPro::ClickButton(ygo::mainGame->btnLanMode);
			DevPro::ClickButton(ygo::mainGame->btnJoinHost);
			return true;
		}
		else if (!strcmp(argv[i], "-ai")) {
			//ClickButton(devPro->btnAiMode);
			return true;
		}

		return false;
	}

	void DevPro::ClickButton(irr::gui::IGUIElement* btn) {
		irr::SEvent event;
		event.EventType = irr::EET_GUI_EVENT;
		event.GUIEvent.EventType = irr::gui::EGET_BUTTON_CLICKED;
		event.GUIEvent.Caller = btn;
		ygo::mainGame->device->postEventFromUser(event);
	}

	//void OnJoinHost()
	//{
	//	char ip[20];
	//	int i = 0;
	//	wchar_t* pstr = (wchar_t *)mainGame->wLan.GetText(EDITBOX_JOINIP);
	//	while (*pstr && i < 16)
	//		ip[i++] = *pstr++;
	//	ip[i] = 0;

	//	struct addrinfo hints;
	//	memset(&hints, 0, sizeof(struct addrinfo));
	//	hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
	//	hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
	//	hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
	//	hints.ai_protocol = 0;          /* Any protocol */
	//	hints.ai_canonname = NULL;
	//	hints.ai_addr = NULL;
	//	hints.ai_next = NULL;
	//	BufferIO::CopyWStr((wchar_t *)mainGame->wLan.GetText(EDITBOX_JOINIP), ip, 20);
	//	unsigned int remote_addr = htonl(inet_addr(ip));
	//	unsigned int remote_port = _wtoi(mainGame->wLan.GetText(EDITBOX_JOINPORT));
	//	BufferIO::CopyWStr(mainGame->wLan.GetText(EDITBOX_JOINIP), mainGame->gameConf.lastip, 20);
	//	BufferIO::CopyWStr(mainGame->wLan.GetText(EDITBOX_JOINPORT), mainGame->gameConf.lastport, 20);
	//	if (DuelClient::StartClient(remote_addr, remote_port, false)) {
	//		mainGame->wLan.Hide();
	//	}
	//}
}