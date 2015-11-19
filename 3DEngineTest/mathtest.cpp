#include "stdafx.h"
#include "CppUnitTest.h"

#include <math/BoundingBox.h>
#include <math/BoundingBoxUtil.h>


#include <glm/gtc/matrix_transform.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test3DEngine
{		
	TEST_CLASS(BoundingBoxTests)
	{
	public:
		
		TEST_METHOD(Creation)
		{
			AABBox box0;
			Assert::IsTrue(box0.d == glm::vec3(0,0,0), L"", LINE_INFO());
			Assert::IsTrue(box0.p == glm::vec3(0, 0, 0), L"", LINE_INFO());

			glm::vec3 v0(1, 2, 3);
			glm::vec3 v1(4, 6, 8);
			AABBox box(v0, v1);

			Assert::IsTrue(box.d == glm::vec3(1.5f, 2.f, 2.5f), L"d is wrong", LINE_INFO());
			Assert::IsTrue(box.p == v0 + box.d, L"p is wrong", LINE_INFO());

			Assert::IsTrue(v0 == box.Minimum()), LINE_INFO();
			Assert::IsTrue(v1 == box.Maximum()), LINE_INFO();
		}

		TEST_METHOD(HullBox)
		{
			glm::vec3 v00(1, 2, 3);
			glm::vec3 v01(4, 6, 8);
			glm::vec3 v10(-1, -2, -3);
			glm::vec3 v11(10, 0, -2);

			AABBox box0(v00,v01);
			AABBox box1(v10, v11);

			AABBox hull_box = box0.HullBox(box1);

			Assert::IsTrue(v10 == hull_box.Minimum()), LINE_INFO();
			Assert::IsTrue(glm::vec3(10, 6, 8) == hull_box.Maximum()), LINE_INFO();

			AABBox hull_box2 = box0 + box1;
			Assert::IsTrue(hull_box == hull_box2, L"",LINE_INFO());
		}

		TEST_METHOD(Transform)
		{
			glm::vec3 v0(-1, -2, -3);
			glm::vec3 v1(3, 2, 1);
			AABBox box(v0, v1);

			//rot 90 deg around x 
			glm::mat4 rmat(glm::vec4(1, 0, 0, 0),
				glm::vec4(0, 0, 1, 0),
				glm::vec4(0, -1, 0, 0),
				glm::vec4(0, 0, 0, 1));
			auto tbox = box.Transform(rmat);
			Assert::IsTrue(glm::vec3(-1,-1,-2) == tbox.Minimum()), LINE_INFO();

			auto tbox2 = rmat * box;
			Assert::IsTrue(tbox2 == tbox, L"", LINE_INFO());
		}

	};

	TEST_CLASS(BoundingBoxUtilTests)
	{
		TEST_METHOD(DirectionalLightFrustumOrtho)
		{
			AABBox box;
			box.d = glm::vec3(1, 2, 3);
			glm::vec3 ldir(0, -1, 0);

			OrthogonalFrustum Frustum;
			Assert::IsTrue(BoundingBoxUtil::DirectionalLightFrustum(box, ldir, Frustum),L"", LINE_INFO());
			Assert::IsTrue(glm::vec3(0,2,0) == Frustum.position), LINE_INFO();
			Assert::AreEqual(0.f, Frustum.nearPlane);
			Assert::AreEqual(4.f, Frustum.farPlane);
			Assert::AreEqual(-3.f, Frustum.left);
			Assert::AreEqual(3.f, Frustum.right);
			Assert::AreEqual(1.f, Frustum.top);
			Assert::AreEqual(-1.f, Frustum.bottom);
		}

		TEST_METHOD(DirectionalLightFrustumSkewed)
		{
			AABBox box;
			box.d = glm::vec3(2, 2, 2);
			glm::vec3 ldir(-1, -1, -1);

			OrthogonalFrustum Frustum;
			Assert::IsTrue(BoundingBoxUtil::DirectionalLightFrustum(box, ldir, Frustum), L"", LINE_INFO());
			Assert::AreEqual(0.f, Frustum.nearPlane);
			Assert::AreEqual(glm::length(box.d)*2.f, Frustum.farPlane);
			Assert::IsTrue(-Frustum.right + Frustum.left < 1e-5);
			Assert::IsTrue(-Frustum.top + Frustum.bottom < 1e-5);
		}

		TEST_METHOD(BasisFromDirection)
		{
			glm::vec3 dir(1.4,0,0);
			glm::mat3 basis = BoundingBoxUtil::BasisFromDirection(dir);
			Assert::IsTrue(glm::normalize(dir) == basis[0]), LINE_INFO();
			Assert::IsTrue(glm::vec3(0, 1, 0) == basis[1]), LINE_INFO();
			Assert::IsTrue(glm::vec3(0, 0, 1) == basis[2]), LINE_INFO();
			Assert::AreEqual(1.f, glm::length(basis[0]), L"Basis vector not normalized", LINE_INFO());
			Assert::AreEqual(1.f, glm::length(basis[1]), L"Basis vector not normalized", LINE_INFO());
			Assert::AreEqual(1.f, glm::length(basis[2]), L"Basis vector not normalized", LINE_INFO());

			dir = glm::vec3(2, 1, 2);
			basis = BoundingBoxUtil::BasisFromDirection(dir);
			Assert::IsTrue(glm::normalize(dir) == basis[0]), LINE_INFO();
			Assert::IsTrue(glm::vec3( -0.235702261,0.942808986, -0.235702261) == basis[1]), LINE_INFO();

			const float eps = 1e-7f; 
			Assert::IsTrue(std::abs(glm::dot(basis[0], basis[1])) < eps, L"Basis vectors not normal", LINE_INFO());
			Assert::IsTrue(std::abs(glm::dot(basis[1], basis[2])) < eps, L"Basis vectors not normal", LINE_INFO());
			Assert::IsTrue(std::abs(glm::dot(basis[0], basis[2])) < eps, L"Basis vectors not normal", LINE_INFO());
			Assert::IsTrue(std::abs(1 - glm::length(basis[0])) < eps, L"Basis vector not normalized", LINE_INFO());
			Assert::IsTrue(std::abs(1 - glm::length(basis[1])) < eps, L"Basis vector not normalized", LINE_INFO());
			Assert::IsTrue(std::abs(1 - glm::length(basis[2])) < eps, L"Basis vector not normalized", LINE_INFO());
		}
	};

	TEST_CLASS(FrustumTest)
	{
		TEST_METHOD(OrthoFrustumCorners)
		{
			OrthogonalFrustum frust;
			frust.left = 1;
			frust.right = 7;
			frust.top = 5;
			frust.bottom = 2;
			frust.nearPlane = 3;
			frust.farPlane = 6;

			glm::vec3 p = glm::vec3(1, 2, 3);
			frust.position = p;

			//90 deg around y axis
			auto rmat = glm::mat3(glm::rotate(glm::mat4(1), glm::half_pi<float>(), glm::vec3(0, 1, 0)));
			frust.frame *= rmat;

			float eps = 1e-6f;

			auto corners = frust.CornerPoints();
			Assert::IsTrue(glm::length(corners[0] - p - glm::vec3(-frust.nearPlane, frust.top, -frust.right)) < eps), LINE_INFO();
			Assert::IsTrue(glm::length(corners[1] - p - glm::vec3(-frust.nearPlane, frust.top,  -frust.left)) < eps), LINE_INFO();
			Assert::IsTrue(glm::length(corners[2] - p - glm::vec3(-frust.nearPlane, frust.bottom,  -frust.right)) < eps), LINE_INFO();
			Assert::IsTrue(glm::length(corners[3] - p - glm::vec3(-frust.nearPlane, frust.bottom,  -frust.left)) < eps), LINE_INFO();
			Assert::IsTrue(glm::length(corners[4] - p - glm::vec3(-frust.farPlane, frust.top, -frust.right)) < eps), LINE_INFO();
			Assert::IsTrue(glm::length(corners[5] - p - glm::vec3(-frust.farPlane, frust.top, -frust.left)) < eps), LINE_INFO();
			Assert::IsTrue(glm::length(corners[6] - p - glm::vec3(-frust.farPlane, frust.bottom, -frust.right)) < eps), LINE_INFO();
			Assert::IsTrue(glm::length(corners[7] - p - glm::vec3(-frust.farPlane, frust.bottom, -frust.left)) < eps), LINE_INFO();
		}

		TEST_METHOD(PerspectiveFrustumCorners)
		{
			PerspectiveFrustum frust;
			frust.nearPlane = 3;
			frust.farPlane = 6;
			frust.fovY = 60;
			frust.aspectRatio = 0.5f;

			glm::vec3 p = glm::vec3(1, 2, 3);
			frust.position = p;

			float gk_y = sqrtf(3.f);
			float gk_x = 3.f * (2.f - sqrtf(3.f));

			//90 deg around y axis
			auto rmat = glm::mat3(glm::rotate(glm::mat4(1), glm::half_pi<float>(), glm::vec3(0, 1, 0)));
			frust.frame *= rmat;
			 
			float eps = 1e-6f;

			auto corners = frust.CornerPoints();
			Assert::IsTrue(glm::length(corners[0] - p - glm::vec3(-frust.nearPlane, gk_y, -gk_x)) < eps), LINE_INFO();
			Assert::IsTrue(glm::length(corners[1] - p - glm::vec3(-frust.nearPlane, gk_y, gk_x)) < eps), LINE_INFO();
			Assert::IsTrue(glm::length(corners[2] - p - glm::vec3(-frust.nearPlane, -gk_y, -gk_x)) < eps), LINE_INFO();
			Assert::IsTrue(glm::length(corners[3] - p - glm::vec3(-frust.nearPlane, -gk_y, gk_x)) < eps), LINE_INFO();
			Assert::IsTrue(glm::length(corners[4] - p - glm::vec3(-frust.farPlane, 2.f * gk_y, 2.f *-gk_x)) < eps), LINE_INFO();
			Assert::IsTrue(glm::length(corners[5] - p - glm::vec3(-frust.farPlane, 2.f * gk_y, 2.f *gk_x)) < eps), LINE_INFO();
			Assert::IsTrue(glm::length(corners[6] - p - glm::vec3(-frust.farPlane, 2.f * -gk_y, 2.f * -gk_x)) < eps), LINE_INFO();
			Assert::IsTrue(glm::length(corners[7] - p - glm::vec3(-frust.farPlane, 2.f * -gk_y, 2.f * gk_x)) < eps), LINE_INFO();
		}
	};
}