#include <iostream>
#include <vector>
#include <memory>

class RiskModel {
public:
    virtual double var(double notional) const = 0;
    virtual ~RiskModel() = 0;
};

RiskModel::~RiskModel() = default;

class ParametricVar : public RiskModel {
public:
    explicit ParametricVar(double volatility) : volatility_(volatility) {}

    double var(double notional) const override {
        constexpr double zScore99 = 2.33;
        return (notional * volatility_ * zScore99);
    }
private:
    double volatility_;
};

class HistoricalVar : public RiskModel {
public:
    explicit HistoricalVar(double lossQuantile) : lossQuantile_(lossQuantile) {}

    double var(double notional) const override {
        return notional * lossQuantile_;
    }
private:
    double lossQuantile_;
};

int main(){
    std::vector<std::unique_ptr<RiskModel>> models;
    models.push_back(std::make_unique<ParametricVar>(0.02));
    models.push_back(std::make_unique<HistoricalVar>(0.045));
    double total = 0;
    for (const auto& m : models) {
        total += m->var(1000000.0);
    }
    std::cout << total << "\n";
}