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
#include "Messages.hpp"
#include "Log.hpp"

typedef std::deque<AuthPacket> auth_message_queue;

class Player
{
public:
  virtual ~Player() {}
  virtual void deliver(const AuthPacket& msg) = 0;
};

typedef boost::shared_ptr<Player> playerPtr;

class Players
{
public:
  void join(playerPtr player)
  {
    connectedPlayers.insert(player);
    std::for_each(recent_msgs_.begin(), recent_msgs_.end(),
        boost::bind(&Player::deliver, player, _1));
  }

  void remove(playerPtr player)
  {
      connectedPlayers.erase(player);
      sLog.outString("removed player");
    
  }

  void deliver(const AuthPacket& msg)
  {
    recent_msgs_.push_back(msg);
    while (recent_msgs_.size() > max_recent_msgs)
      recent_msgs_.pop_front();

    std::for_each(connectedPlayers.begin(), connectedPlayers.end(),
        boost::bind(&Player::deliver, _1, boost::ref(msg)));
  }

private:
  std::set<playerPtr> connectedPlayers;
  enum { max_recent_msgs = 100 };
  auth_message_queue recent_msgs_;
};
