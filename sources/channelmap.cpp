#include "../includes/channelmap.hpp"
#include "../includes/server.hpp"

channelmap::channelmap(){;}

int channelmap::quitserv2(Client client, std::string cmd)
{
	if (cmd.find("#") == std::string::npos)
		return 0;
    std::string channelname = cmd.substr(cmd.find("#"), cmd.find("\r") - 5);
    std::string msg;
    msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost PART " + channelname + "\r\n";
    this->getchannelmap(channelname)->leaveuser(client);
    this->getchannelmap(channelname)->sendtoall(msg);
    if (this->getchannelmap(channelname)->getnbuser() == 0)
    {
        this->deletechan(channelname);
        return (1);
    }
    std::cout << "\033[1;37;44mserver: " << msg.substr(0, cmd.find("\r") - 1) << "\033[0m" << std::endl;
    return (0);
}

std::map<std::string,channel*> channelmap::getmap() const {return this->_map;}

void channelmap::quitserv(Client client)
{
	std::map<std::string, channel*>::iterator it = this->_map.begin();
    std::map<std::string, channel*>::iterator end = this->_map.end();

	while (it != end)
	{
		if (this->_map[it->first]->Userisin(client.getNick()))
		{
			std::string cmd = "PART " + it->first + "\r\n";
			if (this->quitserv2(client,cmd))
            {
                if (this->_map.empty())
                    return ;
                it = this->_map.begin();
            }
		}
		it++;
	}
}

channelmap& channelmap::operator=(const channelmap &cpya)
{
    if (this != &cpya)
    {
        this->_map = cpya.getmap();
    }
    return (*this);
}

int channelmap::check_acces(std::string namechannel, Client client, std::string pass, int fd)
{
    channel *chan = this->_map[namechannel];

    if (chan->getinviteonly() && chan->check_whitelist(client))
    {
        std::string msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost 473 " + namechannel + " Cannot join channel (+i)\r\n"; 
        send(fd, msg.c_str(), msg.size(), 0);
        return (1);
    }
    if (chan->getpassword())
    {
        if (chan->getpass() != pass)
        {
            std::string msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost 475 " + namechannel + " :Cannot join channel (+k)\r\n";
            send(fd, msg.c_str(), msg.size(), 0);
            return (1);
        }
    }
    if (chan->getlimituser())
        if (chan->getnblimituser() == chan->getnbuser())
        {
            std::string msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost 471 " + namechannel + " :Cannot join channel (+l)\r\n";
            send(fd, msg.c_str(), msg.size(), 0);
            return (1);
        }
    return (0);
}

void channelmap::deletechan(std::string name)
{
    std::map<std::string, channel*>::iterator it = this->_map.find(name);
	delete(it->second);
    _map.erase(it);
}

void channelmap::addchannelmap(std::string channelname, Client client){_map[channelname] = new channel(channelname, client);}

void channelmap::joinchannelmap(std::string channelname, Client client){_map[channelname]->joinChannel(client);}

std::string channelmap::getAllUsermap(std::string channelname){return (_map[channelname]->getAllUser());}

void channelmap::privmsgmap(std::string channelname, std::string msg, Client client){_map[channelname]->privmsg(client, msg);}

channel *channelmap::getchannelmap(std::string name){return(this->_map[name]);}

channelmap::channelmap(const channelmap &cpy){*this = cpy;}

int channelmap::check(std::string name){return this->_map.count(name);}

channelmap::~channelmap()
{
	std::map<std::string, channel*>::iterator it = this->_map.begin();
	while (it != this->_map.end())
	{
		delete(it->second);
		this->_map.erase(it);
		it = this->_map.begin();
	}
}
