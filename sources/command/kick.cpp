#include "../../includes/server.hpp"

void    server::kickchan(Client client, std::string cmd, int fd)
{
    std::string namechannel;
    std::string target;
    std::string msg;

	if (cmd.find("#") == std::string::npos)
		return ;
    cmd.erase(0, cmd.find(" ") + 1);
	namechannel = cmd.substr(cmd.find("#"), cmd.find(" ", cmd.find("#")) - cmd.find("#"));
    cmd.erase(0, (cmd.find("#") + cmd.find(" ") + 1));
    target = cmd.substr(0, cmd.find(":") - 1);
    cmd.erase(0, (cmd.find(":")));
    if (!(this->_channelmap.check(namechannel)))
    {
            msg = ":" + client.getNick() + " 403 " + namechannel + ":No such channel\r\n";
            send(fd, msg.c_str(), msg.size(), 0);
            std::cout << "\033[1;37;44m server: " << msg.erase(msg.size() - 2, 2) << "\033[0m" << std::endl;
    }
    else if (this->_map.getClientmapByFd(fd).getNick() != this->_channelmap.getchannelmap(namechannel)->getfoundator().getNick()
    && !(this->_channelmap.getchannelmap(namechannel)->check_op(this->_map.getClientmapByFd(fd).getNick())))
    {
        msg = ":" + client.getNick() + " 482 " + client.getNick() + " " + namechannel + " :You're not channel operator'\r\n";
        send(fd, msg.c_str(), msg.size(), 0);
        std::cout << "\033[1;37;44m server: " << msg.erase(msg.size() - 2, 2) << "\033[0m" << std::endl;
    }
    else if (this->_channelmap.getchannelmap(namechannel)->getuserin().getClientmapByNick(target) == -1)
    {
        msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost 441 " + target + " " + namechannel + " :They aren't on that channel\r\n";
        send(fd, msg.c_str(), msg.size(), 0);
        std::cout << "\033[1;37;44m server: " << msg.erase(msg.size() - 2, 2) << "\033[0m" << std::endl;
    }
	else if (this->_channelmap.getchannelmap(namechannel)->check_op(target))
	{
		msg = ":" + client.getNick() + "!" + client.getNick() + " " + namechannel + " :You cannot kick operator\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
	}
    else
    {
        cmd.erase(0, cmd.find(":") + 1);
        msg = ":" + target + "!" + target + "@localhost PART " + namechannel + "\r\n";
        send(this->_map.getClientmapByNick(target), msg.c_str(), msg.size(), 0);
        msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost" +  " KICK " + namechannel + " " + target + " " + cmd;
		this->_channelmap.getchannelmap(namechannel)->kick(target, msg);
    }
}
