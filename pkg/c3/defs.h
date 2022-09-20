#ifndef C3_DEFS_H
#define C3_DEFS_H

#include "c/portable.h"
#include "c/types.h"

#include <errno.h>

  /** Loobeans - inverse booleans to match nock.
  **/
#     define c3y      0
#     define c3n      1

#     define _(x)        (c3y == (x))
#     define __(x)       ((x) ? c3y : c3n)
#     define c3a(x, y)   __(_(x) && _(y))
#     define c3o(x, y)   __(_(x) || _(y))


  /** Random useful C macros.
  **/
    /* Assert.  Good to capture.
    */

#     if defined(ASAN_ENABLED) && defined(__clang__)
#       define c3_assert(x)                       \
          do {                                    \
            if (!(x)) {                           \
              c3_cooked();                        \
              assert(x);                          \
            }                                     \
          } while(0)
#     else
#       define c3_assert(x)                       \
          do {                                    \
            if (!(x)) {                           \
              fflush(stderr);                     \
              fprintf(stderr, "\rAssertion '%s' " \
                      "failed in %s:%d\r\n",      \
                      #x, __FILE__, __LINE__);    \
              c3_cooked();                        \
              assert(x);                          \
            }                                     \
          } while(0)
#endif

    /* Stub.
    */
#     define c3_stub       c3_assert(!"stub")

    /* Size in words.
    */
#     define c3_wiseof(x)  (((sizeof (x)) + 3) >> 2)

    /* Bit counting.
    */
#     define c3_bits_word(w) ((w) ? (32 - __builtin_clz(w)) : 0)

    /* Min and max.
    */
#     define c3_max(x, y) ( ((x) > (y)) ? (x) : (y) )
#     define c3_min(x, y) ( ((x) < (y)) ? (x) : (y) )


//! Round up/down (respectively).
//!
//! @param[in] x  Integer to round.
//! @param[in] n  Multiple to round to. Must be power of 2.
//!
//! @return  `x` rounded to the nearest multiple of `n`.
#     define c3_rop(x, n) (((x) + ((n) - 1)) & (~((n) - 1)))
#     define c3_rod(x, n) ((x) & ~((n) - 1))

    /* Rotate.
    */
#     define c3_rotw(r, x)  ( ((x) << (r)) | ((x) >> (32 - (r))) )

    /* Emergency stdio fix.
    */
      int
      c3_cooked();

    /* Fill 16 words (64 bytes) with high-quality entropy.
    */
      void
      c3_rand(c3_w* rad_w);

    /* Short integers.
    */
#     define c3_s1(a)          ( (a) )
#     define c3_s2(a, b)       ( ((b) << 8) | c3_s1(a) )
#     define c3_s3(a, b, c)    ( ((c) << 16) | c3_s2(a, b) )
#     define c3_s4(a, b, c, d) ( ((d) << 24) | c3_s3(a, b, c) )

#     define c3_s5(a, b, c, d, e) \
        ( ((uint64_t)c3_s1(e) << 32ULL) | c3_s4(a, b, c, d) )
#     define c3_s6(a, b, c, d, e, f) \
        ( ((uint64_t)c3_s2(e, f) << 32ULL) | c3_s4(a, b, c, d) )
#     define c3_s7(a, b, c, d, e, f, g) \
        ( ((uint64_t)c3_s3(e, f, g) << 32ULL) | c3_s4(a, b, c, d) )
#     define c3_s8(a, b, c, d, e, f, g, h) \
        ( ((uint64_t)c3_s4(e, f, g, h) << 32ULL) | c3_s4(a, b, c, d) )

    /* Byte-order twiddling.
    */
#     define c3_flip32(w) \
        ( (((w) >> 24) & 0xff) \
        | (((w) >> 16) & 0xff) << 8 \
        | (((w) >>  8) & 0xff) << 16 \
        | ( (w)        & 0xff) << 24 )

    /* Asserting allocators.
    */
#     define c3_free(s) free(s)
#     define c3_malloc(s) ({                                    \
        void* rut = malloc(s);                                  \
        if ( 0 == rut ) {                                       \
          fprintf(stderr, "c3_malloc(%" PRIu64 ") failed\r\n",  \
                          (c3_d)s);                             \
          c3_assert(!"memory lost");                            \
        }                                                       \
        rut;})
#     define c3_calloc(s) ({                                    \
        void* rut = calloc(1,s);                                \
        if ( 0 == rut ) {                                       \
          fprintf(stderr, "c3_calloc(%" PRIu64 ") failed\r\n",  \
                          (c3_d)s);                             \
          c3_assert(!"memory lost");                            \
        }                                                       \
        rut;})
#     define c3_realloc(a, b) ({                                \
        void* rut = realloc(a, b);                              \
        if ( 0 == rut ) {                                       \
          fprintf(stderr, "c3_realloc(%" PRIu64 ") failed\r\n", \
                          (c3_d)b);                             \
          c3_assert(!"memory lost");                            \
        }                                                       \
        rut;})

    /* Asserting unix fs wrappers.
    **
    **  these all crash the process if passed a non-canonical
    **  path (i.e., one containing '.', '..', or the empty path
    **  component), so make sure you don't pass them one. if you
    **  find yourself fighting with them, then please delete them
    **  and do a sed search-and-replace to remove the `c3_` from
    **  their call sites; their goal is to decrease maintenance
    **  burden, not increase it.
    */
      // defined in vere/io/unix.c.
      c3_t u3_unix_cane(const c3_c* pax_c);
