/*
** EPITECH PROJECT, 2021
** B-CNA_Trade
** File description:
** Bot
*/

#include <iostream>
#include <sstream>
#include <cstring>
#include "Bot.hpp"

bot::Bot::Bot() noexcept
{
    _candleInterval = 0;
    _candlesGiven = 0;
    _candleTotal = 0;
    _timeBank = 0;
    _timePerMove = 0;
    _initStack = 0;
    _transactionFee = 0;
    _crypto.insert(std::pair<std::string, double>("BTC", 0));
    _crypto.insert(std::pair<std::string, double>("ETH", 0));
    _crypto.insert(std::pair<std::string, double>("USDT", 0));
    _candles.insert(std::pair<std::string, std::vector<candles_t>>("BTC_ETH", NULL));
    _candles.insert(std::pair<std::string, std::vector<candles_t>>("USDT_ETH", NULL));
    _candles.insert(std::pair<std::string, std::vector<candles_t>>("USDT_BTC", NULL));
}

bool bot::Bot::getData(std::string line) noexcept
{
    std::istringstream split(line);

    for (std::string token; std::getline(split, token, ' '); _data.push_back(token));
    if (!_data[0].compare("end"))
        return true;
    return false;
}

void bot::Bot::doSomething() noexcept
{
    std::string line;

    while (std::getline(std::cin, line)) {
        if (getData(line))
            break;
        if (!_data[0].compare("settings"))
            createSettings();
        else if (!_data[0].compare("update"))
            makeUpdate();
        else if (!_data[0].compare("action"))
            makeAction();
        _data.clear();
    }
}
