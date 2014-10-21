#ifndef BASECFD_DEFINITIONS_GRID_GUARD_H
#define BASECFD_DEFINITIONS_GRID_GUARD_H

#if defined(__CUDACC__)
#include <cuda_runtime_api.h>
#define __H__ __host__
#define __D__ __device__
#define __HD__ __host__ __device__
#define __HDF__ __host__ __device__ __forceinline__
#else //__CUDACC__
#define __H__
#define __D__
#define __HD__
#define __HDF__ inline
#endif //__CUDACC__

#define BASECFD_UNUSED_PARAMETER(p) (void)p

// DLL import/export
#if defined(__GNUC__)

#else

#endif

#if defined(__GNUC__)
#if (__GNUC__ >= 4)
#define DLL_EXPORT __attribute__ ((visibility ("default")))
#define DLL_IMPORT __attribute__ ((visibility ("default")))
#define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define DLL_EXPORT __attribute__ ((dllexport))
#define DLL_IMPORT __attribute__ ((dllimport))
#define DLL_LOCAL
#endif
#else
#define DLL_EXPORT __declspec(dllexport) // Note: recent gcc also supports this syntax.
#define DLL_IMPORT __declspec(dllimport) // Note: recent gcc also supports this syntax.
#define DLL_LOCAL
#endif



#ifndef DLL_NOT_USED
#if defined(BUILDING_DLL)
#define BASECFD_DLL DLL_EXPORT
#else
#define BASECFD_DLL DLL_IMPORT
#endif
#else
#define BASECFD_DLL
#endif

#ifdef _MSC_VER
#if _MSC_VER >= 1600
#include <cstdint>
#else
typedef __int8              int8_t;
typedef __int16             int16_t;
typedef __int32             int32_t;
typedef __int64             int64_t;
typedef unsigned __int8     uint8_t;
typedef unsigned __int16    uint16_t;
typedef unsigned __int32    uint32_t;
typedef unsigned __int64    uint64_t;
#endif
#elif __GNUC__ >= 3
#include <stdint.h>
#endif

#define BASECFD_UNUSED_PARAM(p) (void)(p);

#include <limits>

namespace basecfd
{

typedef uint32_t idx_t;
typedef uint32_t dim_t;

/// Invalid index
const idx_t BAD_IDX = std::numeric_limits<idx_t>::is_signed ?
                      std::numeric_limits<idx_t>::min() : std::numeric_limits<idx_t>::max();

/// Invalid dimension
const dim_t BAD_DIM = std::numeric_limits<dim_t>::is_signed ?
                      std::numeric_limits<dim_t>::min() : std::numeric_limits<dim_t>::max();

} //namespace basecfd

#endif //BASECFD_DEFINITIONS_GUARD_H
