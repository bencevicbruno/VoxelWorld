#include <math/Vector.h>

#include <math/Math.h>

Vector::Vector() :
	Vector(0.0f)
{}

Vector::Vector(float constant) :
	x(constant), 
	y(constant), 
	z(constant), 
	w(constant)
{}

Vector::Vector(int x, int y, int z) :
	x(float(x)),
	y(float(y)),
	z(float(z)),
	w(0)
{}

Vector::Vector(float x, float y, float z) :
	x(x),
	y(y),
	z(z),
	w(0)
{}

Vector::Vector(float x, float y, float z, float w) :
	x(x), 
	y(y), 
	z(z), 
	w(w)
{}

Vector Vector::operator+(const Vector& other) const
{
	return Vector(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vector Vector::operator-(const Vector& other) const
{
	return Vector(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vector Vector::operator*(float constant) const
{
	return Vector(x * constant, y * constant, z * constant, w * constant);
}

Vector Vector::operator/(float constant) const
{
	return Vector(x / constant, y / constant, z / constant, w / constant);
}

Vector Vector::operator%(float constant) const
{
	return Vector(std::fmod(x, constant), std::fmod(y, constant), std::fmod(z, constant), std::fmod(w, constant));
}

Vector& Vector::operator+=(const Vector& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;

	return *this;
}

Vector& Vector::operator-=(const Vector& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;

	return *this;
}

Vector& Vector::operator*=(float constant)
{
	x *= constant;
	y *= constant;
	z *= constant;
	w *= constant;

	return *this;
}

Vector& Vector::operator/=(float constant)
{
	x /= constant;
	y /= constant;
	z /= constant;
	w /= constant;

	return *this;
}

Vector& Vector::operator%=(float constant)
{
	x = std::fmod(x, constant);
	y = std::fmod(y, constant);
	z = std::fmod(z, constant);
	w = std::fmod(w, constant);

	return *this;
}

bool Vector::operator==(const Vector& other) const
{
	return (x == other.x) && (y == other.y) && (z == other.z) && (w == other.w);
}

bool Vector::operator<(const Vector& other) const
{
	if (x != other.x) return x < other.x;
	if (y != other.y) return y < other.y;
	if (z != other.z) return z < other.z;

	return w < other.w;
}

float Vector::distanceTo(const Vector& other) const
{
	return Math::Sqrt(distanceTo2(other));
}

float Vector::distanceTo2(const Vector& other) const
{
	return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) + (z - other.z) * (z - other.z);
}

float Vector::length() const
{
	return Math::Sqrt(length2());
}

float Vector::length2() const
{
	return x * x + y * y + z * z + w * w;
}

Vector Vector::goNorth(float n) const
{
	return { x, y, z - n };
}

Vector Vector::goSouth(float n) const
{
	return { x, y, z + n };
}

Vector Vector::goWest(float n) const
{
	return { x - n, y, z };
}

Vector Vector::goEast(float n) const
{
	return { x + n, y, z };
}

void Vector::normalize()
{
	float length = Vector::length();

	x /= length;
	y /= length;
	z /= length;
	w /= length;
}

float Vector::DotProduct(const Vector& first, const Vector& second)
{
	return first.x * second.x + first.y * second.y + first.z * second.z + first.w * second.w;
}

Vector Vector::CrossProduct(const Vector& first, const Vector& second)
{
	return Vector(first.y * second.z - first.z * second.y,
		-(first.x * second.z - first.z * second.x),
		first.x * second.y - first.y * second.x,
		0);
}

Vector Vector::up()
{
	return Vector(0.0f, 1.0f, 0.0f, 0.0f);
}

Vector Vector::down()
{
	return Vector(0.0f, -1.0f, 0.0f, 0.0f);
}

Vector Vector::north()
{
	return Vector(0.0f, 0.0f, -1.0f, 0.0f);
}

Vector Vector::east()
{
	return Vector(1.0f, 0.0f, 0.0f, 0.0f);
}

Vector Vector::south()
{
	return Vector(0.0f, 0.0f, 1.0f, 0.0f);
}

Vector Vector::west()
{
	return Vector(-1.0f, 0.0f, 0.0f, 0.0f);
}

Vector Vector::FromPolar(float radius, float yaw, float pitch)
{
	float x = radius * Math::Cos(pitch) * Math::Cos(yaw);
	float y = radius * Math::Sin(pitch);
	float z = radius * Math::Cos(pitch) * Math::Sin(yaw);

	return Vector(x, y, z, 0.0);
}

Vector Vector::CreateRandom(float min, float max)
{
	float x = Math::CreateRandom(min, max);
	float y = Math::CreateRandom(min, max);
	float z = Math::CreateRandom(min, max);

	return Vector(x, y, z, 0.0);
}

Vector Vector::GlobalToChunk(const Vector& globalPosition)
{
	bool isXNegative = globalPosition.x < 0;
	bool isZNegative = globalPosition.z < 0;

	int chunkX = isXNegative ? (- int(abs(globalPosition.x) - 1) / 16 - 1) : (int(globalPosition.x) / 16);
	int chunkZ = isZNegative ? (- int(abs(globalPosition.z) - 1) / 16 - 1) : (int(globalPosition.z) / 16);

	return Vector(chunkX, 0, chunkZ);
}

Vector Vector::GlobalToLocal(const Vector& globalPosition)
{
	return { std::fmod(globalPosition.x, 16.0f), globalPosition.y, std::fmod(globalPosition.z, 16.0f), 0.0f };
}

std::ostream& operator<<(std::ostream& outputStream, const Vector& vector)
{
	outputStream << "Vector(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";
	return outputStream;
}