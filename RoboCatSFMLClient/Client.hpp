/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

class Client : public Engine
{
public:

	static bool StaticInit();

protected:

	Client();

	virtual void	DoFrame() override;
	virtual void	HandleEvent(sf::Event& p_event) override;
	virtual bool	PollEvent(sf::Event& p_event) override;


private:

};
