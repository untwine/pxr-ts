// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_TS_TEST_API_H
#define PXR_TS_TEST_API_H

#include <pxr/arch/export.h>

#if defined(PXR_STATIC)
#   define TS_TEST_API
#else
#   if defined(TS_TEST_EXPORTS)
#       define TS_TEST_API ARCH_EXPORT
#   else
#       define TS_TEST_API ARCH_IMPORT
#   endif
#endif

#endif
