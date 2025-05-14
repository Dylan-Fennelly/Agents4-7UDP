#include "RoboCatClientPCH.hpp"

PlayerSpriteComponent::PlayerSpriteComponent(GameObject* inGameObject) :
	SpriteComponent(inGameObject)
{}

sf::Sprite& PlayerSpriteComponent::GetSprite()
{
    if (auto* cat = dynamic_cast<RoboCatClient*>(mGameObject))
    {
        LOG("Invincibility timer = %f", cat->GetInvincibilityTimer());
        // if invincible > 0, use the glowing texture…
        if (cat->GetInvincibilityTimer() > 0.f)
        {
            SetTexture(TextureManager::sInstance->GetTexture("invincible"));
        }
        else
        {
            // revert to the normal agent skin based on playerId
            static constexpr int kNumSkins = 7;
            static const char* sSkins[kNumSkins] = {
                "agentOne","agentTwo","agentThree",
                "agentFour","agentFive","agentSix","agentSeven"
            };
            auto id = cat->GetPlayerId();
            int slot = (int(id) - 1 + kNumSkins) % kNumSkins;
            SetTexture(TextureManager::sInstance->GetTexture(sSkins[slot]));
        }
    }

	// Update the sprite based on the game object stuff.
	auto pos = mGameObject->GetLocation();
	auto rot = mGameObject->GetRotation();
	m_sprite.setPosition(pos.mX, pos.mY);
	m_sprite.setRotation(rot);
	RoboCat* player = dynamic_cast<RoboCat*>(mGameObject);
	Vector3 playerColor = player->GetColor();
	m_sprite.setColor(sf::Color(playerColor.mX, playerColor.mY, playerColor.mZ, 255));

	return m_sprite;
}
