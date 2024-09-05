#include "../includes/server.hpp"

server::server(){this->_port = 6667; this->_password = "";}


int server::getport() const {return(this->_port);}
std::string server::getpass() const {return(this->_password);}
int server::getserversocket() const {return(this->_socketfd);}
struct sockaddr_in server::getaddress() const {return(this->_address);}
channelmap server::getchannelmapcpy() const {return(this->_channelmap);}
int server::getfdscpy() const {return(this->_fds);}
Clientmap server::getmapcpy() const {return(this->_map);}

server& server::operator=(const server &cpya)
{
    if (this != &cpya)
    {
       this->_port = cpya.getport();
       this->_password = cpya.getpass();
       this->_socketfd = cpya.getserversocket();
       this->_address = cpya.getaddress();
       this->_channelmap = cpya.getchannelmapcpy();
       this->_fds = cpya.getfdscpy();
       this->_enable = 1;
       this->_map = cpya.getmapcpy();
    }
    return (*this);
}

server::server(const server &cpy){*this = cpy;}

server::~server()
{
    if (this->_socketfd > 0)
        close(this->_socketfd);
}

server::server(int port, std::string password)
{
    this->_password = password;
    this->_port = port;
}
std::string server::getpass(){return(this->_password);}
int server::getport(){return(this->_port);}
int     server::getserversocket(){return (this->_socketfd);}
int server::getmaxfds(){return(this->_fds);}
const char	*server::error_socket::what() const throw(){return ("error socket");}

fd_set    server::get_fds(fd_set  rfds)
{
    std::map<int, std::string> tmp = this->creaClient;
    std::map<int, std::string>::iterator it = tmp.begin();


    int i = 1;
    FD_SET(this->_socketfd, &rfds);
    if (tmp.size() > 1)
    {
		while(it != tmp.end())
        {
            FD_SET(it->first, &rfds);
            it++;
            i++;
        }
    }
    else if (tmp.size() == 1)
    {
        FD_SET(it->first, &rfds);
        i++;
    }
    if (i == 1)
        this->_fds = this->_socketfd;
    else
    {
        it--;
        this->_fds = it->first;
    }
    return (rfds);
}

bool server::serv_add_client(std::string s, int newsocket)
{
	if (s.find("NICK") == std::string::npos || s.find("PASS") == std::string::npos || s.find("USER") == std::string::npos || s.find("CAP LS") == std::string::npos)
		return (false);
	std::string delimiter = "\n";
	std::string pass, Nick, user, FullName, final, token;
	if (s.find("CAP LS") != std::string::npos)
	{
		std::string tmp = s.substr(0, s.find(delimiter) + 1);
		int pos = s.find(delimiter); 
		s.erase(0, pos + delimiter.length());
	}
	if (s.find("PASS") != std::string::npos)
	{
		pass = s.substr(0, s.find(delimiter) - 1);
		int pos = s.find(delimiter); 
		s.erase(0, pos + delimiter.length());
	}
	if (s.find("NICK") != std::string::npos)
	{
		token = s.substr(0, s.find(delimiter) + 1);
		Nick = token.substr(5, s.find("\r") - 5);
		if (this->_map.checkDuplicateByNick(Nick))
		{
			final = "*!@localhost 433 " + Nick + " :Nickname is already in use\r\n";
			send(newsocket, final.c_str(), final.size(), 0);
			this->creaClient.erase(newsocket);
			close(newsocket);
			return(false);
		}
		int pos = s.find(delimiter);
		s.erase(0, pos + delimiter.length());
	}
	if (s.find("USER") != std::string::npos)
	{
		user = s.substr(0, s.find(delimiter) + 1);
		if (s.find(":") != std::string::npos)
			FullName = s.substr(s.find(":"), s.find("\r") - s.find(":"));
		else
			send(newsocket, "Bad USER\r\n", 11, 0);
	}
	if (Nick.size() && user.size())
		final = ":*!@localhost " + token;
	send(newsocket, final.c_str(), final.size(), 0);
	if (final.size() > 3)
		std::cout << "\033[1;37;44mserver: " << final.erase(final.size() - 2, 2) << "\033[0m" << std::endl;
	pass = pass.substr(pass.find(" ") + 1, pass.find("\n") - 1);
	if (pass != this->_password)
	{
		final = "*!@localhost 464 " + Nick + " :Password incorrect\r\n";
		send(newsocket, final.c_str(), final.size(), 0);
		this->creaClient.erase(newsocket);
		close(newsocket);
		return(false);
	}
	if (this->_map.getClientmapByNick("Bot") == -1)
	{
		Client bot(666, "Bot", "botPong");
		this->_map.addClientmap(bot); 
	}
	Client tmp(newsocket, Nick, FullName);
	final = ":localhost 001 " + Nick + " :Welcome to the Internet Relay Network : " + Nick + " !\r\n";
	send(newsocket, final.c_str(), final.size(), 0);
	std::cout << "\033[1;37;44mserver: "  << final.erase(final.size() - 2, 2) << "\033[0m" << std::endl;
	this->_map.addClientmap(tmp);
	this->creaClient[newsocket].clear();
	return (true);
}

