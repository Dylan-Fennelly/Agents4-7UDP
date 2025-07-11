/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

typedef shared_ptr< sf::Texture > TexturePtr;
typedef shared_ptr<sf::Font> FontPtr;

class SpriteComponent
{
public:

	SpriteComponent(GameObject* inGameObject);
	~SpriteComponent();


	void SetTexture(TexturePtr inTexture);
	void SetSize(float inWidth, float inHeight);
	virtual sf::Sprite& GetSprite();

protected:

	sf::Sprite m_sprite;

	//don't want circular reference...
	GameObject* mGameObject;
};

typedef shared_ptr< SpriteComponent >	SpriteComponentPtr;