#pragma once
#define PI (3.1415927f)
#include <corecrt_math.h>

static constexpr float DEG_RAD2 = PI / 360.0f;
static constexpr float DEG_RAD = 180.0f / PI;
static constexpr float RAD_DEG = PI / 180.f;
struct vec2 {
	union {
		struct {
			float x, y;
		};
		float floatArr[2];
	};
	vec2() { x = y = 0; }
	vec2(float a, float b) : x(a), y(b) {}
	vec2(int a, int b) : x((float)a), y((float)b) {}
	vec2& operator=(const vec2& copy) {
		x = copy.x;
		y = copy.y;
		return *this;
	}

	static vec2 fromAngle(float angle) {
		return vec2((float)-sin(angle), (float)cos(angle));
	}

	bool operator==(const vec2& o) const { return x == o.x && y == o.y; }
	bool operator!=(const vec2& o) const { return x != o.x || y != o.y; }

	vec2 sub(float f) {
		return vec2(x - f, y - f);
	}
	vec2 sub(float ox, float oy) {
		return vec2(x - ox, y - oy);
	}
	vec2 div(float f) {
		return vec2(x / f, y / f);
	}

	vec2 div(const vec2& o) {
		return vec2(x / o.x, y / o.y);
	}
	vec2 mul(const vec2& o) {
		return vec2(x * o.x, y * o.y);
	}
	vec2 mul(float f) {
		return vec2(x * f, y * f);
	}
	vec2 sub(const vec2& o) {
		return vec2(x - o.x, y - o.y);
	}
	vec2 add(const vec2& o) {
		return vec2(x + o.x, y + o.y);
	}
	vec2 add(float o) {
		return vec2(x + o, y + o);
	}
	vec2 add(float ox, float oy) {
		return vec2(x + ox, y + oy);
	}

	vec2 normalized() {
		return div(magnitude());
	}

	vec2 cross() {
		return vec2(-y, x);
	}

	float dot(float ox, float oy) const { return x * ox + y * oy; }


	float dot(const vec2& o) const { return x * o.x + y * o.y; }

	vec2 normAngles() {
		float x = this->x;
		float y = this->y;
		while (x > 90.f)
			x -= 180.0f;
		while (x < -90.f)
			x += 180.0f;

		while (y > 180.0f)
			y -= 360.0f;
		while (y < -180.0f)
			y += 360.0f;
		return vec2(x, y);
	}

	float squaredlen() const { return x * x + y * y; }
	float magnitude() const { return sqrtf(squaredlen()); }
	float getDist(vec2 pos) {
		float dx = fmaxf(pos.x, this->x) - fminf(pos.x, this->x);
		float dy = fmaxf(pos.y, this->y) - fminf(pos.y, this->y);
		return sqrtf(dx * dx + dy * dy);
	}
};
template <typename T>
struct vec3 {
	union {
		struct {
			T x, y, z;
		};
		T floatArr[3];
	};

	vec3() { x = y = z = 0; }
	vec3(T a, T b, T c) : x(a), y(b), z(c) {}
	vec3(T a, T b) : x(a), y(b), z(0) {}
	vec3(const vec3& copy) {
		x = copy.x;
		y = copy.y;
		z = copy.z;
	}

	vec3& operator=(const vec3& copy) {
		x = copy.x;
		y = copy.y;
		z = copy.z;

		return *this;
	}

	vec3(T* v) : x(v[0]), y(v[1]), z(v[2]) {}

	bool iszero() const { return x == 0 && y == 0 && z == 0; }

	bool operator==(const vec3& o) const { return x == o.x && y == o.y && z == o.z; };
	bool operator!=(const vec3& o) const { return x != o.x || y != o.y || z != o.z; };
	vec3 operator-() const { return vec3(-x, -y, -z); };

