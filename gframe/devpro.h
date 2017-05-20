#ifndef DEVPRO_H
#define DEVPRO_H

namespace ygo {

	class DevPro {
	public:

		//static DevPro* devPro;

		static irr::gui::IGUIButton* btnAiMode;

		static void SetForceMode(bool value);

		static void Init();
		static bool ParseArg(char** argv, int i);
		static void ClickButton(irr::gui::IGUIElement* btn);
	};

#define CTOS_FORCE_CHAIN	0x30

#define BUTTON_AI_MODE				10106
}

#endif