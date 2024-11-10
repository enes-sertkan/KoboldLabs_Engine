#include "PhysicsManager.h"

glm::vec3 point;

float distancePointToIntersectionTriangle(
	glm::vec3 point, sTriangleP theTriangle)
{
	return glm::distance(point, theTriangle.intersectionPoint);
}

float distancePointToIntersectionTriangleMesh(
	glm::vec3 point, sCollision_RayTriangleInMesh theTriangle)
{
	return glm::distance(point, theTriangle.vecTriangles[0].intersectionPoint);
}


class cSortCloseToPoint
{
public:
	cSortCloseToPoint(glm::vec3 point)
	{
		this->m_point = point;
	}
private:
	glm::vec3 m_point;
public:
	// Predicate comparison method (the "less than") method
	// on predicate function
	bool operator()(sTriangleP triA, sTriangleP triB)
	{
		float distToA = distancePointToIntersectionTriangle(point, triA);
		float distToB = distancePointToIntersectionTriangle(point, triB);
		if (distToA < distToB)
		{
			return true;
		}
		// 
		return false;
	}
};

class cSortMeshCloseToPoint
{
public:
	cSortMeshCloseToPoint(glm::vec3 point)
	{
		this->m_point = point;
	}
private:
	glm::vec3 m_point;
public:
	// Predicate comparison method (the "less than") method
	// on predicate function
	bool operator()(sCollision_RayTriangleInMesh triA, sCollision_RayTriangleInMesh triB)
	{
		float distToA = distancePointToIntersectionTriangleMesh(point, triA);
		float distToB = distancePointToIntersectionTriangleMesh(point, triB);

		if (distToA < distToB)
		{
			return true;
		}
		// 
		return false;
	}
};






bool PhysicsManager::AddTriangleMesh(
	std::string meshModelName,
	glm::vec3 meshWorldPosition,
	glm::vec3 meshWorldOrientation,
	float uniformScale)
{
	if (!this->VAOMan)
	{
		// Haven't set the VAO manager pointer, yet
		return false;
	}

	std::vector<cVAOManager::sTriangle> vecVAOTriangles;
	if (!this->VAOMan->getTriangleMeshInfo(meshModelName, vecVAOTriangles))
	{
		// Can't find mesh
		return false;
	}

	// At this point, the vecTriangles has all the triangle info.

	sTrianglePMesh* pMesh = new sTrianglePMesh();

	// Copy that vector into the local mesh format 
	for (std::vector<cVAOManager::sTriangle>::iterator itVAOTri = vecVAOTriangles.begin();
		itVAOTri != vecVAOTriangles.end(); itVAOTri++)
	{
		cVAOManager::sTriangle curVAOTri = *itVAOTri;

		sTriangleP curTri;
		curTri.vertices[0] = curVAOTri.vertices[0];
		curTri.vertices[1] = curVAOTri.vertices[1];
		curTri.vertices[2] = curVAOTri.vertices[2];
		curTri.normal = curVAOTri.normal;

		pMesh->vecTriangles.push_back(curTri);

	}//for (std::vector<cVAOManager::sTriangleP>::iterator itVAOTri

	// Transform the triangles to where they are in the world
	for (std::vector<sTriangleP>::iterator itTri = pMesh->vecTriangles.begin();
		itTri != pMesh->vecTriangles.end(); itTri++)
	{
		// Take the same matrix transformation technique 
		//	from the render call and apply to each vertex

		glm::mat4 matModel = glm::mat4(1.0f);

		// Translation (movement, position, placement...)
		glm::mat4 matTranslate
			= glm::translate(glm::mat4(1.0f),
				meshWorldPosition);

		// Rotation...
		// Caculate 3 Euler acix matrices...
		glm::mat4 matRotateX =
			glm::rotate(glm::mat4(1.0f),
				glm::radians(meshWorldOrientation.x), // Angle in radians
				glm::vec3(1.0f, 0.0, 0.0f));

		glm::mat4 matRotateY =
			glm::rotate(glm::mat4(1.0f),
				glm::radians(meshWorldOrientation.y), // Angle in radians
				glm::vec3(0.0f, 1.0, 0.0f));

		glm::mat4 matRotateZ =
			glm::rotate(glm::mat4(1.0f),
				glm::radians(meshWorldOrientation.z), // Angle in radians
				glm::vec3(0.0f, 0.0, 1.0f));


		// Scale
		glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
			glm::vec3(uniformScale, uniformScale, uniformScale));


		// Calculate the final model/world matrix
		matModel *= matTranslate;     // matModel = matModel * matTranslate;
		matModel *= matRotateX;
		matModel *= matRotateY;
		matModel *= matRotateZ;
		matModel *= matScale;


		// Like from the vertex shader: 	
		// fvertexWorldLocation = matModel * vec4(finalVert, 1.0);

		sTriangleP curTriangle = *itTri;
		// Transform the vertex to where they are in the world...
		glm::vec4 vert0World = matModel * glm::vec4(curTriangle.vertices[0], 1.0f);
		glm::vec4 vert1World = matModel * glm::vec4(curTriangle.vertices[1], 1.0f);
		glm::vec4 vert2World = matModel * glm::vec4(curTriangle.vertices[2], 1.0f);

		// Copy the transformed vertices bacl
		itTri->vertices[0] = vert0World;
		itTri->vertices[1] = vert1World;
		itTri->vertices[2] = vert2World;

		//itTri->vertices[0] = curTriangle.vertices[0];
		//itTri->vertices[1] = curTriangle.vertices[1];
		//itTri->vertices[2] = curTriangle.vertices[2];

		// Also rotate the normal
		glm::mat4 matModelInverseTranspose = glm::inverse(glm::transpose(matModel));

		itTri->normal
			= glm::vec3(matModelInverseTranspose * glm::vec4(itTri->normal, 1.0f));

	}//for (std::vector<sTriangleP>::iterator itTri

	pMesh->meshInstanceName = meshModelName;
	//	pMesh->meshModelName = ??
	this->meshColliders.push_back(pMesh);


	return true;
}

