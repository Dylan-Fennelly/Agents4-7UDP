/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

//I take care of rendering things!

class RenderManager
{
public:

	static void StaticInit();
	static std::unique_ptr< RenderManager >	sInstance;

	void Render();
	void RenderComponents();

	//vert inefficient method of tracking scene graph...
	void AddComponent(SpriteComponent* inComponent);
	void RemoveComponent(SpriteComponent* inComponent);
	int	 GetComponentIndex(SpriteComponent* inComponent) const;
	sf::View GetView() const { return view; }
	sf::Sprite GetBackgroundSprite() { return mBackgroundSprite; }

private:

	RenderManager();

	//this can't be only place that holds on to component- it has to live inside a GameObject in the world
	vector< SpriteComponent* >		mComponents;

	sf::View view;
	sf::Texture mBackgroundTexture;
	sf::Sprite mBackgroundSprite;

};


