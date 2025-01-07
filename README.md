# IRC Project

The IRC project is a basic implementation of an **Internet Relay Chat (IRC)** server or client, designed as part of the **École 42** curriculum. It allows users to connect to a server, join channels, send and receive messages in real-time, and interact with other users on the network.

## Introduction

IRC is a protocol for real-time text messaging. This project implements an IRC server or client (depending on your implementation) that allows users to interact through various channels. It focuses on networking, multi-threading, and handling various IRC-specific commands.
The project is intended to help understand how real-time communication systems work and how servers and clients interact over a network.

## Features

- **Server-Client Architecture**: Supports multiple clients connecting to a single server.
- **Channels**: Users can join different channels and chat in real-time.
- **Private Messages**: Users can send direct private messages to each other.
- **Nickname Management**: Users can set and change their nicknames.
- **User Authentication**: Basic user authentication for joining channels or performing actions.
- **Commands Support**: Implements IRC commands such as `JOIN`, `PART`, `PRIVMSG`, `NICK`, etc.
- **Multi-threading**: Each client connection is handled in a separate thread.

## Installation

### Prerequisites
- Ensure you have **C++11** or higher installed for compiling the code.
- A working **TCP/IP** connection for server-client communication.

### Steps to Install:

1. Clone the repository:
   ```bash
   git clone https://github.com/jozyjozy/ft_irc.git
   cd ft_irc
   make
