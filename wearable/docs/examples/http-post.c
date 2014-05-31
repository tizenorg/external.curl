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
#include <stdio.h>
#include <curl/curl.h>

int main(void)
{
  CURL *curl;
  CURLcode res;
  struct curl_slist * headers=NULL;

  /* In windows, this will init the winsock stuff */
  curl_global_init(CURL_GLOBAL_ALL);

  /* get a curl handle */
  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */
    curl_easy_setopt(curl, CURLOPT_URL, "https://picasaweb.google.com/data/feed/api/user/tarun123456789123456789");
    /* Now specify the POST data */
    headers = curl_slist_append(headers, "user-agent:Samsung User Agent");
    headers = curl_slist_append(headers, "authorization:GoogleLogin auth=DQAAAMcAAABsZY37cpSekKFM1dBEXYA5S6kpd-WLzbIZuGUyzG8MDpkzWSXRFw2Vu2R4bz7T6Md86ixuTxwE9BIJC_iX5M2CHnbsLh7Px9Bss1Vx4FMO0CK2TQPzIjR2MQQ2D1viEmPc-LODqXh0lHwIq9TPeSJTaeTZOQqd9gh_dBF079Uxu6wV8qSBJsH_DbWS_SeNpBOTu54koGQOZSR0RE63rzIIWmTUSCNt685Rk-z_V3hYyCNmsUNTM4HZnPX-q10dR2sEOWF56MfrzlifArGcwbNi");
    headers = curl_slist_append(headers, "gdata-version:2");
    headers = curl_slist_append(headers, "content-type:application/atom+xml");
    headers = curl_slist_append(headers, "accept-charset:ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    headers = curl_slist_append(headers, "accept-encoding:gzip,deflate,sdch");
    headers = curl_slist_append(headers, "accept-language:en-US,en;q=0.8");

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "<entry xmlns='http://www.w3.org/2005/Atom' xmlns:media='http://search.yahoo.com/mrss/' xmlns:gphoto='http://schemas.google.com/photos/2007'><title type='text'>Trip To Italy</title><summary type='text'>This was the recent trip I took to Italy.</summary><gphoto:location>Italy</gphoto:location><gphoto:access>public</gphoto:access><gphoto:timestamp>1152255600000</gphoto:timestamp><media:group><media:keywords>italy, vacation</media:keywords></media:group><category scheme='http://schemas.google.com/g/2005#kind' term='http://schemas.google.com/photos/2007#album'></category></entry>");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

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