	vec3 mul(const vec3& o) {
		return vec3(x * o.x, y * o.y, z * o.z);
	};
	vec3 mul(T f) {
		return vec3(x * f, y * f, z * f);
	};
	vec3 mul(T x1, T y1, T z1) {
		return vec3(x * x1, y * y1, z * z1);
	};
	vec3 div(T f) {
		return vec3(x / f, y / f, z / f);
	};
	vec3 div(T x1, T y1, T z1) {
		return vec3(x / x1, y / y1, z / z1);
	};
	vec3 div(const vec3& o) {
		return vec3(x / o.x, y / o.y, z / o.z);
	};
	vec3 add(T f) {
		return vec3(x + f, y + f, z + f);
	};
	vec3 add(T x1, T y1, T z1) {
		return vec3(x + x1, y + y1, z + z1);
	};
	vec3 sub(T f) {
		return vec3(x - f, y - f, z - f);
	};

	vec3 sub(T x1, T y1, T z1) {
		return vec3(x - x1, y - y1, z - z1);
	};

	vec3 floor() {
		return vec3(floorf(x), floorf(y), floorf(z));
	};
	vec3 round() {
		return vec3(roundf(x), roundf(y), roundf(z));
	};

	vec3 add(const vec3& o) const {
		return vec3(x + o.x, y + o.y, z + o.z);
	}
	vec3 sub(const vec3& o) const {
		return vec3(x - o.x, y - o.y, z - o.z);
	}

	vec3 below(float o) const {
		return vec3(x, y - o, z);
	}

	vec3 above(float o) const {
		return vec3(x, y + o, z);
	}

	float squaredlen() const { return x * x + y * y + z * z; }
	float squaredxzlen() const { return x * x + z * z; }

	vec3 lerp(const vec3& other, float tx, float ty, float tz) const {
		vec3 ne;
		ne.x = x + tx * (other.x - x);
		ne.y = y + ty * (other.y - y);
		ne.z = z + tz * (other.z - z);
		return ne;
	}

	vec3 lerp(const vec3 other, float val) const {
		vec3 ne;
		ne.x = x + val * (other.x - x);
		ne.y = y + val * (other.y - y);
		ne.z = z + val * (other.z - z);
		return ne;
	}

	vec3 lerp(const vec3* other, float val) const {
		vec3 ne;
		ne.x = x + val * (other->x - x);
		ne.y = y + val * (other->y - y);
		ne.z = z + val * (other->z - z);
		return ne;
	}

	vec2 flatten() const {
		return vec2(x, y);
	}

	float sqrxy() const { return x * x + y * y; }

	float dot(const vec3& o) const { return x * o.x + y * o.y + z * o.z; }
	float dotxy(const vec3& o) const { return x * o.x + y * o.y; }

	float magnitude() const { return sqrtf(squaredlen()); }

	vec3 normalize() {
		return div(magnitude());
	}

	float dist(const vec3& e) const {
		return sub(e).magnitude();
	}

	float Get2DDist(const vec3& e) const {
		float dx = e.x - x, dy = e.y - y;
		return sqrtf(dx * dx + dy * dy);
	}

	float magnitudexy() const { return sqrtf(x * x + y * y); }
	float magnitudexz() const { return sqrtf(x * x + z * z); }

	vec3 cross(const vec3& b) {
		return vec3(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
	}
	float cxy(const vec3& a) { return x * a.y - y * a.x; }

	vec2 CalcAngle(vec3 dst) {
		vec3 diff = dst.sub(*this);

		diff.y = diff.y / diff.magnitude();
		vec2 angles;
		angles.x = asinf(diff.y) * -DEG_RAD;
		angles.y = (float)-atan2f(diff.x, diff.z) * DEG_RAD;

		return angles;
	}
};
struct vec4 {
	union {
		struct {
			float x, y, z, w;
		};
		float v[4];
	};
	vec4() { x = 0, y = 0, z = 0, w = 0; }
	explicit vec4(const vec3<float>& p, float w = 0) : x(p.x), y(p.y), z(p.z), w(w) {};
	vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
	float& operator[](int i) { return v[i]; };
	float operator[](int i) const { return v[i]; };

