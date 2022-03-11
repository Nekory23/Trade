/*
** EPITECH PROJECT, 2021
** B-CNA_Trade
** File description:
** BotAction
*/

#include <iostream>
#include <cmath>

#include "Bot.hpp"

static double computeG(std::vector<double> &values, std::size_t nbr)
{
    double g = 0;

    for (std::size_t i = values.size() - nbr; i != values.size(); ++i) {
        double temp = values[i] - values[i - 1];
        if (temp < 0)
            continue;
        else
            g += temp;
    }
    g /= nbr;
    return g;
}

static double computeR(std::vector<double> &values, std::size_t nbr)
{
    double V1 = values[values.size() - (nbr + 1)];
    double V2 = values.back();
    double res = 0;

    res = V2 - V1;
    res /= V1;
    res *= 100;
    return res;
}

static double computeMiddleBand(std::vector<bot::candles_t> &candles, std::vector<double> closeValues, int given)
{
    std::size_t nbr = candles.size() - 1 - given;
    double res = 0;

    for (std::size_t i = candles.size() - 1; i > nbr; --i)
        res += closeValues[i];
    res /= given;
    return res;
}

static double computeStandDev(std::vector<bot::candles_t> &candles, bot::indicators_t *ind, int given)
{
    std::size_t nbr = candles.size() - 1 - given;
    double res = 0;

    for (std::size_t i = candles.size() - 1; i > nbr; --i)
        res += std::pow(std::abs(ind->closeValues[i] - ind->middleBand), 2);
    res = std::sqrt(res / given);
    return res;
}

static int getSignal(std::vector<bot::candles_t> &candles, bot::indicators_t *ind, std::vector<double> &macd, std::vector<double> &signal)
{
    int firstPeriod = 12;
    int secondPeriod = 26;
    int signalPeriod = 9;
    double firstMean = 0;
    double secondMean = 0;
    double constVal = 0;

    (void) ind;
    for (std::size_t i = candles.size() - 1; i != candles.size() - 1 - firstPeriod; --i)
        firstMean += candles[i].close;
    firstMean /= firstPeriod;
    constVal = 2 / (firstPeriod + 1);
    firstMean = candles[candles.size() - 1].close + firstMean * constVal + firstMean;

    for (std::size_t i = candles.size() - 1; i != candles.size() - 1 - secondPeriod; --i)
        secondMean += candles[i].close;
    secondMean /= secondPeriod;
    constVal = 2 / (secondPeriod + 1);
    firstMean = candles[candles.size() - 1].close + secondMean * constVal + secondMean;

    macd.push_back(firstMean - secondMean);
    if (macd.size() <= 9)
        return NOTHING;
    firstMean = 0;
    for (std::size_t i = macd.size(); i != macd.size() - 1 - signalPeriod; --i)
        firstMean += macd[i];
    signal.push_back(firstMean / signalPeriod);

    if (macd[macd.size() - 2] < signal[signal.size() - 2] && macd[macd.size() - 1] > signal[signal.size() - 1])
        return BUY;
    if (macd[macd.size() - 2] > signal[signal.size() - 2] && macd[macd.size() - 1] < signal[signal.size() - 1])
        return SELL;
    return NOTHING;
}

//void bot::Bot::getRandG(indicators_t *ind) noexcept
//{
//    if (ind->r < 0 && ind->g > 1) {
//        _indicators.push_back(BUY);
//        return;
//    }
//    if (ind->r > 0 && ind->g < 1) {
//        _indicators.push_back(SELL);
//        return;
//    }
//    _indicators.push_back(NOTHING);
//}
//
//void bot::Bot::getBoolinger(indicators_t *ind) noexcept
//{
//    if (ind->closeValues.back() < ind->lowBand) {
//        _indicators.push_back(BUY);
//        return;
//    }
//    if (ind->closeValues.back() > ind->highBand) {
//        _indicators.push_back(SELL);
//        return;
//    }
//    _indicators.push_back(NOTHING);
//}

