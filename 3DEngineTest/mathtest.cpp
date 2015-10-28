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

		TEST_METHOD(Hull)
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
		TEST_METHOD(DirectionalLightFrustrumOrtho)
		{
			AABBox box;
			box.d = glm::vec3(1, 2, 3);
			glm::vec3 ldir(0, -1, 0);
			glm::vec3 lpos;

			float l, r, u, d, n, f;
			Assert::IsTrue(BoundingBoxUtil::DirectionalLightFrustrum(box, ldir, lpos, l, r, u, d, n, f),L"", LINE_INFO());
			Assert::IsTrue(glm::vec3(0,2,0) == lpos), LINE_INFO();
			Assert::AreEqual(0.f, n);
			Assert::AreEqual(4.f, f);
			Assert::AreEqual(-3.f, l);
			Assert::AreEqual(3.f, r);
			Assert::AreEqual(1.f, u);
			Assert::AreEqual(-1.f, d);
		}
	};
}