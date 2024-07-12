#ifndef HY_RAW_STORAGE_ITERATOR_HPP_
#define HY_RAW_STORAGE_ITERATOR_HPP_
#include <iterator>
namespace hy {
template <class OutputIt, class T> class [[deprecated]] raw_storage_iterator {
public:
  /*Member types*/
  using iterator_category = std::output_iterator_tag;
  using value_type = void;
  using difference_type = void;
  using pointer = void;
  using reference = void;

  /*Member functions*/
  /*Constructors*/
  explicit raw_storage_iterator(OutputIt it) : iter_{it} {}

  /*Assignment*/
  raw_storage_iterator &operator=(const T &el) {
    (*iter_) = el;
    return *this;
  }

  raw_storage_iterator &operator=(T &&el) {
    (*iter_) = std::move(el);
    return *this;
  }

  /*Overload functions*/
  raw_storage_iterator &operator*() { return *this; }

  raw_storage_iterator &operator++() {
    ++iter_;
    return *this;
  }

  raw_storage_iterator operator++(int) {
    auto ret = *this;
    ++iter_;
    return ret;
  }

  /*Access base*/
  OutputIt base() const { return iter_; }

private:
  OutputIt iter_;
}; /// class [[deprecated]] raw_storage_iterator

} // namespace hy

#endif /// HY_RAW_STORAGE_ITERATOR_HPP_