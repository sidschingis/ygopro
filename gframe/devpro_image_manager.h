#ifndef DEVPRO_IMAGE_MANAGER_H
#define DEVPRO_IMAGE_MANAGER_H

namespace ygo {

	enum TextureType
	{
		SLEEVE = 0,
		AVATAR = 1
	};

	struct TextureData
	{
		TextureType type;
		int player;
		char hostname[256];
		char filename[256];
		char fakename[256];
	};
	class DevProImageManager {
	public:
		bool Init();
		void SetDevice(irr::IrrlichtDevice* dev);
		void LoadSleeve(int player, wchar_t* site, wchar_t* dir);
		void LoadPendingTextures();
	private:
		IrrlichtDevice *device;
		irr::video::IVideoDriver* driver;
		ITexture* DownloadTexture(TextureData *textureData);
		std::vector<TextureData *> pendingTextures;
		void ApplyTexture(TextureData *textureData, ITexture *texture);
	};

	extern DevProImageManager devProImageManager;
}	

#endif // IMAGEMANAGER_H
