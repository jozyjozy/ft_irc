#include "../../includes/server.hpp"

void server::privmsg(Client client, std::string cmd)
{
    std::string namechannel;
    std::string msg;

    if (cmd.find("#") != std::string::npos && cmd.find("#") < cmd.find(":"))
	{
        namechannel = cmd.substr(cmd.find("#"), cmd.find(" ", cmd.find("#")) - cmd.find("#"));
        msg = ":" + client.getNick() + " " + cmd + "\r\n";
		if (this->_channelmap.check(namechannel) != 0)
            this->_channelmap.privmsgmap(namechannel, msg, client);
	}
    else
    {
        std::string user = cmd.substr(cmd.find(" ") + 1, cmd.find(":"));
        user = user.substr(0, user.find(" "));
        msg = ":" + client.getNick() + " " + cmd + "\r\n";
        std::cout << "server :" << msg << std::endl;
        if (this->_map.getClientmapByNick(user) != -1)
        {
            send(this->_map.getClientmapByNick(user), msg.c_str(), msg.size(), 0);
        }
    }
}