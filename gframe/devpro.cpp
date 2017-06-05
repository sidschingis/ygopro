#include "game.h"
#include "duelclient.h"
#include "devpro.h"
#include "devpro_network.h"
#include "devpro_image_manager.h"


namespace ygo {
	irr::gui::IGUIButton* DevPro::btnAiMode;
	DevPro* DevPro::devPro = 0;
	DevPro::DevPro(){}

	DevPro* DevPro::Instance()
	{
		if (!devPro)
		{
			devPro = new DevPro();
		}

		return devPro;
	}

	void DevPro::Init()
	{
		InitConfig();

		irr::gui::IGUIWindow* wMainMenu = ygo::mainGame->wMainMenu;
		irr::gui::IGUIEnvironment* env = mainGame->env;

		wAI.Load();

		// Add Ai mode after Lan mode
		wMainMenu->setRelativePosition(rect<s32>(370, 200, 650, 415 + 35));

		int y = 65;
		int y2 = 95;

		btnAiMode = env->addButton(rect<s32>(10, y, 270, y2), wMainMenu, BUTTON_AI_MODE, dataManager.GetSysString(2017));
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

	void DevPro::InitConfig()
	{
		config.enablesleeveloading = false; 
		config.forced = false;
		BufferIO::CopyWStr(L"DevBot", config.botname, 20);

		FILE* fp = fopen("system.conf", "r");
		if (!fp)
			return;
		char linebuf[256];
		char strbuf[32];
		char valbuf[256];
		wchar_t wstr[256];
		fseek(fp, 0, SEEK_END);
		int fsize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		while (ftell(fp) < fsize) {
			fgets(linebuf, 250, fp);
			sscanf(linebuf, "%s = %99[^\n]", strbuf, valbuf);
			if (!strcmp(strbuf, "enable_sleeve_loading")) {
				config.enablesleeveloading = atoi(valbuf) > 0;
			}
			else if (!strcmp(strbuf, "botname")) {
				BufferIO::DecodeUTF8(valbuf, wstr);
				BufferIO::CopyWStr(wstr, config.botname, 20);
			}
			else if (!strcmp(strbuf, "forced")) {
				config.forced = atoi(valbuf) > 0;
			}
		}
		fclose(fp);
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
			ClickButton(devPro->btnAiMode);
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

	bool DevPro::HandleSTOCPacketLan(char* data, unsigned int len) {
		char* pdata = data;
		unsigned char pktType = BufferIO::ReadUInt8(pdata);
		switch (pktType) {
		case STOC_DUEL_START: {
			mainGame->btnLeaveGame->setText(dataManager.GetSysString(1350));
			mainGame->btnLeaveGame->setVisible(true);
			break;
		}
		case STOC_UPDATE_SLEEVE: {
			STOC_Sleeve* pkt = (STOC_Sleeve*)pdata;
			wchar_t site[256];
			wchar_t dir[256];
			BufferIO::CopyWStr(pkt->site, site, 256);
			BufferIO::CopyWStr(pkt->dir, dir, 256);
			mainGame->gMutex.Lock();
			if (config.enablesleeveloading)
				ygo::devProImageManager.LoadSleeve(pkt->player, site, dir);
			mainGame->gMutex.Unlock();
			break;
		}
		case STOC_HS_PLAYER_ENTER: {
			//PlaySound("./sound/playerenter.wav");
			STOC_HS_PlayerEnter_DevPro* pkt = (STOC_HS_PlayerEnter_DevPro*)pdata;
			if (pkt->pos > 3)
				break;
			wchar_t name[26];
			int offset = 0;
			offset = BufferIO::CopyWStr(pkt->name, name, 20);
			wchar_t elo[5];

			if (pkt->elo[0])
			{
				offset = BufferIO::CopyWStr(L"|", name, 2, offset);
				BufferIO::CopyWStr(pkt->elo, name, 5, offset);
			}

			if (mainGame->dInfo.isTag) {
				if (pkt->pos == 0)
					BufferIO::CopyWStr(pkt->name, mainGame->dInfo.hostname, 20);
				else if (pkt->pos == 1)
					BufferIO::CopyWStr(pkt->name, mainGame->dInfo.hostname_tag, 20);
				else if (pkt->pos == 2)
					BufferIO::CopyWStr(pkt->name, mainGame->dInfo.clientname, 20);
				else if (pkt->pos == 3)
					BufferIO::CopyWStr(pkt->name, mainGame->dInfo.clientname_tag, 20);
			}
			else {
				if (pkt->pos == 0)
					BufferIO::CopyWStr(pkt->name, mainGame->dInfo.hostname, 20);
				else if (pkt->pos == 1)
					BufferIO::CopyWStr(pkt->name, mainGame->dInfo.clientname, 20);
			}
			mainGame->gMutex.Lock();
			mainGame->stHostPrepDuelist[pkt->pos]->setText(name);
			//mainGame->stHostPrepDuelist[pkt->pos]->setText(name);
			mainGame->gMutex.Unlock();
			mainGame->FlashWindow();
			return true;
			//break;
		}
		case STOC_JOIN_GAME: {
			//forced mode for DevPro ranked duels
			if (config.forced) {
				if (mainGame->cbDeckSelect->getSelected() == -1 ||
					!deckManager.LoadDeck(mainGame->cbDeckSelect->getItem(mainGame->cbDeckSelect->getSelected()))) {

					return;
				}
				BufferIO::CopyWStr(mainGame->cbDeckSelect->getItem(mainGame->cbDeckSelect->getSelected()),
					mainGame->gameConf.lastdeck, 20);
				char deckbuf[1024];
				char* pdeck = deckbuf;
				BufferIO::WriteInt32(pdeck, deckManager.current_deck.main.size() + deckManager.current_deck.extra.size());
				BufferIO::WriteInt32(pdeck, deckManager.current_deck.side.size());
				for (size_t i = 0; i < deckManager.current_deck.main.size(); ++i)
					BufferIO::WriteInt32(pdeck, deckManager.current_deck.main[i]->first);
				for (size_t i = 0; i < deckManager.current_deck.extra.size(); ++i)
					BufferIO::WriteInt32(pdeck, deckManager.current_deck.extra[i]->first);
				for (size_t i = 0; i < deckManager.current_deck.side.size(); ++i)
					BufferIO::WriteInt32(pdeck, deckManager.current_deck.side[i]->first);
				DuelClient::SendBufferToServer(CTOS_UPDATE_DECK, deckbuf, pdeck - deckbuf);
				DuelClient::SendPacketToServer(CTOS_HS_READY);
				mainGame->cbDeckSelect->setEnabled(false);
				int selftype = (mainGame->chkHostPrepReady[0]->isEnabled()) ? 0 : 1;
				mainGame->chkHostPrepReady[selftype]->setChecked(true);
			}

			break;
		}
		}

		return false;
	}

	recti DevPro::ResizeWin(s32 x, s32 y, s32 x2, s32 y2, bool chat)
	{
		/*s32 sx = x2 - x;
		s32 sy = y2 - y;
		if (chat)
		{
			y = window_size.Height - sy;
			x2 = window_size.Width;
			y2 = y + sy;
			return recti(x, y, x2, y2);
		}
		x = (x + sx / 2) * window_size.Width / 1024 - sx / 2;
		y = (y + sy / 2) * window_size.Height / 640 - sy / 2;
		x2 = sx + x;
		y2 = sy + y;
		return recti(x, y, x2, y2);*/
		return recti(0, 0, 0, 0);
	}
}