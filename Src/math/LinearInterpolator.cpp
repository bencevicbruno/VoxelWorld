#include "math/LinearInterpolator.h"

#include <algorithm>

LinearInterpolator::LinearInterpolator(std::vector<std::tuple<double, double>> && pairs):
	pairs(std::move(pairs))
{}

double LinearInterpolator::interpolate(double value) const
{
    size_t lowerIndex = 0;

    for (size_t i = 0; i < pairs.size(); ++i) {
        if (std::get<0>(pairs[i]) <= value)
        {
            lowerIndex = i;
        }
        else
        {
            break;
        }
    }

    const auto& lowerTuple = pairs[lowerIndex];
    const auto& upperTuple = pairs[std::min(lowerIndex + 1, pairs.size() - 1)];

    double interpolation = std::get<1>(lowerTuple) + (value - std::get<0>(lowerTuple)) * (std::get<1>(upperTuple) - std::get<1>(lowerTuple)) / (std::get<0>(upperTuple) - std::get<0>(lowerTuple));
    return interpolation;
}