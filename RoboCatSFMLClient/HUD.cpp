/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#include "RoboCatClientPCH.hpp"

std::unique_ptr< HUD >	HUD::sInstance;


HUD::HUD() :
	mScoreBoardOrigin(50.f, 60.f, 0.0f),
	mBandwidthOrigin(50.f, 10.f, 0.0f),
	mRoundTripTimeOrigin(580.f, 10.f, 0.0f),
	mScoreOffset(0.f, 50.f, 0.0f),
	mHealthOffset(1000, 10.f, 0.0f),
	mHealth(0)
{
}


void HUD::StaticInit()
{
	sInstance.reset(new HUD());
}

void HUD::Render()
{
	RenderBandWidth();
	RenderRoundTripTime();
	RenderScoreBoard();
	RenderHealth();
	RenderNameTags();
}

void HUD::RenderNameTags()
{
	//Getting all entries
	const auto& entries = ScoreBoardManager::sInstance->GetEntries();

	//Getting respective clients
	for (const auto& entry : entries)
	{
		for (const auto& goPtr : World::sInstance->GetGameObjects())
		{
			auto* cat = dynamic_cast<RoboCatClient*>(goPtr.get());
			if (!cat || cat->GetPlayerId() != entry.GetPlayerId())
				continue;

			//Getting sprites
			auto* spriteComp = cat->GetSpriteComponent();
			if (!spriteComp)
				break;

			//Getting sprite bounds
			const auto bounds = spriteComp->GetSprite().getGlobalBounds();

			//Setting up the text
			sf::Text nameText;
			nameText.setFont(*FontManager::sInstance->GetFont("carlito"));
			nameText.setString(entry.GetPlayerName());
			nameText.setCharacterSize(18);
			//Changing color to match the entry color
			const auto& c = entry.GetColor();
			nameText.setFillColor(
				sf::Color(uint8_t(c.mX),
					uint8_t(c.mY),
					uint8_t(c.mZ),
					255));

			//Positioning the text
			float x = bounds.left + bounds.width * 0.5f;
			float y = bounds.top + bounds.height + 4.f;
			const auto tb = nameText.getLocalBounds();
			nameText.setOrigin(tb.width * 0.5f, 0.f);
			nameText.setPosition(x, y);

			WindowManager::sInstance->draw(nameText);

			break;
		}
	}
}

void HUD::RenderHealth()
{
	if (mHealth > 0)
	{
		string healthString = StringUtils::Sprintf("Health %d", mHealth);
		RenderText(healthString, mHealthOffset, Colors::Red);
	}
}

void HUD::RenderBandWidth()
{
	string bandwidth = StringUtils::Sprintf("In %d  Bps, Out %d Bps",
		static_cast<int>(NetworkManagerClient::sInstance->GetBytesReceivedPerSecond().GetValue()),
		static_cast<int>(NetworkManagerClient::sInstance->GetBytesSentPerSecond().GetValue()));
	RenderText(bandwidth, mBandwidthOrigin, Colors::White);
}

void HUD::RenderRoundTripTime()
{
	float rttMS = NetworkManagerClient::sInstance->GetAvgRoundTripTime().GetValue() * 1000.f;

	string roundTripTime = StringUtils::Sprintf("RTT %d ms", (int)rttMS);
	RenderText(roundTripTime, mRoundTripTimeOrigin, Colors::White);
}

void HUD::RenderScoreBoard()
{
	const vector< ScoreBoardManager::Entry >& entries = ScoreBoardManager::sInstance->GetEntries();
	Vector3 offset = mScoreBoardOrigin;

	for (const auto& entry : entries)
	{
		RenderText(entry.GetFormattedNameScore(), offset, entry.GetColor());
		offset.mX += mScoreOffset.mX;
		offset.mY += mScoreOffset.mY;
	}

}

void HUD::RenderText(const string& inStr, const Vector3& origin, const Vector3& inColor)
{
	sf::Text text;
	text.setString(inStr);
	text.setFillColor(sf::Color(inColor.mX, inColor.mY, inColor.mZ, 255));
	text.setCharacterSize(50);
	text.setPosition(origin.mX, origin.mY);
	text.setFont(*FontManager::sInstance->GetFont("carlito"));
	WindowManager::sInstance->draw(text);
}

