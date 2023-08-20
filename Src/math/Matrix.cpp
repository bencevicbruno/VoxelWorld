#include "math/Matrix.h"

#include <algorithm>

#include "math/Math.h"

Matrix::Matrix()
{
	data[0][0] = 0;
	data[0][1] = 0;
	data[0][2] = 0;
	data[0][3] = 0;

	data[1][0] = 0;
	data[1][1] = 0;
	data[1][2] = 0;
	data[1][3] = 0;

	data[2][0] = 0;
	data[2][1] = 0;
	data[2][2] = 0;
	data[2][3] = 0;

	data[3][0] = 0;
	data[3][1] = 0;
	data[3][2] = 0;
	data[3][3] = 0;
}

Matrix::Matrix(float f00, float f01, float f02, float f03, 
				float f10, float f11, float f12, float f13, 
				float f20, float f21, float f22, float f23, 
				float f30, float f31, float f32, float f33)
{
	data[0][0] = f00;
	data[0][1] = f01;
	data[0][2] = f02;
	data[0][3] = f03;

	data[1][0] = f10;
	data[1][1] = f11;
	data[1][2] = f12;
	data[1][3] = f13;

	data[2][0] = f20;
	data[2][1] = f21;
	data[2][2] = f22;
	data[2][3] = f23;

	data[3][0] = f30;
	data[3][1] = f31;
	data[3][2] = f32;
	data[3][3] = f33;
}

Matrix Matrix::operator+(const Matrix& other) const
{
	Matrix matrix;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			matrix.data[i][j] = data[i][j] + other.data[i][j];
		}
	}

	return matrix;
}

Matrix Matrix::operator-(const Matrix& other) const
{
	Matrix matrix;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			matrix.data[i][j] = data[i][j] - other.data[i][j];
		}
	}

	return matrix;
}

Matrix Matrix::operator*(const Matrix& other) const
{
	Matrix matrix;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			matrix.data[i][j] = 0;

			for (int k = 0; k < 4; k++)
			{
				matrix.data[i][j] += data[i][k] * other.data[k][j];
			}
		}
	}

	return matrix;
}

Vector Matrix::operator*(const Vector& vector) const
{
	return {
		this->data[0][0] * vector.x
	  + this->data[0][1] * vector.y
	  + this->data[0][2] * vector.z
	  + this->data[0][3] * vector.w,
	    this->data[1][0] * vector.x
	  + this->data[1][1] * vector.y
	  + this->data[1][2] * vector.z
	  + this->data[1][3] * vector.w,
	    this->data[2][0] * vector.x
	  + this->data[2][1] * vector.y
	  + this->data[2][2] * vector.z
	  + this->data[2][3] * vector.w,
		this->data[3][0] * vector.x
	  + this->data[3][1] * vector.y
	  + this->data[3][2] * vector.z
	  + this->data[3][3] * vector.w };
}

void Matrix::invert()
{
	Matrix result = Matrix::GetIdentity();

	for (int i = 0; i < 3; i++) {
		if (this->data[i][i] == 0) {
			for (int j = i + 1; j < 4; j++) {
				if (this->data[j][i] != 0) {
					for (int k = 0; k < 4; k++) {
						std::swap(this->data[i][k], this->data[j][k]);
						std::swap(result.data[i][k], result.data[j][k]);
					}
					break;
				}
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i != j) {
				float factor = -this->data[j][i] / this->data[i][i];

				for (int k = 0; k < 4; k++) {
					this->data[j][k] += this->data[i][k] * factor;
					result.data[j][k] += result.data[i][k] * factor;
				}
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.data[i][j] /= this->data[i][i];
		}
	}

	(*this) = result;
}

Matrix Matrix::GetIdentity()
{
	return Matrix(1, 0, 0, 0,
				  0, 1, 0, 0,
				  0, 0, 1, 0,
				  0, 0, 0, 1);
}

Matrix Matrix::GetTranslation(const Vector& vector)
{
	return Matrix::GetTranslation(vector.x, vector.y, vector.z);
}

Matrix Matrix::GetTranslation(float dx, float dy, float dz)
{
	Matrix matrix = GetIdentity();

	matrix.data[0][3] = dx;
	matrix.data[1][3] = dy;
	matrix.data[2][3] = dz;

	return matrix;
}

Matrix Matrix::GetScaling(float sx, float sy, float sz)
{
	Matrix matrix = GetIdentity();

	matrix.data[0][0] = sx;
	matrix.data[1][1] = sy;
	matrix.data[2][2] = sz;

	return matrix;
}

Matrix Matrix::RotateX(float angle)
{
	float s = Math::Sin(angle);
	float c = Math::Cos(angle);

	return Matrix(1, 0, 0, 0,
		0, c, -s, 0,
		0, s, c, 0,
		0, 0, 0, 1);
}

Matrix Matrix::RotateY(float angle)
{
	float s = Math::Sin(angle);
	float c = Math::Cos(angle);

	return Matrix(c, 0, s, 0,
		0, 1, 0, 0,
		-s, 0, c, 0,
		0, 0, 0, 1);
}

Matrix Matrix::RotateZ(float angle)
{
	float s = Math::Sin(angle);
	float c = Math::Cos(angle);

	return Matrix(c, -s, 0, 0,
		s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}