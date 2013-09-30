#include <fcntl.h>
#include <mongoc.h>
#include <stdlib.h>
#include <unistd.h>

#include "mongoc-tests.h"

static void
test_buffered_basic (void)
{
   mongoc_stream_t *stream;
   mongoc_stream_t *buffered;
   ssize_t r;
   struct iovec iov;
   char buf[16236];
   int fd;

   fd = open("tests/binary/reply2.dat", O_RDONLY);
   assert(fd != -1);

   /* stream assumes ownership of fd */
   stream = mongoc_stream_unix_new(fd);

   /* buffered assumes ownership of stream */
   buffered = mongoc_stream_buffered_new(stream, 4096);

   /* try to read large chunk larger than buffer. */
   iov.iov_len = sizeof buf;
   iov.iov_base = buf;
   r = mongoc_stream_readv(buffered, &iov, 1, iov.iov_len, 0);
   BSON_ASSERT(r == iov.iov_len);

   /* cleanup */
   mongoc_stream_destroy(buffered);
}

int
main (int argc,
      char *argv[])
{
   run_test("/mongoc/stream/buffered/basic", test_buffered_basic);

   return 0;
}