#include "../../includes/server.hpp"

void server::invitechan(Client client, std::string cmd, int fd)
{
	if (cmd.find("#") == std::string::npos)
		return ;
    std::string namechannel = cmd.substr(cmd.find("#"), cmd.find("\n") - 1 - cmd.find("#"));
    std::string target = cmd.substr(7, cmd.find("#") - 8);
    int         targetfd = this->_map.getClientmapByNick(target);
    std::string msg;

    if (targetfd == -1)
    {
        msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost 401 " + target + " :No such nick/channel\r\n";
        send(fd, msg.c_str(), msg.size(),0);
    }
    else if (this->_channelmap.check(namechannel) == 0)
    {
        msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost 403 " + namechannel + " :No such channel\r\n";
        send(fd, msg.c_str(), msg.size(),0);
    }
    else
    {
        this->_channelmap.getchannelmap(namechannel)->addwhitelist(this->_map.getClientmapByFd(this->_map.getClientmapByNick(target)));
        msg = ":" + target + "!" + target + "@localhost INVITE " + namechannel + "\r\n";
        send(fd, msg.c_str(), msg.size(), 0);
        std::cout << "\033[1;37;44m server: " << msg.erase(msg.size() - 2, 2) << "\033[0m" << std::endl;
        msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost INVITE " + target + " " + namechannel + "\r\n";
        send(targetfd, msg.c_str(), msg.size(), 0);
        std::cout << "\033[1;37;44m server: " << msg.erase(msg.size() - 2, 2) << "\033[0m" << std::endl;
    }
}