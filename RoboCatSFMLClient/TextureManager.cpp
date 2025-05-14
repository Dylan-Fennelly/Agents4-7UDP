#include "RoboCatClientPCH.hpp"

std::unique_ptr< TextureManager >		TextureManager::sInstance;

void TextureManager::StaticInit()
{
	sInstance.reset(new TextureManager());
}

TextureManager::TextureManager()
{
	CacheTexture("cat", "../Assets/Media/Textures/AgentOne.png");
	//CacheTexture("mouse", "../Assets/mouse.png");
	CacheTexture("yarn", "../Assets/Media/Textures/Bullet.png");

	//Menu Textures
	CacheTexture("button", "../Assets/Media/Textures/Buttons.png");
	CacheTexture("title", "../Assets/Media/Textures/TitleScreen2.png");

	//Background
	CacheTexture("background", "../Assets/Media/Textures/Background.png");
	GetTexture("background")->setRepeated(true);
	

	//Power Ups
	CacheTexture("health", "../Assets/Media/Textures/Health.png");
	CacheTexture("machinegun", "../Assets/Media/Textures/Machinegun.png");
	CacheTexture("invincibility", "../Assets/Media/Textures/Invincibility.png");

	//Player Sprites
	CacheTexture("AgentOne", "../Assets/Media/Textures/AgentOne.png");
	CacheTexture("AgentTwo", "../Assets/Media/Textures/AgentTwo.png");
	CacheTexture("AgentThree", "../Assets/Media/Textures/AgentThree.png");
	CacheTexture("AgentFour", "../Assets/Media/Textures/AgentFour.png");
	CacheTexture("AgentFive", "../Assets/Media/Textures/AgentFive.png");
	CacheTexture("AgentSix", "../Assets/Media/Textures/AgentSix.png");
	CacheTexture("AgentSeven", "../Assets/Media/Textures/AgentSeven.png");
}

TexturePtr	TextureManager::GetTexture(const string& inTextureName)
{
	return mNameToTextureMap[inTextureName];
}

bool TextureManager::CacheTexture(string inTextureName, const char* inFileName)
{
	TexturePtr newTexture(new sf::Texture());
	if (!newTexture->loadFromFile(inFileName))
	{
		return false;
	}

	mNameToTextureMap[inTextureName] = newTexture;

	return true;

}
