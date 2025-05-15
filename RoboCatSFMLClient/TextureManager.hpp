/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

class TextureManager
{
public:
	static void StaticInit();

	static std::unique_ptr<TextureManager>		sInstance;

	TexturePtr	GetTexture(const string& inTextureName);

private:
	TextureManager();

	bool CacheTexture(string inName, const char* inFileName);

	unordered_map< string, TexturePtr >	mNameToTextureMap;
};

