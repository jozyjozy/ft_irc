#ifndef server_hpp
# define server_hpp

#include "AllInclude.hpp"
#include "clientmap.hpp"
#include "client.hpp"
#include "channel.hpp"
#include "channelmap.hpp"

class Clientmap;

class server
{
    public:
    server();
	server& operator=(const server &cpya);
	server(const server &cpy);
    ~server();
    server(int ip, std::string password);
    std::string getpass();
    int getport();
    void getsocket();
    int listen_serv(int fd);
    int get_who_talk(fd_set rfds);
    int getmaxfds();
    int getserversocket();
	void interpretCmd(std::string cmd, int fd);
    fd_set get_fds(fd_set  rfds);
    bool serv_add_client(std::string s, int newsocket);
    int getport() const;
    std::string getpass() const;
    int getserversocket() const;
    void notice(Client client, std::string cmd);
    struct sockaddr_in getaddress() const;
    channelmap getchannelmapcpy() const;
    int getfdscpy() const;
    Clientmap getmapcpy() const;

    void    msg_join(Client client, std::string namechannel);
    void    joinchan(Client client, std::string cmd, int fd);
    void    kickchan(Client client, std::string cmd, int fd);
    void    topicchan(Client client, std::string cmd, int fd);
    void    privmsg(Client client, std::string cmd);
    void    partchan(Client client, std::string cmd, int fd);
    void    modechan(Client client, std::string cmd, int fd);
    void    mode_i(Client client, std::string channelname, std::string mode);
    void    mode_l(Client client, std::string channelname, std::string mode, std::string arg);
    void    mode_k(Client client, int fd, std::string channelname, std::string mode, std::string arg);
    void    mode_T(Client client, std::string channelname, std::string mode);
    void    mode_o(Client client, int fd, std::string channelname, std::string mode, std::string arg);
    void    invitechan(Client client, std::string cmd, int fd);
    void    changeNick(Client client,std::string cmd, int fd);
    class error_socket: public std::exception
	{
		virtual const char	*what() const throw();
	};
    private:
    int                 _fds;
    int                 _enable;
    int                 _port;
    int                 _socketfd;
    Clientmap           _map;
	channelmap          _channelmap;
	std::map<int,std::string> creaClient;
	std::string         _password;
    struct sockaddr_in  _address;
};

#endif
