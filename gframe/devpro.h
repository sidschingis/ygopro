#ifndef DEVPRO_H
#define DEVPRO_H
#include "devpro_GUIAIMode.h"

namespace ygo {

	class DevPro {
	public:
		static DevPro* Instance();

		static irr::gui::IGUIButton* btnAiMode;

		void SetForceMode(bool value);

		bool OnEvent(const irr::SEvent& event);
		void Init();
		bool ParseArg(char** argv, int i);
		void ClickButton(irr::gui::IGUIElement* btn);
		GUIAIMode wAI;
		recti ResizeWin(s32 x, s32 y, s32 x2, s32 y2, bool chat);
		void HandleSTOCPacketLan(char* data, unsigned int len);
	private:
		static DevPro* devPro;
		DevPro();
	}; 

#define CTOS_FORCE_CHAIN	0x30

#define BUTTON_AI_MODE				10106
}

#endif