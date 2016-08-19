#ifndef C_VEC
#define C_VEC

template <typename t>
struct Vec3
{
	t x;
	t y;
	t z;
	const Vec3 Vec3::operator += (const Vec3 other) const
	{
		Vec3  vec= *this;
		vec.x += other.x;
		vec.y += other.y;
		vec.z += other.z;
		return vec;
	}
	const Vec3 Vec3::operator + (const Vec3 other) const
	{
		Vec3  vec= *this;
		vec.x += other.x;
		vec.y += other.y;
		vec.z += other.z;
		return vec;
	}
	Vec3(t x, t y, t z) : x(x), y(y), z(z)
	{
	}

};

typedef Vec3<float> Vec3F;
typedef Vec3<int> Vec3I;

#endif