#ifndef channel_HPP
# define channel_HPP
# include "clientmap.hpp"
# include "client.hpp"

class channel
{
private:
    Clientmap       _whitelist;
    Clientmap       _userin;
    Clientmap       _random;
    Clientmap       _oplist;
    Client          _foundator;
    bool            _whiteliston_off;
    bool            _invite_only;
    bool            _topic_only_op;
    bool            _password;
    bool            _limit_user;
    int             _nb_limit_user;
    std::string     _pass;
    std::string     _topic;
    std::string     _name;
    int             _nb_user;
public:
    channel();
    channel& operator=(const channel &cpya);
	channel(const channel &cpy);
    ~channel();
    channel(std::string name, Client first);
    void leaveuser(Client leaver);
    int  check_whitelist(Client client);
    void addwhitelist(Client client);
    std::string gettopic() const;
    Clientmap getwhitelist() ;
    Clientmap getuserin() const;
    Client getfoundator() const;
    Clientmap getrandom() const;
    Clientmap getwhitelist() const;
    Clientmap getoplist() const;
	void setFoundator(std::string newNick);
    void setpassword(std::string pass);
    void unsetpassword();
    void setuserlimit(int nb);
    void unsetuserlimit();
    void settopoiconlyop(int i);
    bool getwhiteliston_off() const;
    std::string getname() const;
    int Userisin(std::string target);
    bool getinviteonly() const;
    bool gettopiconly() const;
    bool getpassword() const;
    int getnbuser() const;
    void setinviteonly(int i);
    std::string getpass() const;
    bool getlimituser() const;
    int getnblimituser() const;
    void sendtoall(std::string msg);
    void setTopic(std::string const topic);
    int check_op(std::string op);
    std::string getAllUser();
    void privmsg(Client sender, std::string msg);
    void joinChannel(Client client);
    void kick(std::string client, std::string msg);
    void addop(Client client);
    void remop(Client client);
    void whitelistonoff(Client op);
    void addwhitelist(Client op, Client client);
};



#endif