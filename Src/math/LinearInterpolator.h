#pragma once

#include <tuple>
#include <vector>

class LinearInterpolator
{
public:
	LinearInterpolator() = default;
	LinearInterpolator(std::vector<std::tuple<double, double>>&& pairs);

	double interpolate(double value) const;
private:
	std::vector<std::tuple<double, double>> pairs;
};