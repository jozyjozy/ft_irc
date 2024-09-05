#include "../../includes/server.hpp"

void    server::joinchan(Client client, std::string cmd, int fd)
{
    std::string namechannel;
    std::string pass;

	if (cmd.find("#") == std::string::npos)
		return ;
	if (cmd.find(",") != std::string::npos)
	{
		std::string newmsg = "JOIN ";
		std::string newpass;
		if (cmd.find(" ", cmd.find("#")) != std::string::npos)
		{
				newpass = cmd.substr(cmd.find(" ", cmd.find("#")) + 1, cmd.size() - cmd.find(" ", cmd.find("#")));
				cmd.erase(cmd.find(" ",cmd.find("#")), cmd.size());
		}
		while (cmd.find(",") != std::string::npos) 
		{
			std::string tmp;
			std::string cmd2 = cmd.substr(cmd.find("#"), cmd.find(",") - cmd.find("#"));
			if (cmd2.size() == 1)
				return ;	
			if (newpass.find(",") != std::string::npos)
				tmp = newpass.substr(0, newpass.find(","));
			else if (newpass.size())
				tmp = newpass;
			server::joinchan(client, newmsg + cmd2 + " " + tmp + "\r\n",fd);
			if (newpass.find(",") != std::string::npos)
				newpass.erase(0, newpass.find(",") + 1);
			cmd.erase(0, cmd.find(",") + 1);
		}
		std::string tmp;
		if (newpass.size())
		{
			tmp = newpass;
		}
		cmd = cmd.substr(cmd.find("#"), cmd.find("\n" - 1 - cmd.find("#")));
		std::string cmd2 = newmsg + cmd + " " + tmp;
		server::joinchan(client, cmd2, fd);
		return ;
	}
    if (cmd.find(" ", cmd.find("#")) == std::string::npos)
        namechannel = cmd.substr(cmd.find("#"), cmd.find("\n") - 1 - cmd.find("#"));
    else
	    namechannel = cmd.substr(cmd.find("#"), cmd.find(" ", cmd.find("#")) - cmd.find("#"));
    cmd.erase(0, cmd.find(" ", cmd.find("#")));
    if (this->_channelmap.check(namechannel) != 0)
    {
        pass = cmd;
        if (!this->_channelmap.check_acces(namechannel,client, pass, fd))
            this->_channelmap.joinchannelmap(namechannel, client);
        else
        {
            std::string msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost PART " + namechannel + "\r\n";
            std::string final =":" + client.getNick() + " JOIN :" + namechannel + "\r\n";
            final += ":" + client.getNick() + " 353 " + client.getNick() + " = " + namechannel + " :" + this->_channelmap.getAllUsermap(namechannel) + "\r\n";
            final += ":" + client.getNick() + " 366 " + client.getNick() + " " +  namechannel + " :End of /NAMES list\r\n";
            send(client.getFd(), final.c_str(), final.size(), 0);
            send(fd,msg.c_str(), msg.size(), 0);
            return ;
        }
    }
    else
        this->_channelmap.addchannelmap(namechannel, client);
    std::string final =":" + client.getNick() + " JOIN :" + namechannel + "\r\n";
    std::string msg = ":" + client.getNick() + " PRIVMSG " + namechannel + " :has joined the channel " + namechannel + "\r\n";
	this->_channelmap.privmsgmap(namechannel, msg, client);
    if (this->_channelmap.getchannelmap(namechannel)->gettopic().size() != 0)
        final +=":" + client.getNick() + "!" + client.getNick() + "@localhost " + namechannel + " :" + this->_channelmap.getchannelmap(namechannel)->gettopic() + "\r\n";
    final += ":" + client.getNick() + " 353 " + client.getNick() + " = " + namechannel + " :" + this->_channelmap.getAllUsermap(namechannel) + "\r\n";
    final += ":" + client.getNick() + " 366 " + client.getNick() + " " +  namechannel + " :End of /NAMES list\r\n";
    send(client.getFd(), final.c_str(), final.size(), 0);
    std::cout << "\033[1;37;44m server: " << final.erase(final.size() - 2, 2) << "\033[0m" << std::endl;
}
