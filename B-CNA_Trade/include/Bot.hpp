/*
** EPITECH PROJECT, 2021
** B-CNA_Trade
** File description:
** Bot
*/

#ifndef BOT_HPP_
#define BOT_HPP_

#include <string>
#include <vector>
#include <map>

namespace bot {

    typedef struct candles_s {
        std::string type;
        double date;
        double high;
        double low;
        double open;
        double close;
        double volume;
    } candles_t;

    typedef struct indicators_s {
        std::vector<double> closeValues;
        double middleBand;
        double standDev;
        double lowBand;
        double highBand;
        double r;
        double g;
    } indicators_t;

    enum types {
        pair,
        date,
        high,
        low,
        open,
        close,
        volume,
        undefined = -1
    };

    class Bot {
        public:
            Bot() noexcept;
            ~Bot() = default;

            /* parsing */
            bool getData(std::string line) noexcept;
            void doSomething() noexcept;
        
            /* settings */
            void createCandleFormat() noexcept;
            void createSettings() noexcept;
        
            /* update */
            void makeUpdate() noexcept;
            void updateStacks() noexcept;
            void updateValues() noexcept;
            void fillNewCandle(std::vector<std::string> &values, candles_t &candle) noexcept;
        
            /* actions */
            void makeAction() noexcept;
            bool manageMoney(std::vector<candles_t> &candles, double money, double sell, bool notFirst) noexcept;
            int getAction(indicators_t *ind) noexcept;
            // compute
            int computeIndicators(std::vector<candles_t> &candles, indicators_t *ind) noexcept;
            //void getRandG(indicators_t *ind) noexcept;
            //void getBoolinger(indicators_t *ind) noexcept;

        protected:
        private:
            std::vector<std::string> _data;
            std::map<std::string, double> _crypto;
            std::map<std::string, std::vector<candles_t>> _candles;
            std::vector<int> _indicators;
            std::vector<double> _macd;
            std::vector<double> _signal;
            // settings
            std::vector<types> _candleFormat;
            int _candleInterval;
            int _candleTotal;
            int _candlesGiven;
            int _timeBank;
            int _timePerMove;
            int _initStack;
            double _transactionFee;
    };
}

/* STATIC CONST */
static const int SUCCESS = 0;
static const int ERROR = 84;

static const int NOTHING = 0;
static const int BUY = 1;
static const int SELL = -1;

static const double INDICATORS = 2;

#endif /* !BOT_HPP_ */
