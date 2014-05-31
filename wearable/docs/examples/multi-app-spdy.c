/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2011, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
/* This is an example application source code using the multi interface. */

#include <stdio.h>
#include <string.h>

/* somewhat unix-specific */
#include <sys/time.h>
#include <unistd.h>

/* curl stuff */
#include <curl/curl.h>

/*
 * Download a HTTP file and upload an FTP file simultaneously.
 */

#define HANDLECOUNT 10   /* Number of simultaneous transfers */
#define HTTP_HANDLE 0   /* Index for the HTTP transfer */
#define FTP_HANDLE 1    /* Index for the FTP transfer */
#define GL_TIME_USEC_PER_SEC 1000000L

int main(void)
{
  CURL *handles[HANDLECOUNT];
  CURLM *multi_handle;
  struct curl_slist * headers=NULL;
  long g_time_usec = 0L;
  double totaltime = 0.0;
  struct timeval time;

  int still_running; /* keep number of running handles */
  int i;

  CURLMsg *msg; /* for picking up messages with the transfer status */
  int msgs_left; /* how many messages are left */

  /* init a multi stack */
  multi_handle = curl_multi_init();
  curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, 1L);

  /* Allocate one CURL handle per transfer */
  for (i=0; i<HANDLECOUNT; i++) {
      handles[i] = curl_easy_init();
      headers = NULL;
      /* set the options (I left out a few, you'll get the point anyway) */
      curl_easy_setopt(handles[i], CURLOPT_URL, "https://picasaweb.google.com/data/feed/api/user/tarun123456789123456789@gmail.com");
      //curl_easy_setopt(handles[i], CURLOPT_VERBOSE, 1L);
#if 0
      headers = curl_slist_append(headers, "user-agent:Samsung User Agent");
      headers = curl_slist_append(headers, "authorization:GoogleLogin auth=DQAAAM4AAACYjUbeFEvdJK351FV7Vu-NuDQN2PPpH9pl3xTYvK_iv8zihmLq-amYI4yQoBal3wZc0c_hm1yvT7Mm0OLXQNgqz0zUCvZFbuvVp369pmr6WJFFyFdlRp0bEvyjL8SviDvcxjls57qBZ64eU4Lb0Ey5NjgoM6JK-a1vfA_NVS0r3XEB9bmc8nhEgtCvWvQ39Dv2RKrgX86D-JGsTbyNWCqPxwx09n-V4iGtbslEcCUjNnbGV74QRme8avI9gzUoqzciuUJkFI2Boc8Hbc6RsO-w");
      headers = curl_slist_append(headers, "gdata-version:2");
      headers = curl_slist_append(headers, "content-type:application/atom+xml");
      headers = curl_slist_append(headers, "accept-charset:ISO-8859-1,utf-8;q=0.7,*;q=0.3");
      headers = curl_slist_append(headers, "accept-encoding:gzip,deflate,sdch");
      headers = curl_slist_append(headers, "accept-language:en-US,en;q=0.8");

      curl_easy_setopt(handles[i], CURLOPT_HTTPHEADER, headers);
#endif
      curl_easy_setopt(handles[i], CURLOPT_PROTOCOLS , CURLPROTO_SPDY);
      curl_easy_setopt(handles[i], CURLOPT_SSL_VERIFYPEER, 0L);
      /* add the individual transfers */
      curl_multi_add_handle(multi_handle, handles[i]);
   }
  memset(&time,0x00,sizeof(struct timeval));
  gettimeofday(&time, NULL);
  g_time_usec = time.tv_sec * GL_TIME_USEC_PER_SEC + time.tv_usec;

  /* we start some action by calling perform right away */
  curl_multi_perform(multi_handle, &still_running);

  do {
    struct timeval timeout;
    int rc; /* select() return code */

    fd_set fdread;
    fd_set fdwrite;
    fd_set fdexcep;
    int maxfd = -1;

    long curl_timeo = -1;

    FD_ZERO(&fdread);
    FD_ZERO(&fdwrite);
    FD_ZERO(&fdexcep);

    /* set a suitable timeout to play around with */
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    curl_multi_timeout(multi_handle, &curl_timeo);
    if(curl_timeo >= 0) {
      timeout.tv_sec = curl_timeo / 1000;
      if(timeout.tv_sec > 1)
        timeout.tv_sec = 1;
      else
        timeout.tv_usec = (curl_timeo % 1000) * 1000;
    }

    /* get file descriptors from the transfers */
    curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);

    /* In a real-world program you OF COURSE check the return code of the
       function calls.  On success, the value of maxfd is guaranteed to be
       greater or equal than -1.  We call select(maxfd + 1, ...), specially in
       case of (maxfd == -1), we call select(0, ...), which is basically equal
       to sleep. */

    rc = select(maxfd+1, &fdread, &fdwrite, &fdexcep, &timeout);

    switch(rc) {
    case -1:
      /* select error */
      break;
    case 0: /* timeout */
    default: /* action */
      curl_multi_perform(multi_handle, &still_running);
      break;
    }
  } while(still_running);

  memset(&time,0x00,sizeof(struct timeval));
  gettimeofday(&time, NULL);
  totaltime = (double)(time.tv_sec * GL_TIME_USEC_PER_SEC + time.tv_usec - g_time_usec) / CLOCKS_PER_SEC;

  fprintf(stderr,"\n\n\ntime : %f \n\n\n", totaltime);

  /* See how the transfers went */
  while ((msg = curl_multi_info_read(multi_handle, &msgs_left))) {
    if (msg->msg == CURLMSG_DONE) {
        printf("transfer completed with status %d\n", msg->data.result);
    }
  }

  curl_multi_cleanup(multi_handle);

  /* Free the CURL handles */
  for (i=0; i<HANDLECOUNT; i++)
      curl_easy_cleanup(handles[i]);

  return 0;
}
