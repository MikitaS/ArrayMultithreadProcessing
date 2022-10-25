#include <gtest/gtest.h>
#include <ctime>
#include <cstdlib>

#include "array_processor.h"

static constexpr size_t CPU_AMOUNT = 4;
static constexpr size_t TEST_THRESHOLD = 1000000;

TEST(ArrayProcessor, SpansChecker)
{
    std::vector<int> vector;
    for (size_t i = 0; i < TEST_THRESHOLD; ++i) {
        vector.push_back(i);
    }

    ArrayProcessing<CPU_AMOUNT> ap(vector);
    for (size_t i = 0; i < TEST_THRESHOLD; ++i) {
        ASSERT_EQ(ap.GetSpans()[i / (TEST_THRESHOLD / CPU_AMOUNT)]
                        [i % (TEST_THRESHOLD / CPU_AMOUNT)], static_cast<int>(i));
    }
    size_t total_size = 0;
    for (size_t i = 0; i < CPU_AMOUNT; ++i) {
        total_size += ap.GetSpans()[i].size;
    }
    ASSERT_EQ(total_size, vector.size());
}

TEST(ArrayProcessor, CheckMedian)
{
    std::vector<int> vector;
    for (size_t i = 0; i < TEST_THRESHOLD; ++i) {
        vector.push_back(i);
    }

    ArrayProcessing<1> ap(vector);
    ap.CountMedian(1);
    ASSERT_EQ(ap.GetMedians()[1], TEST_THRESHOLD / 2);
}

TEST(ArrayProcessor, Check4Medians)
{
    std::vector<int> vector {55,4,10,12,33,2,15,99,4,12,44,23};

    ArrayProcessing<4> ap(vector);
    ap.CountMedian(1);
    ap.CountMedian(2);
    ap.CountMedian(3);
    ap.CountMedian(4);
    ASSERT_EQ(ap.GetMedians()[0], INT_MIN);
    ASSERT_EQ(ap.GetMedians()[1], 10);
    ASSERT_EQ(ap.GetMedians()[2], 12);
    ASSERT_EQ(ap.GetMedians()[3], 15);
    ASSERT_EQ(ap.GetMedians()[4], 23);
    ASSERT_EQ(ap.GetMedians()[5], INT_MAX);
}

TEST(ArrayProcessor, CheckResult)
{
    std::vector<int> vector {55,4,10,12,33,2,15,99,4,12,44,23};

    ArrayProcessing<4> ap(vector);
    ap.CountMedian(1);
    ap.CountMedian(2);
    ap.CountMedian(3);
    ap.CountMedian(4);

    ap.ProcessSpan(1);
    ap.ProcessSpan(2);
    ap.ProcessSpan(3);
    ap.ProcessSpan(4);

    ASSERT_EQ(ap.GetProcessedSpans()[0], std::vector<int>());
    ASSERT_EQ(ap.GetProcessedSpans()[1], std::vector<int>({4, 10}));
    ASSERT_EQ(ap.GetProcessedSpans()[2], std::vector<int>({12}));
    ASSERT_EQ(ap.GetProcessedSpans()[3], std::vector<int>({15}));
    ASSERT_EQ(ap.GetProcessedSpans()[4], std::vector<int>({23, 44}));

    std::vector<int> out_buffer;
    ap.ConcatinateAll(&out_buffer);
    ASSERT_EQ(out_buffer, std::vector<int>({4, 10, 12, 15, 23, 44}));
}

TEST(ArrayProcessor, TestSingleThread)
{
    std::vector<int> vector {55,4,10,12,33,2,15,99,4,12,44,23};

    ArrayProcessingSingleThread<CPU_AMOUNT> ap(vector);

    std::vector<int> out_buffer;
    ap.ProcessArray(&out_buffer);
    ASSERT_EQ(out_buffer, std::vector<int>({4, 10, 12, 15, 23, 44}));
}

TEST(ArrayProcessor, TestMultiThread)
{
    std::vector<int> vector {55,4,10,12,33,2,15,99,4,12,44,23};

    ArrayProcessingMultiThread<CPU_AMOUNT> ap(vector);

    std::vector<int> out_buffer;
    ap.ProcessArray(&out_buffer);
    ASSERT_EQ(out_buffer, std::vector<int>({4, 10, 12, 15, 23, 44}));
}

// Testing that single thread and multithread have the same result
TEST(ArrayProcessor, BigCompatibilityTest)
{
    std::srand(std::time(nullptr));

    std::vector<int> vector;

    for (size_t i = 0; i < TEST_THRESHOLD; ++i) {
        vector.push_back(std::rand());
    }

    std::vector<int> out_buffer_single;
    std::vector<int> out_buffer_multi;

    ArrayProcessingSingleThread<CPU_AMOUNT> ap_s(vector);
    ap_s.ProcessArray(&out_buffer_single);
    ArrayProcessingMultiThread<CPU_AMOUNT> ap_m(vector);
    ap_m.ProcessArray(&out_buffer_multi);
    ASSERT_EQ(out_buffer_single, out_buffer_multi);

    ArrayProcessingSingleThread<CPU_AMOUNT - 1> ap_s2(vector);
    ap_s.ProcessArray(&out_buffer_single);
    ArrayProcessingMultiThread<CPU_AMOUNT - 1> ap_m2(vector);
    ap_m.ProcessArray(&out_buffer_multi);
    ASSERT_EQ(out_buffer_single, out_buffer_multi);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}