bool PhysicsManager::RayCast(glm::vec3 start, glm::vec3 end,
	std::vector<sCollision_RayTriangleInMesh>& vec_RayTriangle_Collisions,
	bool bIgnoreBackFacingTriangles /*= true*/)
{
	sLine theRay;
	theRay.startXYZ = start;
	theRay.endXYZ = end;

	// Clear the vector being passed in...
	vec_RayTriangle_Collisions.clear();

	// Got through all the triangle mesh objects and test 
	//	the ray triangle for each triangle in each mesh
	for (std::vector< sTrianglePMesh*>::iterator itMesh = this->meshColliders.begin();
		itMesh != this->meshColliders.end(); itMesh++)
	{
		// Dereference the iterator to get the mesh
		sTrianglePMesh* pCurTriMesh = *itMesh;

		sCollision_RayTriangleInMesh intersectionInfo;
		intersectionInfo.theRay = theRay;
		intersectionInfo.meshInstanceName = pCurTriMesh->meshInstanceName;

		// Go through each triangle
		for (std::vector<sTriangleP>::iterator itTri = pCurTriMesh->vecTriangles.begin();
			itTri != pCurTriMesh->vecTriangles.end(); itTri++)
		{

			sTriangleP CurTriangle = *itTri;
			// Sorry, but the method takes pointers to the 
			//	line(ray) and triangle.
			// I did this because the other methods did this
			float u, v, w, t;
			if (this->bLineSegment_TriangleCollision(theRay, CurTriangle, u, v, w, t))
			{
				// They intersect, so add this triangle to the "intersected triangles" of this mesh		
				CurTriangle.intersectionPoint.x =
					CurTriangle.vertices[0].x * u
					+ CurTriangle.vertices[1].x * v
					+ CurTriangle.vertices[2].x * w;
				CurTriangle.intersectionPoint.y =
					CurTriangle.vertices[0].y * u
					+ CurTriangle.vertices[1].y * v
					+ CurTriangle.vertices[2].y * w;
				CurTriangle.intersectionPoint.z =
					CurTriangle.vertices[0].z * u
					+ CurTriangle.vertices[1].z * v
					+ CurTriangle.vertices[2].z * w;

				//				// Ignoring backfacing triangle? 
				//				if (bIgnoreBackFacingTriangles)
				//				{
				//					// See if this traingle is facing "away" from the ray
				//					glm::vec3 rayDirection = (start - end);
				//					rayDirection = glm::normalize(rayDirection);
				//					float dotProd = glm::dot(rayDirection, CurTriangle.normal);
				//					//
				//					if (dotProd > 0.0f)
				//					{
				//						// Facing towards, so add
				//						intersectionInfo.vecTriangles.push_back(CurTriangle);
				//					}
				//				}
				//				else
				//				{
									// Add all triangles, even back facing
				intersectionInfo.vecTriangles.push_back(CurTriangle);
				//				}
			}

		}//for (std::vector<sTriangleP>::iterator itTri

		// Sort the traingles from nearest to farthest
		// Compare the intersection point with the "glm::vec3 start" ray point
		point = theRay.startXYZ;
		std::sort(intersectionInfo.vecTriangles.begin(),
			intersectionInfo.vecTriangles.end(),
			cSortCloseToPoint(theRay.startXYZ));

		
		//std::sort(intersectionInfo.vecTriangles.begin(),
		//		  intersectionInfo.vecTriangles.end(), 
		//	      isTriACloserThanB);


		// Add this mesh-triangle(s) to the vector of collisions
		// (one entry per mesh)
		if (!intersectionInfo.vecTriangles.empty())
		{
			vec_RayTriangle_Collisions.push_back(intersectionInfo);
		}

	}//for (std::vector< sTrianglePMesh*>::iterator itMesh



	// Any collisions? 
	if (vec_RayTriangle_Collisions.empty())
	{
		// No
		return false;
	}

	std::sort(vec_RayTriangle_Collisions.begin(), vec_RayTriangle_Collisions.end(), cSortMeshCloseToPoint(theRay.startXYZ));

	// There were collisions
	return true;
}