#     define c3_open(a, ...) ({                                 \
        open(a, __VA_ARGS__);})
#     define c3_opendir(a) ({                                   \
        opendir(a);})
#     define c3_mkdir(a, b) ({                                  \
        mkdir(a, b);})
#     define c3_rmdir(a) ({                                     \
        rmdir(a);})
#     define c3_unlink(a) ({                                    \
        unlink(a);})
#     define c3_fopen(a, b) ({                                  \
        fopen(a, b);})

      /// Reads the contents of a file into memory.
      ///
      /// This function correctly handles the case in which read() reads fewer
      /// bytes than was requested by retrying until all bytes have been read or
      /// until an error occurs, whichever comes first.
      ///
      /// If interrupted by a signal or if the read() call would block, this
      /// function will retry up to a specified maximum number of attempts.
      ///
      /// @param[in]  fd_i        Open file descriptor to read from.
      /// @param[out] data_v      Destination buffer.
      /// @param[in]  data_len_i  Length of `data_v` in bytes.
      ///
      /// @return >0  Number of bytes read. Guaranteed to be less than or equal
      ///             to `data_len_i`. If less than `data_len_i`, then `fd_i`
      ///             reached EOF.
      /// @return  0  `fd_i` is at EOF.
      /// @return <0  Error occurred. The error number is the absolute value of
      ///             the return value and can be fed into strerror().
      inline ssize_t
      c3_read(c3_i fd_i, void* const data_v, const size_t data_len_i)
      {
        static const size_t max_attempts_i    = 100;
        size_t              attempts_i        = 0;
        c3_y*               ptr_y             = data_v;
        ssize_t             bytes_remaining_i = data_len_i;

        off_t cur_offset_i = lseek(fd_i, 0, SEEK_CUR);
        if ( cur_offset_i < 0 ) {
          return -errno;
        }

        do {
          attempts_i++;
          ssize_t bytes_read_i = read(fd_i, ptr_y, bytes_remaining_i);
          if ( -1 == bytes_read_i ) {
            if ( (EINTR == errno || EAGAIN == errno || EWOULDBLOCK == errno)
                 && attempts_i <= max_attempts_i )
            {
              // From the read(2) man page: "On error, -1 is returned, and errno
              // is set to indicate the error. In this case, it is left
              // unspecified whether the file position (if any) changes."
              //
              // This means that we have to restore the file cursor to the
              // position that it was at immediately before the call to read()
              // that failed.
              if ( lseek(fd_i, SEEK_SET, cur_offset_i) < 0 ) {
                return -errno;
              }
              continue;
            }
            else {
              return -errno;
            }
          }
          else if ( 0 == bytes_read_i ) {
            break;
          }
          else {
            ptr_y += bytes_read_i;
            bytes_remaining_i -= bytes_read_i;
            cur_offset_i += bytes_read_i;
            attempts_i = 0;
          }
        } while ( bytes_remaining_i > 0 );

        return data_len_i - bytes_remaining_i;
      }

      /// Writes the contents of a buffer to a file.
      ///
      /// This function correctly handles the case in which write() writes fewer
      /// bytes than was requested by retrying until all bytes have been written
      /// or until an error occurs, whichever comes first.
      ///
      /// If interrupted by a signal or if the write() call would block, this
      /// function will retry up to a specified maximum number of attempts.
      ///
      ///
      /// @param[in] fd_i        Open file descriptor to write to.
      /// @param[in] data_v      Source buffer.
      /// @param[in] data_len_i  Length of `data_v` in bytes.
      ///
      /// @return >=0  Number of bytes written. Guaranteed to be equal to
      ///              `data_len_i`.
      /// @return <0   Error occurred. The error number is the absolute value of
      ///              the return value and can be fed into strerror().
      inline ssize_t
      c3_write(c3_i fd_i, const void* const data_v, const size_t data_len_i)
      {
        static const size_t max_attempts_i    = 100;
        size_t              attempts_i        = 0;
        const c3_y*         ptr_y             = data_v;
        ssize_t             bytes_remaining_i = data_len_i;
        do {
          attempts_i++;
          ssize_t bytes_written_i = write(fd_i, ptr_y, bytes_remaining_i);
          if ( -1 == bytes_written_i ) {
            // Attempt to read again if we were interrupted by a signal.
            if ( (EINTR == errno || EAGAIN == errno || EWOULDBLOCK == errno)
                 && attempts_i <= max_attempts_i )
            {
              continue;
            }
            else {
              return -errno;
            }
          }
          else {
            ptr_y += bytes_written_i;
            bytes_remaining_i -= bytes_written_i;
            attempts_i = 0;
          }
        } while ( bytes_remaining_i > 0 );

        return data_len_i - bytes_remaining_i;
      }

#endif /* ifndef C3_DEFS_H */
