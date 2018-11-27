#include "core.h"

/**
 Calculate intersection point of segment against triangle
 a, b, c: Vertices of Triangle
 p, q: Segment with direction
 pd, pt: Barycentric coordinates pd(u, v, w) of the intersection point
 */
bool intersect_segment_against_triangle(
	D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c,
	D3DXVECTOR3 p, D3DXVECTOR3 q,
	D3DXVECTOR3* pd, FLOAT *pt
) {
	D3DXVECTOR3
		ab = b - a,
		ac = c - a,
		ap = p - a,
		qp = p - q,
		n;
	D3DXVec3Cross(&n, &ab, &ac);

	/* Check if point p is on the plane */
	if (fabsf(D3DXVec3Dot(&n, &ap)) < D3DX_16F_EPSILON) {
		/* p is on the plane */

		/* Check if point q is on the plane (thus pq and plane are coplanar) */
		if (fabs(D3DXVec3Dot(&n, &qp)) < D3DX_16F_EPSILON) {
			/* coplanar */

			// TODO: return coplanar_check()
			return false;
		}
	}

	FLOAT d = D3DXVec3Dot(&qp, &n);

	if (d <= 0.0f)
		return false;

	FLOAT t = D3DXVec3Dot(&ap, &n);

	if (t < 0.0f || t > d)
		return false;

	D3DXVECTOR3 e;
	FLOAT u, v, w;

	D3DXVec3Cross(&e, &qp, &ap);
	v = D3DXVec3Dot(&ac, &e);

	if (v < 0.0f || v > d)
		return false;

	w = -D3DXVec3Dot(&ab, &e);

	if (w < 0.0f || v + w > d)
		return false;

	FLOAT ood = 1.0f / d;
	t *= ood;
	v *= ood;
	w *= ood;
	u = 1.0f - v - w;

	*pd = D3DXVECTOR3(u, v, w);
	*pt = t;

	return true;
}