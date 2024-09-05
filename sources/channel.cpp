#include "../includes/channel.hpp"

std::string channel::gettopic() const {return (this->_topic);}
Clientmap channel::getwhitelist() {return (this->_whitelist);}
Clientmap channel::getwhitelist() const {return (this->_whitelist);}
Clientmap channel::getuserin() const {return (this->_userin);}
Clientmap channel::getoplist() const {return (this->_oplist);}
Clientmap channel::getrandom() const{return(this->_random);}
Client channel::getfoundator() const{return(this->_foundator);}
bool channel::getwhiteliston_off() const {return (this->_whiteliston_off);}
std::string channel::getname() const{return(this->_name);}

bool channel::getinviteonly() const {return(this->_invite_only);}
bool channel::gettopiconly() const {return(this->_topic_only_op);}
bool channel::getpassword() const {return(this->_password);}
std::string channel::getpass() const {return(this->_pass);}
bool channel::getlimituser() const {return(this->_limit_user);}
int channel::getnblimituser() const {return(this->_nb_limit_user);}
int channel::getnbuser() const {return(this->_nb_user);}
channel::channel(){;}

channel& channel::operator=(const channel &cpya)
{
    if (this != &cpya)
    {
        this->_whitelist = cpya.getwhitelist();
        this->_userin = cpya.getuserin();
        this->_oplist = cpya.getoplist();
        this->_whiteliston_off = cpya.getwhiteliston_off();
        this->_topic = cpya.gettopic();
        this->_random = cpya.getrandom();
        this->_foundator = cpya.getfoundator();
        this->_name = cpya.getname();
        this->_invite_only = cpya.getinviteonly();
        this->_topic_only_op = cpya.gettopiconly();
        this->_password = cpya.getpassword();
        this->_pass = cpya.getpass();
        this->_limit_user = cpya.getlimituser();
        this->_nb_limit_user = cpya.getnblimituser();
        this->_nb_user = cpya.getnbuser();
    }
    return (*this);
}

void channel::leaveuser(Client leaver)
{
    this->_userin.deleteClientmap(leaver.getFd());
    this->_nb_user--;
}

channel::channel(const channel &cpy){*this = cpy;}

channel::~channel(){;}

channel::channel(std::string name, Client first)
{
    this->_name = name;
    this->_foundator = first;
    this->_userin.addClientmap(first);
    this->_whiteliston_off = 0;
    this->_topic = "";
    this->_invite_only = 0;
    this->_topic_only_op = 1;
    this->_password = 0;
    this->_limit_user = 0;
    this->_nb_limit_user = 0;
    this->_nb_user = 1;
    std::string     _pass;
}
int channel::check_op(std::string op)
{
    std::map<int, Client> map = this->_oplist.getMap();
    std::map<int, Client>::iterator it = map.begin();

    if (op == this->_foundator.getNick())
        return (1);
    while (it != map.end())
    {
        if (it->second.getNick() == op)
        {
            return (1);
        }
        it++;
    }
    return (0);
}

void channel::joinChannel(Client client)
{
    this->_userin.addClientmap(client);
    this->_random.addClientmap(client);
    this->_nb_user++;
}

void channel::kick(std::string client, std::string msg)
{
	Client clientToKick = this->getrandom().getClientmapByFd(this->getrandom().getClientmapByNick(client));
	this->_nb_user--;
	std::map<int, Client> map = this->_userin.getMap();
	std::map<int, Client>::iterator it = map.begin();
	while (it != map.end())
	{
		send(it->first, msg.c_str(), msg.size(), 0);
		it++;
	}
	this->_userin.deleteClientmap(clientToKick.getFd());
	return ;
}

void channel::addop(Client client)
{
    this->_oplist.addClientmap(client);
    this->_random.deleteClientmap(client.getFd());
}

void channel::remop(Client client)
{
    this->_random.addClientmap(client);
    this->_oplist.deleteClientmap(client.getFd());
}


void channel::setFoundator(std::string newNick)
{
    this->_foundator.setNick(newNick);
}

void    channel::whitelistonoff(Client op)
{
    if (check_op(op.getNick()))
    {
        if (this->_whiteliston_off)
            this->_whiteliston_off = 0;
        else
            this->_whiteliston_off = 1;
    }
}

void channel::addwhitelist(Client op, Client client)
{
    if(check_op(op.getNick()))
        this->_whitelist.addClientmap(client);
}

void channel::privmsg(Client sender, std::string msg)
{
    std::map<int, Client> map = this->_userin.getMap();
    std::map<int, Client>::iterator it = map.begin();

    while (it != map.end())
    {
        if (it->first != sender.getFd())
            send(it->first, msg.c_str(), msg.size(), 0);
        it++;
    }
}

void channel::sendtoall(std::string msg)
{
    std::map<int, Client> map = this->_userin.getMap();
    std::map<int, Client>::iterator it = map.begin();
    while (it != map.end())
    {
        send(it->first, msg.c_str(), msg.size(), 0);
        it++;
    }
}

int channel::Userisin(std::string target)
{
    std::map<int, Client> map = this->_userin.getMap();
    std::map<int, Client>::iterator it = map.begin();
    while (it != map.end())
    {
        if (it->second.getNick() == target)
            return (1);
        it++;
    }
    return (0);
}

void channel::addwhitelist(Client client)
{
    this->_whitelist.addClientmap(client);
}

int channel::check_whitelist(Client client)
{
    std::map<int, Client> map = this->_whitelist.getMap();
    std::map<int, Client>::iterator it = map.begin();
    while (it != map.end())
    {
        if (it->second.getRealName() == client.getRealName())
		{
		    return (0);
		}
        it++;
    }
    return (1);
}

void channel::setuserlimit(int nb)
{
    this->_limit_user = 1;
    this->_nb_limit_user = nb;
}

void channel::unsetuserlimit()
{
    this->_limit_user = 0;
}

void channel::setTopic(std::string const topic)
{
    this->gettopic() = topic;
}

void channel::setinviteonly(int i)
{
    this->_invite_only = i;
}

void channel::settopoiconlyop(int i)
{
    this->_topic_only_op = i;
}

void channel::setpassword(std::string pass)
{
    this->_password = 1;
    this->_pass = pass;
}

void channel::unsetpassword()
{
    this->_password = 0;
}

std::string channel::getAllUser()
{
    std::string final;

    final += "@" + this->_foundator.getNick();
    std::map<int, Client> op = this->_oplist.getMap();
    std::map<int, Client>::iterator itop = op.begin();
    std::map<int, Client> v = this->_random.getMap();
    std::map<int, Client>::iterator itv = v.begin();
    while (itop != op.end())
    {
        final += " @" + itop->second.getNick();
        itop++;
    }
    while (itv != v.end())
    {
        final += " " + itv->second.getNick();
        itv++;
    }
    return final;
}
