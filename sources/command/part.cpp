#include "../../includes/server.hpp"

void server::partchan(Client client, std::string cmd, int fd)
{
	if (cmd.find("#") == std::string::npos)
		return ;
    std::string channelname = cmd.substr(cmd.find("#"), cmd.find("\r") - 5);
    std::string msg;
    if (!this->_channelmap.check(channelname))
    {
        msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost 403 " + channelname + " :No such channel\r\n";
        send(fd, msg.c_str(), msg.size(), 0);
    }
    else
    {
        if (!this->_channelmap.getchannelmap(channelname)->getuserin().getMap().count(fd))
		{
			msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost 442 " + channelname + " :You're not in channel\r\n";
			send(fd,msg.c_str(), msg.size(), 0);
			return ;
		}
        msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost PART " + channelname + "\r\n";
        this->_channelmap.getchannelmap(channelname)->sendtoall(msg);
        this->_channelmap.getchannelmap(channelname)->leaveuser(client);
        if (this->_channelmap.getchannelmap(channelname)->getnbuser() == 0)
        {
            this->_channelmap.deletechan(channelname);
        }
    }
    std::cout << "\033[1;37;44mserver: " << msg.substr(0, cmd.find("\r") - 1) << "\033[0m" << std::endl;
}
