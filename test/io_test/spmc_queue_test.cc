#include "freelock_queue.hpp"

// #include <atomic>
// #include <cassert>
// #include <memory>
// #include <new>
// #include <cstddef>
// #include <iostream>
// template<typename T, std::size_t mask_, typename Alloc>
// class FIFO_Base : private Alloc {
// public:
//     using value_type = T;
//     using allocator_traits = std::allocator_traits<Alloc>;
//     using size_type = typename allocator_traits::size_type;
//     using CursorType = std::atomic<size_type>;
//     static_assert(CursorType::is_always_lock_free, "CursorType::is_always_lock_free");
//     explicit FIFO_Base(Alloc const& alloc = Alloc{}) :
//         Alloc{ alloc }, ring_{ allocator_traits::allocate(*this, mask_ + 1) } {}
//     ~FIFO_Base() {
//         while (!empty()) {
//             element(popCursor_)->~T();
//             ++popCursor_;
//         }
//         allocator_traits::deallocate(*this, ring_, mask_ + 1);
//     }
//     [[nodiscard]] auto size() const noexcept {
//         auto pushCursor = pushCursor_.load(std::memory_order_relaxed);
//         auto popCursor = popCursor_.load(std::memory_order_relaxed);
//         assert(popCursor <= pushCursor);
//         return pushCursor - popCursor;
//     }
//     [[nodiscard]] auto empty() const noexcept {
//         return size() == 0;
//     }
//     [[nodiscard]] auto push(T const& value) {
//         auto pushCursor = pushCursor_.load(std::memory_order_relaxed);
//         if (full(pushCursor, popCursorCached_)) {
//             popCursorCached_ = popCursor_.load(std::memory_order_acquire);
//             if (full(pushCursor, popCursorCached_)) {
//                 return false;
//             }
//         }
//         new (element(pushCursor)) T(value);
//         pushCursor_.store(pushCursor + 1, std::memory_order_release);
//         return true;
//     }
//     [[nodiscard]] auto pop(T& value) {
//         auto popCursor = popCursor_.load(std::memory_order_relaxed);
//         if (empty(pushCursorCached_, popCursor)) {
//             pushCursorCached_ = pushCursor_.load(std::memory_order_acquire);
//             if (empty(pushCursorCached_, popCursor)) {
//                 return false;
//             }
//         }
//         value = *element(popCursor);
//         element(popCursor)->~T();
//         popCursor_.store(popCursor + 1, std::memory_order_release);
//         return true;
//     }
// private:
//     auto full(size_type pushCursor, size_type popCursor) const noexcept {
//         return (pushCursor - popCursor) == mask_ + 1;
//     }
//     static auto empty(size_type pushCursor, size_type popCursor) noexcept {
//         return pushCursor == popCursor;
//     }
//     auto element(size_type cursor) noexcept {
//         return &ring_[cursor & mask_];
//     }
// private:
//     T* ring_;
//     alignas(std::hardware_destructive_interference_size) CursorType pushCursor_;
//     alignas(std::hardware_destructive_interference_size) size_type popCursorCached_ {};
//     alignas(std::hardware_destructive_interference_size) CursorType popCursor_;
//     alignas(std::hardware_destructive_interference_size) size_type pushCursorCached_ {};
//     char padding_[std::hardware_destructive_interference_size - sizeof(size_type)];
// };
// consteval auto calc_mask(std::size_t min_capacity) noexcept {
//     std::size_t index = 0, mask = 0;
//     for (std::size_t i = 0; i < sizeof(std::size_t) * 8; ++i) {
//         if ((min_capacity >> i) & 1) {
//             index = i;
//         }
//     }
//     for (std::size_t i = 0; i <= index; ++i) {
//         mask |= std::size_t{ 1 } << i;
//     }
//     return mask;
// }
// template<typename T, std::size_t min_capacity, typename Alloc = std::allocator<T>>
// class FIFO : public FIFO_Base<T, calc_mask(min_capacity), Alloc> {

// };

// #include <thread>
// #include <cstdint>
// #include <iostream>

// constexpr int64_t MAXN = int64_t(1e8);
// struct Timer {
// 	std::chrono::steady_clock::time_point start;
// 	Timer() : start(std::chrono::steady_clock::now()) {}
// 	~Timer() {
// 		auto finish = std::chrono::steady_clock::now();
// 		auto runtime = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
// 		std::cerr << runtime / 1e6 << "s" << std::endl;
// 	}
// };
// FIFO<int64_t, 10> queue;
// FIFO<int64_t, 10> queue_2;
// int main() {
// 	Timer timer;
// 	std::thread pop_thread([]() {
// 		int64_t cnt = 0, last = -1;
// 		while (cnt != MAXN) {
// 			int64_t value;
// 			if (queue.pop(value)) {
// 				cnt += 1;
// 				if (value <= last) {
// 					abort();
// 				}
// 				last = value;
// 			}
// 		}
// 		});
// 	std::thread push_thread([]() {
// 		for (int64_t i = 1; i <= MAXN;) {
// 			if (queue.push(i)) {
// 				i += 1;
// 			}
// 		}
// 		});


