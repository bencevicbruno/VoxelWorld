#pragma once

#include <iostream>

class Vector
{
public:
	float x, y, z, w;

	Vector();
	Vector(float constant);
	Vector(int x, int y, int z);
	Vector(float x, float y, float z);
	Vector(float x, float y, float z, float w);

	Vector operator+(const Vector& other) const;
	Vector operator-(const Vector& other) const;
	Vector operator*(float constant) const;
	Vector operator/(float constant) const;
	Vector operator%(float constant) const;

	Vector& operator+=(const Vector& other);
	Vector& operator-=(const Vector& other);
	Vector& operator*=(float constant);
	Vector& operator/=(float constant);
	Vector& operator%=(float constant);

	bool operator==(const Vector& other) const;
	bool operator<(const Vector& other) const;

	float distanceTo(const Vector& other) const;
	float distanceTo2(const Vector& other) const;

	float length() const;
	float length2() const;

	Vector goNorth(float n = 1) const;
	Vector goSouth(float n = 1) const;
	Vector goWest(float n = 1) const;
	Vector goEast(float n = 1) const;

	void normalize();

	static float DotProduct(const Vector& first, const Vector& second);
	static Vector CrossProduct(const Vector& first, const Vector& second);

	static Vector up();
	static Vector down();
	static Vector north();
	static Vector east();
	static Vector south();
	static Vector west();
	static Vector FromPolar(float radius, float yaw, float pitch);
	static Vector CreateRandom(float min, float max);

	static Vector GlobalToChunk(const Vector& globalPosition);
	static Vector GlobalToLocal(const Vector& globalPosition);
};

std::ostream& operator<<(std::ostream& outputStream, const Vector& vector);

namespace std {
	template <>
	struct hash<Vector> {
		std::size_t operator()(const Vector& v) const {
			std::size_t seed = 0;
			hash_combine(seed, v.x);
			hash_combine(seed, v.y);
			hash_combine(seed, v.z);
			hash_combine(seed, v.w);
			return seed;
		}
	private:
		template <class T>
		static inline void hash_combine(std::size_t& seed, const T& val) {
			seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
	};
}