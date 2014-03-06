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


class Player
{
public:
  virtual ~Player() {}
  virtual void deliver(const WebSocketMessage& msg) = 0;
};

typedef boost::shared_ptr<Player> playerPtr;

class PlayerList
{
public:
  void join(playerPtr player)
  {
    connectedPlayers.insert(player);
  }

  void remove(playerPtr player)
  {
      connectedPlayers.erase(player);
      sLog.outString("A player left the game");
  }

  void sendToAll(const WebSocketMessage& msg)
  {
    std::for_each(connectedPlayers.begin(), connectedPlayers.end(),
        boost::bind(&Player::deliver, _1, boost::ref(msg)));
  }

private:
  std::set<playerPtr> connectedPlayers;
};
#endif