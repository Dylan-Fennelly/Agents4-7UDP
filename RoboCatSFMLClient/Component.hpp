/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#pragma once
namespace gui
{
	class Component : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
	{
	public:
		typedef std::shared_ptr<Component> Ptr;

	public:
		Component();
		virtual ~Component();
		virtual bool IsSelectable() const = 0;
		bool IsSelected() const;
		virtual void Select();
		virtual void Deselect();
		virtual bool IsActive() const;
		virtual void Activate();
		virtual void Deactivate();

		virtual void HandleEvent(const sf::Event& event) = 0;

	private:
		bool m_is_selected;
		bool m_is_active;
	};
}