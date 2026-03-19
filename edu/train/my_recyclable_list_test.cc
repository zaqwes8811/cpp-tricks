#include <gtest/gtest.h>

#include <iostream>
#include <iterator>
#include <list>
#include <memory_resource>
#include <type_traits>

template <typename T>
class RecyclableQueue final {
public:
    RecyclableQueue(int StartSize = 64)
        : start_size_{StartSize},
          arena_(start_size_ * sizeof(T) * 4),
          pool_{std::data(arena_), std::size(arena_)},
          list_(&pool_),
          reusable_list_{&pool_} {}

    void Push(T data) {
        if (std::empty(reusable_list_)) {
            list_.push_back(std::move(data));
            return;
        }

        auto it = std::begin(reusable_list_);
        *it = std::move(data);
        list_.splice(list_.end(), reusable_list_, it);
    }

    bool empty() const { return list_.empty(); }

    decltype(auto) Pop() {
        auto it = std::begin(list_);
        auto data = std::move(*it);
        reusable_list_.splice(reusable_list_.end(), list_, it);
        return data;
    }

private:
    int start_size_;
    std::vector<uint8_t> arena_;
    std::pmr::monotonic_buffer_resource pool_;
    std::pmr::list<T> list_;
    std::pmr::list<T> reusable_list_;
};

TEST(RecyclableQueueTest, Creation) {
    RecyclableQueue<int> q;

    q.Push(1);
    q.Push(2);

    EXPECT_EQ(q.Pop(), 1);

    q.Push(3);

    EXPECT_EQ(q.Pop(), 2);

    q.Push(4);
    q.Push(5);

    q.Pop();
    q.Push(6);
}

template <typename T>
class Matrix {
    struct BoolWrapper {
        bool value;
    };

    using U = typename std::conditional<std::is_same_v<T, bool>, BoolWrapper, T>::type;

    std::vector<U> buffer_;
    int rows_;
    int cols_;

public:
    Matrix(int rows, int cols) : buffer_(rows * cols), rows_(rows), cols_(cols) {}

    struct Row {
        Row(typename std::vector<U>::iterator it) : row_it{it} {}

        T& operator[](int col) && {
            std::advance(row_it, col);
            if constexpr (std::is_same_v<T, bool>) {
                return row_it->value;
            } else {
                return *row_it;
            }
        }

    private:
        typename std::vector<U>::iterator row_it;
    };

    Row operator[](int row) { return Row(std::begin(buffer_) + row * cols_); }
};

TEST(MatrixTest, Creation) {
    auto m = Matrix<int>(3, 2);
    int& v = m[0][0];
    v = 1;
    EXPECT_EQ(1, m[0][0]);
}

TEST(MatrixTest, Bool) {
    auto m = Matrix<bool>(3, 2);
    bool& v = m[0][0];
    v = true;
    EXPECT_EQ(true, m[0][0]);
}