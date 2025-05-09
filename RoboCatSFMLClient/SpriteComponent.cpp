#include "RoboCatClientPCH.hpp"


SpriteComponent::SpriteComponent(GameObject* inGameObject) :
	mGameObject(inGameObject)
{
	//and add yourself to the rendermanager...
	RenderManager::sInstance->AddComponent(this);
}

SpriteComponent::~SpriteComponent()
{
	//don't render me, I'm dead!
	RenderManager::sInstance->RemoveComponent(this);
}

void SpriteComponent::SetTexture(TexturePtr inTexture)
{
	auto tSize = inTexture->getSize();
	m_sprite.setTexture(*inTexture);
	m_sprite.setOrigin(tSize.x / 2, tSize.y / 2);
	m_sprite.setScale(sf::Vector2f(1.f * mGameObject->GetScale(), 1.f * mGameObject->GetScale()));
}

void SpriteComponent::SetSize(float inWidth, float inHeight)
{
	// get the sprite’s original pixel dimensions
	const auto bounds = m_sprite.getLocalBounds();
	if (bounds.width > 0 && bounds.height > 0)
	{
		// compute uniform scale in X and Y so neither axis is stretched
		float scaleX = inWidth / bounds.width;
		float scaleY = inHeight / bounds.height;
		float uniform = std::min(scaleX, scaleY);
		m_sprite.setScale(uniform, uniform);
	}
}

sf::Sprite& SpriteComponent::GetSprite()
{
	// Update the sprite based on the game object stuff.
	auto pos = mGameObject->GetLocation();
	auto rot = mGameObject->GetRotation();
	m_sprite.setPosition(pos.mX, pos.mY);
	m_sprite.setRotation(rot);

	return m_sprite;
}

