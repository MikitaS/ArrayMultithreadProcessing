#include <climits>
#include <vector>
#include <array>
#include <cassert>

#include <bits/stdc++.h>

struct ArrSubSpan
{
    int operator [](size_t i) const
    {
        assert(i < size);
        return data[i];
    }
    int *data;
    size_t size;
};

template <size_t N>
class ArrayProcessing {
public:

    // CPU counter starts with 1, on 0 index we have INT_MIN
    using MediansArray = std::array<int, N + 2>;
    using SpansArray = std::array<ArrSubSpan, N>;
    using ResultVectorArray = std::array<std::vector<int>, N + 1>;

    explicit ArrayProcessing(const std::vector<int> &array) : array_(array)
    {
        if (array_.size() < N) {
            // can't divide array
            return;
        }
        const size_t span_lenght = array_.size() / N;
        // Divide array for spans
        size_t i = 0;
        for (; i < N - 1; ++i) {
            ArrSubSpan s_span = {&array_[i * span_lenght], span_lenght};
            spans_[i] = s_span;
        }
        // The last span might have another lenght
        ArrSubSpan s_span = {&array_[i * span_lenght], array_.size() - i * span_lenght};
        spans_[i] = s_span;

        medians_[0] = INT_MIN;
        medians_[N + 1] = INT_MAX;
    }

    // Count median of n-th array
    void CountMedian(size_t cpu_num)
    {
        assert(cpu_num != 0 && cpu_num <= N);

        int *pointer = spans_[cpu_num - 1].data;
        const size_t size = spans_[cpu_num - 1].size;

        std::sort(pointer, pointer + size);

        medians_[cpu_num] = pointer[size / 2];
        return;
    }

    // Find elements in bounds of 2 neighbour medians 
    void ProcessSpan(size_t cpu_num)
    {
        assert(cpu_num != 0 && cpu_num <= N);

        const int *pointer = spans_[cpu_num - 1].data;
        const size_t size = spans_[cpu_num - 1].size;

        const int LOWER_MEDIAN = medians_[cpu_num - 1];
        const int UPPER_MEDIAN = medians_[cpu_num + 1];

        for (size_t i = 0; i < size; ++i) {
            if (LOWER_MEDIAN < pointer[i] && pointer[i] < UPPER_MEDIAN) {
                results_[cpu_num].push_back(pointer[i]);
            }
        }
    }

    void ConcatinateAll(std::vector<int> *out_buffer)
    {
        out_buffer->clear();
        for (size_t i = 1; i < N + 1; ++i) {
            out_buffer->insert(out_buffer->end(), results_[i].begin(), results_[i].end());
        }
    }

    const SpansArray& GetSpans() const
    {
        return spans_; 
    }

    const MediansArray& GetMedians() const
    {
        return medians_; 
    }

    const ResultVectorArray& GetProcessedSpans() const
    {
        return results_; 
    }

protected:
    // 0 and last elemants are INT_MIN and INT_MAX
    MediansArray medians_;
    SpansArray spans_;
    ResultVectorArray results_;
    std::vector<int> array_;
};

/* Single thread*/
template <size_t N>
class ArrayProcessingSingleThread final : public ArrayProcessing<N> {
public:
    explicit ArrayProcessingSingleThread(const std::vector<int> &array) : ArrayProcessing<N>(array) {}

    void ProcessArray(std::vector<int> *out_buffer)
    {
        for (size_t i = 1; i < N + 1; ++i) {
            this->CountMedian(i);
        }

        for (size_t i = 1; i < N + 1; ++i) {
            this->ProcessSpan(i);
        }

        this->ConcatinateAll(out_buffer);
    }
};

/* 4 threads */
template <size_t N>
class ArrayProcessingMultiThread final : public ArrayProcessing<N> {
public:
    using ThreadsArray = std::array<std::thread *, N>;
    explicit ArrayProcessingMultiThread(const std::vector<int> &array) : ArrayProcessing<N>(array) {}

    void ProcessArray(std::vector<int> *out_buffer)
    {
        ThreadsArray threads;
        for (size_t i = 1; i < N + 1; ++i) {
            threads[i - 1] = new std::thread(&ArrayProcessing<N>::CountMedian, this, i);
        }

        for (size_t i = 0; i < N; ++i) {
            threads[i]->join();
            delete threads[i];
        }

        for (size_t i = 1; i < N + 1; ++i) {
            threads[i - 1] = new std::thread(&ArrayProcessing<N>::ProcessSpan, this, i);
        }

        for (size_t i = 0; i < N; ++i) {
            threads[i]->join();
            delete threads[i];
        }

        this->ConcatinateAll(out_buffer);
    }
};