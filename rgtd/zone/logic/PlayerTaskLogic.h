#pragma once


class PlayerAgent;
class PlayerTaskLogic
{
public:
    PlayerTaskLogic(PlayerAgent * p):player(p){}
private:
    PlayerAgent * player;
};