int bot::Bot::computeIndicators(std::vector<candles_t> &candles, indicators_t *ind) noexcept
{
    std::size_t nbr = candles.size() - 1 - _candlesGiven;
    double delta = 1.8;
    int signal = NOTHING;
    //int buy = 0;
    //int sell = 0;

    ind->middleBand = computeMiddleBand(candles, ind->closeValues, _candlesGiven);
    ind->standDev = computeStandDev(candles, ind, _candlesGiven);
    ind->lowBand = ind->middleBand - (delta * ind->standDev);
    ind->highBand = ind->middleBand + (delta * ind->standDev);
    ind->r = computeR(ind->closeValues, nbr);
    ind->g = computeG(ind->closeValues, nbr);
    signal = getSignal(candles, ind, _macd, _signal);
    
    //getRandG(ind);
    //getBoolinger(ind);
    //_indicators.push_back(signal);
    if (ind->closeValues.back() > ind->highBand || (ind->r > 0 && ind->g < 0.5) || signal == SELL)
        return SELL;
    if (ind->closeValues.back() < ind->lowBand || (ind->r < 0 && ind->g > 1) || signal == BUY)
        return BUY;
    //for (auto i = 0; i != _indicators.size(); i++) {
    //    if (_indicators[i] == BUY)
    //        buy++;
    //    else if (_indicators[i] == SELL)
    //        sell++;
    //}
    //if (buy > sell)
    //    return BUY;
    //if (sell < buy)
    //    return SELL;
    return NOTHING;
}

bool bot::Bot::manageMoney(std::vector<candles_t> &candles, double money, double sell, bool notFirst) noexcept
{
    indicators_t ind;
    std::vector<double> closeValues;
    int action = 0;

    for (std::size_t i = 0; i != candles.size() - 1; ++i)
        closeValues.push_back(candles[i].close);
    ind.closeValues = closeValues;
    action = computeIndicators(candles, &ind);
    
    double toBuy = ((ind.lowBand - ind.closeValues.back()) / 10) * money;
    double toSell = ((ind.closeValues.back() - ind.highBand) / 10) * sell;
    //std::cerr << "Tobuy = " << toBuy << std::endl;// debug
    //std::cerr << "Tosell = " << toSell << std::endl;// debug

    if (action == BUY && money > toBuy && toBuy > 0.001) {
        //std::cerr << "money = " << money << " low = " << ind.lowBand << " close = " << ind.closeValues.back()<< std::endl;
        //std::cerr << "BOUGHT ===================================== " << std::endl << std::endl;
        if (notFirst)
            std::cout << ";";
        std::cout << "buy " << candles[0].type << " " << toBuy;
        return true;
    }
    else if (action == SELL && sell > toSell && toSell > 0.5) {
        //std::cerr << "sell = " << sell << " low = " << ind.highBand << " close = " << ind.closeValues.back() << std::endl;
        //std::cerr << "SOLD ===================================== " << std::endl << std::endl;
        if (notFirst)
            std::cout << ";";
        std::cout << "sell " << candles[0].type << " " << toSell;
        return true;
    }
    //std::cerr << std::endl;
    return false;
}

static double getPrice(std::map<std::string, double> crypto, std::map<std::string, std::vector<bot::candles_t>> candles, std::string curr, std::string trad)
{
    return (crypto[curr] / candles[trad][candles[trad].size() - 1].close);
}

void bot::Bot::makeAction() noexcept
{
    bool action = false;
    bool bougth = manageMoney(_candles["USDT_ETH"], getPrice(_crypto, _candles, "USDT", "USDT_ETH"), _crypto["ETH"], action);
    if (bougth)
        action = bougth;
    bougth = manageMoney(_candles["BTC_ETH"], getPrice(_crypto, _candles, "BTC", "BTC_ETH"), _crypto["ETH"], action);
    if (bougth)
        action = bougth;
    bougth = manageMoney(_candles["USDT_BTC"], getPrice(_crypto, _candles, "USDT", "USDT_BTC"), _crypto["BTC"], action);
    if (bougth)
        action = bougth;
    if (!action)
        std::cout << "pass" << std::endl;
    else
        std::cout << std::endl;
}