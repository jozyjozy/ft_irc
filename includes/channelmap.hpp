#ifndef channelmap_hpp
# define channelmap_hpp

# include "AllInclude.hpp"
# include "channel.hpp"
# include "client.hpp"

class channelmap
{
private:
    std::map<std::string,channel*> _map;
public:
    channelmap();
    ~channelmap();
    int quitserv2(Client client, std::string cmd);
    channelmap& operator=(const channelmap &cpya);
	channelmap(const channelmap &cpy);
    void joinchannelmap(std::string channelname, Client client);
    std::string getAllUsermap(std::string channelname);
	void quitserv(Client client);
	void privmsgmap(std::string channelname, std::string msg, Client client);
    std::map<std::string,channel*> getmap() const;
    void addchannelmap(std::string channelname, Client client);
    int check_acces(std::string namechannel,Client client, std::string pass, int fd);
    channel *getchannelmap(std::string name);
    int check(std::string name);
    void deletechan(std::string name);
};


#endif
