/*===================== begin_copyright_notice ==================================

 Copyright (c) 2021, Intel Corporation


 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
======================= end_copyright_notice ==================================*/

#ifndef CM_CMTL_MASK_H
#define CM_CMTL_MASK_H

#include "../cm_mask.h"

namespace cmtl {

template<int size>
class Mask final {
  // Bits [size, sizeof(mask)) must be 0,
  // all methods must preserve this invariant.
  MaskIntT mask = 0;

  // to avoid static_cast and literals
  static constexpr MaskIntT one = 1;
  static constexpr MaskIntT zero = 0;
  static constexpr int mask_bit_size = sizeof(MaskIntT) * 8;
  static_assert(size <= mask_bit_size, "too big mask size");

  CM_INLINE Mask(MaskIntT in) : mask(in) {}

  CM_INLINE void set(MaskVecT<size> in) {
    mask = cm_pack_mask(in);
  }

public:
  static constexpr MaskIntT full =
    size == mask_bit_size ? ~zero : (one << size) - one;

  CM_INLINE Mask(MaskVecT<size> in) {
    set(in);
  }

  CM_INLINE Mask(bool in) : mask(in ? full : zero) {}

  CM_INLINE MaskIntT get() const { return mask; }

  CM_INLINE MaskVecT<size> get_vector() const {
    // really result will always have ushort type
    // as T parameter doesn't affect a thing
    return cm_unpack_mask<MaskVecElemT, size>(mask);
  }

  template<typename T>
  CM_INLINE vector<T, size> get_vector() const {
    // really result will always have ushort type
    // as T parameter doesn't affect a thing
    auto ushort_res = cm_unpack_mask<T, size>(mask);
    vector<T, size> res = ushort_res;
    return res;
  }

  CM_INLINE operator MaskIntT () const { return get(); }

  CM_INLINE bool is_all() const { return mask == full; }
  CM_INLINE bool is_any() const { return mask; }
  CM_INLINE int count() const { return cm_cbit(mask); }

  CM_INLINE Mask &operator&=(Mask rhs) {
    mask &= rhs.mask;
    return *this;
  }

  CM_INLINE Mask &operator|=(Mask rhs) {
    mask |= rhs.mask;
    return *this;
  }

  CM_INLINE Mask &operator^=(Mask rhs) {
    mask ^= rhs.mask;
    return *this;
  }

  CM_INLINE Mask operator~() {
    return (~mask) & full;
  }

  CM_INLINE friend bool operator==(Mask lhs, Mask rhs) {
    return lhs.mask == rhs.mask;
  }
};

template<int size>
Mask(MaskVecT<size>) -> Mask<size>;

template<int size>
CM_INLINE bool operator!=(Mask<size> lhs, Mask<size> rhs) {
  return !(lhs == rhs);
}

template<int size>
CM_INLINE Mask<size> operator&(Mask<size> lhs, Mask<size> rhs) {
  return lhs &= rhs;
}

template<int size>
CM_INLINE Mask<size> operator|(Mask<size> lhs, Mask<size> rhs) {
  return lhs |= rhs;
}

template<int size>
CM_INLINE Mask<size> operator^(Mask<size> lhs, Mask<size> rhs) {
  return lhs ^= rhs;
}

} // namespace cmtl
#endif // CM_CMTL_MASK_H
