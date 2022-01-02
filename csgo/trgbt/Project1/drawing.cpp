#include "includes.h"
#define SEGMENTS 7
#define LAYERS 4
#define PI 3.14159265358979323846
#define _USE_MATH_DEFINES
extern Hack* hack;

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color) {
	D3DRECT rect = { x,y,x + w, y + h };
	pDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color) {

	if (!hack->LineL) //comprueba si hay una, si no, crea
		D3DXCreateLine(pDevice, &hack->LineL);

	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);
	hack->LineL->SetWidth(thickness);
	hack->LineL->Draw(Line, 2, color);


}

void DrawLine(Vec2 src, Vec2 dst, int thickness, D3DCOLOR color) {
	DrawLine(src.x, src.y, dst.x, dst.y, thickness, color);
}
void DrawLine(Vec3 src, Vec3 dst, int thickness, D3DCOLOR color) {
	DrawLine(src.x, src.y, dst.x, dst.y, thickness, color);
}

void DrawCube(Vector pt1, Vector pt2, int thickness, D3DCOLOR color)
{
	Vec3 pt111(pt1.x, pt1.y, pt1.z);
	Vec3 pt112(pt1.x, pt1.y, pt2.z);
	Vec3 pt121(pt1.x, pt2.y, pt1.z);
	Vec3 pt122(pt1.x, pt2.y, pt2.z);
	Vec3 pt212(pt2.x, pt1.y, pt2.z);
	Vec3 pt221(pt2.x, pt2.y, pt1.z);
	Vec3 pt211(pt2.x, pt1.y, pt1.z);
	Vec3 pt222(pt2.x, pt2.y, pt2.z);

	Vec2 pt111_2D, pt112_2D, pt121_2D, pt122_2D, pt211_2D, pt212_2D, pt221_2D, pt222_2D;
	hack->WorldToScreen(pt111, pt111_2D);
	hack->WorldToScreen(pt112, pt112_2D);
	hack->WorldToScreen(pt121, pt121_2D);
	hack->WorldToScreen(pt122, pt122_2D);
	hack->WorldToScreen(pt211, pt211_2D);
	hack->WorldToScreen(pt212, pt212_2D);
	hack->WorldToScreen(pt221, pt221_2D);
	hack->WorldToScreen(pt222, pt222_2D);


	DrawLine(pt111_2D, pt112_2D, thickness, color);
	DrawLine(pt111_2D, pt121_2D, thickness, color);
	DrawLine(pt111_2D, pt211_2D, thickness, color);

	DrawLine(pt112_2D, pt122_2D, thickness, color);
	DrawLine(pt112_2D, pt212_2D, thickness, color);

	DrawLine(pt121_2D, pt221_2D, thickness, color);
	DrawLine(pt121_2D, pt122_2D, thickness, color);

	DrawLine(pt211_2D, pt212_2D, thickness, color);
	DrawLine(pt211_2D, pt221_2D, thickness, color);

	DrawLine(pt122_2D, pt222_2D, thickness, color);

	DrawLine(pt212_2D, pt222_2D, thickness, color);

	DrawLine(pt221_2D, pt222_2D, thickness, color);

}

float DotProduct_sdk(const float* v1, const float* v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}
void VectorTransform_sdk(const float* in1, const matrix3x4_t& in2, float* out)
{
	out[0] = DotProduct_sdk(in1, in2[0]) + in2[0][3];
	out[1] = DotProduct_sdk(in1, in2[1]) + in2[1][3];
	out[2] = DotProduct_sdk(in1, in2[2]) + in2[2][3];
}

void VectorTransform_sdk(const Vector& in1, const matrix3x4_t& in2, Vector& out)
{
	VectorTransform_sdk(&in1.x, in2, &out.x);
}

void Normalize(Vec3& vec)
{
	double nor = sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
	vec.x = vec.x / nor;
	vec.y = vec.y / nor;
	vec.z = vec.z / nor;
}

