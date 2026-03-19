//
// Created by zaqwes on 17.09.2022.
//

#pragma once

#include <map>
#include <string>
#include <vector>

#include "mission-critical-sw-experiments/tests/arena.hpp"

namespace leetcode {
using namespace std;

// TODO(me) RVO check

string convert_ref(string s, int numRows) {
    if (numRows == 1) return s;

    vector<string> rows(min(numRows, int(s.size())));
    int curRow = 0;
    bool goingDown = false;

    for (char c : s) {
        rows[curRow] += c;
        if (curRow == 0 || curRow == numRows - 1) goingDown = !goingDown;
        curRow += goingDown ? 1 : -1;
    }

    string ret;
    for (string row : rows) ret += row;
    return ret;
}

string convert_base(string s, int numRows) {
    // Idea: Add some to s in order to align it, store size. It can break alg

    // string result;
    if (numRows == 1) {
        // result.swap(s);
        return s;  // result;  // RVO?
    }

    static const auto sso_capacity = std::string().capacity();

    const auto ssz = s.size();

    static constexpr auto maybeReserve = [&](string& s, int sz) {
        if (sz > sso_capacity) {
            s.reserve(sz);
        }
    };

    // Idea: Can reuse s, but no RVO
    // strings
    // https://habr.com/ru/company/oleg-bunin/blog/352280/

    // Idea: allocate close. Automatically with SSO
    string* row_to_slice = new string[numRows];

    const int kStride = (numRows - 1) * 2;
    //    const int kMaxStrideLength = ssz / kStride + 1;
    const int kMaxStrideLength = ssz / numRows + 1;

    // Preallocate
    maybeReserve(row_to_slice[0], ssz);  // First one will be result
    for (auto i = 1; i < numRows; ++i) {
        maybeReserve(row_to_slice[i], kMaxStrideLength);
    }

    const int size = [&]() {
        int size = 0;
        if (ssz % kStride) {
            size = ssz / kStride + 1;  // had bug really, it was protected by condition inside
        } else {
            size = ssz / kStride;
        }
        return size;
    }();

    // Idea: Split to boxes
    // Idea: Prefect part
    for (auto iter = 0; iter < size; ++iter) {
        int counter = 0;
        int direction = 1;

        // char buf[kStride];
        // for (int subidx = 0; subidx < kStride; ++subidx) {
        //     const auto internal_ptr = iter * kStride + subidx;
        //     if (internal_ptr >= ssz) {
        //         break;
        //     }
        //     buf[subidx] = s[internal_ptr];
        // }
        for (int subidx = 0; subidx < kStride; ++subidx) {
            const auto internal_ptr = iter * kStride + subidx;
            if (internal_ptr >= ssz) {
                break;
            }

            if (subidx == numRows) {
                counter -= 2;
                direction = -1;
            }

            // Idea: It's not cache write efficiton it's good idea to add two at same time
            //   But it's read cache efficient
            row_to_slice[counter] += s[internal_ptr];

            counter += direction;
        }
    }

    // SSO guess. With SSO reserver is pessimization
    string result = std::move(row_to_slice[0]);

    for (auto i = 1; i < numRows; ++i) {
        result += row_to_slice[i];  // no copy needed if we store directly
    }

    delete[] row_to_slice;

    return result;
}

string convert_best(string s, int numRows) {
    // Idea: Add some to s in order to align it, store size. It can break alg

    // string result;
    if (numRows == 1) {
        return s;
        //        auto r = s;
        //        return r;
    }

    static const auto sso_capacity = std::string().capacity();

    const auto ssz = s.size();

    static constexpr auto maybeReserve = [&](string& s, int sz) {
        if (sz > sso_capacity) {
            s.reserve(sz);
        }
    };

    // Idea: Can reuse s, but no RVO
    // strings
    // https://habr.com/ru/company/oleg-bunin/blog/352280/

    // Idea: allocate close. Automatically with SSO
    string* row_to_slice = new string[numRows - 1];

    const int kStride = (numRows - 1) * 2;
    const int kMaxStrideLength = ssz / kStride + 1;

    // Preallocate
    // maybeReserve(row_to_slice[0], ssz);  // First one will be result
    for (auto i = 0; i < numRows - 1; ++i) {
        maybeReserve(row_to_slice[i], kMaxStrideLength);
    }

    int size = 0;
    if (ssz % kStride) {
        size = ssz / kStride + 1;  // had bug really, it was protected by condition inside
    } else {
        size = ssz / kStride;
    }

    // Idea: Split to boxes
    // Idea: Prefect part
    int in_place_counter = 0;
    auto siter = s.begin();
    auto send = s.cend();

    // TODO() Rangebase loop
    for (auto iter = 0; iter < size; ++iter) {
        int counter = 0;
        int direction = 1;

        for (int subidx = 0; subidx < kStride; ++subidx) {
            // const auto internal_ptr = iter * kStride + subidx;
            // if (internal_ptr >= ssz) {
            //     break;
            // }

            if (siter == send) {
                break;
            }

            if (subidx == numRows) {
                counter -= 2;
                direction = -1;
            }

            // Idea: It's not cache write efficiton it's good idea to add two at same time
            //   But it's read cache efficient
            const auto current_letter = *siter;  // s[internal_ptr];
            // ++internal_ptr;
            ++siter;
            if (counter == 0) {
                s[in_place_counter] = current_letter;
                // *wr_iter = current_letter;  // ? why doesn't work. Iter inavalidation?
                ++in_place_counter;
                // ++wr_iter;
            } else {
                row_to_slice[counter - 1] += current_letter;
            }

            counter += direction;
        }
    }

    // s.insert(s.begin() + in_place_counter, row_to_slice[0].begin(), row_to_slice[0].end());
    //    s.resize(in_place_counter);
    //    for (auto i = 0; i < numRows - 1; ++i) {
    //        s.insert(s.end(), row_to_slice[i].begin(), row_to_slice[i].end());
    //    }

    s.replace(in_place_counter, row_to_slice[0].size(), row_to_slice[0]);
    in_place_counter += row_to_slice[0].size();
    for (auto i = 1; i < numRows - 1; ++i) {
        s.replace(in_place_counter, row_to_slice[i].size(), row_to_slice[i]);
        in_place_counter += row_to_slice[i].size();
    }

    delete[] row_to_slice;

    return s;
}

// TODO() arena
// TODO() split direct move and back
// TODO() don't use input string - less branches

string convert_forge(string s, int numRows) {
    // Idea: Expand s and use it expansion as storage - Too tricky
    if (numRows == 1) {
        return s;
    }

    static const auto sso_capacity = std::string().capacity();

    const auto ssz = s.size();

    static constexpr auto maybeReserve = [&](string& s, int sz) {
        if (sz > sso_capacity) {
            s.reserve(sz);
        }
    };

    string* row_to_slice = new string[numRows];  // Vectors very bad
                                                 //    string::iterator* iter_to_slice = new string::iterator[numRows];

    const int kStride = (numRows - 1) * 2;
    const int kMaxStrideLength = ssz / kStride + 1;

    // No re-allocations! It'll invalidate iterators
    maybeReserve(row_to_slice[0], ssz);  // First one will be result
    for (auto i = 1; i < numRows - 1; ++i) {
        maybeReserve(row_to_slice[i], kMaxStrideLength);
    }

    int subidx = 0;
    int counter = 0;
    int direction = 1;
    for (auto&& elem : s) {
        if (subidx == numRows) {
            counter -= 2;
            direction = -1;
        }

        row_to_slice[counter] += elem;

        counter += direction;

        ++subidx;
        if (subidx >= kStride) {
            subidx = 0;
            counter = 0;
            direction = 1;
        }
    }

    string& result = row_to_slice[0];  // std::move here is slow down 10%
    for (auto i = 1; i < numRows; ++i) {
        result += row_to_slice[i];
    }

    string r = std::move(result);

    delete[] row_to_slice;

    return r;
}

string convert_arena(string s, int numRows) {
    // Idea: Expand s and use it expansion as storage - Too tricky
    if (numRows == 1) {
        return s;
    }

    auto arena = VectorBasedArena{s.size() * 16};

    static const auto sso_capacity = ArenaString().capacity();

    const auto ssz = s.size();
    const int kStride = (numRows - 1) * 2;
    const int kMaxStrideLength = (ssz / kStride + 1);
    //    const int kMaxStrideLength = (ssz / kStride + 1) / numRows;

    static constexpr auto maybeReserve = [&](ArenaString& s, int sz) {
        if (sz > sso_capacity) {
            s.reserve(sz);
        }
    };

    // Create smartly
    // AVector<AString> split string into two parts
    // Vector<ptr>/map(Storage)/strings
    //    auto row_to_slice =
    //        ArenaVector<ArenaString*>(numRows, nullptr, ArenaAllocator<ArenaString*>{&arena});  // For fast access

    auto row_to_slice =
        ArenaVector<ArenaString>(numRows, ArenaString{AllocatorStr{&arena}}, ArenaAllocator<ArenaString>{&arena});

    //    using IdxToArenaStr = std::map<int, ArenaString, std::less<>, AllocatorIntStrPair>;
    //    auto a = static_cast<IdxToArenaStr*>(arena.alloc(sizeof(IdxToArenaStr), alignof(IdxToArenaStr)));
    //    auto allocator = AllocatorIntStrPair{&arena};
    //    auto raw_ptr = new (a) IdxToArenaStr(allocator);
    //    auto idx_to_str = std::unique_ptr<IdxToArenaStr, decltype(global_arena_deleter)>(raw_ptr,
    //    global_arena_deleter);
    //
    //    for (int i = 0; i < numRows; ++i) {
    //        auto inserted = idx_to_str->emplace_hint(idx_to_str->end(), std::piecewise_construct,
    //        std::forward_as_tuple(i),
    //                                                 std::forward_as_tuple(AllocatorStr{&arena}));  // Repalancing!
    //                                                 extra allocations
    ////        if (kMaxStrideLength >= inserted->second.capacity()) {
    ////            inserted->second.reserve(kMaxStrideLength);
    ////        }
    //
    //        row_to_slice[i] = &inserted->second;
    //    }

    for (auto&& slice : row_to_slice) {
        if (kMaxStrideLength >= slice.capacity()) {
            slice.reserve(kMaxStrideLength);
        }
    }

    int subidx = 0;
    int counter = 0;
    int direction = 1;
    for (auto&& elem : s) {
        if (subidx == numRows) {
            counter -= 2;
            direction = -1;
        }

        row_to_slice[counter] += elem;

        counter += direction;

        ++subidx;
        if (subidx >= kStride) {
            subidx = 0;
            counter = 0;
            direction = 1;
        }
    }

    // It's slow to get bytes back
    s.clear();
    s.assign(row_to_slice[0].begin(), row_to_slice[0].end());
    for (auto i = 1; i < numRows; ++i) {
        s.append(row_to_slice[i].begin(), row_to_slice[i].end());  // takes a lot of time
    }

    return s;
}

string convert_best1(string s, int numRows) {
    // Idea: Add some to s in order to align it, store size. It can break alg

    // string result;
    if (numRows == 1) {
        return s;
        //        auto r = s;
        //        return r;
    }

    static const auto sso_capacity = std::string().capacity();

    const auto ssz = s.size();

    static constexpr auto maybeReserve = [&](string& s, int sz) {
        if (sz > sso_capacity) {
            s.reserve(sz);
        }
    };

    // Idea: Can reuse s, but no RVO
    // strings
    // https://habr.com/ru/company/oleg-bunin/blog/352280/

    // Idea: allocate close. Automatically with SSO
    string* row_to_slice = new string[numRows - 1];

    const int kStride = (numRows - 1) * 2;
    const int kMaxStrideLength = ssz / kStride + 1;

    // Preallocate
    // maybeReserve(row_to_slice[0], ssz);  // First one will be result
    for (auto i = 0; i < numRows - 1; ++i) {
        maybeReserve(row_to_slice[i], kMaxStrideLength);
    }

    int size = 0;
    if (ssz % kStride) {
        size = ssz / kStride + 1;  // had bug really, it was protected by condition inside
    } else {
        size = ssz / kStride;
    }

    // Idea: Split to boxes
    // Idea: Prefect part
    int in_place_counter = 0;
    auto siter = s.begin();
    auto send = s.cend();

    // TODO() Rangebase loop
    for (auto iter = 0; iter < size; ++iter) {
        for (int subidx = 0; subidx < numRows; ++subidx) {
            if (siter == send) {
                break;
            }
            const auto current_letter = *siter;
            if (subidx == 0) {
                s[in_place_counter++] = current_letter;
            } else {
                row_to_slice[subidx - 1] += current_letter;
            }
            ++siter;
        }

        for (int subidx = numRows - 2; subidx > 0; --subidx) {
            if (siter == send) {
                break;
            }
            const auto current_letter = *siter;

            if (subidx == 0) {
                s[in_place_counter++] = current_letter;
            } else {
                row_to_slice[subidx - 1] += current_letter;
            }

            ++siter;
        }
    }

    // s.insert(s.begin() + in_place_counter, row_to_slice[0].begin(), row_to_slice[0].end());
    //    s.resize(in_place_counter);
    //    for (auto i = 0; i < numRows - 1; ++i) {
    //        s.insert(s.end(), row_to_slice[i].begin(), row_to_slice[i].end());
    //    }

    s.replace(in_place_counter, row_to_slice[0].size(), row_to_slice[0]);
    in_place_counter += row_to_slice[0].size();
    for (auto i = 1; i < numRows - 1; ++i) {
        s.replace(in_place_counter, row_to_slice[i].size(), row_to_slice[i]);
        in_place_counter += row_to_slice[i].size();
    }

    delete[] row_to_slice;

    return s;
}

string convert_orig_string_expansion(string s, int numRows) {
    // Resize and use expansion for store "hash_table"
    // s = orig/hashtable
    // vector<pair<start, current>>

    if (numRows == 1) {
        return s;
    }

    static const auto sso_capacity = std::string().capacity();

    const auto ssz = s.size();
    const int kStride = (numRows - 1) * 2;
    const int kMaxStrideLength = ssz / kStride + 1;
    auto spans = std::vector<pair<int, int>>(numRows, {0, 0});

    const auto new_size = s.size() + numRows * kMaxStrideLength;
    const auto orig_size = s.size();
    s.resize(new_size);  // and fill
    // it' will totally copy string content

    return {};
}
}  // namespace leetcode