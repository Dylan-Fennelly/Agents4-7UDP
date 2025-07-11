/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#include "RoboCatServerPCH.hpp"


#if _WIN32
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	if (Server::StaticInit())
	{
		return Server::s_instance->Run();
	}
	else
	{
		//error
		return 1;
	}

}
#else
const char** __argv;
int __argc;
int main(int argc, const char** argv)
{
	__argc = argc;
	__argv = argv;

	if (Server::StaticInit())
	{
		return Server::sInstance->Run();
	}
	else
	{
		//error
		return 1;
	}
}
#endif
