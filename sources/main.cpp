#include "../includes/main.hpp"

int g_end = 1;

void	sig_int(__attribute__((unused)) int code)
{
	g_end = 0;
}

void	signal_controller(void)
{
	signal(SIGINT, sig_int);
}


int main(int ac, char **argv)
{
	signal_controller();
    if (ac != 3)
    {
        std::cout << "./ircserv {Port} {Password}" << std::endl;
        return 1;
    }
    std::string s = argv[1];
    if (atol(argv[1]) > INT_MAX || atol(argv[1]) <= 0 || s.size() > 10)
    {
        std::cout << "Invalid Port" << std::endl;
        return 1;
    }
    server irc(atoi(argv[1]), argv[2]);
    try
    {
        irc.getsocket();
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return (0);
    }
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    while (g_end)
    {
        int recVal;
        fd_set rfds;
        FD_ZERO(&rfds);
        rfds = irc.get_fds(rfds);
        recVal = select(irc.getmaxfds() + 1, &rfds, NULL, NULL, &tv);
		switch (recVal)
        {
            case (0):
                break;
            case (-1):
                break;
            default:
            {
                while (irc.get_who_talk(rfds) != -1)
                {
                    int fd = irc.get_who_talk(rfds);
					irc.listen_serv(fd);
                    FD_CLR(fd, &rfds);
                }
                break;
            }
        }

    }
	irc.getmapcpy().closeFds();
    return 0;
}
