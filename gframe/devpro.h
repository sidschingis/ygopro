#ifndef DEVPRO_H
#define DEVPRO_H
#include "devpro_GUIAIMode.h"
#include "devpro_event_handler.h"

namespace ygo {

	class DevPro {
	public:
		static DevPro* Instance();

		static irr::gui::IGUIButton* btnAiMode;

		void SetForceMode(bool value);

		DevProEventHandler eventHandler;
		DevProMenuHandler menuHandler;
		GUIAIMode wAI;
		void Init();
		bool ParseArg(char** argv, int i);
		void ClickButton(irr::gui::IGUIElement* btn);
		recti ResizeWin(s32 x, s32 y, s32 x2, s32 y2, bool chat);
		bool HandleSTOCPacketLan(char* data, unsigned int len);
	private:
		static DevPro* devPro;
		DevPro();
	}; 

#define CTOS_FORCE_CHAIN	0x30

#define BUTTON_AI_MODE				10106
}

#endif