#include "RoboCatClientPCH.hpp"

std::unique_ptr< TextureManager >		TextureManager::sInstance;

void TextureManager::StaticInit()
{
	sInstance.reset(new TextureManager());
}

TextureManager::TextureManager()
{
	CacheTexture("cat", "../Assets/AgentOne.png");
	CacheTexture("mouse", "../Assets/mouse.png");
	CacheTexture("yarn", "../Assets/Bullet.png");

	CacheTexture("health", "../Assets/Health.png");
	CacheTexture("machinegun", "../Assets/Machinegun.png");
	CacheTexture("invincibility", "../Assets/Invincibility.png");

	CacheTexture("agentOne", "../Assets/AgentOne.png");
	CacheTexture("agentTwo", "../Assets/AgentTwo.png");
	CacheTexture("agentThree", "../Assets/AgentThree.png");
	CacheTexture("agentFour", "../Assets/AgentFour.png");
	CacheTexture("agentFive", "../Assets/AgentFive.png");
	CacheTexture("agentSix", "../Assets/AgentSix.png");
	CacheTexture("agentSeven", "../Assets/AgentSeven.png");

	CacheTexture("zombie", "../Assets/Zombie.png");

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
