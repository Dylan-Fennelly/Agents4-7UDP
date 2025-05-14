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
	CacheTexture("agentOne", "../Assets/Media/Textures/AgentOne.png");
	CacheTexture("agentTwo", "../Assets/Media/Textures/AgentTwo.png");
	CacheTexture("agentThree", "../Assets/Media/Textures/AgentThree.png");
	CacheTexture("agentFour", "../Assets/Media/Textures/AgentFour.png");
	CacheTexture("agentFive", "../Assets/Media/Textures/AgentFive.png");
	CacheTexture("agentSix", "../Assets/Media/Textures/AgentSix.png");
	CacheTexture("agentSeven", "../Assets/Media/Textures/AgentSeven.png");
  
	CacheTexture("zombie", "../Assets/Zombie.png");
	CacheTexture("fastZombie", "../Assets/FastZombie.png");

	CacheTexture("invincible", "../Assets/Invincible.png");
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
