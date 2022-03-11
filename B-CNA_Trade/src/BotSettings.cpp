/*
** EPITECH PROJECT, 2021
** B-CNA_Trade
** File description:
** BotSettings
*/

#include <cstring>
#include <iostream>
#include <sstream>

#include "Bot.hpp"

void bot::Bot::createCandleFormat() noexcept
{
    std::istringstream split(_data.back());
    std::vector<std::string> splitted;

    for (std::string token; std::getline(split, token, ','); splitted.push_back(token));
    for (std::size_t i = 0; i != splitted.size(); ++i) {
        if (!splitted[i].compare("pair"))
            _candleFormat.push_back(types::pair);
        else if (!splitted[i].compare("date"))
            _candleFormat.push_back(types::date);
        else if (!splitted[i].compare("high"))
            _candleFormat.push_back(types::high);
        else if (!splitted[i].compare("low"))
            _candleFormat.push_back(types::low);
        else if (!splitted[i].compare("open"))
            _candleFormat.push_back(types::open);
        else if (!splitted[i].compare("close"))
            _candleFormat.push_back(types::close);
        else if (!splitted[i].compare("volume"))
            _candleFormat.push_back(types::volume);
        else
            _candleFormat.push_back(types::undefined);
    }
}

void bot::Bot::createSettings() noexcept
{
    if (_data.size() > 0)
        _data.erase(_data.begin());

    if (!_data.front().compare("timebank"))
        _timeBank = std::stoi(_data.back());
    if (!_data.front().compare("time_per_move"))
        _timePerMove = std::stoi(_data.back());
    if (!_data.front().compare("candle_interval"))
        _candleInterval = std::stoi(_data.back());
    if (!_data.front().compare("candle_format"))
        createCandleFormat();
    if (!_data.front().compare("candles_total"))
        _candleTotal = std::stoi(_data.back());
    if (!_data.front().compare("candles_given"))
        _candlesGiven = std::stoi(_data.back());
    if (!_data.front().compare("initial_stack"))
        _initStack = std::stoi(_data.back());
    if (!_data.front().compare("transaction_fee_percent"))
        _transactionFee = std::stod(_data.back());
}