#include "RoboCatClientPCH.hpp"

CredentialEntryState::CredentialEntryState(StateStack& stack, bool is_host)
	: State(stack)
	, m_is_host(is_host)
{
	// Set the background
	const auto texture_ptr = TextureManager::sInstance->GetTexture("title");
	m_background_sprite.setTexture(*texture_ptr);

	FontPtr fontPtr = FontManager::sInstance->GetFont("carlito");
	if (!fontPtr)
	{
		std::cerr << "Failed to load font 'carlito'. Check the file path and name.\n";
		std::terminate();
	}

	TextBox ipTextbox(*fontPtr, sf::Vector2f(100, 100), sf::Vector2f(200, 50), 20, 20);
	std::ifstream input_file("ip.txt");
	std::string ip_address;
	if (input_file >> ip_address)
	{
		ipTextbox.setText(ip_address);
	}
	else
	{
		std::ofstream create_file("ip.txt");
		create_file << "";
	}

	// Label for IP (clients only)
	if (!m_is_host)
	{
		auto ipLabel = std::make_shared<gui::Label>("IP Address:",26,sf::Color::Black);
		ipLabel->setPosition(100, 70);
		ipLabel->SetText("IP Address:");
		m_gui_container.Pack(ipLabel);
	}

	TextBox userNameTextbox(*fontPtr, sf::Vector2f(100, 200), sf::Vector2f(200, 50), 20, 20);
	std::ifstream input_file2("username.txt");
	std::string username;
	if (input_file2 >> username)
	{
		userNameTextbox.setText(username);
	}
	else
	{
		std::ofstream create_file("username.txt");
		create_file << "";
	}

	auto usernameLabel = std::make_shared<gui::Label>("Username:",26,sf::Color::Black);
	usernameLabel->setPosition(100, 170);
	usernameLabel->SetText("Username:");
	m_gui_container.Pack(usernameLabel);

	TextBox portTextbox(*fontPtr, sf::Vector2f(100, 300), sf::Vector2f(200, 50), 20, 20);
	std::ifstream input_file3("port.txt");
	std::string port;
	if (input_file3 >> port)
	{
		portTextbox.setText(port);
	}
	else
	{
		std::ofstream create_file("port.txt");
		create_file << "";
	}

	auto portLabel = std::make_shared<gui::Label>("Port:",26,sf::Color::Black);
	portLabel->setPosition(100, 270);
	portLabel->SetText("Port:");
	m_gui_container.Pack(portLabel);

	// Add the text boxes to the vector
	if (!m_is_host)
	{
		m_text_boxes.push_back(ipTextbox); // index 0
	}
	m_text_boxes.push_back(userNameTextbox); // index 0 (host) or 1 (client)
	m_text_boxes.push_back(portTextbox);     // index 1 (host) or 2 (client)

	auto confirm_button = std::make_shared<gui::Button>();
	confirm_button->setPosition(100, 400);
	confirm_button->SetText(m_is_host ? "Host Game" : "Join Game");

	confirm_button->SetCallback([this]()
		{
			if (!m_is_host)
			{
				std::ofstream output_file("ip.txt");
				std::string local_address = m_text_boxes[0].getText(); // IP
				output_file << local_address;
			}

			std::ofstream output_file2("username.txt");
			std::string username = m_text_boxes[m_text_boxes.size() - 2].getText(); // Username
			output_file2 << username;

			std::ofstream output_file3("port.txt");
			std::string port = m_text_boxes[m_text_boxes.size() - 1].getText(); // Port
			output_file3 << port;

			RequestStackPop();

			if (m_is_host)
			{
				STARTUPINFOA si = { sizeof(si) };
				PROCESS_INFORMATION pi;
				std::string commandLineStr = "\"D:\\Multiplayer\\ca3\\Agents4-7UDP\\x64\\Debug\\RoboCatSFMLServer.exe\" " + port;
				char commandLine[MAX_PATH * 2];
				strcpy_s(commandLine, commandLineStr.c_str());

				BOOL result = CreateProcessA(
					NULL,
					commandLine,
					NULL,
					NULL,
					FALSE,
					0,
					NULL,
					NULL,
					&si,
					&pi
				);

				if (result)
				{
					CloseHandle(pi.hProcess);
					CloseHandle(pi.hThread);
				}
				else
				{
					MessageBoxA(NULL, "Failed to launch server.exe", "Error", MB_OK | MB_ICONERROR);
				}
			}

			RequestStackPush(StateID::kGame);
		});

	auto back_button = std::make_shared<gui::Button>();
	back_button->setPosition(100, 450);
	back_button->SetText("Back");
	back_button->SetCallback([this]()
		{
			RequestStackPop();
			RequestStackPush(StateID::kMenu);
		});

	m_gui_container.Pack(confirm_button);
	m_gui_container.Pack(back_button);
	for (auto& textBox : m_text_boxes)
	{
		textBox.handleEvent(sf::Event());
	}
}

void CredentialEntryState::Draw()
{
	WindowManager::sInstance->setView(WindowManager::sInstance->getView());
	WindowManager::sInstance->draw(m_background_sprite);
	WindowManager::sInstance->draw(m_gui_container);

	for (int i = 0; i < m_text_boxes.size(); i++)
	{
		if (i == m_text_box_Selected)
		{
			m_text_boxes[i].draw(true);
		}
		else
		{
			m_text_boxes[i].draw(false);
		}
	}
}

bool CredentialEntryState::Update(float dt)
{
	if (m_text_box_Selected != -1)
	{
		m_text_boxes[m_text_box_Selected].update();
	}
	return true;
}

bool CredentialEntryState::HandleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Key::Escape)
		{
			m_text_box_Selected = -1;
		}
		else if (event.key.code == sf::Keyboard::Key::Tab)
		{
			if (m_text_box_Selected == -1)
			{
				m_text_box_Selected = 0;
			}
			else
			{
				m_text_box_Selected++;
				if (m_text_box_Selected >= m_text_boxes.size())
				{
					m_text_box_Selected = 0;
				}
			}
		}
	}

	if (event.type == sf::Event::MouseButtonPressed)
	{
		sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
		m_text_box_Selected = -1;
		for (int i = 0; i < m_text_boxes.size(); i++)
		{
			if (m_text_boxes[i].getBox().getGlobalBounds().contains(mousePos))
			{
				m_text_box_Selected = i;
				std::cout << "Selected: " << i << std::endl;
				break;
			}
		}
	}

	if (m_text_box_Selected == -1)
	{
		m_gui_container.HandleEvent(event);
	}
	else
	{
		m_text_boxes[m_text_box_Selected].handleEvent(event);
	}

	return true;
}