int server::get_who_talk(fd_set rfds)
{
    std::map<int, std::string> map = this->creaClient;
    std::map<int, std::string>::iterator it = map.begin();

    if (FD_ISSET(this->_socketfd, &rfds))
        return (this->_socketfd);
    while (it != map.end())
    {
        if(FD_ISSET(it->first, &rfds))
            return (it->first);
        it++;
    }
    return (-1);
}

int server::listen_serv(int fd)
{
    int newsocket;
    char buffer[1024] = { 0 };
    std::string s;

    if (fd > 3)
    {
		recv(fd, buffer, 1024 - 1, 0);
		s += buffer;
		this->creaClient[fd] += s;
		if (s.size() == 0)
		{
			if (this->_map.ClientmapExists(fd))
				this->_map.deleteClientmap(fd);
			this->creaClient.erase(fd);
			close(fd);
			return (0);
		}
		if (s.find("\r\n") == std::string::npos)
		{
			return (0);
		}
		if (!this->_map.ClientmapExists(fd))
		{
			std::cout << "client : " << s << std::endl;
			serv_add_client(this->creaClient[fd], fd);
			return (0);
		}
		std::cout << "client : " << this->creaClient[fd] << std::endl;
		if (this->creaClient[fd].size())
			this->interpretCmd(this->creaClient[fd], fd);
		if (this->_map.ClientmapExists(fd))
			this->creaClient[fd].clear();
		return (0);
    }
    if (listen(fd, 10) < 0)
        return (1);
    socklen_t addrlen = sizeof(this->_address);
    newsocket = accept(fd, (struct sockaddr*)&this->_address, &addrlen);
    recv(newsocket, buffer, 1024 - 1, 0);
	std::cout<< "client :" << buffer<< std::endl;
    s = buffer;
	this->creaClient[newsocket] = s;
    serv_add_client(s, newsocket);
    return (0);
}

void server::interpretCmd(std::string cmd, int fd)
{
    std::map<int, Client> map = this->_map.getMap();
    Client client = map[fd];
	if (cmd.empty())
		return ;
	if (cmd.length() > 6 && (cmd.substr(0, 5) == "JOIN "))
		server::joinchan(client, cmd, fd);
	if (cmd.length() > 6 && (cmd.substr(0, 5) == "KICK "))
		server::kickchan(client, cmd, fd);
	if (cmd.length() > 9 && (cmd.substr(0, 8) == "PRIVMSG "))
        server::privmsg(client, cmd);
	if (cmd.length() > 9 && (cmd.substr(0, 8) == "PRIVMSG ") && (cmd.substr(cmd.find(":") + 1, 10) == "pong\r\n"))
		server::privmsg(this->_map.getClientmapByFd(666), cmd.substr(0, cmd.find(":")) + "Ping for " + client.getNick() + "\r\n");
    if (cmd.length() > 9 && (cmd.substr(0, 8) == "PRIVMSG ") && (cmd.substr(cmd.find(":") + 1, 10) == "ping\r\n"))
		server::privmsg(this->_map.getClientmapByFd(666), cmd.substr(0, cmd.find(":")) + "Pong for " + client.getNick() + "\r\n");
	if (cmd.length() > 6 && (cmd.substr(0, 5) == "MODE "))
		server::modechan(client, cmd, fd);
	if (cmd.length() > 6 && (cmd == "QUIT :leaving\r\n"
		|| cmd.substr(0, 5) == "EXIT "))
    {
		this->creaClient.erase(fd);
		this->_channelmap.quitserv(client);
        this->_map.deleteClientmap(fd);
        close(fd);
    }
	if (cmd.length() > 8 && (cmd.substr(0, 7) == "NOTICE "))
		server::privmsg(client, cmd);
    if (cmd.length() > 7 && (cmd.substr(0, 6)== "TOPIC "))
		server::topicchan(client, cmd, fd);
    if (cmd.length() > 6 && (cmd.substr(0, 5) == "PART "))
        server::partchan(client, cmd, fd);
    if (cmd.length() > 6 && (cmd.substr(0, 5) == "PING "))
    {
        std::string msg = ":" + client.getNick() + "!" + client.getNick() + "@localhost PONG\r\n";
        send(fd, msg.c_str(), msg.size(), 0);
    }
    if (cmd.length() > 8 && (cmd.substr(0, 7) == "INVITE "))
        server::invitechan(client, cmd, fd);
    if (cmd.length() > 6 && (cmd.substr(0, 5) == "NICK "))
        server::changeNick(client, cmd, fd);
}

void server::getsocket()
{
    this->_socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_socketfd < 0)
        throw error_socket();
    this->_enable = 1;
    if (setsockopt(this->_socketfd, SOL_SOCKET, SO_REUSEADDR, &this->_enable, sizeof(int)) == -1)
    {
        close(this->_socketfd);
        return ;
    }
    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = INADDR_ANY;
    this->_address.sin_port = htons(this->_port);
    if (bind(this->_socketfd, (struct sockaddr*)&this->_address, sizeof(this->_address)) < 0)
        throw error_socket();
    listen(this->_socketfd, 10);
}
