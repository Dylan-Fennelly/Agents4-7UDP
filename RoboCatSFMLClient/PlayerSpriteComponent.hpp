/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#pragma once
class PlayerSpriteComponent : public SpriteComponent
{
public:
	PlayerSpriteComponent(GameObject* inGameObject);
	virtual sf::Sprite& GetSprite() override;
};
typedef shared_ptr<PlayerSpriteComponent >	PlayerSpriteComponentPtr;

