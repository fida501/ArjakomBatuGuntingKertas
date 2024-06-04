#ifndef PLAYER_H
#define PLAYER_H
#include <string>

class Player
{
    public:
        Player();
        virtual ~Player();
        void setPlayerName(std::string playerName);
        std::string getPlayerName();
        bool getPlayerConnectedStatus();
        void setPlayerConnectedStatus(bool status);

    protected:

    private:
        std::string playerName;
        bool isPlayerConnected;
};

#endif // PLAYER_H
