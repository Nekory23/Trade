/*
** EPITECH PROJECT, 2021
** B-CNA_Trade
** File description:
** BotUpdate
*/

#include <iostream>
#include <cstring>
#include <sstream>

#include "Bot.hpp"

void bot::Bot::updateStacks() noexcept
{
    std::vector<std::string> parse;
    std::vector<std::string> values;
    std::istringstream split(_data.back());

    for (std::string token; std::getline(split, token, ','); parse.push_back(token));
    for (std::size_t i = 0; i != parse.size(); ++i) {
        std::istringstream splitBis(parse[i]);
        for (std::string token; std::getline(splitBis, token, ':'); values.push_back(token));
        if (!values[0].compare("BTC"))
            _crypto["BTC"] = std::stod(values[1]);
        else if (!values[0].compare("ETH"))
            _crypto["ETH"] = std::stod(values[1]);
        else if (!values[0].compare("USDT"))
            _crypto["USDT"] = std::stod(values[1]);
        values.clear();
    }
}

void bot::Bot::fillNewCandle(std::vector<std::string> &values, candles_t &candle) noexcept
{
    candle.type = values[0];
    for (std::size_t j = 0; j != values.size(); ++j)  {
        if (_candleFormat[j] == types::date)
            candle.date = std::stod(values[j]);
        else if (_candleFormat[j] == types::high)
            candle.high = std::stod(values[j]);
        else if (_candleFormat[j] == types::low)
            candle.low = std::stod(values[j]);
        else if (_candleFormat[j] == types::open)
            candle.open = std::stod(values[j]);
        else if (_candleFormat[j] == types::close)
            candle.close = std::stod(values[j]);
        else if (_candleFormat[j] == types::volume)
            candle.volume = std::stod(values[j]);
    }
}

void bot::Bot::updateValues() noexcept
{
    std::vector<std::string> values;
    std::vector<std::string> splitted;
    std::istringstream split(_data.back());
    candles_t newCandle;

    for (std::string token; std::getline(split, token, ';'); splitted.push_back(token));
    for (std::size_t i = 0; i != splitted.size(); ++i) {
        std::istringstream splitBis(splitted[i]);
        for (std::string token; std::getline(splitBis, token, ','); values.push_back(token));
        fillNewCandle(values, newCandle);
        if (!newCandle.type.compare("BTC_ETH"))
            _candles["BTC_ETH"].push_back(newCandle);
        else if (!newCandle.type.compare("USDT_ETH"))
            _candles["USDT_ETH"].push_back(newCandle);
        else if (!newCandle.type.compare("USDT_BTC"))
            _candles["USDT_BTC"].push_back(newCandle);
        values.clear();
    }
}

void bot::Bot::makeUpdate() noexcept
{
    if (_data[1].compare("game") != 0)
        return;
    if (!_data[2].compare("stacks"))
        updateStacks();
    else if (!_data[2].compare("next_candles"))
        updateValues();
}