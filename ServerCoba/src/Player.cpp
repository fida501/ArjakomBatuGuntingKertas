#include "Player.h"

Player::Player()
{
    playerName = "";
    //ctor
}

Player::~Player()
{
    //dtor
}

std::string Player::getPlayerName(){
    return playerName;
}

void Player::setPlayerName(std::string newPlayerName){
    playerName = newPlayerName;
}

bool Player::getPlayerConnectedStatus(){
    return isPlayerConnected;
}
void Player::setPlayerConnectedStatus(bool status){
    isPlayerConnected = status;
}

