/*========================== begin_copyright_notice ============================

Copyright (C) 2016-2021 Intel Corporation

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice (including the next
paragraph) shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

SPDX-License-Identifier: MIT
============================= end_copyright_notice ===========================*/

// XFAIL: *

#include <cm/cm.h>
#include <cm/cmtl.h>

#define WIDTH 4

_GENX_MAIN_ void test(SurfaceIndex ibuf)
{
  // SurfaceIndex ibuf passed in as parameter
  // WIDTH previously defined

  vector<int, WIDTH> block;
  int x = get_thread_origin_x() * WIDTH;

  // Read block from input surface
  cmtl::ReadLinear(ibuf, x, block.select_all());
}

// output a warning just to have some output from the compiler to check
#warning 5_1_3_a.cpp

// RUN: %cmc -emit-llvm -Wno-pass-failed -- %s 2>&1 | FileCheck --implicit-check-not error %s
// CHECK: warning: 5_1_3_a.cpp
// CHECK: 1 warning generated