bool PhysicsManager::bLineSegment_TriangleCollision(sLine theLine, sTriangleP theTri)
{
	glm::vec3 ab = theTri.vertices[1] - theTri.vertices[0];		//	Vector ab = b - a;
	glm::vec3 ac = theTri.vertices[2] - theTri.vertices[0];		//	Vector ac = c - a;
	glm::vec3 qp = theLine.startXYZ - theLine.endXYZ;			//	Vector qp = p - q;
	//
	//	Compute triangle normal. Can be precalculated or cached if
	//	intersecting multiple segments against the same triangle
	glm::vec3 n = glm::cross(ab, ac);	//	Vector n = Cross(ab, ac);
	//
	//	Compute denominator d. If d <= 0, segment is parallel to or points
	//	away from triangle, so exit early
	float d = glm::dot(qp, n);		//	float d = Dot(qp, n);

	if (d <= 0.0f) {
		return false;
	}
	//
	//	Compute intersection t value of pq with plane of triangle. A ray
	//	intersects iff 0 <= t. Segment intersects iff 0 <= t <= 1. Delay
	//	dividing by d until intersection has been found to pierce triangle
	glm::vec3 ap = theLine.startXYZ - theTri.vertices[0];		//	Vector ap = p - a;
	float t = glm::dot(ap, n);									//	t = Dot(ap, n);

	if (t < 0.0f) {
		return false;
	}

	// For segment; exclude this code line for a ray test
	if (t > d) {
		return 0;
	}

	//	Compute barycentric coordinate components and test if within bounds
	glm::vec3 e = glm::cross(qp, ap);	//	Vector e = Cross(qp, ap);
	float v = glm::dot(ac, e);		//	v = Dot(ac, e);

	if (v < 0.0f || v > d) {
		return false;
	}

	float w = -glm::dot(ab, e);		//	w = -Dot(ab, e);

	if ((w < 0.0f) || (v + w > d)) {
		return 0;
	}

	//	Segment/ray intersects triangle. Perform delayed division and
	//	compute the last barycentric coordinate component
	// Discard this:
	float u = 0.0f;
	return this->bLineSegment_TriangleCollision(theLine, theTri, u, v, w, t);
}

