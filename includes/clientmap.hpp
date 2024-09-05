#ifndef Clientmap_hpp
# define Clientmap_hpp

#include "AllInclude.hpp"
#include "client.hpp"

class Client;

class Clientmap {
public:
	Clientmap();
	~Clientmap();
	Clientmap& operator=(const Clientmap &cpya);
	Clientmap(const Clientmap &cpy);
	std::map<int, Client>::iterator getIt() const ;
	std::map<int, Client> getMap() const ;
	std::map<int, Client> getMap();
	void addClientmap(Client client);
	void deleteClientmap(int fd);
	bool checkDuplicateByNick(std::string Nick);
	void closeFds();
	Client getClientmapByFd(int ClientFd);
	void setnewnickfromfd(std::string newnick, int fd);
	int	getClientmapByNick(std::string NickyMinaj);
	bool ClientmapExists(int ClientFd);

protected:
	std::map<int, Client>::iterator _iterator;
	std::map<int, Client> _map;
};

#endif
