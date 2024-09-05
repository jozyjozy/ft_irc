#include "../../includes/server.hpp"

void server::changeNick(Client client,std::string cmd, int fd)
{
    std::string newnick = cmd.substr(5, cmd.find("\r") - 5);
    std::string msg;

    if (this->_map.getClientmapByNick(newnick) == -1)
    {

        msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost NICK " + newnick + "\r\n";
        this->_map.setnewnickfromfd(newnick, fd);
		std::map<std::string, channel*>map = this->_channelmap.getmap();
		std::map<std::string, channel*>::iterator it;
		it = map.begin();
		while (it != map.end())
		{
			channel*		map_tmp = *(&it->second);
			if (map_tmp->getfoundator().getNick() == client.getNick())
				map_tmp->setFoundator(newnick);
			if (map_tmp->getwhitelist().checkDuplicateByNick(client.getNick()))
			{
				int tmp = map_tmp->getwhitelist().getClientmapByNick(client.getNick());
				map_tmp->getwhitelist().setnewnickfromfd(newnick, tmp);
			}
			if (map_tmp->getuserin().checkDuplicateByNick(client.getNick()))
			{
				int tmp = map_tmp->getuserin().getClientmapByNick(client.getNick());
				map_tmp->getuserin().setnewnickfromfd(newnick, tmp);
			}
			if (map_tmp->getrandom().checkDuplicateByNick(client.getNick()))
			{
				int tmp = map_tmp->getrandom().getClientmapByNick(client.getNick());
				map_tmp->getrandom().setnewnickfromfd(newnick, tmp);
			}
			if (map_tmp->getoplist().checkDuplicateByNick(client.getNick()))
			{
				int tmp = map_tmp->getoplist().getClientmapByNick(client.getNick());
				map_tmp->getoplist().setnewnickfromfd(newnick, tmp);
			}
			it->second = map_tmp;
			it++;
		}
        send(fd, msg.c_str(), msg.size(), 0);
        std::cout << "\033[1;37;44m server: " << msg.erase(msg.size() - 2, 2) << "\033[0m" << std::endl;
    }
    else
    {
        msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost 433 " + newnick + " :Nickname is already in use\r\n";
        send(fd, msg.c_str(), msg.size(), 0);
        std::cout << "\033[1;37;44m server: " << msg.erase(msg.size() - 2, 2) << "\033[0m" << std::endl;
    }
}