double getAngle(Vec3& vec1, Vec3& vec2)
{
	double dot = vec1.x * vec2.x + vec1.y * vec2.y + vec1.z + vec2.z;
	double lenSq1 = vec1.x * vec1.x + vec1.y * vec1.y + vec1.z + vec1.z;
	double lenSq2 = vec2.x * vec2.x + vec2.y * vec2.y + vec2.z + vec2.z;
	return acos(dot / sqrt(lenSq1 * lenSq2));
}
Vec3 Cross(Vec3& v1, Vec3& v2)
{
	Vec3 result_v;
	result_v.x = v1.y * v2.z - v1.z * v2.y;
	result_v.y = v1.z * v2.x - v1.x * v2.z;
	result_v.z = v1.x * v2.y - v1.y * v2.x;
	return result_v;

}
float Mult_Matr(Vec4& v1, Vec3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w ;
}

void GetOrthogonalAxis(Vec3& normal, Vec3& xAxis, Vec3& yAxis, Vec3& zAxis)
{
	Normalize(normal);

	Vec3 axisZ(0, 0, 1);
	double angle = getAngle(normal, axisZ);
	if (angle < PI * 0.25 || angle > PI * 0.75)
	{
		Vec3 aux(0, 1, 0);
		xAxis = Cross(aux, normal);
	}
	else
	{
		xAxis = Cross(normal, axisZ);
	}
	Normalize(xAxis);
	yAxis = Cross(normal, xAxis);
	Normalize(yAxis);
	zAxis = normal;
}


void GetCirclesPoints2D(Vec3 &Origin, double radius, Vec3 vertices[SEGMENTS+1]){
	//static Vec3 vertices[SEGMENTS + 1];
	double step = (PI * 2) / SEGMENTS;
	for (int i = 0; i < SEGMENTS; i++)
	{
		double theta = step * i;

		vertices[i].x = Origin.x + radius * cos(theta);
		vertices[i].y = Origin.y + radius * sin(theta);
		vertices[i].z = 0;
	}

	vertices[SEGMENTS] = vertices[0];
	//return vertices;
}

void GetCirclePoints(Vec3 origin, Vec3 normal, double radius, Vec3 vertices[SEGMENTS+1])
{
	Vec3 xAxis, yAxis, zAxis;

	GetOrthogonalAxis(normal, xAxis, yAxis, zAxis);
	Vec4 Row1(xAxis.x, yAxis.x, zAxis.x, origin.x);
	Vec4 Row2(xAxis.y, yAxis.y, zAxis.y, origin.y);
	Vec4 Row3(xAxis.z, yAxis.z, zAxis.z, origin.z);

	/*Vec4 Row1(xAxis.x, xAxis.y, xAxis.z, origin.x);
	Vec4 Row2(yAxis.x, yAxis.y, yAxis.z, origin.y);
	Vec4 Row3(zAxis.x, zAxis.y, zAxis.z, origin.z);*/

	//static Vec3* vertices;
	Vec3 empty(0, 0, 0);
	GetCirclesPoints2D(empty, radius, vertices);
	for (int i = 0; i < SEGMENTS + 1; i++)
	{
		Vec3 vert = vertices[i];
		//Vec4 aux(vertices[i]);
		vertices[i].x = Mult_Matr(Row1, vert);
		vertices[i].y = Mult_Matr(Row2, vert);
		vertices[i].z = Mult_Matr(Row3, vert);
	}
	//return vertices;
}
void GetHalfSphere(Vec3 origin, Vec3 normal, float radius, Vec3 verticesByLayer[LAYERS][SEGMENTS+1])
{
	Normalize(normal);
	//static Vec3 verticesByLayer[LAYERS][SEGMENTS+1];
	for (int i = 0; i < LAYERS; i++)
	{
		float radiusLayer = radius - i * (radius / LAYERS);
		float multiplier = cos(asin(radiusLayer / radius)) * radius;
		Vec3 originLayer(origin.x + normal.x * multiplier, origin.y + normal.y * multiplier, origin.z + normal.z * multiplier);
		GetCirclePoints(originLayer, normal, radiusLayer, verticesByLayer[i]);
	}
	//Vec3* a = &verticesByLayer;
}

