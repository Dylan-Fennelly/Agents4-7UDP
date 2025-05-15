/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

class WindowManager
{

public:

	static bool StaticInit();
	static std::unique_ptr< sf::RenderWindow >	sInstance;
};

