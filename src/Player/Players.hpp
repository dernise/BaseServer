#ifndef _PLAYERS_H
#define _PLAYERS_H

#include <algorithm>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <set>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include "../Network/Messages.hpp"
#include "../Utils/Log.hpp"

class Client
{
public:
  virtual ~Client() {}
  virtual void deliver(const AuthMessage& msg) = 0;
};

typedef boost::shared_ptr<Client> clientPtr;

class ClientList
{
public:
  void join(clientPtr client)
  {
    connectedClients.insert(client);
  }

  void remove(clientPtr client)
  {
      connectedClients.erase(client);
      sLog.outString("A client left the game");
  }

  void sendToAll(const AuthMessage& msg)
  {
    std::for_each(connectedClients.begin(), connectedClients.end(),
        boost::bind(&Client::deliver, _1, boost::ref(msg)));
  }

private:
  std::set<clientPtr> connectedClients;
};

class PlayerList
{
public:
  void join(clientPtr client)
  {
    connectedPlayers.insert(client);
  }

  void remove(clientPtr client)
  {
      connectedPlayers.erase(client);
      sLog.outString("A player left the game");
  }

  void sendToAll(const AuthMessage& msg)
  {
    std::for_each(connectedPlayers.begin(), connectedPlayers.end(),
        boost::bind(&Client::deliver, _1, boost::ref(msg)));
  }

private:
  std::set<clientPtr> connectedPlayers;
};
#endif