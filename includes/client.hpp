#ifndef Client_hpp
# define Client_hpp
# include "AllInclude.hpp"

class Client
{
    public:
    Client(int fd, std::string nick, std::string realName);
	Client& operator=(const Client &cpya);
	Client(const Client &cpy);
    Client();
    ~Client();
    int getFd();
    std::string getNick();
    void setNick(std::string newnick);
    std::string getRealName();
    int getFd() const;
    std::string getNick() const;
    std::string getRealName() const;
    class error_socket: public std::exception
	{
		virtual const char	*what() const throw();
	};
    protected:
	int			_fd;
	std::string	_nick;
	std::string	_realName;
};

#endif
