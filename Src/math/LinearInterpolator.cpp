#include "math/LinearInterpolator.h"

#include <algorithm>

LinearInterpolator::LinearInterpolator(std::vector<std::tuple<double, double>>&& pairs) :
	pairs(std::move(pairs))
{}

double LinearInterpolator::interpolate(double value) const
{
	size_t lowerIndex = 0;

	for (size_t i = 0; i < pairs.size(); ++i)
	{
		if (std::get<0>(pairs[i]) <= value)
		{
			lowerIndex = i;
		}
		else
		{
			break;
		}
	}

	const auto& [x1, y1] = pairs[lowerIndex];
	const auto& [x2, y2] = pairs[std::min(lowerIndex + 1, pairs.size() - 1)];

	double coefficient = (y2 - y1) / (x2 - x1);
	double interpolation = y1 + (value - x1) * coefficient;

	return interpolation;
}