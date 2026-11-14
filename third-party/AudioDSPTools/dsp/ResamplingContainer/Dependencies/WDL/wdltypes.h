#ifndef _WDLTYPES_
#define _WDLTYPES_

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <strings.h>
#include <new>

typedef long long WDL_INT64;
typedef unsigned long long WDL_UINT64;

#define WDL_UINT64_CONST(x) (x##ULL)
#define WDL_INT64_CONST(x) (x##LL)

#define WDL_PRI_UINT64 "llu"
#define WDL_PRI_INT64 "lld"

typedef intptr_t INT_PTR;
typedef uintptr_t UINT_PTR;
typedef bool WDL_bool;

#ifndef WDL_LITTLE_ENDIAN
  #define WDL_LITTLE_ENDIAN
#endif

#define WDL_FIXALIGN __attribute__((aligned(8)))
#define WDL_VARARG_WARN(x, n, s) __attribute__((format(x, n, s)))
#define WDL_STATICFUNC_UNUSED __attribute__((unused))

#ifndef WDL_WANT_NEW_EXCEPTIONS
  #define WDL_NEW (std::nothrow)
#else
  #define WDL_NEW
#endif

#if !defined(max) && defined(WDL_DEFINE_MINMAX)
  #define max(x, y) ((x) < (y) ? (y) : (x))
  #define min(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifndef wdl_max
  #define wdl_max(x, y) ((x) < (y) ? (y) : (x))
  #define wdl_min(x, y) ((x) < (y) ? (x) : (y))
  #define wdl_abs(x) ((x) < 0 ? -(x) : (x))
  #define wdl_clamp(x, minv, maxv)                                                                                     \
    (WDL_NOT_NORMALLY((maxv) < (minv)) || (x) < (minv) ? (minv) : ((x) > (maxv) ? (maxv) : (x)))
#endif

#ifndef strnicmp
  #define strnicmp(x, y, z) strncasecmp(x, y, z)
#endif
#ifndef stricmp
  #define stricmp(x, y) strcasecmp(x, y)
#endif

#ifdef WDL_BACKSLASHES_ARE_ORDINARY
  #define WDL_IS_DIRCHAR(x) ((x) == '/')
#else
  #define WDL_IS_DIRCHAR(x) ((x) == '/' || (x) == '\\')
#endif

#define WDL_DIRCHAR '/'
#define WDL_DIRCHAR_STR "/"

#define wdl_filename_cmp(x, y) stricmp(x, y)
#define wdl_filename_cmpn(x, y, n) strnicmp(x, y, n)

#define WDL_likely(x) (__builtin_expect(!!(x), 1))
#define WDL_unlikely(x) (__builtin_expect(!!(x), 0))

#if defined(_DEBUG) || defined(DEBUG)
  #include <cassert>
  #define WDL_ASSERT_BEGIN() (1)
  #define WDL_ASSERT_END() (0)
  #define WDL_ASSERT(x) assert(x)
  #define WDL_NORMALLY(x) ((x) ? 1 : (WDL_ASSERT_BEGIN() && (assert(0 /*ignorethis*/ && (x)), WDL_ASSERT_END())))
  #define WDL_NOT_NORMALLY(x) ((x) ? !WDL_ASSERT_BEGIN() || (assert(0 /*ignorethis*/ && !(x)), !WDL_ASSERT_END()) : 0)
#else
  #define WDL_ASSERT(x)
  #define WDL_NORMALLY(x) WDL_likely(x)
  #define WDL_NOT_NORMALLY(x) WDL_unlikely(x)
#endif

typedef unsigned int WDL_TICKTYPE;

static WDL_bool WDL_STATICFUNC_UNUSED WDL_TICKS_IN_RANGE(WDL_TICKTYPE current, WDL_TICKTYPE refstart, int len)
{
  WDL_ASSERT(len > 0);
  return (current - refstart) < (WDL_TICKTYPE)len;
}

static WDL_bool WDL_STATICFUNC_UNUSED WDL_TICKS_IN_RANGE_ENDING_AT(WDL_TICKTYPE current, WDL_TICKTYPE refend, int len)
{
  const WDL_TICKTYPE refstart = refend - len;
  WDL_ASSERT(len > 0);
  return (current - refstart) < (WDL_TICKTYPE)len;
}

#define WDL_fopenA(fn, mode) fopen(fn, mode)

#ifndef WDL_ALLOW_UNSIGNED_DEFAULT_CHAR
typedef char wdl_assert_failed_unsigned_char[((char)-1) > 0 ? -1 : 1];
#endif

#if !defined(_DEBUG) && !defined(WDL_LOG_ON_RELEASE)
static void WDL_STATICFUNC_UNUSED WDL_VARARG_WARN(printf, 1, 2) wdl_log(const char* format, ...) {}
#else
  #define wdl_log printf
#endif

static void WDL_STATICFUNC_UNUSED wdl_bswap_copy(void* bout, const void* bin, size_t nelem, size_t elemsz)
{
  char p[8], po[8];
  WDL_ASSERT(elemsz > 0);
  if (elemsz > 1 && WDL_NORMALLY(elemsz <= sizeof(p)))
  {
    size_t i, x;
    for (i = 0; i < nelem; i++)
    {
      memcpy(p, bin, elemsz);
      for (x = 0; x < elemsz; x++)
        po[x] = p[elemsz - 1 - x];
      memcpy(bout, po, elemsz);
      bin = (const char*)bin + elemsz;
      bout = (char*)bout + elemsz;
    }
  }
  else if (bout != bin)
    memmove(bout, bin, elemsz * nelem);
}

static void WDL_STATICFUNC_UNUSED wdl_memcpy_le(void* bout, const void* bin, size_t nelem, size_t elemsz)
{
  WDL_ASSERT(elemsz > 0 && elemsz <= 8);
  if (bout != bin)
    memmove(bout, bin, elemsz * nelem);
}

static void WDL_STATICFUNC_UNUSED wdl_memcpy_be(void* bout, const void* bin, size_t nelem, size_t elemsz)
{
  WDL_ASSERT(elemsz > 0 && elemsz <= 8);
  if (elemsz > 1)
    wdl_bswap_copy(bout, bin, nelem, elemsz);
  else if (bout != bin)
    memmove(bout, bin, elemsz * nelem);
}

static void WDL_STATICFUNC_UNUSED wdl_mem_store_int(void* bout, int v)
{
  memcpy(bout, &v, sizeof(v));
}

static void WDL_STATICFUNC_UNUSED wdl_mem_store_int_le(void* bout, int v)
{
  wdl_memcpy_le(bout, &v, 1, sizeof(v));
}

static void WDL_STATICFUNC_UNUSED wdl_mem_store_int_be(void* bout, int v)
{
  wdl_memcpy_be(bout, &v, 1, sizeof(v));
}

static int WDL_STATICFUNC_UNUSED wdl_mem_load_int(const void* rd)
{
  int v;
  memcpy(&v, rd, sizeof(v));
  return v;
}

static int WDL_STATICFUNC_UNUSED wdl_mem_load_int_le(const void* rd)
{
  int v;
  wdl_memcpy_le(&v, rd, 1, sizeof(v));
  return v;
}

static int WDL_STATICFUNC_UNUSED wdl_mem_load_int_be(const void* rd)
{
  int v;
  wdl_memcpy_be(&v, rd, 1, sizeof(v));
  return v;
}

#endif
