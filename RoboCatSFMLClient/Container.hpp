/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#pragma once
namespace gui
{
	class Container  : public Component
	{
	public:
		using Ptr = std::shared_ptr<Container>;

	public:
		Container();
		void Pack(const Component::Ptr& component);
		bool IsSelectable() const override;
		void HandleEvent(const sf::Event& event) override;
		void Pull(const Component::Ptr& component);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		bool HasSelection() const;
		void Select(std::size_t index);
		void SelectNext();
		void SelectPrevious();

	private:
		std::vector<Component::Ptr> m_children;
		int m_selected_child;
	};
}