// Reworked from the Christer Ericsson collision book
// == = Section 5.3.6: ============================================================ =
// Given segment pq and triangle abc, returns whether segment intersects
// triangle and if so, also returns the barycentric coordinates (u,v,w)
// of the intersection point
//	int IntersectSegmentTriangle(Point p, Point q, Point a, Point b, Point c,
//		float& u, float& v, float& w, float& t)
bool PhysicsManager::bLineSegment_TriangleCollision(
	sLine theLine, sTriangleP theTri,
	float& u, float& v, float& w, float& t)
{
	glm::vec3 ab = theTri.vertices[1] - theTri.vertices[0];		//	Vector ab = b - a;
	glm::vec3 ac = theTri.vertices[2] - theTri.vertices[0];		//	Vector ac = c - a;
	glm::vec3 qp = theLine.startXYZ - theLine.endXYZ;			//	Vector qp = p - q;
	//
	//	Compute triangle normal. Can be precalculated or cached if
	//	intersecting multiple segments against the same triangle
	glm::vec3 n = glm::cross(ab, ac);	//	Vector n = Cross(ab, ac);
	//
	//	Compute denominator d. If d <= 0, segment is parallel to or points
	//	away from triangle, so exit early
	float d = glm::dot(qp, n);		//	float d = Dot(qp, n);

	if (d <= 0.0f) {
		return false;
	}
	//
	//	Compute intersection t value of pq with plane of triangle. A ray
	//	intersects iff 0 <= t. Segment intersects iff 0 <= t <= 1. Delay
	//	dividing by d until intersection has been found to pierce triangle
	glm::vec3 ap = theLine.startXYZ - theTri.vertices[0];		//	Vector ap = p - a;
	t = glm::dot(ap, n);									//	t = Dot(ap, n);

	if (t < 0.0f) {
		return false;
	}

	// For segment; exclude this code line for a ray test
	if (t > d) {
		return 0;
	}

	//	Compute barycentric coordinate components and test if within bounds
	glm::vec3 e = glm::cross(qp, ap);	//	Vector e = Cross(qp, ap);
	v = glm::dot(ac, e);		//	v = Dot(ac, e);

	if (v < 0.0f || v > d) {
		return false;
	}

	w = -glm::dot(ab, e);		//	w = -Dot(ab, e);

	if ((w < 0.0f) || (v + w > d)) {
		return 0;
	}

	//	Segment/ray intersects triangle. Perform delayed division and
	//	compute the last barycentric coordinate component
	float ood = 1.0f / d;
	t *= ood;
	v *= ood;
	w *= ood;
	u = 1.0f - v - w;

	return true;
}


bool PhysicsManager::bLineSegment_TriangleCollisionBack(
	sLine theLine, sTriangleP theTri,
	float& u, float& v, float& w, float& t)
{
	glm::vec3 ab = theTri.vertices[1] - theTri.vertices[0];		//	Vector ab = b - a;
	glm::vec3 ac = theTri.vertices[2] - theTri.vertices[0];		//	Vector ac = c - a;
	glm::vec3 qp = theLine.endXYZ - theLine.startXYZ;			//	Vector qp = p - q;
	//
	//	Compute triangle normal. Can be precalculated or cached if
	//	intersecting multiple segments against the same triangle
	glm::vec3 n = glm::cross(ab, ac);	//	Vector n = Cross(ab, ac);
	//
	//	Compute denominator d. If d <= 0, segment is parallel to or points
	//	away from triangle, so exit early
	float d = glm::dot(qp, n);		//	float d = Dot(qp, n);

	if (d <= 0.0f) {
		return false;
	}
	//
	//	Compute intersection t value of pq with plane of triangle. A ray
	//	intersects iff 0 <= t. Segment intersects iff 0 <= t <= 1. Delay
	//	dividing by d until intersection has been found to pierce triangle
	glm::vec3 ap = theLine.startXYZ - theTri.vertices[0];		//	Vector ap = p - a;
	t = glm::dot(ap, n);									//	t = Dot(ap, n);

	if (t < 0.0f) {
		return false;
	}

	// For segment; exclude this code line for a ray test
	if (t > d) {
		return 0;
	}

	//	Compute barycentric coordinate components and test if within bounds
	glm::vec3 e = glm::cross(qp, ap);	//	Vector e = Cross(qp, ap);
	v = glm::dot(ac, e);		//	v = Dot(ac, e);

	if (v < 0.0f || v > d) {
		return false;
	}

	w = -glm::dot(ab, e);		//	w = -Dot(ab, e);

	if ((w < 0.0f) || (v + w > d)) {
		return 0;
	}

	//	Segment/ray intersects triangle. Perform delayed division and
	//	compute the last barycentric coordinate component
	float ood = 1.0f / d;
	t *= ood;
	v *= ood;
	w *= ood;
	u = 1.0f - v - w;

	return true;
}
