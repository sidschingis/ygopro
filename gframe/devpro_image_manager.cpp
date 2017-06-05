//#include "image_manager.h"
#include "game.h"
#include "devpro_image_manager.h"
#include "image_manager.h"
#include <SFML/Network.hpp>

namespace ygo {
	DevProImageManager devProImageManager;

	bool DevProImageManager::Init() {
		SetDevice(mainGame->device);

		return true;
	}

	void DevProImageManager::SetDevice(irr::IrrlichtDevice* dev) {
		device = dev;
		driver = dev->getVideoDriver();
	}

	ITexture* DevProImageManager::DownloadTexture(TextureData *textureData)
	{
		sf::Http::Request request(textureData->filename, sf::Http::Request::Get);
		sf::Http http(textureData->hostname);
		sf::Http::Response response = http.sendRequest(request);

		if (response.getStatus() == sf::Http::Response::Ok)
		{
			std::string *body = new std::string(response.getBody());
			void *memory = (void *)body->c_str();
			IReadFile *f = device->getFileSystem()->createMemoryReadFile(memory, body->size(), textureData->fakename, false);
			ITexture *texture = driver->getTexture(f);
			return texture;
		}
		return NULL;
	}

	void DevProImageManager::LoadSleeve(int player, wchar_t* site, wchar_t* dir)
	{
		TextureData *sleeve = new TextureData;
		sleeve->type = SLEEVE;
		sleeve->player = player;
		std::wcstombs(sleeve->hostname, site, 256);
		std::wcstombs(sleeve->filename, dir, 256);
		if (player == 0)
			std::wcstombs(sleeve->fakename, L"cover0.jpg", 256);
		else
			std::wcstombs(sleeve->fakename, L"cover1.jpg", 256);
		pendingTextures.push_back(sleeve);
	}

	void DevProImageManager::LoadPendingTextures()
	{
		while (!pendingTextures.empty())
		{
			TextureData *textureData(pendingTextures.back());
			pendingTextures.pop_back();
			ITexture *texture = DownloadTexture(textureData);
			if (texture)
				ApplyTexture(textureData, texture);
			delete textureData;
		}
	}

	void DevProImageManager::ApplyTexture(TextureData *textureData, ITexture *texture)
	{
		switch (textureData->type)
		{
		case SLEEVE:
			if (textureData->player >= 0 && textureData->player < 2)
				imageManager.tCover[textureData->player] = texture;
			break;
		/*case AVATAR:
			if (textureData->player >= 0 && textureData->player < 4)
				tAvatar[textureData->player] = texture;
			break;
		*/
		}
	}
}