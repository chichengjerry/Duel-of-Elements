#include "util.h"

FLOAT BarryCentric(D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 p3, D3DXVECTOR2 v)
{
	FLOAT det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	FLOAT l1 = ((p2.z - p3.z) * (v.x - p3.x) + (p3.x - p2.x) * (v.y - p3.z)) / det;
	FLOAT l2 = ((p3.z - p1.z) * (v.x - p3.x) + (p1.x - p3.x) * (v.y - p3.z)) / det;
	FLOAT l3 = 1.0f - l1 - l2;

	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}
