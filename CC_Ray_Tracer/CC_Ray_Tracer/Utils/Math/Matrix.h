#pragma once
#include "UtilFunctions.h"

class Matrix
{
private:
	float matrix[3][3];

public:
	Matrix() {
		matrix[0][0] = 1;
		matrix[0][1] = 0;
		matrix[0][2] = 0;
		matrix[1][0] = 0;
		matrix[1][1] = 1;
		matrix[1][2] = 0;
		matrix[2][0] = 0;
		matrix[2][1] = 0;
		matrix[2][2] = 1;
	}

	Matrix(float a00, float a01, float a02,
		float a10, float a11, float a12,
		float a20, float a21, float a22)
	{
		matrix[0][0] = a00;
		matrix[0][1] = a10;
		matrix[0][2] = a20;
		matrix[1][0] = a01;
		matrix[1][1] = a11;
		matrix[1][2] = a21;
		matrix[2][0] = a02;
		matrix[2][1] = a12;
		matrix[2][2] = a22;
	}

	float& getValueAt(int i, int j)
	{
		return matrix[j][i];
	}

	float getValueAt(int i, int j) const
	{
		return matrix[j][i];
	}

	Matrix& operator*=(const Matrix& other)
	{
		float a00 = matrix[0][0] * other.matrix[0][0] + matrix[1][0] * other.matrix[0][1] + matrix[2][0] * other.matrix[0][2];
		float a01 = matrix[0][0] * other.matrix[1][0] + matrix[1][0] * other.matrix[1][1] + matrix[2][0] * other.matrix[1][2];
		float a02 = matrix[0][0] * other.matrix[2][0] + matrix[1][0] * other.matrix[2][1] + matrix[2][0] * other.matrix[2][2];

		float a10 = matrix[0][1] * other.matrix[0][0] + matrix[1][1] * other.matrix[0][1] + matrix[2][1] * other.matrix[0][2];
		float a11 = matrix[0][1] * other.matrix[1][0] + matrix[1][1] * other.matrix[1][1] + matrix[2][1] * other.matrix[1][2];
		float a12 = matrix[0][1] * other.matrix[2][0] + matrix[1][1] * other.matrix[2][1] + matrix[2][1] * other.matrix[2][2];

		float a20 = matrix[0][2] * other.matrix[0][0] + matrix[1][2] * other.matrix[0][1] + matrix[2][2] * other.matrix[0][2];
		float a21 = matrix[0][2] * other.matrix[1][0] + matrix[1][2] * other.matrix[1][1] + matrix[2][2] * other.matrix[1][2];
		float a22 = matrix[0][2] * other.matrix[2][0] + matrix[1][2] * other.matrix[2][1] + matrix[2][2] * other.matrix[2][2];

		matrix[0][0] = a00;
		matrix[0][1] = a10;
		matrix[0][2] = a20;
		matrix[1][0] = a01;
		matrix[1][1] = a11;
		matrix[1][2] = a21;
		matrix[2][0] = a02;
		matrix[2][1] = a12;
		matrix[2][2] = a22;

		return *this;
	}

	void transpose()
	{
		Matrix tr(matrix[0][0], matrix[0][1], matrix[0][2],
			matrix[1][0], matrix[1][1], matrix[1][2],
			matrix[2][0], matrix[2][1], matrix[2][2]);

		*this = tr;
	}
};

inline Matrix operator*(Matrix matrix, const Matrix& other)
{
	matrix *= other;
	return matrix;
}

inline Matrix rotateX(float deg)
{
	float rad = degToRad(deg);
	return Matrix(
		1,		0,		0,
		0, cosf(rad), -sinf(rad),
		0, sinf(rad), cosf(rad));
}

inline Matrix rotateY(float deg)
{
	float rad = degToRad(deg);
	return Matrix(
		cosf(rad), 0, sinf(rad),
		0,		1,		0,
		-sinf(rad),	0, cosf(rad));
}

inline Matrix rotateZ(float deg)
{
	float rad = degToRad(deg);
	return Matrix(
		cosf(rad), -sinf(rad), 0,
		sinf(rad), cosf(rad), 0,
		0,		0,		1);
}