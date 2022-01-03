#pragma once
#include "hack.h"


void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR col);

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color);

void DrawLine(Vec2& src, Vec2& dst, int thickness, D3DCOLOR color);

void DrawLine(Vec3& src, Vec3& dst, int thickness, D3DCOLOR color);

void DrawCube(Vec3& pt1, Vec3& pt2, int thickness, D3DCOLOR color);

void VectorTransform_sdk(const Vec3& in1, const matrix3x4_t& in2, Vec3& out);

void VectorTransform_sdk(const float* in1, const matrix3x4_t& in2, float* out);

float DotProduct_sdk(const float* v1, const float* v2);

void DrawCapsule(Vec3* start, Vec3* end, float radius, float thickness, D3DCOLOR color, Vec3** halfSphere0, Vec3** halfSphere1);

void GetHalfSphere(Vec3* origin, Vec3* normal, float radius, Vec3** verticesByLayer);

void GetCirclePoints(Vec3* origin, Vec3* normal, double radius, Vec3* vertices);

void GetCirclesPoints2D(double radius, Vec3* vertices);

void GetOrthogonalAxis(Vec3* normal, Vec3*& xAxis, Vec3*& yAxis, Vec3*& zAxis);

float Mult_Matr(Vec4* v1, Vec3* v2);

Vec3* Cross(Vec3* v1, Vec3* v2);

float getAngle(Vec3* vec1, Vec3* vec2);

float FastInvSqrt(float x);

void Normalize(Vec3* vec);