//      	std::thread pop_thread_2([]() {
// 		int64_t cnt = 0, last = -1;
// 		while (cnt != MAXN) {
// 			int64_t value;
// 			if (queue_2.pop(value)) {
// 				cnt += 1;
// 				if (value <= last) {
// 					abort();
// 				}
// 				last = value;
// 			}
// 		}
// 		});
// 	std::thread push_thread_2([]() {
// 		for (int64_t i = 1; i <= MAXN;) {
// 			if (queue_2.push(i)) {
// 				i += 1;
// 			}
// 		}
// 		});
// 	push_thread.join();
// 	pop_thread.join();   
// 	push_thread_2.join();
// 	pop_thread_2.join();
// 	return 0;
// }

/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// #include <atomic>
// #include <cassert>
// #include <cstdlib>
// #include <memory>
// #include <stdexcept>
// #include <type_traits>
// #include <utility>

// namespace folly {
// using namespace std;
// /*
//  * ProducerConsumerQueue is a one producer and one consumer queue
//  * without locks.
//  */
// template <class T>
// struct ProducerConsumerQueue {

//   typedef T value_type;

//   ProducerConsumerQueue(const ProducerConsumerQueue&) = delete;
//   ProducerConsumerQueue& operator=(const ProducerConsumerQueue&) = delete;

//   // size must be >= 2.
//   //
//   // Also, note that the number of usable slots in the queue at any
//   // given time is actually (size-1), so if you start with an empty queue,
//   // isFull() will return true after size-1 insertions.
//   explicit ProducerConsumerQueue(uint32_t size)
//       : size_(size),
//         records_(static_cast<T*>(std::malloc(sizeof(T) * size))),
//         readIndex_(0),
//         writeIndex_(0) {
//     assert(size >= 2);
//     if (!records_) {
//       throw std::bad_alloc();
//     }
//   }

//   ~ProducerConsumerQueue() {
//     // We need to destruct anything that may still exist in our queue.
//     // (No real synchronization needed at destructor time: only one
//     // thread can be doing this.)
//     if (!std::is_trivially_destructible<T>::value) {
//       size_t readIndex = readIndex_;
//       size_t endIndex = writeIndex_;
//       while (readIndex != endIndex) {
//         records_[readIndex].~T();
//         if (++readIndex == size_) {
//           readIndex = 0;
//         }
//       }
//     }

//     std::free(records_);
//   }

//   template <class... Args>
//   bool write(Args&&... recordArgs) {
//     auto const currentWrite = writeIndex_.load(std::memory_order_relaxed);
//     auto nextRecord = currentWrite + 1;
//     if (nextRecord == size_) {
//       nextRecord = 0;
//     }
//     if (nextRecord != readIndex_.load(std::memory_order_acquire)) {
//       new (&records_[currentWrite]) T(std::forward<Args>(recordArgs)...);
//       writeIndex_.store(nextRecord, std::memory_order_release);
//       return true;
//     }

//     // queue is full
//     return false;
//   }

//   // move (or copy) the value at the front of the queue to given variable
//   bool read(T& record) {
//     auto const currentRead = readIndex_.load(std::memory_order_relaxed);
//     if (currentRead == writeIndex_.load(std::memory_order_acquire)) {
//       // queue is empty
//       return false;
//     }

//     auto nextRecord = currentRead + 1;
//     if (nextRecord == size_) {
//       nextRecord = 0;
//     }
//     record = std::move(records_[currentRead]);
//     records_[currentRead].~T();
//     readIndex_.store(nextRecord, std::memory_order_release);
//     return true;
//   }

//   // pointer to the value at the front of the queue (for use in-place) or
//   // nullptr if empty.
//   T* frontPtr() {
//     auto const currentRead = readIndex_.load(std::memory_order_relaxed);
//     if (currentRead == writeIndex_.load(std::memory_order_acquire)) {
//       // queue is empty
//       return nullptr;
//     }
//     return &records_[currentRead];
//   }

//   // queue must not be empty
//   void popFront() {
//     auto const currentRead = readIndex_.load(std::memory_order_relaxed);
//     assert(currentRead != writeIndex_.load(std::memory_order_acquire));

