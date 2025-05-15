#pragma once
namespace gui
{
	class Label : public Component
	{
	public:
		using PTR = std::shared_ptr<Label>;
	public:
		Label(const std::string& text,int fontSize, sf::Color fontColour);
		bool IsSelectable() const override;
		void SetText(const std::string& text);
		void HandleEvent(const sf::Event& event) override;
		sf::Text GetText() const { return m_text; }

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		sf::Text m_text;

	};
}