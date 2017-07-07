#ifndef DEVPRO_H
#define DEVPRO_H
#include "devpro_GUIAIMode.h"
#include "devpro_event_handler.h"
#include "devpro_config.h"

namespace ygo {

	class DevPro {
	public:
		static DevPro* Instance();

		static irr::gui::IGUIButton* btnAiMode;

		void SetForceMode(bool value);

		irr::core::dimension2d<irr::u32> window_size;
		DevProEventHandler eventHandler;
		DevProMenuHandler menuHandler;
		GUIAIMode wAI;
		void Init();
		bool ParseArg(char** argv, int i);
		bool HandleSTOCPacketLan(char* data, unsigned int len);
		void ClickButton(irr::gui::IGUIElement* btn);
		void RunStep();
		recti Resize(s32 x, s32 y, s32 x2, s32 y2, bool chat = false);
		recti Resize(recti rect, bool chat = false);
		DevProConfig config;

		// Skin System
		int lpplayer;
		int lpccolor;
		int turncolor;
		int playerlpcolor;
		int extracolor;
		int statcolor;
		int bonuscolor;
		int negativecolor;
		int setcolor;
		int special_color;
		CGUISkinSystem *skinSystem;
	private:
		static DevPro* devPro;
		void InitConfig();
		void OnResize();
		DevPro();
		int ExtractColor(const stringw name, CGUISkinSystem *skinSystem, unsigned int normalColor);
	}; 

#define CTOS_FORCE_CHAIN	0x30

#define STOC_UPDATE_SLEEVE		0x30

#define BUTTON_AI_MODE				10106
}

#endif