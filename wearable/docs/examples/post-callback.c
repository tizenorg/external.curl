/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2012, Daniel Stenberg, <daniel@haxx.se>, et al.
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
/* An example source code that issues a HTTP POST and we provide the actual
 * data through a read callback.
 */
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
//#define USE_CHUNKED

const char data[]="<entry xmlns='http://www.w3.org/2005/Atom' xmlns:media='http://search.yahoo.com/mrss/' xmlns:gphoto='http://schemas.google.com/photos/2007'><title type='text'>Trip To Italy</title><summary type='text'>This was the recent trip I took to Italy.</summary><gphoto:location>Italy</gphoto:location><gphoto:access>public</gphoto:access><gphoto:timestamp>1152255600000</gphoto:timestamp><media:group><media:keywords>italy, vacation</media:keywords></media:group><category scheme='http://schemas.google.com/g/2005#kind' term='http://schemas.google.com/photos/2007#album'></category></entry>";

struct WriteThis {
  const char *readptr;
  long sizeleft;
};

size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp)
{
  struct WriteThis *pooh = (struct WriteThis *)userp;

  if(size*nmemb < 20)
    return 0;
  memset(ptr, 0, size*nmemb);

  if(pooh->sizeleft>0) {
    if(pooh->sizeleft>20) {
      strncpy((char *)ptr , pooh->readptr, 20); /* copy one single byte */
      pooh->readptr+=20;                 /* advance pointer */
      pooh->sizeleft-=20;                /* less data left */
      fprintf(stderr, "[data: (%s) , left %d]\n",
            ptr, pooh->sizeleft);
      return 20;                        /* we return 10 byte at a time! */
    }
    else {
      int temp = pooh->sizeleft;
      strncpy((char *)ptr , pooh->readptr, pooh->sizeleft); /* copy one single byte */
      pooh->readptr+=pooh->sizeleft;                 /* advance pointer */
      pooh->sizeleft = 0;                /* less data left */
      fprintf(stderr, "[data: (%s) , left %d]\n",
            ptr, pooh->sizeleft);
      return temp;                        /* we return 10 byte at a time! */
    }
  }

  return 0;                          /* no more data left to deliver */
}

int main(void)
{
  CURL *curl;
  CURLcode res;
  struct curl_slist * headers=NULL;

  struct WriteThis pooh;

  pooh.readptr = data;
  pooh.sizeleft = strlen(data);

  /* In windows, this will init the winsock stuff */
  res = curl_global_init(CURL_GLOBAL_DEFAULT);
  /* Check for errors */
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_global_init() failed: %s\n",
            curl_easy_strerror(res));
    return 1;
  }

  /* get a curl handle */
  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. */
    curl_easy_setopt(curl, CURLOPT_URL, "http://picasaweb.google.com/data/feed/api/user/tarun123456789123456789");

    headers = curl_slist_append(headers, "user-agent:Samsung User Agent");
    headers = curl_slist_append(headers, "authorization:GoogleLogin auth=DQAAAMcAAAA7AjRq8qREIQOPk48TzcNwbdemS4Xbyb7zUce-JQ6SlVOUudVz6diFbIdoTqWmQNUZIY2snntlSj_dUQf7za_S_A-XAJJk_gMbXbo74eHsH2D_Tf5nQ8MC26Pn1rHboiTM7F0UPkbB_B9B0xx4DbNYXhvfdsrkL_tiUW3MLBTh0ARhym3iUNMHtH7rYIUQRG88K-6tLFRls73B79iwVyKxBRkqIJjgkD0uqLcw0nQlv1nhytXTmPdmQED30tWg5QU7qdYifteMrKRU6-3pTHSr");
    headers = curl_slist_append(headers, "gdata-version:2");
    headers = curl_slist_append(headers, "content-type:application/atom+xml");
    headers = curl_slist_append(headers, "accept-charset:ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    headers = curl_slist_append(headers, "accept-encoding:gzip,deflate,sdch");
    headers = curl_slist_append(headers, "accept-language:en-US,en;q=0.8");

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

    /* Now specify we want to POST data */
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    /* we want to use our own read function */
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);

    /* pointer to pass to our read function */
    curl_easy_setopt(curl, CURLOPT_READDATA, &pooh);

    /* get verbose debug output please */
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    /*
      If you use POST to a HTTP 1.1 server, you can send data without knowing
      the size before starting the POST if you use chunked encoding. You
      enable this by adding a header like "Transfer-Encoding: chunked" with
      CURLOPT_HTTPHEADER. With HTTP 1.0 or without chunked transfer, you must
      specify the size in the request.
    */
#ifdef USE_CHUNKED
    {
      headers = curl_slist_append(headers, "Transfer-Encoding: chunked");
      res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
      /* use curl_slist_free_all() after the *perform() call to free this
         list again */
    }
#else
    /* Set the expected POST size. If you want to POST large amounts of data,
       consider CURLOPT_POSTFIELDSIZE_LARGE */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, pooh.sizeleft);
#endif

#ifdef DISABLE_EXPECT
    /*
      Using POST with HTTP 1.1 implies the use of a "Expect: 100-continue"
      header.  You can disable this header with CURLOPT_HTTPHEADER as usual.
      NOTE: if you want chunked transfer too, you need to combine these two
      since you can only set one list of headers with CURLOPT_HTTPHEADER. */

    /* A less good option would be to enforce HTTP 1.0, but that might also
       have other implications. */
    {
      struct curl_slist *chunk = NULL;

      chunk = curl_slist_append(chunk, "Expect:");
      res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
      /* use curl_slist_free_all() after the *perform() call to free this
         list again */
    }
#endif

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return 0;
}

