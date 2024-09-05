#include "../../includes/server.hpp"


void    server::topicchan(Client client, std::string cmd, int fd)
{
    std::string namechannel;
	if (cmd.find("#") == std::string::npos)
		return ;
    cmd.erase(0, cmd.find(" "));
    namechannel = cmd.substr(cmd.find("#"), cmd.find(" ", cmd.find("#")) - cmd.find("#"));
	if (cmd.find(":") == std::string::npos)
	{
		std::string msg = "Bad cmd format\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		std::cout << "server :" << msg << std::endl;
	}
    std::string topic = cmd.substr(cmd.find(":"), cmd.find("\r") - cmd.find(":"));
    if (this->_channelmap.check(namechannel))
    {
        if (this->_channelmap.getmap()[namechannel]->gettopiconly())
        {
            if (this->_channelmap.getmap()[namechannel]->check_op(client.getNick()))
            {
                 this->_channelmap.getchannelmap(namechannel)->setTopic(topic);
                std::string msg = ":" + client.getNick() + "!" + topic.substr(1, topic.size() - 1) +  " TOPIC " + namechannel + " " + topic + "\r\n";
                this->_channelmap.getchannelmap(namechannel)->sendtoall(msg);
                std::cout << "\033[1;37;44m server: " << msg.erase(msg.size() - 2, 2) << "\033[0m" <<  std::endl;
            }
            else
            {
                std::string msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost 482 " + namechannel + " :You're not channel operator\r\n";
                send(fd, msg.c_str(), msg.size(), 0); 
            }
        }
        else
        {
            this->_channelmap.getchannelmap(namechannel)->setTopic(topic);
            std::string msg = ":" + client.getNick() + "!" + topic.substr(1, topic.size() - 1) +  " TOPIC " + namechannel + " " + topic + "\r\n";
            this->_channelmap.getchannelmap(namechannel)->sendtoall(msg);
            std::cout << "\033[1;37;44m server: " << msg.erase(msg.size() - 2, 2) << "\033[0m" <<  std::endl;
        }
    }
}
