#include "RoboCatClientPCH.hpp"

std::unique_ptr<sf::RenderWindow>	WindowManager::sInstance;

bool WindowManager::StaticInit()
{
	
	//sInstance.reset(new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Agents 4-7", sf::Style::Fullscreen));
	sInstance.reset(new sf::RenderWindow(sf::VideoMode(1280, 720), "Agents 4-7", sf::Style::Default));
	return true;
}


