NAME			= ircserv
CXX			= c++
CXXFLAG			= -Wall -Wextra -Werror -std=c++98 -g
SRC_DIR		= sources/
SRC			= $(addprefix $(SRC_DIR), $(CPP_FILE))
CPP_FILE		= main.cpp\
			server.cpp\
			client.cpp\
			clientmap.cpp\
			channel.cpp\
			channelmap.cpp\
			command/join.cpp\
			command/kick.cpp\
			command/topic.cpp\
			command/privmsg.cpp\
			command/mode.cpp\
			command/part.cpp\
			command/invite.cpp\
			command/ChangeNick.cpp
INC_DIR		= includes/
INC			= $(addprefix $(INC_DIR), $(INC_FILE))
INC_FILE	= main.hpp\
			AllInclude.hpp\
			server.hpp\
			client.hpp\
			clientmap.hpp\
			channel.hpp\
			channelmap.hpp
RM			= rm -f
OBJS_PATH		= .objs/
OBJS_DIR		= $(addprefix $(OBJS_PATH), $(OBJS))
OBJS			= $(SRC:%.cpp=%.o)
DEPS_DIR 		= $(addprefix $(OBJS_PATH), $(SRC:%.cpp=%.d))


all: $(NAME)

$(OBJS_PATH)%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAG) -MMD -MP -c $< -o $@

$(OBJS_PATH):
	mkdir -p $(OBJS_PATH)

$(NAME): $(OBJS_PATH) $(OBJS_DIR)
		$(CXX) $(CXXFLAG) $(OBJS_DIR) -o $(NAME)

-include $(DEPS_DIR)

clean:
	rm -fr $(OBJS_PATH)

fclean:	clean
	$(RM) $(NAME)

re:	fclean all


.PHONY:	all clean re fclean

