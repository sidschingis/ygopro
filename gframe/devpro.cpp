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

		window_size = dimension2du(1024, 640);
		//mainGame->device->setResizable(true);
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
					!deckManager.LoadDeck(mainGame->cbDeckSelect->getItem(mainGame->cbDeckSelect->getSelected()))) 
				{
					return true;
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


	void DevPro::RunStep()
	{
		dimension2du size = mainGame->driver->getScreenSize();
		if (window_size != size) {
			window_size = size;
			OnResize();
		}
	}

	recti DevPro::Resize(s32 x, s32 y, s32 x2, s32 y2, bool chat)
	{
		s32 sx = x2 - x;
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
		return recti(x, y, x2, y2);
	}

	recti DevPro::Resize(recti rect, bool chat)
	{
		s32 x = rect.LowerRightCorner.X;
		s32 y = rect.LowerRightCorner.Y;
		s32 x2 = rect.UpperLeftCorner.X;
		s32 y2 = rect.UpperLeftCorner.Y;

		return Resize(x, y, x2, y2, chat);
	}

	void DevPro::OnResize()
	{
		wAI.OnResize();
		/*wMenu.OnResize();
		wEdit.OnResize();
		wLan.OnResize();
		wHost.OnResize();
		wHostRoom.OnResize();
		wReplayList.OnResize();
		wSingleList.OnResize();
		wHand->setRelativePosition(ResizeWin(500, 450, 825, 605));
		wFTSelect->setRelativePosition(ResizeWin(550, 240, 780, 340));
		wMessage->setRelativePosition(ResizeWin(490, 200, 840, 340));
		wACMessage->setRelativePosition(ResizeWin(490, 240, 840, 300));
		wQuery->setRelativePosition(ResizeWin(490, 200, 840, 340));
		wOptions->setRelativePosition(ResizeWin(490, 200, 840, 340));
		wPosSelect->setRelativePosition(ResizeWin(340, 200, 935, 410));
		wCardSelect->setRelativePosition(ResizeWin(320, 100, 1000, 400));
		wANNumber->setRelativePosition(ResizeWin(550, 200, 780, 295));
		wANCard->setRelativePosition(ResizeWin(560, 170, 970, 370));
		wANAttribute->setRelativePosition(ResizeWin(500, 200, 830, 285));
		wANRace->setRelativePosition(ResizeWin(480, 200, 850, 385));
		wReplaySave->setRelativePosition(ResizeWin(510, 200, 820, 320));
		stHintMsg->setRelativePosition(ResizeWin(500, 60, 820, 90));

		wChat.OnResize();
		wInfoTab.OnResize();

		btnLeaveGame->setRelativePosition(Resize(205, 5, 295, 80));
		wReplayControl->setRelativePosition(Resize(205, 143, 295, 273));
		btnReplayStart->setRelativePosition(Resize(5, 5, 85, 25));
		btnReplayPause->setRelativePosition(Resize(5, 5, 85, 25));
		btnReplayStep->setRelativePosition(Resize(5, 55, 85, 75));
		btnReplayUndo->setRelativePosition(Resize(5, 80, 85, 100));
		btnReplaySwap->setRelativePosition(Resize(5, 30, 85, 50));
		btnReplayExit->setRelativePosition(Resize(5, 105, 85, 125));

		wPhase->setRelativePosition(Resize(480, 310, 855, 330));
		btnDP->setRelativePosition(Resize(0, 0, 50, 20));
		btnSP->setRelativePosition(Resize(65, 0, 115, 20));
		btnM1->setRelativePosition(Resize(130, 0, 180, 20));
		btnBP->setRelativePosition(Resize(195, 0, 245, 20));
		btnM2->setRelativePosition(Resize(260, 0, 310, 20));
		btnEP->setRelativePosition(Resize(325, 0, 375, 20));
		btnShuffle->setRelativePosition(Resize(0, 0, 50, 20));
		btnChainAlways->setRelativePosition(Resize(205, 140, 295, 175));
		btnChainIgnore->setRelativePosition(Resize(205, 100, 295, 135));
		btnChainWhenAvail->setRelativePosition(Resize(205, 180, 295, 215));
		btnCancelOrFinish->setRelativePosition(Resize(205, 230, 295, 265));
		*/

	}
}