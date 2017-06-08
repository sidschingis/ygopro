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
	irr::video::ITexture* DevProImageManager::GetTexture(int code) {
		if (code == 0)
			return ygo::imageManager.tUnknown;

		auto tit = ygo::imageManager.tMap.find(code);
		if (tit == ygo::imageManager.tMap.end()) {
			char file[256];
			sprintf(file, "expansions/pics/%d.png", code);
			irr::video::ITexture* img = driver->getTexture(file);
			if (img == NULL) {
				sprintf(file, "pics/%d.png", code);
				img = driver->getTexture(file);
			}
			if (img == NULL) {
				ygo::imageManager.tMap[code] = NULL;
				return GetTextureThumb(code);
			}
			else {
				ygo::imageManager.tMap[code] = img;
				return img;
			}
		}
		if (tit->second)
			return tit->second;
		else
			return GetTextureThumb(code);
	}
	irr::video::ITexture* DevProImageManager::GetTextureThumb(int code) {
		if (code == 0)
			return ygo::imageManager.tUnknown;
		auto tit = ygo::imageManager.tThumb.find(code);
		if (tit == ygo::imageManager.tThumb.end()) {
			char file[256];
			sprintf(file, "expansions/pics/thumbnail/%d.png", code);
			irr::video::ITexture* img = driver->getTexture(file);
			if (img == NULL) {
				sprintf(file, "pics/thumbnail/%d.png", code);
				img = driver->getTexture(file);
			}
			if (img == NULL) {
				ygo::imageManager.tThumb[code] = NULL;
				return ygo::imageManager.tUnknown;
			}
			else {
				ygo::imageManager.tThumb[code] = img;
				return img;
			}
		}
		if (tit->second)
			return tit->second;
		else
			return ygo::imageManager.tUnknown;
	}
}