void DrawCapsule(Vec3 start, Vec3 end, float radius, float thickness, D3DCOLOR color, Vec3 halfSphere0[LAYERS][SEGMENTS + 1], Vec3 halfSphere1[LAYERS][SEGMENTS + 1])
{
	Vec3 normal(end.x - start.x, end.y - start.y, end.z - start.z);
	Normalize(normal);
	Vec3 normal_i(-normal.x, -normal.y, -normal.z);

	//Vec3 halfSphere0[LAYERS][SEGMENTS+1]; 
	//Vec3 halfSphere1[LAYERS][SEGMENTS+1]; 
	
	GetHalfSphere(start, normal_i, radius, halfSphere0);
	GetHalfSphere(end, normal, radius, halfSphere1);

	for (int i = 0; i < LAYERS; i++)
	{
		hack->WorldToScreen(halfSphere0[i][0]);
		hack->WorldToScreen(halfSphere1[i][0]);

		for (int j = 1; j < SEGMENTS+1; j++)
		{
			hack->WorldToScreen(halfSphere0[i][j]);
			hack->WorldToScreen(halfSphere1[i][j]);

			DrawLine(halfSphere0[i][j], halfSphere0[i][j - 1], thickness, color);
			DrawLine(halfSphere1[i][j], halfSphere1[i][j - 1], thickness, color);
		}
	}

	for (int j = 0; j < SEGMENTS+1; j++)
	{
		for (int i = 0; i < LAYERS-1; i++)
		{
			DrawLine(halfSphere0[i][j], halfSphere0[i+1][j], thickness, color);
			DrawLine(halfSphere1[i][j], halfSphere1[i+1][j], thickness, color);
		}
	}
	/*int closest[SEGMENTS];
	for (int j = 0; j < SEGMENTS; j++)
	{
		int aux = 1000000;
		int pos = 0;
		for (int i = 0; i < SEGMENTS; i++)
		{
			if (sqrt((halfSphere0[0][j].x - halfSphere1[0][i].x) * (halfSphere0[0][j].x - halfSphere1[0][i].x) + (halfSphere0[0][j].y - halfSphere1[0][i].y) * (halfSphere0[0][j].y - halfSphere1[0][i].y) + (halfSphere0[0][j].z - halfSphere1[0][i].z) * (halfSphere0[0][j].z - halfSphere1[0][i].z)) < aux)
			{
				aux = sqrt((halfSphere0[0][j].x - halfSphere1[0][i].x) * (halfSphere0[0][j].x - halfSphere1[0][i].x) + (halfSphere0[0][j].y - halfSphere1[0][i].y) * (halfSphere0[0][j].y - halfSphere1[0][i].y) + (halfSphere0[0][j].z - halfSphere1[0][i].z) * (halfSphere0[0][j].z - halfSphere1[0][i].z));
				pos = i;
			}
		}

		closest[j] = pos;
		DrawLine(halfSphere0[0][j], halfSphere1[0][SEGMENTS - j], thickness, color);
	}*/
	Vec3 vertices0[SEGMENTS + 1];
	Vec3 vertices1[SEGMENTS + 1];
	GetCirclePoints(start, normal, radius, vertices0);
	GetCirclePoints(end, normal, radius, vertices1);
	for (int j = 0; j < SEGMENTS; j++)
	{
		hack->WorldToScreen(vertices0[j]);
		hack->WorldToScreen(vertices1[j]);

		DrawLine(vertices0[j], vertices1[j], thickness, color);
		
	}


}