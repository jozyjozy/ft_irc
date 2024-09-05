#include "../../includes/server.hpp"


void    server::mode_i(Client client, std::string channelname, std::string mode)
{
    std::string msg;

    if (mode.find("+") != std::string::npos)
    {
        this->_channelmap.getmap()[channelname]->setinviteonly(1);
        msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost MODE " + channelname + " +i\r\n";
    }
    else if (mode.find("-") != std::string::npos)
    {
        this->_channelmap.getmap()[channelname]->setinviteonly(0);
        msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost MODE " + channelname + " -i\r\n";
    }
    this->_channelmap.getchannelmap(channelname)->sendtoall(msg);
}

void    server::mode_T(Client client, std::string channelname, std::string mode)
{
    std::string msg;

    if (mode.find("+") != std::string::npos)
    {
        this->_channelmap.getmap()[channelname]->settopoiconlyop(1);
        msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost MODE " + channelname + " +t\r\n";
    }
    else if (mode.find("-") != std::string::npos)
    {
        this->_channelmap.getmap()[channelname]->settopoiconlyop(0);
        msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost MODE " + channelname + " -t\r\n";
    }
    this->_channelmap.getchannelmap(channelname)->sendtoall(msg);
}

void    server::mode_l(Client client, std::string channelname, std::string mode, std::string arg)
{
    std::string msg;

    if (mode.find("+") != std::string::npos)
    {
        this->_channelmap.getmap()[channelname]->setuserlimit(atoi(arg.c_str()));
        msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost MODE " + channelname + " +l\r\n";
    }
    else if (mode.find("-") != std::string::npos)
    {
        this->_channelmap.getmap()[channelname]->unsetuserlimit();
        msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost MODE " + channelname + " -l\r\n";
    }
    this->_channelmap.getchannelmap(channelname)->sendtoall(msg);
}

void server::mode_k(Client client, int fd, std::string channelname, std::string mode, std::string arg)
{
    std::string msg;

    if (!arg.size())
    {
        msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost code 461 MODE " + mode + " :Not enough parameters\r\n";
        send(fd, msg.c_str(), msg.size(), 0);
    }
    else if (mode.find("+") != std::string::npos)
    {
        this->_channelmap.getmap()[channelname]->setpassword(arg);
        msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost MODE " + channelname + " +k\r\n";
        this->_channelmap.getchannelmap(channelname)->sendtoall(msg);
    }
    else if (mode.find("-") != std::string::npos)
    {
        this->_channelmap.getmap()[channelname]->unsetpassword();
        msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost MODE " + channelname + " -k\r\n";
        this->_channelmap.getchannelmap(channelname)->sendtoall(msg);
    }
    
}

void server::mode_o(Client client, int fd, std::string channelname, std::string mode, std::string arg)
{
    std::string target = arg.substr(1, arg.find("\r") - 1);
    std::string msg;
    channel *chan = this->_channelmap.getchannelmap(channelname);

    if (chan->check_op(target) && mode.find("+") != std::string::npos)
    {
        msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost " + channelname + " " + target + " already operator\r\n";
        send(fd, msg.c_str(), msg.size(), 0);
    }
	else if (target == client.getNick())
	{
		msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost " + channelname + " :You can't mode -o yourself\r\n";
	}
    else if (!chan->Userisin(target))
    {
        msg = ":" + client.getNick() + " 441 " + client.getNick() + " " + target + " " + channelname + " :They aren't on that channel\r\n";
        send(fd, msg.c_str(), msg.size(), 0);
    }
    else
    {
        msg =":" + client.getNick() + "!" + client.getNick() + "@localhost " + " MODE " + channelname + " " + mode + " " + target + "\r\n";
        if (mode.find("+") != std::string::npos)
            chan->addop(this->_map.getClientmapByFd(this->_map.getClientmapByNick(target)));
        else if (mode.find("-") != std::string::npos)
            chan->remop(this->_map.getClientmapByFd(this->_map.getClientmapByNick(target)));
        chan->sendtoall(msg);
    }
}

void server::modechan(Client client,std::string cmd, int fd)
{
    std::string channelname;
    std::string mode;
    std::string arg;
    std::string msg;

    cmd.erase(0, cmd.find(" ") + 1);
	if (cmd.find("#") == std::string::npos)
		return ;
	if (cmd.find("#") > (cmd.find("+") || cmd.find("-")))
	{
		msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost : wrong format\r\n";
		std::cout << "Server : " << msg << std::endl;
		send(fd, msg.c_str(), msg.size(), 0);
		return ;
	}
    if (cmd.find("#") != std::string::npos)
    {
        channelname = cmd.substr(cmd.find("#"), cmd.find(" ", cmd.find("#")) - cmd.find("#"));
        cmd.erase(cmd.find("#"), cmd.find(" ", cmd.find("#")));
    }
    if (cmd.find("+") != std::string::npos)
    {
        mode = cmd.substr(cmd.find("+"), cmd.find(" ", cmd.find("+")));
        cmd.erase(cmd.find("+"), cmd.find(" ", cmd.find("+")));
    }
    else if (cmd.find("-") != std::string::npos)
    {
        mode = cmd.substr(cmd.find("-"), cmd.find(" ", cmd.find("-")));
        cmd.erase(cmd.find("-"), cmd.find(" ", cmd.find("-")));
    }
    arg = cmd;
    if (channelname.size())
    {
        if (this->_channelmap.check(channelname))
        {
            if (this->_channelmap.getmap()[channelname]->check_op(client.getNick()) || this->_channelmap.getmap()[channelname]->getfoundator().getNick() == client.getNick())
            {
                if (mode.find("i") != std::string::npos)
                    server::mode_i(client, channelname, mode);
                if (mode.find("t") != std::string::npos)
                    server::mode_T(client, channelname, mode);
                if (mode.find("k") != std::string::npos)
                    server::mode_k(client, fd, channelname, mode, arg);
                if (mode.find("o") != std::string::npos)
                    server::mode_o(client, fd, channelname, mode, arg);
                if (mode.find("l") != std::string::npos)
                    server::mode_l(client, channelname, mode, arg);
            }
            else
            {
                msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost 482 " + channelname + " :You're not channel operator\r\n";
                send(fd, msg.c_str(), msg.size(), 0);
            }
        }
        else
        {
            if (mode.size())
            {
                msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost 403 " + channelname + " :No such channel\r\n";
                send(fd, msg.c_str(), msg.size(), 0);
            }
        }
    }
    return ;
}
