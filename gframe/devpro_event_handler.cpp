#include "devpro_event_handler.h"
#include "game.h"
#include "network.h"
#include "devpro.h"
#include "duelclient.h"

namespace ygo {


	bool DevProMenuHandler::OnEvent(const irr::SEvent& event) {
		switch (event.EventType) {
		case irr::EET_GUI_EVENT: {
			s32 id = event.GUIEvent.Caller->getID();
			switch (event.GUIEvent.EventType) {
			case irr::gui::EGET_BUTTON_CLICKED: {
				switch (id) {
				case BUTTON_AI_MODE: {
					mainGame->HideElement(mainGame->wMainMenu);
					DevPro::Instance()->wAI.Show();
					return true;
				}
				}
			}
			}
		}
		}
		return false;
	}

	
	bool DevProEventHandler::OnEvent(const irr::SEvent& event) {
		switch (event.EventType) {
		case irr::EET_GUI_EVENT: {
			/*
			s32 id = event.GUIEvent.Caller->getID();
			switch (event.GUIEvent.EventType) {
			case irr::gui::EGET_BUTTON_CLICKED: {
				switch (id) {
				case BUTTON_AI_MODE: {
					mainGame->HideElement(mainGame->wMainMenu);
					DevPro::Instance()->wAI.Show();
					return true;
				}
				}
			}
			}
			*/
		}
		}

		return false;
	}

}