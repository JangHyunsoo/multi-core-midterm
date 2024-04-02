#pragma once

typedef struct _tagResolution {
	unsigned int iW;
	unsigned int iH;
}RESOLUTION, *PRESOULUTION;

typedef struct _tagPosition {
	float x;
	float y;

	_tagPosition() :
		x(0.0f),
		y(0.0f) {}

	_tagPosition(float _x, float _y) :
		x(_x),
		y(_y) {}

	_tagPosition(const _tagPosition& pos) {
		x = pos.x;
		y = pos.y;
	}

	_tagPosition(const POINT& pt) {
		x = (float)pt.x;
		y = (float)pt.y;
	}

	_tagPosition Reserve() {
		return _tagPosition(y, x);
	}

	void operator = (const _tagPosition& pos) {
		x = pos.x;
		y = pos.y;
	}

	void operator = (const POINT& pt) {
		x = (float)pt.x;
		y = (float)pt.y;
	}

	void operator = (float f[2]) {
		x = f[0];
		y = f[1];
	}

	_tagPosition operator + (const _tagPosition& pos) {
		_tagPosition tPos;
		tPos.x = x + pos.x;
		tPos.y = y + pos.y;
		return tPos;
	}

	_tagPosition operator + (float f[2]) {
		_tagPosition tPos;
		tPos.x = x + f[0];
		tPos.y = y + f[1];
		return tPos;
	}

	_tagPosition operator + (float f) {
		_tagPosition tPos;
		tPos.x = x + f;
		tPos.y = y + f;
		return tPos;
	}

	_tagPosition operator + (const POINT& pt) {
		_tagPosition tPos;
		tPos.x = x + pt.x;
		tPos.y = y + pt.y;
		return tPos;
	}

	// =========================== - ===============================

	_tagPosition operator - (const _tagPosition& pos) {
		_tagPosition tPos;
		tPos.x = x - pos.x;
		tPos.y = y - pos.y;
		return tPos;
	}

	_tagPosition operator - (float f[2]) {
		_tagPosition tPos;
		tPos.x = x - f[0];
		tPos.y = y - f[1];
		return tPos;
	}

	_tagPosition operator - (float f) {
		_tagPosition tPos;
		tPos.x = x - f;
		tPos.y = y - f;
		return tPos;
	}

	_tagPosition operator - (const POINT& pt) {
		_tagPosition tPos;
		tPos.x = x - pt.x;
		tPos.y = y - pt.y;
		return tPos;
	}
	
	// =========================== * ===============================

	_tagPosition operator * (const _tagPosition& pos) {
		_tagPosition tPos;
		tPos.x = x * pos.x;
		tPos.y = y * pos.y;
		return tPos;
	}

	_tagPosition operator * (float f[2]) {
		_tagPosition tPos;
		tPos.x = x * f[0];
		tPos.y = y * f[1];
		return tPos;
	}

	_tagPosition operator * (float f) {
		_tagPosition tPos;
		tPos.x = x * f;
		tPos.y = y * f;
		return tPos;
	}

	_tagPosition operator * (const POINT& pt) {
		_tagPosition tPos;
		tPos.x = x * pt.x;
		tPos.y = y * pt.y;
		return tPos;
	}

	// =========================== / ===============================

	_tagPosition operator / (const _tagPosition& pos) {
		_tagPosition tPos;
		tPos.x = x / pos.x;
		tPos.y = y / pos.y;
		return tPos;
	}

	_tagPosition operator / (float f[2]) {
		_tagPosition tPos;
		tPos.x = x / f[0];
		tPos.y = y / f[1];
		return tPos;
	}

	_tagPosition operator / (float f) {
		_tagPosition tPos;
		tPos.x = x / f;
		tPos.y = y / f;
		return tPos;
	}

	_tagPosition operator / (const POINT& pt) {
		_tagPosition tPos;
		tPos.x = x / pt.x;
		tPos.y = y / pt.y;
		return tPos;
	}

}POSITION, *PPOSITION, _SIZE, *_PSIZE;