#pragma once

#include <math/Vector.h>

class Matrix
{
public:
	float data[4][4];

	Matrix();
	Matrix(float f00, float f01, float f02, float f03,
		float f10, float f11, float f12, float f13,
		float f20, float f21, float f22, float f23,
		float f30, float f31, float f32, float f33);

	Matrix operator+(const Matrix& other) const;
	Matrix operator-(const Matrix& other) const;
	Matrix operator*(const Matrix& other) const;

	Vector operator*(const Vector& vector) const;

	void invert();

	static Matrix GetIdentity();
	static Matrix GetTranslation(const Vector& vector);
	static Matrix GetTranslation(float dx, float dy, float dz);
	static Matrix GetScaling(float sx, float sy, float sz);
	static Matrix RotateX(float angle);
	static Matrix RotateY(float angle);
	static Matrix RotateZ(float angle);
};