#include <vector>
#include <algorithm>
#include <random>

#include "Range/Range.h"
#include "gtest/gtest.h"


namespace
{
    struct RangeTest : public ::testing::Test
    {
        virtual void SetUp () {}

        virtual void TearDown () {}

        virtual ~RangeTest () {}



        template <typename T, class Range>
        void loopCheck (T first, T last, T step, Range range, bool closed = false)
        {
            //SCOPED_TRACE(loopCheck)

            std::vector<T> regularLoop, rangeLoop;

            T finish = (last - first) * step;

            for(auto x = first; closed ? finish >= 0 : finish > 0; x += step, finish = (last - x) * step)
                regularLoop.push_back(x);

            for(auto x : range)
                rangeLoop.push_back(x);

            expectVector(regularLoop, rangeLoop);
        }


        template <typename T, std::enable_if_t<std::is_integral<std::decay_t<T>>::value, int> = 0>
        void expectVector (const std::vector<T>& v, const std::vector<T>& u)
        {
            EXPECT_EQ(v, u);
        }

        template <typename T, std::enable_if_t<std::is_floating_point<std::decay_t<T>>::value, int> = 0>
        void expectVector (const std::vector<T>& v, const std::vector<T>& u)
        {
            for(int i = 0; i < v.size(); ++i)
                EXPECT_NEAR(v[i], u[i], 1e-8);
        }


        template <class Range>
        void rangeLoopInt (bool closed)
        {
            //SCOPED_TRACE(RangeTestInt);

            loopCheck(0, 10, 1, Range(10), closed);
            loopCheck(0, 10, 1, Range(0, 10), closed);
            loopCheck(0, 10, 1, Range(0, 10, 1), closed);
    
            loopCheck(0, -10, -1, Range(-10), closed);
            loopCheck(0, -10, -1, Range(0, -10), closed);
            loopCheck(0, -10, -1, Range(0, -10, -1), closed);
    
            loopCheck(-10, -20, -1, Range(-10, -20), closed);
            loopCheck(-10, -20, -1, Range(-10, -20, -1), closed);
    
            loopCheck(0, 10, 3, Range(0, 10, 3), closed);
            loopCheck(10, -5, -2, Range(10, -5, -2), closed);
            loopCheck(-10, 3, 7, Range(-10, 3, 7), closed);
            loopCheck(-1, -5, -100, Range(-1, -5, -100), closed);
        }
    };


    TEST_F(RangeTest, HalfClosedRangeLoopInt)
    {
        //SCOPED_TRACE(HalfClosedRangeLoopInt);

        rangeLoopInt<handy::Range<int, handy::impl::HalfClosedInterval>>(false);
    }

    TEST_F(RangeTest, ClosedRangeLoopInt)
    {
        //SCOPED_TRACE(ClosedRangeLoopInt);

        rangeLoopInt<handy::Range<int, handy::impl::ClosedInterval>>(true);
    }
}