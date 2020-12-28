/*------------------------------------
  ///\  Plywood C++ Framework
  \\\/  https://plywood.arc80.com/
------------------------------------*/
#include <ply-math/Base.h>
#include <ply-test/TestSuite.h>

namespace ply {

#define PLY_TEST_CASE_PREFIX Float2_

PLY_TEST_CASE("Float2 constructors") {
    Float2 v = {1};
    PLY_TEST_CHECK(v.x == 1 && v.y == 1);
    Float2 v2 = {1, 2};
    PLY_TEST_CHECK(v2.x == 1 && v2.y == 2);
    Float2 v3 = v2;
    PLY_TEST_CHECK(v3.x == 1 && v3.y == 2);
}

PLY_TEST_CASE("Float2 conversions") {
    auto v = Float2{1, 2}.to<IntVec2>();
    PLY_TEST_CHECK(v.x == 1 && v.y == 2);
    auto v2 = Float2{1, 2}.to<Int2<s16>>();
    PLY_TEST_CHECK(v2.x == 1 && v2.y == 2);
}

PLY_TEST_CASE("Float2 copy assignment") {
    Float2 v;
    v = Float2{1, 2};
    PLY_TEST_CHECK(v.x == 1 && v.y == 2);
}

PLY_TEST_CASE("Float2 comparisons") {
    PLY_TEST_CHECK(Float2{1, 2} == Float2{1, 2});
    PLY_TEST_CHECK(Float2{0, 0} == Float2{0, -0.f});
    PLY_TEST_CHECK(Float2{1, 2} != Float2{2, 1});
    PLY_TEST_CHECK(!(Float2{1, 2} != Float2{1, 2}));
    PLY_TEST_CHECK(!(Float2{0, 0} != Float2{0, -0.f}));
    PLY_TEST_CHECK(!(Float2{1, 2} == Float2{2, 1}));
}

PLY_TEST_CASE("Float2 unary negation") {
    Float2 v = -Float2{1, 2};
    PLY_TEST_CHECK(v.x == -1 && v.y == -2);
}

PLY_TEST_CASE("Float2 addition") {
    Float2 v = Float2{1, 2} + Float2{3, 4};
    PLY_TEST_CHECK(v.x == 4 && v.y == 6);
    Float2 v2 = Float2{1, 2} + 3;
    PLY_TEST_CHECK(v2.x == 4 && v2.y == 5);
    Float2 v3 = 3 + Float2{1, 2};
    PLY_TEST_CHECK(v3.x == 4 && v3.y == 5);
    v3 += Float2{1, 2};
    PLY_TEST_CHECK(v3.x == 5 && v3.y == 7);
    v3 += 1;
    PLY_TEST_CHECK(v3.x == 6 && v3.y == 8);
}

PLY_TEST_CASE("Float2 subtraction") {
    Float2 v = Float2{3, 4} - Float2{1, 2};
    PLY_TEST_CHECK(v.x == 2 && v.y == 2);
    Float2 v2 = Float2{2, 3} - 1;
    PLY_TEST_CHECK(v2.x == 1 && v2.y == 2);
    Float2 v3 = 6 - Float2{1, 2};
    PLY_TEST_CHECK(v3.x == 5 && v3.y == 4);
    v3 -= Float2{1, 2};
    PLY_TEST_CHECK(v3.x == 4 && v3.y == 2);
    v3 -= 1;
    PLY_TEST_CHECK(v3.x == 3 && v3.y == 1);
}

PLY_TEST_CASE("Float2 component-wise multiplication") {
    Float2 v = Float2{1, 2} * Float2{3, 4};
    PLY_TEST_CHECK(v.x == 3 && v.y == 8);
    Float2 v2 = Float2{1, 2} * 3;
    PLY_TEST_CHECK(v2.x == 3 && v2.y == 6);
    Float2 v3 = 3 * Float2{1, 2};
    PLY_TEST_CHECK(v3.x == 3 && v3.y == 6);
    v3 *= Float2{1, 2};
    PLY_TEST_CHECK(v3.x == 3 && v3.y == 12);
    v3 *= 2;
    PLY_TEST_CHECK(v3.x == 6 && v3.y == 24);
}

PLY_TEST_CASE("Float2 component-wise division") {
    Float2 v = Float2{2, 6} / Float2{2, 3};
    PLY_TEST_CHECK(v.x == 1 && v.y == 2);
    Float2 v2 = Float2{4, 6} / 2;
    PLY_TEST_CHECK(v2.x == 2 && v2.y == 3);
    Float2 v3 = 8 / Float2{4, 2};
    PLY_TEST_CHECK(v3.x == 2 && v3.y == 4);
    v3 /= Float2{1, 2};
    PLY_TEST_CHECK(v3.x == 2 && v3.y == 2);
    v3 /= 2;
    PLY_TEST_CHECK(v3.x == 1 && v3.y == 1);
}

PLY_TEST_CASE("Float2 lengths") {
    PLY_TEST_CHECK(Float2{3, 4}.length2() == 25);
    PLY_TEST_CHECK(Float2{3, 4}.length() == 5);
    PLY_TEST_CHECK(Float2{1, 0}.isUnit());
    PLY_TEST_CHECK(!Float2{1, 1}.isUnit());
    PLY_TEST_CHECK(Float2{999, 666}.normalized().isUnit());
    PLY_TEST_CHECK(Float2{0}.safeNormalized().isUnit());
}

PLY_TEST_CASE("Float2 swizzles") {
    Float2 v = {4, 5};
    PLY_TEST_CHECK(v.swizzle(1, 0) == Float2{5, 4});
    PLY_TEST_CHECK(v.swizzle(1, 0, 1) == Float3{5, 4, 5});
    PLY_TEST_CHECK(v.swizzle(1, 1, 1, 0) == Float4{5, 5, 5, 4});
}

PLY_TEST_CASE("Float2 dot product") {
    Float2 v1 = {2, 3};
    Float2 v2 = {4, 5};
    PLY_TEST_CHECK(dot(v1, v2) == 23);
    PLY_TEST_CHECK(v1.length2() == dot(v1, v1));
}

PLY_TEST_CASE("Float2 cross product") {
    PLY_TEST_CHECK(cross(Float2{1, 0}, Float2{0, 1}) == 1.f);
    PLY_TEST_CHECK(cross(Float2{1, 0}, Float2{1, 0}) == 0.f);
    PLY_TEST_CHECK(cross(Float2{2, 2}, Float2{0, 4}) == 8);
    PLY_TEST_CHECK(cross(Float2{2, 2}, Float2{4, 0}) == -8);
}

PLY_TEST_CASE("Float2 clamp") {
    PLY_TEST_CHECK(clamp(Float2{2, 2}, 0, 1) == Float2{1, 1});
    PLY_TEST_CHECK(clamp(Float2{2, 0.5f}, 0, 1) == Float2{1, 0.5f});
    PLY_TEST_CHECK(clamp(Float2{-1, -1}, 0, 1) == Float2{0, 0});
    PLY_TEST_CHECK(clamp(Float2{3, 4}, Float2{0, 1}, Float2{1, 2}) == Float2{1, 2});
    PLY_TEST_CHECK(clamp(Float2{3, 1.5f}, Float2{0, 1}, Float2{1, 2}) == Float2{1, 1.5f});
    PLY_TEST_CHECK(clamp(Float2{-1, -1}, Float2{0, 1}, Float2{1, 2}) == Float2{0, 1});
}

PLY_TEST_CASE("Float2 abs") {
    PLY_TEST_CHECK(abs(Float2{1, 1}) == Float2{1, 1});
    PLY_TEST_CHECK(abs(Float2{-1, -1}) == Float2{1, 1});
    PLY_TEST_CHECK(abs(Float2{-2, 3}) == Float2{2, 3});
    PLY_TEST_CHECK(abs(Float2{2, -3}) == Float2{2, 3});
}

PLY_TEST_CASE("Float2 min") {
    PLY_TEST_CHECK(min(Float2{1, 0}, Float2{0, 1}) == Float2{0, 0});
    PLY_TEST_CHECK(min(Float2{0, 1}, Float2{1, 0}) == Float2{0, 0});
    PLY_TEST_CHECK(min(Float2{2, 2}, Float2{3, 3}) == Float2{2, 2});
    PLY_TEST_CHECK(min(Float2{3, 3}, Float2{2, 2}) == Float2{2, 2});
}

PLY_TEST_CASE("Float2 max") {
    PLY_TEST_CHECK(max(Float2{1, 0}, Float2{0, 1}) == Float2{1, 1});
    PLY_TEST_CHECK(max(Float2{0, 1}, Float2{1, 0}) == Float2{1, 1});
    PLY_TEST_CHECK(max(Float2{2, 2}, Float2{3, 3}) == Float2{3, 3});
    PLY_TEST_CHECK(max(Float2{3, 3}, Float2{2, 2}) == Float2{3, 3});
}

PLY_TEST_CASE("Float2 allLess") {
    PLY_TEST_CHECK(allLess(Float2{-1, -1}, 0));
    PLY_TEST_CHECK(!allLess(Float2{1, -1}, 0));
    PLY_TEST_CHECK(!allLess(Float2{-1, 1}, 0));
    PLY_TEST_CHECK(!allLess(Float2{1, 1}, 0));
    PLY_TEST_CHECK(!allLess(Float2{0, 0}, 0));
    PLY_TEST_CHECK(allLess(Float2{0, 1}, Float2{1, 2}));
    PLY_TEST_CHECK(!allLess(Float2{2, 1}, Float2{1, 2}));
    PLY_TEST_CHECK(!allLess(Float2{0, 3}, Float2{1, 2}));
    PLY_TEST_CHECK(!allLess(Float2{2, 3}, Float2{1, 2}));
    PLY_TEST_CHECK(!allLess(Float2{1, 2}, Float2{1, 2}));
}

PLY_TEST_CASE("Float2 allLessOrEqual") {
    PLY_TEST_CHECK(allLessOrEqual(Float2{-1, -1}, 0));
    PLY_TEST_CHECK(!allLessOrEqual(Float2{1, -1}, 0));
    PLY_TEST_CHECK(!allLessOrEqual(Float2{-1, 1}, 0));
    PLY_TEST_CHECK(!allLessOrEqual(Float2{1, 1}, 0));
    PLY_TEST_CHECK(allLessOrEqual(Float2{0, 0}, 0));
    PLY_TEST_CHECK(allLessOrEqual(Float2{0, 1}, Float2{1, 2}));
    PLY_TEST_CHECK(!allLessOrEqual(Float2{2, 1}, Float2{1, 2}));
    PLY_TEST_CHECK(!allLessOrEqual(Float2{0, 3}, Float2{1, 2}));
    PLY_TEST_CHECK(!allLessOrEqual(Float2{2, 3}, Float2{1, 2}));
    PLY_TEST_CHECK(allLessOrEqual(Float2{1, 2}, Float2{1, 2}));
}

PLY_TEST_CASE("Float2 quantizeNearest") {
    PLY_TEST_CHECK(quantizeNearest(Float2{0.3f, 1.4f}, 2) == Float2{0, 2});
    PLY_TEST_CHECK(quantizeNearest(Float2{-0.3f, 1.4f}, 1) == Float2{0, 1});
    PLY_TEST_CHECK(quantizeNearest(Float2{0.3f, -1.4f}, 1) == Float2{0, -1});
    PLY_TEST_CHECK(quantizeNearest(Float2{-0.3f, 1.4f}, 0.5f) == Float2{-0.5f, 1.5f});
}

PLY_TEST_CASE("Float2 quantizeUp") {
    PLY_TEST_CHECK(quantizeUp(Float2{0.3f, 1.4f}, 2) == Float2{2, 2});
    PLY_TEST_CHECK(quantizeUp(Float2{-0.3f, 1.4f}, 1) == Float2{0, 2});
    PLY_TEST_CHECK(quantizeUp(Float2{0.3f, -1.4f}, 1) == Float2{1, -1});
    PLY_TEST_CHECK(quantizeUp(Float2{-0.3f, 1.4f}, 0.5f) == Float2{0, 1.5f});
}

PLY_TEST_CASE("Float2 quantizeDown") {
    PLY_TEST_CHECK(quantizeDown(Float2{0.3f, 1.4f}, 2) == Float2{0, 0});
    PLY_TEST_CHECK(quantizeDown(Float2{-0.3f, 1.4f}, 1) == Float2{-1, 1});
    PLY_TEST_CHECK(quantizeDown(Float2{0.3f, -1.4f}, 1) == Float2{0, -2});
    PLY_TEST_CHECK(quantizeDown(Float2{-0.3f, 1.4f}, 0.5f) == Float2{-0.5f, 1});
}

PLY_TEST_CASE("Float2 isQuantized") {
    PLY_TEST_CHECK(isQuantized(Float2{0, 1}, 1));
    PLY_TEST_CHECK(!isQuantized(Float2{0.5f, 0}, 1));
    PLY_TEST_CHECK(!isQuantized(Float2{1, -0.5}, 1));
    PLY_TEST_CHECK(isQuantized(Float2{0, 1}, 0.5f));
    PLY_TEST_CHECK(isQuantized(Float2{0.5f, -0.f}, 0.5f));
    PLY_TEST_CHECK(isQuantized(Float2{1, -0.5}, 0.5f));
    PLY_TEST_CHECK(!isQuantized(Float2{-0.5f, 0.3f}, 0.5f));
    PLY_TEST_CHECK(!isQuantized(Float2{0, 1}, 2));
    PLY_TEST_CHECK(isQuantized(Float2{4, 8}, 2));
}

} // namespace ply

int main() {
    return ply::test::run() ? 0 : 1;
}