	__forceinline bool contains(vec2* point) {
		if (point->x <= x || point->y <= y)
			return false;

		if (point->x >= z || point->y >= w)
			return false;
		return true;
	};
};
struct glmatrixf {
	union {
		float v[16];
		float v_nested[4][4];
	};

	__forceinline float operator[](int i) const { return v[i]; }
	__forceinline float& operator[](int i) { return v[i]; }

#define MULMAT(row, col) v[col + row] = x[row] * y[col] + x[row + 4] * y[col + 1] + x[row + 8] * y[col + 2] + x[row + 12] * y[col + 3];

	template <class XT, class YT>
	void mul(const XT x[16], const YT y[16]) {
		MULMAT(0, 0);
		MULMAT(1, 0);
		MULMAT(2, 0);
		MULMAT(3, 0);
		MULMAT(0, 4);
		MULMAT(1, 4);
		MULMAT(2, 4);
		MULMAT(3, 4);
		MULMAT(0, 8);
		MULMAT(1, 8);
		MULMAT(2, 8);
		MULMAT(3, 8);
		MULMAT(0, 12);
		MULMAT(1, 12);
		MULMAT(2, 12);
		MULMAT(3, 12);
	}

#undef MULMAT

	glmatrixf* correct() {
		glmatrixf* newMatPtr = new glmatrixf;

		for (int i = 0; i < 4; i++) {
			newMatPtr->v[i * 4 + 0] = v[0 + i];
			newMatPtr->v[i * 4 + 1] = v[4 + i];
			newMatPtr->v[i * 4 + 2] = v[8 + i];
			newMatPtr->v[i * 4 + 3] = v[12 + i];
		}
		return newMatPtr;
	};

	inline bool OWorldToScreen(vec3<float> origin, vec3<float> pos, vec2& screen, vec2 fov, vec2 displaySize) {
		pos = pos.sub(origin);

		float x = transformx(pos);
		float y = transformy(pos);
		float z = transformz(pos);

		if (z > 0)
			return false;

		float mX = (float)displaySize.x / 2.0F;
		float mY = (float)displaySize.y / 2.0F;

		screen.x = mX + (mX * x / -z * fov.x);
		screen.y = mY - (mY * y / -z * fov.y);

		return true;
	}

	inline void mul(const glmatrixf& x, const glmatrixf& y) {
		mul(x.v, y.v);
	}

	inline void translate(float x, float y, float z) {
		v[12] += x;
		v[13] += y;
		v[14] += z;
	}

	inline void translate(const vec3<float>& o) {
		translate(o.x, o.y, o.z);
	}

	inline void scale(float x, float y, float z) {
		v[0] *= x;
		v[1] *= x;
		v[2] *= x;
		v[3] *= x;
		v[4] *= y;
		v[5] *= y;
		v[6] *= y;
		v[7] *= y;
		v[8] *= z;
		v[9] *= z;
		v[10] *= z;
		v[11] *= z;
	}

	inline void invertnormal(vec3<float>& dir) const {
		vec3 n(dir);
		dir.x = n.x * v[0] + n.y * v[1] + n.z * v[2];
		dir.y = n.x * v[4] + n.y * v[5] + n.z * v[6];
		dir.z = n.x * v[8] + n.y * v[9] + n.z * v[10];
	}

	inline void invertvertex(vec3<float>& pos) const {
		vec3 p(pos);
		p.x -= v[12];
		p.y -= v[13];
		p.z -= v[14];
		pos.x = p.x * v[0] + p.y * v[1] + p.z * v[2];
		pos.y = p.x * v[4] + p.y * v[5] + p.z * v[6];
		pos.z = p.x * v[8] + p.y * v[9] + p.z * v[10];
	}

	inline void transform(const vec3<float>& in, vec4& out) const {
		out.x = transformx(in);
		out.y = transformy(in);
		out.z = transformz(in);
		out.w = transformw(in);
	}

