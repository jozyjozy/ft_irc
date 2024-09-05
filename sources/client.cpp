#include "../includes/client.hpp"

Client::Client(int fd, std::string nick, std::string realName) : _fd(fd), _nick(nick), _realName(realName) {}

Client& Client::operator=(const Client &cpya)
{
    if (this != &cpya)
    {
        this->_fd = cpya.getFd();
        this->_nick = cpya.getNick();
        this->_realName = cpya.getRealName();
    }
    return (*this);
}

void Client::setNick(std::string newnick)
{
    this->_nick = newnick;
}

Client::Client(const Client &cpy){*this = cpy;}

Client::Client() {}

Client::~Client() {}


int Client::getFd(){return (this->_fd);}

std::string Client::getNick(){return (this->_nick);}

std::string Client::getRealName(){return (this->_realName);}

int Client::getFd() const{return (this->_fd);}

std::string Client::getNick() const{return (this->_nick);}

std::string Client::getRealName() const{return (this->_realName);}

const char	*Client::error_socket::what() const throw(){return ("error socket");}

