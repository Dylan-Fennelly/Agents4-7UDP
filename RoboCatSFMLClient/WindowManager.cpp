/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#include "RoboCatClientPCH.hpp"

std::unique_ptr<sf::RenderWindow>	WindowManager::sInstance;

bool WindowManager::StaticInit()
{
	
	//sInstance.reset(new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Agents 4-7", sf::Style::Fullscreen));
	sInstance.reset(new sf::RenderWindow(sf::VideoMode(1920, 1080), "Agents 4-7", sf::Style::Fullscreen));
	return true;
}