	__forceinline float transformx(const vec3<float>& p) const {
		return p.x * v[0] + p.y * v[4] + p.z * v[8] + v[12];
	}

	__forceinline float transformy(const vec3<float>& p) const {
		return p.x * v[1] + p.y * v[5] + p.z * v[9] + v[13];
	}

	__forceinline float transformz(const vec3<float>& p) const {
		return p.x * v[2] + p.y * v[6] + p.z * v[10] + v[14];
	}

	__forceinline float transformw(const vec3<float>& p) const {
		return p.x * v[3] + p.y * v[7] + p.z * v[11] + v[15];
	}

	__forceinline vec3<float> gettranslation() const {
		return vec3(v[12], v[13], v[14]);
	}

	//assault cube world2screen
	vec3<float> transform(glmatrixf* matrix, vec3<float>& totransform) {
		return vec3(matrix->transformx(totransform),
			matrix->transformy(totransform),
			matrix->transformz(totransform))
			.div(matrix->transformw(totransform));
	}

	///pos should be the exact center of the enemy model for scaling to work properly
	vec3<float> WorldToScreen(vec3 <float>pos, int width, int height) {
		//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
		vec4 clipCoords;
		clipCoords.x = pos.x * v[0] + pos.y * v[4] + pos.z * v[8] + v[12];
		clipCoords.y = pos.x * v[1] + pos.y * v[5] + pos.z * v[9] + v[13];
		clipCoords.z = pos.x * v[2] + pos.y * v[6] + pos.z * v[10] + v[14];
		clipCoords.w = pos.x * v[3] + pos.y * v[7] + pos.z * v[11] + v[15];

		//perspective division, dividing by clip.W = Normalized Device Coordinates
		vec3<float> NDC;
		NDC.x = clipCoords.x / clipCoords.w;
		NDC.y = clipCoords.y / clipCoords.w;
		NDC.z = clipCoords.z / clipCoords.w;

		//viewport tranform to screenCooords

		vec3<float> playerscreen;
		playerscreen.x = (width / 2 * NDC.x) + (NDC.x + width / 2);
		playerscreen.y = -(height / 2 * NDC.y) + (NDC.y + height / 2);

		return playerscreen;
	}
};
struct AABB {
	vec3<float> lower;
	vec3<float> upper;
	bool isZero = false;
	char padding[3];
	AABB() {}
	AABB(vec3<float> l, vec3<float> h) : lower(l), upper(h) {};
	AABB(const AABB& aabb) {
		lower = vec3(aabb.lower);
		upper = vec3(aabb.upper);
	}
	AABB(vec3<float> lower, float width, float height, float eyeHeight) {
		lower = lower.sub(vec3<float>(width, eyeHeight * 2, width).div(1.f));
		this->lower = lower;
		this->upper = { lower.x + width, lower.y + height, lower.z + width };
	}

	bool operator==(const AABB& rhs) const {
		return lower == rhs.lower && upper == rhs.upper;
	}

	bool isFullBlock() {
		auto diff = lower.sub(upper);
		return fabsf(diff.y) == 1 && fabsf(diff.x) == 1 && fabsf(diff.z) == 1;
	}

	AABB expanded(float amount) {
		return AABB(lower.sub(amount), upper.add(amount));
	}

	AABB expandedXZ(float amount) {
		return AABB(lower.sub(amount, 0.f, amount), upper.add(amount, 0.f, amount));
	}

	vec3<float> centerPoint() {
		vec3 diff = upper.sub(lower);
		return lower.add(diff.mul(0.5f));
	}

	bool intersects(AABB aabb) {
		return aabb.upper.x > lower.x && upper.x > aabb.lower.x &&
			aabb.upper.y > lower.y && upper.y > aabb.lower.y &&
			aabb.upper.z > lower.z && upper.z > aabb.lower.z;
	}

	bool intersectsXZ(AABB aabb) {
		return aabb.upper.x > lower.x && upper.x > aabb.lower.x &&
			aabb.upper.z > lower.z && upper.z > aabb.lower.z;
	}
};
