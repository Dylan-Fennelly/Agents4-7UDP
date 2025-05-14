#include "RoboCatClientPCH.hpp"

std::unique_ptr< TextureManager >		TextureManager::sInstance;

void TextureManager::StaticInit()
{
	sInstance.reset(new TextureManager());
}

TextureManager::TextureManager()
{
	//CacheTexture("cat", "../Assets/AgentOne.png");
	//CacheTexture("mouse", "../Assets/mouse.png");
	//CacheTexture("yarn", "../Assets/Bullet.png");

	//Menu Textures
	CacheTexture("button", "../Assets/Media/Textures/Buttons.png");
	CacheTexture("background", "../Assets/Media/Textures/Background.png");

	//Power Ups
	CacheTexture("health", "../Assets/Health.png");
	CacheTexture("machinegun", "../Assets/Machinegun.png");
	CacheTexture("invincibility", "../Assets/Invincibility.png");

	//Player Sprites
	CacheTexture("AgentOne", "../Assets/AgentOne.png");
	CacheTexture("AgentTwo", "../Assets/AgentTwo.png");
	CacheTexture("AgentThree", "../Assets/AgentThree.png");
	CacheTexture("AgentFour", "../Assets/AgentFour.png");
	CacheTexture("AgentFive", "../Assets/AgentFive.png");
	CacheTexture("AgentSix", "../Assets/AgentSix.png");
	CacheTexture("AgentSeven", "../Assets/AgentSeven.png");
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
