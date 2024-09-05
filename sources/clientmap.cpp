#include "../includes/clientmap.hpp"

Clientmap::Clientmap() {;}

std::map<int, Client> Clientmap::getMap(){return (this->_map);}
void Clientmap::addClientmap(Client client){_map[client.getFd()] = client;}
bool Clientmap::ClientmapExists(int ClientFd){return (_map.find(ClientFd) != _map.end());}
std::map<int, Client>::iterator Clientmap::getIt() const {return (this->_iterator);}
std::map<int, Client> Clientmap::getMap() const {return (this->_map);}

Clientmap::Clientmap(const Clientmap &cpy)
{
	if (this != &cpy)
	{
		this->_map = cpy.getMap();
		this->_iterator = cpy.getIt();
	}
}

Clientmap::~Clientmap(){};

void Clientmap::closeFds()
{
	this->_iterator = this->_map.begin();
	while (this->_iterator != this->_map.end())
	{
		int fd = this->_iterator->first;
		try
		{
			close(fd);
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error closing socket for Client with fd " << fd << ": " << e.what() << std::endl;
		}
		this->_iterator++;
	}
}

bool    Clientmap::checkDuplicateByNick(std::string Nick)
{
    this->_iterator = this->_map.begin();
    while (this->_iterator != this->_map.end())
    {
        if (this->_iterator->second.getNick() == Nick)
            return (true);
        this->_iterator++;
    }
    return (false);
}

Clientmap& Clientmap::operator=(const Clientmap &cpya)
{
	if (this != &cpya)
	{
		this->_map = cpya.getMap();
		this->_iterator = cpya.getIt();
	}
	return (*this);
}


void Clientmap::deleteClientmap(int fd)
{	
	std::map<int, Client>::iterator it = this->_map.find(fd);
	Client client;
	_map.erase(it);
}
void Clientmap::setnewnickfromfd(std::string newnick, int fd)
{
	this->_map[fd].setNick(newnick);
}

int	Clientmap::getClientmapByNick(std::string NickyMinaj)
{
	this->_iterator = this->_map.begin();
	while (this->_iterator != this->_map.end())
	{
		if (this->_iterator->second.getNick() == NickyMinaj)
			return (this->_iterator->second.getFd());
		this->_iterator++;
	}
	return (-1);
}

Client Clientmap::getClientmapByFd(int ClientFd)
{
	this->_iterator = _map.find(ClientFd);
	return this->_iterator->second;
}