//     auto nextRecord = currentRead + 1;
//     if (nextRecord == size_) {
//       nextRecord = 0;
//     }
//     records_[currentRead].~T();
//     readIndex_.store(nextRecord, std::memory_order_release);
//   }

//   bool isEmpty() const {
//     return readIndex_.load(std::memory_order_acquire) ==
//         writeIndex_.load(std::memory_order_acquire);
//   }

//   bool isFull() const {
//     auto nextRecord = writeIndex_.load(std::memory_order_acquire) + 1;
//     if (nextRecord == size_) {
//       nextRecord = 0;
//     }
//     if (nextRecord != readIndex_.load(std::memory_order_acquire)) {
//       return false;
//     }
//     // queue is full
//     return true;
//   }

//   // * If called by consumer, then true size may be more (because producer may
//   //   be adding items concurrently).
//   // * If called by producer, then true size may be less (because consumer may
//   //   be removing items concurrently).
//   // * It is undefined to call this from any other thread.
//   size_t sizeGuess() const {
//     int ret = writeIndex_.load(std::memory_order_acquire) -
//         readIndex_.load(std::memory_order_acquire);
//     if (ret < 0) {
//       ret += size_;
//     }
//     return ret;
//   }

//   // maximum number of items in the queue.
//   size_t capacity() const { return size_ - 1; }

//  private:
//   using AtomicIndex = std::atomic<unsigned int>;

//   char pad0_[hardware_destructive_interference_size];
//   const uint32_t size_;
//   T* const records_;

//   alignas(hardware_destructive_interference_size) AtomicIndex readIndex_;
//   alignas(hardware_destructive_interference_size) AtomicIndex writeIndex_;

//   char pad1_[hardware_destructive_interference_size - sizeof(AtomicIndex)];
// };

// } // namespace folly

// #include <thread>
// #include <cstdint>
// #include <iostream>
// constexpr int64_t MAXN = int64_t(1e8);
// struct Timer {
// 	std::chrono::steady_clock::time_point start;
// 	Timer() : start(std::chrono::steady_clock::now()) {}
// 	~Timer() {
// 		auto finish = std::chrono::steady_clock::now();
// 		auto runtime = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
// 		std::cerr << runtime / 1e6 << "s" << std::endl;
// 	}
// };
// folly::ProducerConsumerQueue<int64_t> queue(1023);
// int main() {
// 	Timer timer;
// 	std::thread pop_thread([]() {
// 		int64_t cnt = 0, last = -1;
// 		while (cnt != MAXN) {
// 			int64_t value;
// 			if (queue.read(value)) {
// 				cnt += 1;
// 				if (value <= last) {
// 					abort();
// 				}
// 				last = value;
// 			}
// 		}
// 		});
// 	std::thread push_thread([]() {
// 		for (int64_t i = 1; i <= MAXN;) {
// 			if (queue.write(i)) {
// 				i += 1;
// 			}
// 		}
// 		});
// 	push_thread.join();
// 	pop_thread.join();
// 	return 0;
// }

#include <thread>
#include <cstdint>
#include <iostream>
constexpr int64_t MAXN = int64_t(1e8);
struct Timer
{
    std::chrono::steady_clock::time_point start;
    Timer() : start(std::chrono::steady_clock::now()) {}
    ~Timer()
    {
        auto finish = std::chrono::steady_clock::now();
        auto runtime = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
        std::cerr << runtime / 1e6 << "s" << std::endl;
    }
};
hy::FreelockQueue<int64_t, 1023> queue{};
// hy::FreelockQueue<int64_t, 1023, std::allocator<int64_t>, 10> queue_2{};
int main()
{
    Timer timer;
    std::thread pop_thread([]()
                           {
		int64_t cnt = 0, last = -1;
		while (cnt != MAXN) {
			int64_t value;
			queue.pop(value) ;
				cnt += 1;
				if (value <= last) {
					abort();
				}
				last = value;
			
		} });
    std::thread push_thread([]()
                            {
		for (int64_t i = 1; i <= MAXN;) {
			queue.push(i) ;
				i += 1;
			
		} });
    //         std::thread pop_thread_2([]()
    //                        {
	// 	int64_t cnt = 0, last = -1;
	// 	while (cnt != MAXN) {
	// 		int64_t value;
	// 		queue_2.pop(value) ;
	// 			cnt += 1;
	// 			if (value <= last) {
	// 				abort();
	// 			}
	// 			last = value;
			
	// 	} });
    // std::thread push_thread_2([]()
    //                         {
	// 	for (int64_t i = 1; i <= MAXN;) {
	// 		queue_2.push(i) ;
	// 			i += 1;
			
	// 	} });
    push_thread.join();
    pop_thread.join();
    // push_thread_2.join();
    // pop_thread_2.join();
    return 0;
}

