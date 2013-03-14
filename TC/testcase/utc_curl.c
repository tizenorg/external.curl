/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <sys/poll.h>
#include <curl/curl.h>
//#include <event.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <tet_api.h>

enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct WriteThis {
  const char *readptr;
  int sizeleft;
};

#define UTC_REMOTE_HTTP_URL		"http://google.com"
#define UTC_REMOTE_HTTP_USERNAME ""
#define UTC_REMOTE_HTTP_PASSWORD ""
#define UTC_REMOTE_HTTP_USERPWD UTC_REMOTE_HTTP_USERNAME ":" UTC_REMOTE_HTTP_PASSWORD

#define UTC_UPLOAD_FILE 		"upload.txt"
#define UTC_REMOTE_FTP_URL		"ftp://ftp.cn.freebsd.org/pub/FreeBSD/"

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;

/* Global information, common to all connections */ 
typedef struct _GlobalInfo {
  //struct event fifo_event;
  //struct event timer_event;
  CURLM *multi;
  int still_running;
  FILE* input;
} GlobalInfo;

#if 0
typedef struct _ConnInfo {
  CURL *easy;
  char *url;
  GlobalInfo *global;
  char error[CURL_ERROR_SIZE];
} ConnInfo;
#endif

/* Information associated with a specific socket */ 
typedef struct _SockInfo {
  curl_socket_t sockfd;
  CURL *easy;
  int action;
  long timeout;
  //struct event ev;
  int evset;
  //GlobalInfo *global;
} SockInfo;

/*******callback**************/
static size_t __curlopt_func_cb(void *ptr, size_t size, size_t nmemb, void *data)
{
	return (size_t)(size * nmemb);
}

static size_t __curlopt_func_err_cb(void *ptr, size_t size, size_t nmemb)
{
	return (size_t)(size * nmemb);
}

static size_t __curlopt_prog_cb (void *p, double dltotal, double dlnow, double ult, double uln)
{
	return 0;
}

static size_t __curlopt_prog_err_cb (void *p, double dltotal, double dlnow, double ult)
{
	return 0;
}

static int __curlopt_debug_cb(CURL *handle, curl_infotype type, char *data, size_t size, void *userp)
{
	return 0;
}

static int __curlopt_debug_err_cb(CURL *handle, curl_infotype type, char *data, size_t size)
{
	return 0;
}

static int __curlopt_sockopt_cb(void *clientp, curl_socket_t curlfd, curlsocktype purpose)
{
	return 0;
}

static int __curlopt_sockopt_err_cb(void *clientp, curl_socket_t curlfd)
{
	return 0;
}

static CURLcode __curlopt_sslctxfun_cb(CURL * curl, void * sslctx, void * parm)
{
	return CURLE_OK;
}

static CURLcode __curlopt_sslctxfun_err_cb(CURL * curl, void * sslctx)
{
	return CURLE_OK;
}

static curlioerr __curlopt_ioctl_cb(CURL *handle, int cmd, void *clientp)
{
	return CURLIOE_OK;
}

static curlioerr __curlopt_ioctl_err_cb(CURL *handle, int cmd)
{
	return CURLIOE_OK;
}

static int __curlmopt_socket_cb(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp)
{
	return CURLM_OK;
}

static int __curlmopt_multi_timer_cb(CURLM *multi, long timeout_ms, GlobalInfo *g)
{
	return CURLM_OK;
}

/*******easy interface*********/
static void utc_curl_easy_init_p(void);
//static void utc_curl_easy_cleanup_n(void);
//static void utc_curl_easy_cleanup_p(void);
static void utc_curl_easy_setopt_n1(void);
static void utc_curl_easy_setopt_n2(void);
static void utc_curl_easy_setopt_p1(void);
static void utc_curl_easy_setopt_p2(void);
static void utc_curl_easy_setopt_p3(void);
static void utc_curl_easy_setopt_p4(void);
static void utc_curl_easy_setopt_p5(void);
static void utc_curl_easy_setopt_p6(void);

/* because of the function limitation,
 * some options cant be tested negatively.
 */
static void utc_curl_easy_perform_n1(void);
static void utc_curl_easy_perform_n2(void);
static void utc_curl_easy_perform_n3(void);
#if 0
static void utc_curl_easy_perform_n4(void);
static void utc_curl_easy_perform_n5(void);
static void utc_curl_easy_perform_n6(void);
static void utc_curl_easy_perform_n7(void);
static void utc_curl_easy_perform_n8(void);
static void utc_curl_easy_perform_n9(void);
static void utc_curl_easy_perform_n10(void);
static void utc_curl_easy_perform_n11(void);
static void utc_curl_easy_perform_n12(void);
static void utc_curl_easy_perform_n13(void);
static void utc_curl_easy_perform_n14(void);
static void utc_curl_easy_perform_n15(void);
static void utc_curl_easy_perform_n16(void);
static void utc_curl_easy_perform_n17(void);
static void utc_curl_easy_perform_n18(void);
#endif
static void utc_curl_easy_perform_p1(void);
static void utc_curl_easy_perform_p2(void);
static void utc_curl_easy_perform_p3(void);
static void utc_curl_easy_perform_p4(void);
static void utc_curl_easy_perform_p5(void);
static void utc_curl_easy_perform_p6(void);
static void utc_curl_easy_perform_p7(void);
static void utc_curl_easy_perform_p8(void);
static void utc_curl_easy_perform_p9(void);
static void utc_curl_easy_perform_p10(void);
static void utc_curl_easy_perform_p11(void);
static void utc_curl_easy_perform_p12(void);
static void utc_curl_easy_perform_p13(void);
//static void utc_curl_easy_perform_p14(void);
static void utc_curl_easy_perform_p15(void);
static void utc_curl_easy_perform_p16(void);
static void utc_curl_easy_perform_p17(void);
static void utc_curl_easy_perform_p18(void);
static void utc_curl_easy_perform_p19(void);
static void utc_curl_easy_perform_p20(void);
static void utc_curl_easy_perform_p21(void);
static void utc_curl_easy_perform_p22(void);
static void utc_curl_easy_perform_p23(void);
static void utc_curl_easy_perform_p24(void);
static void utc_curl_easy_perform_p25(void);
static void utc_curl_easy_perform_p26(void);
static void utc_curl_easy_perform_p27(void);
static void utc_curl_easy_perform_p28(void);
static void utc_curl_easy_perform_p29(void);
static void utc_curl_easy_perform_p30(void);
static void utc_curl_easy_perform_p31(void);
static void utc_curl_easy_perform_p32(void);
static void utc_curl_easy_perform_p33(void);
static void utc_curl_easy_perform_p34(void);
static void utc_curl_easy_perform_p35(void);
static void utc_curl_easy_perform_p36(void);
static void utc_curl_easy_perform_p37(void);
static void utc_curl_easy_perform_p38(void);
static void utc_curl_easy_perform_p39(void);
static void utc_curl_easy_perform_p40(void);
static void utc_curl_easy_perform_p41(void);
static void utc_curl_easy_perform_p42(void);
static void utc_curl_easy_perform_p43(void);
static void utc_curl_easy_perform_p44(void);
static void utc_curl_easy_perform_p45(void);
static void utc_curl_easy_perform_p46(void);
static void utc_curl_easy_perform_p47(void);
static void utc_curl_easy_perform_p48(void);
/*static void utc_curl_easy_perform_p49(void);*/
static void utc_curl_easy_getinfo_n1(void);
static void utc_curl_easy_getinfo_n2(void);
static void utc_curl_easy_getinfo_n3(void);
static void utc_curl_easy_getinfo_n4(void);
static void utc_curl_easy_getinfo_n5(void);
static void utc_curl_easy_getinfo_n6(void);
static void utc_curl_easy_getinfo_n7(void);
static void utc_curl_easy_getinfo_n8(void);
static void utc_curl_easy_getinfo_p(void);
//static void utc_curl_easy_escape_n(void);
static void utc_curl_easy_escape_p(void);
static void utc_curl_easy_strerror_n(void);
static void utc_curl_easy_strerror_p(void);
static void utc_curl_version_p(void);
static void utc_curl_version_info_p(void);
static void utc_curl_formadd_n1(void);
static void utc_curl_formadd_n2(void);
static void utc_curl_formadd_n3(void);
static void utc_curl_formadd_n4(void);
static void utc_curl_formadd_p(void);
static void utc_curl_global_init_p(void);
//static void utc_curl_slist_append_n(void);
static void utc_curl_slist_append_p(void);
static void utc_curl_share_init_p(void);
static void utc_curl_share_cleanup_n(void);
static void utc_curl_share_cleanup_p(void);
static void utc_curl_share_setopt_n1(void);
//static void utc_curl_share_setopt_n2(void);
static void utc_curl_share_setopt_p(void);


/******multi interface*********/
static void utc_curl_multi_init_p(void);
static void utc_curl_multi_add_handle_n1(void);
static void utc_curl_multi_add_handle_n2(void);
static void utc_curl_multi_add_handle_p(void);
static void utc_curl_multi_socket_action_p(void);
static void utc_curl_multi_assign_n(void);
//static void utc_curl_multi_assign_p(void);
static void utc_curl_multi_cleanup_n(void);
static void utc_curl_multi_cleanup_p(void);
static void utc_curl_multi_fdset_n(void);
static void utc_curl_multi_fdset_p(void);
static void utc_curl_multi_info_read_n(void);
static void utc_curl_multi_info_read_p(void);
static void utc_curl_multi_perform_n(void);
static void utc_curl_multi_perform_p1(void);
static void utc_curl_multi_perform_p2(void);
static void utc_curl_multi_perform_p3(void);
static void utc_curl_multi_perform_p4(void);
static void utc_curl_multi_perform_p5(void);
static void utc_curl_multi_perform_p6(void);
static void utc_curl_multi_perform_p7(void);
static void utc_curl_multi_remove_handle_n1(void);
static void utc_curl_multi_remove_handle_n2(void);
static void utc_curl_multi_remove_handle_p(void);
static void utc_curl_multi_setopt_n1(void);
static void utc_curl_multi_setopt_n2(void);
static void utc_curl_multi_setopt_p1(void);
static void utc_curl_multi_setopt_p2(void);
static void utc_curl_multi_setopt_p3(void);
static void utc_curl_multi_setopt_p4(void);
static void utc_curl_multi_setopt_p5(void);
static void utc_curl_multi_setopt_p6(void);
static void utc_curl_multi_strerror_n(void);
static void utc_curl_multi_strerror_p(void);
static void utc_curl_multi_timeout_n(void);
static void utc_curl_multi_timeout_p(void);

struct tet_testlist tet_testlist[] = {
	/*******positive test*********/
	/*******easy interface*********/
	{ utc_curl_easy_init_p,			POSITIVE_TC_IDX },
//	{ utc_curl_easy_cleanup_p,		POSITIVE_TC_IDX },
	{ utc_curl_easy_setopt_p1,		POSITIVE_TC_IDX },
	{ utc_curl_easy_setopt_p2,		POSITIVE_TC_IDX },
	{ utc_curl_easy_setopt_p3,		POSITIVE_TC_IDX },
	{ utc_curl_easy_setopt_p4,		POSITIVE_TC_IDX },
	{ utc_curl_easy_setopt_p5,		POSITIVE_TC_IDX },
	{ utc_curl_easy_setopt_p6,		POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_n1,		NEGATIVE_TC_IDX },
	{ utc_curl_easy_perform_n2,		NEGATIVE_TC_IDX },
	{ utc_curl_easy_perform_n3,		NEGATIVE_TC_IDX },
#if 0
	{ utc_curl_easy_perform_n4,		NEGATIVE_TC_IDX },
	{ utc_curl_easy_perform_n5,		NEGATIVE_TC_IDX },
	{ utc_curl_easy_perform_n6,		NEGATIVE_TC_IDX },
	{ utc_curl_easy_perform_n7,		NEGATIVE_TC_IDX },
	{ utc_curl_easy_perform_n8,		NEGATIVE_TC_IDX },
	{ utc_curl_easy_perform_n9,		NEGATIVE_TC_IDX },
	{ utc_curl_easy_perform_n10,	NEGATIVE_TC_IDX },
	{ utc_curl_easy_perform_n11,	NEGATIVE_TC_IDX },
	{ utc_curl_easy_perform_n12,	NEGATIVE_TC_IDX },
	{ utc_curl_easy_perform_n13,	NEGATIVE_TC_IDX },
	{ utc_curl_easy_perform_n14,	NEGATIVE_TC_IDX },
	{ utc_curl_easy_perform_n15,	NEGATIVE_TC_IDX },
	{ utc_curl_easy_perform_n16,	NEGATIVE_TC_IDX },
	{ utc_curl_easy_perform_n17,	NEGATIVE_TC_IDX },
	{ utc_curl_easy_perform_n18,	NEGATIVE_TC_IDX },
#endif
	{ utc_curl_easy_perform_p1, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p2, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p3, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p4, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p5, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p6, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p7, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p8, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p9, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p10, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p11, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p12, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p13, 	POSITIVE_TC_IDX },
//	{ utc_curl_easy_perform_p14, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p15, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p16, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p17, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p18, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p19, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p20, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p21, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p22, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p23, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p24, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p25, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p26, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p27, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p28, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p29, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p30, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p31, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p32, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p33, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p34, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p35, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p36, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p37, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p38, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p39, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p40, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p41, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p42, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p43, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p44, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p45, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p46, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p47, 	POSITIVE_TC_IDX },
	{ utc_curl_easy_perform_p48, 	POSITIVE_TC_IDX },
	/* Lacking of right SSL engine and *.pem file, so remove it */
	/*{ utc_curl_easy_perform_p48, 	POSITIVE_TC_IDX },*/
	{ utc_curl_easy_getinfo_p, 		POSITIVE_TC_IDX },
	{ utc_curl_easy_escape_p, 		POSITIVE_TC_IDX },
	{ utc_curl_easy_strerror_p,		POSITIVE_TC_IDX },
	{ utc_curl_version_p, 			POSITIVE_TC_IDX },
	{ utc_curl_version_info_p, 		POSITIVE_TC_IDX },
	{ utc_curl_formadd_p,			POSITIVE_TC_IDX },
	{ utc_curl_global_init_p,		POSITIVE_TC_IDX },
	{ utc_curl_slist_append_p,		POSITIVE_TC_IDX },
	{ utc_curl_share_init_p,		POSITIVE_TC_IDX },
	{ utc_curl_share_cleanup_p,		POSITIVE_TC_IDX },
	{ utc_curl_share_setopt_p, 		POSITIVE_TC_IDX },
	/******multi interface*********/
	{ utc_curl_multi_init_p,          POSITIVE_TC_IDX },
	{ utc_curl_multi_add_handle_p,    POSITIVE_TC_IDX },
	{ utc_curl_multi_socket_action_p, POSITIVE_TC_IDX },
//	{ utc_curl_multi_assign_p,        POSITIVE_TC_IDX },	
	{ utc_curl_multi_cleanup_p,       POSITIVE_TC_IDX },
	{ utc_curl_multi_fdset_p,         POSITIVE_TC_IDX },
	{ utc_curl_multi_info_read_p,     POSITIVE_TC_IDX },
	{ utc_curl_multi_perform_p1,       POSITIVE_TC_IDX },
	{ utc_curl_multi_perform_p2,       POSITIVE_TC_IDX },
	{ utc_curl_multi_perform_p3,       POSITIVE_TC_IDX },
	{ utc_curl_multi_perform_p4,       POSITIVE_TC_IDX },
	{ utc_curl_multi_perform_p5,       POSITIVE_TC_IDX },
	{ utc_curl_multi_perform_p6,       POSITIVE_TC_IDX },
	{ utc_curl_multi_perform_p7,       POSITIVE_TC_IDX },
	{ utc_curl_multi_remove_handle_p, POSITIVE_TC_IDX },
	{ utc_curl_multi_setopt_p1,        POSITIVE_TC_IDX },
	{ utc_curl_multi_setopt_p2,        POSITIVE_TC_IDX },
	{ utc_curl_multi_setopt_p3,        POSITIVE_TC_IDX },
	{ utc_curl_multi_setopt_p4,        POSITIVE_TC_IDX },
	{ utc_curl_multi_setopt_p5,        POSITIVE_TC_IDX },
	{ utc_curl_multi_setopt_p6,        POSITIVE_TC_IDX },
	{ utc_curl_multi_strerror_p,      POSITIVE_TC_IDX },
	{ utc_curl_multi_timeout_p,       POSITIVE_TC_IDX },


	/******negative test*********/
	/*******easy interface*********/
//	{ utc_curl_easy_cleanup_n,		NEGATIVE_TC_IDX },
	{ utc_curl_easy_setopt_n1,		NEGATIVE_TC_IDX },
	{ utc_curl_easy_setopt_n2,		NEGATIVE_TC_IDX },
	{ utc_curl_easy_getinfo_n1,		NEGATIVE_TC_IDX },
	{ utc_curl_easy_getinfo_n2,		NEGATIVE_TC_IDX },
	{ utc_curl_easy_getinfo_n3,		NEGATIVE_TC_IDX },
	{ utc_curl_easy_getinfo_n4,		NEGATIVE_TC_IDX },
	{ utc_curl_easy_getinfo_n5,		NEGATIVE_TC_IDX },
	{ utc_curl_easy_getinfo_n6,		NEGATIVE_TC_IDX },
	{ utc_curl_easy_getinfo_n7,		NEGATIVE_TC_IDX },
	{ utc_curl_easy_getinfo_n8,		NEGATIVE_TC_IDX },
//	{ utc_curl_easy_escape_n,		NEGATIVE_TC_IDX },
	{ utc_curl_easy_strerror_n,		NEGATIVE_TC_IDX },
	{ utc_curl_formadd_n1, 			NEGATIVE_TC_IDX },
	{ utc_curl_formadd_n2, 			NEGATIVE_TC_IDX },
	{ utc_curl_formadd_n3, 			NEGATIVE_TC_IDX },
	{ utc_curl_formadd_n4, 			NEGATIVE_TC_IDX },
//	{ utc_curl_slist_append_n,		NEGATIVE_TC_IDX },
	{ utc_curl_share_cleanup_n,		NEGATIVE_TC_IDX },
	{ utc_curl_share_setopt_n1,		NEGATIVE_TC_IDX },
//	{ utc_curl_share_setopt_n2, 	NEGATIVE_TC_IDX },
	/******multi interface*********/
	{ utc_curl_multi_add_handle_n1,     NEGATIVE_TC_IDX },                        
	{ utc_curl_multi_add_handle_n2,     NEGATIVE_TC_IDX },                        
	{ utc_curl_multi_assign_n,          NEGATIVE_TC_IDX },
	{ utc_curl_multi_cleanup_n,         NEGATIVE_TC_IDX },	
	{ utc_curl_multi_fdset_n,           NEGATIVE_TC_IDX },
	{ utc_curl_multi_info_read_n,       NEGATIVE_TC_IDX },	
	{ utc_curl_multi_perform_n,         NEGATIVE_TC_IDX },
	{ utc_curl_multi_remove_handle_n1,  NEGATIVE_TC_IDX },
	{ utc_curl_multi_remove_handle_n2,  NEGATIVE_TC_IDX },
	{ utc_curl_multi_setopt_n1,         NEGATIVE_TC_IDX },	
	{ utc_curl_multi_setopt_n2,         NEGATIVE_TC_IDX },	
	{ utc_curl_multi_strerror_n,        NEGATIVE_TC_IDX },
	{ utc_curl_multi_timeout_n,         NEGATIVE_TC_IDX },
	{ NULL, 0 },
};


static void startup(void)
{
	/* start of TC */
	tet_printf("\n TC start");
}

static void cleanup(void)
{
	/* end of TC */
	tet_printf("\n TC end");
}

/**
 * @brief Positive test case of curl_easy_init()
 */
static void utc_curl_easy_init_p(void)
{
	CURL *url = NULL;

	//  allocation test of new CURL handle
	url = curl_easy_init();
	dts_check_ne("curl_easy_init", url, NULL,
		"Must not return NULL in case of new CURL handle allocation");

	curl_easy_cleanup(url);
}

#if 0 // curl_easy_cleanup don't reset handle parameter to NULL. So ignore test case.
/**
 * @brief Negative test case of curl_easy_cleanup()
 */
static void utc_curl_easy_cleanup_n(void)
{
	CURL *url = NULL;

	curl_easy_cleanup(url);
	dts_check_eq("curl_easy_cleanup", url, NULL,
		"Must return NULL in case of valid handle");
}

/**
 * @brief Positive test case of curl_easy_cleanup()
 */
static void utc_curl_easy_cleanup_p(void)
{
	CURL *url = NULL;

	//  allocation test of new CURL handle
	url = curl_easy_init();

	curl_easy_cleanup(url);
	dts_check_eq("curl_easy_cleanup", url, NULL,
		"Must return NULL in case of valid handle");
}
#endif

/**
 * @brief Negative 1st test case of curl_easy_setopt()
 */
static void utc_curl_easy_setopt_n1(void)
{
	int ret = -1;
	CURL *url = NULL;
	CURLoption option = -1;

	//  allocation test of new CURL handle
	url = curl_easy_init();
	ret = curl_easy_setopt(url, option, 1L);
	dts_check_eq("curl_easy_setopt", ret, CURLE_FAILED_INIT,
		"Must return CURLE_FAILED_INIT in case of invalid option");

	curl_easy_cleanup(url);
}

/**
 * @brief Negative 2st test case of curl_easy_setopt()
 */
static void utc_curl_easy_setopt_n2(void)
{
	int ret = -1;
	CURL *url = NULL;

	//  allocation test of new CURL handle
	ret = curl_easy_setopt(url, CURLOPT_VERBOSE, 1L);
	dts_check_eq("curl_easy_setopt", ret, CURLE_BAD_FUNCTION_ARGUMENT,
		"Must return CURLE_BAD_FUNCTION_ARGUMENT in case of invalid handle");

	curl_easy_cleanup(url);
}

/**
 * @brief Positive test case of curl_easy_setopt()
 */
static void utc_curl_easy_setopt_p1(void)
{
	int ret = -1;
	CURL *url = NULL;

	//	allocation test of new CURL handle
	url = curl_easy_init();

	ret = curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	dts_check_eq("curl_easy_setopt", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid url");

	curl_easy_cleanup(url);
}

/**
 * @brief Positive test case of curl_easy_setopt()
 */
static void utc_curl_easy_setopt_p2(void)
{
	int ret = -1;
	CURL *url = NULL;

	//	allocation test of new CURL handle
	url = curl_easy_init();

	ret = curl_easy_setopt(url, CURLOPT_CAPATH, "/opt/etc/ssl/certs/");
	dts_check_eq("curl_easy_setopt", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid url");

	curl_easy_cleanup(url);
}

/**
 * @brief Positive test case of curl_easy_setopt()
 */
static void utc_curl_easy_setopt_p3(void)
{
	int ret = -1;
	CURL *url = NULL;

	//	allocation test of new CURL handle
	url = curl_easy_init();

	ret = curl_easy_setopt(url, CURLOPT_CONNECT_ONLY, 1L);
	dts_check_eq("curl_easy_setopt", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid url");

	curl_easy_cleanup(url);
}

/**
 * @brief Positive test case of curl_easy_setopt()
 */
static void utc_curl_easy_setopt_p4(void)
{
	int ret = -1;
	CURL *url = NULL;

	//	allocation test of new CURL handle
	url = curl_easy_init();

	ret = curl_easy_setopt(url, CURLOPT_ENCODING, NULL);
	dts_check_eq("curl_easy_setopt", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid url");

	curl_easy_cleanup(url);
}

/**
 * @brief Positive test case of curl_easy_setopt()
 */
static void utc_curl_easy_setopt_p5(void)
{
	int ret = -1;
	CURL *url = NULL;

	//	allocation test of new CURL handle
	url = curl_easy_init();

	ret = curl_easy_setopt(url, CURLOPT_MAXREDIRS, 3L);
	dts_check_eq("curl_easy_setopt", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid url");

	curl_easy_cleanup(url);
}

/**
 * @brief Positive test case of curl_easy_setopt()
 */
static void utc_curl_easy_setopt_p6(void)
{
	int ret = -1;
	CURL *url = NULL;

	//	allocation test of new CURL handle
	url = curl_easy_init();

	ret = curl_easy_setopt(url, CURLOPT_NOBODY, 1L);
	dts_check_eq("curl_easy_setopt", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid url");

	curl_easy_cleanup(url);
}

/**
 * @brief Negative 1st test case of setting proxy option to curl_easy_perform()
 */
static void utc_curl_easy_perform_n1(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_PROXY, "st");
	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_COULDNT_RESOLVE_PROXY,
		"Must return CURLE_COULDNT_RESOLVE_PROXY in case of invalid proxy");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Negative 2st test case of setting interface option to curl_easy_perform()
 */
static void utc_curl_easy_perform_n2(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_INTERFACE, "unknown");
	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_INTERFACE_FAILED,
		"Must return CURLE_INTERFACE_FAILED in case of invalid interface");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Negative 3st test case of setting sslengine option to curl_easy_perform()
 */
static void utc_curl_easy_perform_n3(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	ret = curl_easy_setopt(url, CURLOPT_SSLENGINE, "chil");
	dts_check_eq("curl_easy_perform", ret, CURLE_SSL_ENGINE_INITFAILED,
		"Must return CURLE_SSL_ENGINE_INITFAILED in case of invalid cainfo");

	curl_easy_perform(url);
	curl_easy_cleanup(url);
	curl_global_cleanup();
}

#if 0 // Not checked by curl - So ignore it.
/**
 * @brief Negative 4st test case of setting writefunction option to curl_easy_perform()
 */
static void utc_curl_easy_perform_n4(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_WRITEFUNCTION, NULL);
	curl_easy_setopt(url, CURLOPT_WRITEDATA, NULL);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_COULDNT_RESOLVE_HOST,
		"Must return CURLE_COULDNT_RESOLVE_HOST in case of invalid writefunction");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Negative 5st test case of setting cookie option to curl_easy_perform()
 */
static void utc_curl_easy_perform_n5(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_COOKIE, "123");

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_URL_MALFORMAT,
		"Must return CURLE_URL_MALFORMAT in case of invalid cookie");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Negative 6st test case of setting httpheader option to curl_easy_perform()
 */
static void utc_curl_easy_perform_n6(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_HTTPHEADER, NULL);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_URL_MALFORMAT,
		"Must return CURLE_URL_MALFORMAT in case of invalid httpheader");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Negative 7st test case of setting writeheader option to curl_easy_perform()
 */
static void utc_curl_easy_perform_n7(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_WRITEHEADER, NULL);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_URL_MALFORMAT,
		"Must return CURLE_URL_MALFORMAT in case of invalid writeheader");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Negative 8st test case of setting cookiefile option to curl_easy_perform()
 */
static void utc_curl_easy_perform_n8(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_COOKIEFILE, NULL);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_URL_MALFORMAT,
		"Must return CURLE_URL_MALFORMAT in case of invalid cookiefile");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Negative 9st test case of setting customrequest option to curl_easy_perform()
 */
static void utc_curl_easy_perform_n9(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_CUSTOMREQUEST, "TEST");

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_COULDNT_CONNECT,
		"Must return CURLE_COULDNT_CONNECT in case of invalid customrequest");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Negative 10st test case of setting progressdata option to curl_easy_perform()
 */
static void utc_curl_easy_perform_n10(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(url, CURLOPT_PROGRESSFUNCTION, NULL);
	curl_easy_setopt(url, CURLOPT_PROGRESSDATA, NULL);
	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_URL_MALFORMAT,
		"Must return CURLE_URL_MALFORMAT in case of invalid progressdata");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Negative 11st test case of setting cainfo option to curl_easy_perform()
 */
static void utc_curl_easy_perform_n11(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_SSL_VERIFYPEER , 1L);
	ret = curl_easy_setopt(url, CURLOPT_CAINFO, "DUMMY");

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_URL_MALFORMAT,
		"Must return CURLE_URL_MALFORMAT in case of invalid cainfo");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Negative 12st test case of setting headerfunction option to curl_easy_perform()
 */
static void utc_curl_easy_perform_n12(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_HEADERFUNCTION, NULL);
	ret = curl_easy_setopt(url, CURLOPT_HEADERDATA, NULL);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_URL_MALFORMAT,
		"Must return CURLE_URL_MALFORMAT in case of invalid header function");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Negative 13st test case of setting cookiejar option to curl_easy_perform()
 */
static void utc_curl_easy_perform_n13(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_COOKIEJAR, "test");

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_COULDNT_RESOLVE_HOST,
		"Must return CURLE_COULDNT_RESOLVE_HOST in case of invalid cookiejar");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Negative 14st test case of setting sslkey option to curl_easy_perform()
 */
static void utc_curl_easy_perform_n14(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_SSLKEY, "test");

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_COULDNT_RESOLVE_HOST,
		"Must return CURLE_COULDNT_RESOLVE_HOST in case of invalid sslkey");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Negative 15st test case of setting debugfunction option to curl_easy_perform()
 */
static void utc_curl_easy_perform_n15(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(url, CURLOPT_DEBUGFUNCTION, NULL);
	curl_easy_setopt(url, CURLOPT_DEBUGDATA, NULL);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_URL_MALFORMAT,
		"Must return CURLE_URL_MALFORMAT in case of invalid debugfunction");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Negative 16st test case of setting cookielist option to curl_easy_perform()
 */
static void utc_curl_easy_perform_n16(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_COOKIELIST, NULL);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_URL_MALFORMAT,
		"Must return CURLE_URL_MALFORMAT in case of invalid cookielist");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Negative 17st test case of setting username option to curl_easy_perform()
 */
static void utc_curl_easy_perform_n17(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_USERNAME, "test");

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_URL_MALFORMAT,
		"Must return CURLE_URL_MALFORMAT in case of invalid username");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Negative 18st test case of setting password option to curl_easy_perform()
 */
static void utc_curl_easy_perform_n18(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_PASSWORD, "123");

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_URL_MALFORMAT,
		"Must return CURLE_URL_MALFORMAT in case of invalid password");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}
#endif

/**
 * @brief Positive 1st test case of setting proxy option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p1(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_PROXY, NULL);
	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid proxy");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 2st test case of setting userpwd option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p2(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);

	/* authentication method */
	curl_easy_setopt(url, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
	curl_easy_setopt(url, CURLOPT_USERPWD, NULL);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid userpwd");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 3st test case of setting errorbuffer option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p3(void)
{
	int ret = -1;
	CURL *url = NULL;
	char *buffer = NULL;
	int buf_size = 0;

	buf_size = CURL_ERROR_SIZE;
	buffer = (char *)calloc(1, buf_size);

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(url, CURLOPT_ERRORBUFFER, buffer);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid errorbuffer");

	curl_easy_cleanup(url);
	curl_global_cleanup();

	if (buffer) {
		free(buffer);
		buffer = NULL;
	}
}

/**
 * @brief Positive 4st test case of setting writefunction option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p4(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_WRITEFUNCTION, __curlopt_func_cb);
	curl_easy_setopt(url, CURLOPT_WRITEDATA, NULL);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid writefunction");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 5st test case of setting timeout option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p5(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_TIMEOUT, 30L);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid timeout");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 6st test case of setting useragent option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p6(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_USERAGENT, NULL);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid useragent");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 7st test case of setting httpheader option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p7(void)
{
	int ret = -1;
	CURL *url = NULL;
    struct curl_slist *chunk = NULL;

    chunk = curl_slist_append(chunk, "Accept: moo");
    chunk = curl_slist_append(chunk, "Another: yes");

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_HTTPHEADER, chunk);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid httpheader");

	curl_slist_free_all(chunk);
	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 8st test case of setting httppost option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p8(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_HTTPPOST, NULL);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid httppost");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 9st test case of setting writeheader option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p9(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_WRITEHEADER, stderr);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid writeheader");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 10st test case of setting timevalue option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p10(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_TIMECONDITION, CURL_TIMECOND_IFMODSINCE);
	curl_easy_setopt(url, CURLOPT_TIMEVALUE, time(NULL));

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid timevalue");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 11st test case of setting customrequest option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p11(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_CUSTOMREQUEST, "GET");

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid customrequest");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 12st test case of setting progressdata option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p12(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(url, CURLOPT_PROGRESSFUNCTION, __curlopt_prog_cb);
	curl_easy_setopt(url, CURLOPT_PROGRESSDATA, NULL);
	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid progressdata");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 13st test case of setting postfieldsize option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p13(void)
{
	int ret = -1;
	CURL *url = NULL;
	static const char *postthis = "moo mooo moo moo";

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_POSTFIELDS, postthis);
	curl_easy_setopt(url, CURLOPT_POSTFIELDSIZE, strlen(postthis));
	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid postfieldsize");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

// It's network dependency test case.
// So ignore it until version upgrade to 7.24.0 to support "if!"(any interface)
#if 0 
/**
 * @brief Positive 14st test case of setting interface option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p14(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_INTERFACE, "pdp0");
	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid interface");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}
#endif

/**
 * @brief Positive 15st test case of setting cainfo option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p15(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_CAINFO, NULL);
	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid cainfo");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 16st test case of setting connecttimeout option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p16(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_CONNECTTIMEOUT, 30L);
	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid connect timeout");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 17st test case of setting headerfunction option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p17(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_HEADERFUNCTION, __curlopt_func_cb);
	curl_easy_setopt(url, CURLOPT_HEADERDATA, NULL);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid header function");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 18st test case of setting debugfunction option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p18(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(url, CURLOPT_DEBUGFUNCTION, __curlopt_debug_cb);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid default function");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 19st test case of setting private option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p19(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_PRIVATE, "test");

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid private");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 20st test case of setting maxfilesize option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p20(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_MAXFILESIZE, 10240L);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid maxfilesize");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 21st test case of setting infilesize large option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p21(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_INFILESIZE_LARGE, (curl_off_t)1024L);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid infilesize large");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 22st test case of setting localport option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p22(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_LOCALPORT, 8000L);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid localport");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 23st test case of setting timeout ms option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p23(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_TIMEOUT_MS, 30000L);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid timeout ms");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 24st test case of setting connecttimeout ms option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p24(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_CONNECTTIMEOUT_MS, 30000L);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid connectimeout ms");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 25st test case of setting username option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p25(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_USERNAME, NULL);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid username");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 26st test case of setting password option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p26(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_PASSWORD, NULL);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid password");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}



/**
 * @brief Positive 27st test case of setting crlf option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p27(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_CRLF, 1L);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid crlf");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 28st test case of setting verbose option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p28(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_VERBOSE, 1L);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid verbose");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 29st test case of setting header option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p29(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_HEADER, 1L);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid header");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 30st test case of setting noprogress option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p30(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_NOPROGRESS, 1L);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid noprogress");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 31st test case of setting upload option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p31(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_UPLOAD, 1L);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid upload");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 32st test case of setting post option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p32(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_POST, 1L);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid post");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}


/**
 * @brief Positive 33st test case of setting followlocation option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p33(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_FOLLOWLOCATION, 0L);
	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid followlocation");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 34st test case of setting put option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p34(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_PUT, 1L);
	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid put");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 35st test case of setting httpproxytunnel option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p35(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_HTTPPROXYTUNNEL, 1L);
	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid httpproxytunnel");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 36st test case of setting verifypeer option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p36(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_SSL_VERIFYPEER, 1L);
	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid ssl verifypeer");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 37st test case of setting forbid reuse option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p37(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_FORBID_REUSE, 1L);
	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid forbid reuse");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 38st test case of setting httpget option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p38(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_HTTPGET, 1L);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid httpget");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 39st test case of setting ssl verifyhost option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p39(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_SSL_VERIFYHOST, 0L);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid ssl verifyhost");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 40st test case of setting http version option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p40(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid http version");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 41st test case of setting ftp use epsv option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p41(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_FTP_URL);
	curl_easy_setopt(url, CURLOPT_FTP_USE_EPSV, 0L);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid ftp use epsv");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 42st test case of setting cookiesession option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p42(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_COOKIESESSION, 1L);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid cookiesession");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 43st test case of setting nosignal option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p43(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_NOSIGNAL, 1L);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid nosignal");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 44st test case of setting proxytype option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p44(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_PROXYTYPE, 1L);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid proxytype");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 45st test case of setting httpauth option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p45(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_HTTPAUTH, 1L);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid httpauth");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 46st test case of setting ipresolve option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p46(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_IPRESOLVE, 1L);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid ipresolve");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 47st test case of setting nodelay option to curl_easy_perform()
 */
static void utc_curl_easy_perform_p47(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_TCP_NODELAY, 1L);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid tcp nodelay");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive 48st test case of curl_easy_perform()
 */
static void utc_curl_easy_perform_p48(void)
{
	CURL *url = NULL;
	int ret = -1;
	char nline[256] = {0};

	curl_global_init(CURL_GLOBAL_ALL);
	url = curl_easy_init();

	if (NULL == url) {
		curl_global_cleanup();
		return;
	}

	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
	curl_easy_setopt(url, CURLOPT_HTTPGET, 1L);
	curl_easy_setopt(url, CURLOPT_COOKIE,
		"DvForum=userid=24122&usercookies=0&userhidden=2&password=w0reu3g775VrY745&userclass=%96%7C&username=%A1%FA%A1%FA%B8%D2%CB%C0%B6%D3%A1%FA&StatUserID=2194783945 ");
	curl_easy_setopt(url, CURLOPT_COOKIEFILE, "");
	curl_easy_setopt(url, CURLOPT_COOKIESESSION, 1223L);

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK, "Must return CURLE_OK in case of perform");

	curl_easy_setopt(url, CURLOPT_COOKIELIST, "ALL");

	/* Netscape format cookie */
	snprintf(nline, sizeof(nline), "%s\t%s\t%s\t%s\t%lu\t%s\t%s",
	  ".google.com", "TRUE", "/", "FALSE", time(NULL) + 31337, "PREF", "hello google, i like you very much!");
	curl_easy_setopt(url, CURLOPT_COOKIELIST, nline);

	/* HTTP-header style cookie */
	snprintf(nline, sizeof(nline),
	  "Set-Cookie: OLD_PREF=3d141414bf4209321; "
	  "expires=Sun, 17-Jan-2038 19:14:07 GMT; path=/; domain=.google.com");
	curl_easy_setopt(url, CURLOPT_COOKIELIST, nline);
	curl_easy_setopt(url, CURLOPT_COOKIEJAR, "log/jar506");

	ret = curl_easy_perform(url);
	dts_check_eq("curl_easy_perform", ret, CURLE_OK, "Must return CURLE_OK in case of perform");

    curl_easy_cleanup(url);
	curl_global_cleanup();
}
#if 0
/**
 * @brief Positive 49st test case of ssl negivation curl_easy_perform()
 */
static void utc_curl_easy_perform_p49(void)
{
	CURL *url = NULL;
	int ret = -1;
	static const char *pCertFile = "testcert.pem";
	static const char *pCACertFile="cacert.pem";
	const char *pKeyName;
	const char *pKeyType;
	const char *pEngine;

	pKeyName  = "rsa_test";
	pKeyType  = "ENG";
	pEngine   = "chil";

	curl_global_init(CURL_GLOBAL_ALL);
	url = curl_easy_init();

	if (NULL == url) {
		curl_global_cleanup();
		return;
	}

	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);

	while (url) {
		if (pEngine) {
			ret = curl_easy_setopt(url, CURLOPT_SSLENGINE, pEngine);
			if (ret != CURLE_OK) {
				fprintf(stderr,"can't set crypto engine\n");
				break;
			}

			ret = curl_easy_setopt(url, CURLOPT_SSLENGINE_DEFAULT, 1L);
			if (ret != CURLE_OK) {
				/* set the crypto engine as default */
				/* only needed for the first time you load
				 a engine in a curl object... */
				fprintf(stderr,"can't set crypto engine as default\n");
				break;
			}
		}

		/* cert is stored PEM coded in file... */
		/* since PEM is default, we needn't set it for PEM */
		curl_easy_setopt(url, CURLOPT_SSLCERTTYPE, "PEM");

		/* set the cert for client authentication */
		curl_easy_setopt(url, CURLOPT_SSLCERT, pCertFile);

		/* if we use a key stored in a crypto engine,
		   we must set the key type to "ENG" */
		curl_easy_setopt(url, CURLOPT_SSLKEYTYPE, pKeyType);

		/* set the private key (file or ID in engine) */
		curl_easy_setopt(url, CURLOPT_SSLKEY, pKeyName);

		/* set the file with the certs vaildating the server */
		curl_easy_setopt(url, CURLOPT_CAINFO, pCACertFile);

		/* disconnect if we can't validate server's cert */
		curl_easy_setopt(url, CURLOPT_SSL_VERIFYPEER, 1L);

		ret = curl_easy_perform(url);
		dts_check_eq("curl_easy_perform", ret, CURLE_OK, "Must return CURLE_OK in case of valid perform");

		break;
	}

	/* always cleanup */
	curl_easy_cleanup(url);
	curl_global_cleanup();
}
#endif

/**
 * @brief Negative 1st test case of curl_easy_getinfo()
 */
static void utc_curl_easy_getinfo_n1(void)
{
	int ret = -1;
	CURL *url = NULL;
	long response;

	ret = curl_easy_getinfo(url, CURLINFO_RESPONSE_CODE, &response);
	dts_check_eq("curl_easy_getinfo", ret, CURLE_BAD_FUNCTION_ARGUMENT,
		"Must return CURLE_BAD_FUNCTION_ARGUMENT in case of invalid handle");
}

/**
 * @brief Negative 2st test case of curl_easy_getinfo()
 */
static void utc_curl_easy_getinfo_n2(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);

	ret = curl_easy_perform(url);
    if (ret != CURLE_OK) {
		fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(ret));
		return;
    }

	int opt = -1;
	void *code = NULL;

	ret = curl_easy_getinfo(url, opt, &code);
	dts_check_eq("curl_easy_getinfo", ret, CURLE_BAD_FUNCTION_ARGUMENT,
		"Must return CURLE_BAD_FUNCTION_ARGUMENT in case of invalid option");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Negative 3st test case of curl_easy_getinfo()
 */
static void utc_curl_easy_getinfo_n3(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
    curl_easy_setopt(url, CURLOPT_VERBOSE, 1L);

	ret = curl_easy_perform(url);
    if (ret != CURLE_OK) {
		fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(ret));
		return;
    }

	int opt = CURLINFO_RESPONSE_CODE;
	char *value = NULL; /*invalid type*/

	ret = curl_easy_getinfo(url, opt, value);
	dts_check_eq("curl_easy_getinfo", ret, CURLE_BAD_FUNCTION_ARGUMENT,
		"Must return CURLE_BAD_FUNCTION_ARGUMENT in case of invalid response code");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Negative 4st test case of curl_easy_getinfo()
 */
static void utc_curl_easy_getinfo_n4(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
    curl_easy_setopt(url, CURLOPT_VERBOSE, 1L);

	ret = curl_easy_perform(url);
    if (ret != CURLE_OK) {
		fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(ret));
		return;
    }

	int opt = CURLINFO_TOTAL_TIME;
	char *value = NULL; /*invalid type*/

	ret = curl_easy_getinfo(url, opt, value);
	dts_check_eq("curl_easy_getinfo", ret, CURLE_BAD_FUNCTION_ARGUMENT,
		"Must return CURLE_BAD_FUNCTION_ARGUMENT in case of invalid total time");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Negative 5st test case of curl_easy_getinfo()
 */
static void utc_curl_easy_getinfo_n5(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
    curl_easy_setopt(url, CURLOPT_VERBOSE, 1L);

	ret = curl_easy_perform(url);
    if (ret != CURLE_OK) {
		fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(ret));
		return;
    }

	int opt = CURLINFO_CONNECT_TIME;
	char *value = NULL; /*invalid type*/

	ret = curl_easy_getinfo(url, opt, value);
	dts_check_eq("curl_easy_getinfo", ret, CURLE_BAD_FUNCTION_ARGUMENT,
		"Must return CURLE_BAD_FUNCTION_ARGUMENT in case of invalid connect time");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Negative 6st test case of curl_easy_getinfo()
 */
static void utc_curl_easy_getinfo_n6(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
    curl_easy_setopt(url, CURLOPT_VERBOSE, 1L);

	ret = curl_easy_perform(url);
    if (ret != CURLE_OK) {
		fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(ret));
		return;
    }

	int opt = CURLINFO_CONTENT_TYPE;
	char *value = 0; /*invalid type*/

	ret = curl_easy_getinfo(url, opt, value);
	dts_check_eq("curl_easy_getinfo", ret, CURLE_BAD_FUNCTION_ARGUMENT,
		"Must return CURLE_BAD_FUNCTION_ARGUMENT in case of invalid connect type");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Negative 7st test case of curl_easy_getinfo()
 */
static void utc_curl_easy_getinfo_n7(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
    curl_easy_setopt(url, CURLOPT_VERBOSE, 1L);

	ret = curl_easy_perform(url);
    if (ret != CURLE_OK) {
		fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(ret));
		return;
    }

	int opt = CURLINFO_PRIVATE;
	char *value = 0; /*invalid type*/

	ret = curl_easy_getinfo(url, opt, value);
	dts_check_eq("curl_easy_getinfo", ret, CURLE_BAD_FUNCTION_ARGUMENT,
		"Must return CURLE_BAD_FUNCTION_ARGUMENT in case of invalid private");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Negative 8st test case of curl_easy_getinfo()
 */
static void utc_curl_easy_getinfo_n8(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
    curl_easy_setopt(url, CURLOPT_VERBOSE, 1L);

	ret = curl_easy_perform(url);
    if (ret != CURLE_OK) {
		fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(ret));
		return;
    }

	int opt = CURLINFO_REDIRECT_URL;
	char *value = 0; /*invalid type*/

	ret = curl_easy_getinfo(url, opt, value);
	dts_check_eq("curl_easy_getinfo", ret, CURLE_BAD_FUNCTION_ARGUMENT,
		"Must return CURLE_BAD_FUNCTION_ARGUMENT in case of invalid redirect url");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

/**
 * @brief Positive test case of curl_easy_getinfo()
 */
static void utc_curl_easy_getinfo_p(void)
{
	int ret = -1;
	CURL *url = NULL;

	/* Init CURL before usage */
	curl_global_init(CURL_GLOBAL_ALL);

	//  allocation test of new CURL handle
	url = curl_easy_init();
	curl_easy_setopt(url, CURLOPT_URL, UTC_REMOTE_HTTP_URL);
    curl_easy_setopt(url, CURLOPT_VERBOSE, 1L);

	ret = curl_easy_perform(url);
    if (ret != CURLE_OK) {
		fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(ret));
		return;
    }

	long response;
	ret = curl_easy_getinfo(url, CURLINFO_RESPONSE_CODE, &response);
	dts_check_eq("curl_easy_getinfo", ret, CURLE_OK,
		"Must return CURLE_OK in case of response code");

	curl_easy_cleanup(url);
	curl_global_cleanup();
}

#if 0 // Not checked string parameter by curl - Ignore it
/**
 * @brief Negative test case of curl_easy_escape()
 */
static void utc_curl_easy_escape_n(void)
{
	CURL *url = NULL;
	char *urlencode = NULL;

	//  allocation test of new CURL handle
	url = curl_easy_init();
	urlencode = curl_easy_escape(url, NULL, 0);
	dts_check_eq("curl_easy_escape", urlencode, NULL,
		"Must return NULL in case of invalid string");

	curl_free(urlencode);
	curl_easy_cleanup(url);
}
#endif

/**
 * @brief Positive test case of curl_easy_escape()
 */
static void utc_curl_easy_escape_p(void)
{
	CURL *url = NULL;
	char *urlencode = NULL;
	int asize = 0;
	const char base[] = {0x9c, 0x26, 0x4b, 0x3d, 0x49, 0x4, 0xa1, 0x1,
						 0xe0, 0xd8, 0x7c,	0x20, 0xb7, 0xef, 0x53, 0x29,
						 0xfa, 0x1d, 0x57, 0xe1};

	//  allocation test of new CURL handle
	url = curl_easy_init();
	asize = (int)sizeof(base);
	urlencode = curl_easy_escape(url, base, asize);
	printf("convert: %s\n", urlencode);
	dts_check_ne("curl_easy_escape", urlencode, NULL,
		"Must not return NULL in case of valid string");

	curl_free(urlencode);
	curl_easy_cleanup(url);
}

/**
 * @brief Negative test case of curl_easy_strerror()
 */
static void utc_curl_easy_strerror_n(void)
{
	char *dec = NULL;
	int err = -1;
	int ret = -1;

	dec = curl_easy_strerror(err);
	ret = strncmp(dec, "Unknown error", 13);
	dts_check_eq("curl_easy_strerror", ret, CURLE_OK,
		"Must also return error in case of invalid errno");
}

/**
 * @brief Positive test case of curl_easy_strerror()
 */
static void utc_curl_easy_strerror_p(void)
{
	char *dec = NULL;
	int err = CURLE_FAILED_INIT;
	int ret = -1;

	dec = curl_easy_strerror(err);
	ret = strncmp(dec, "Failed initialization", 21);
	dts_check_eq("curl_easy_strerror", ret, CURLE_OK,
		"Must return Failed initialization in case of CURLE_FAILED_INIT");
}

/**
 * @brief Positive test case of curl_version()
 */
static void utc_curl_version_p(void)
{
	char *version = NULL;
	version = curl_version();
	dts_check_ne("curl_version", version, NULL,
		"Must not return NULL in case of version");
}

/**
 * @brief Positive test case of curl_version_info()
 * because the params is not valid, so there is no negative case
 */
static void utc_curl_version_info_p(void)
{
	curl_version_info_data *data = NULL;
	CURLversion version = CURLVERSION_FIRST; /*it is not used in API actually*/

	data = curl_version_info(version);
	dts_check_ne("curl_version_info", data, NULL,
		"Must not return NULL in case of valid version info");
}

/**
 * @brief Negative 1st test case of curl_formadd()
 */
static void utc_curl_formadd_n1(void)
{
	int ret = -1;
	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;

	ret = curl_formadd(&formpost, &lastptr,
				 CURLFORM_COPYNAME, NULL,
				 CURLFORM_COPYCONTENTS, NULL,
				 CURLFORM_END);

	dts_check_eq("curl_formadd", ret, CURL_FORMADD_NULL,
		"Must return CURL_FORMADD_NULL in case of invalid content");

	curl_formfree(formpost);
}

/**
 * @brief Negative 2st test case of curl_formadd()
 */
static void utc_curl_formadd_n2(void)
{
	int ret = -1;
	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;

	ret = curl_formadd(&formpost, &lastptr,
				 -1, "name",
				 -2, "content",
				 CURLFORM_END);

	dts_check_eq("curl_formadd", ret, CURL_FORMADD_UNKNOWN_OPTION,
		"Must return CURL_FORMADD_UNKNOWN_OPTION in case of invalid option");

	curl_formfree(formpost);
}

/**
 * @brief Negative 3st test case of curl_formadd()
 */
static void utc_curl_formadd_n3(void)
{
	int ret = -1;
	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;

	ret = curl_formadd(&formpost, &lastptr,
				 CURLFORM_COPYNAME, "name",
				 CURLFORM_END);

	dts_check_eq("curl_formadd", ret, CURL_FORMADD_INCOMPLETE,
		"Must return CURL_FORMADD_INCOMPLETE in case of invalid format");

	curl_formfree(formpost);
}

/**
 * @brief Negative 4st test case of curl_formadd()
 */
static void utc_curl_formadd_n4(void)
{
	int ret = -1;
	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
#if 0
	struct curl_forms forms[3];
	forms[0].option = CURLFORM_COPYNAME;
	forms[0].value = "name";
	forms[1].option = CURLFORM_COPYCONTENTS;
	forms[1].value = "content";
	forms[2].option = CURLFORM_END;
#endif

	ret = curl_formadd(&formpost, &lastptr,
				 CURLFORM_ARRAY, "st",
				 CURLFORM_END);

	dts_check_eq("curl_formadd", ret, CURL_FORMADD_UNKNOWN_OPTION,
		"Must return CURL_FORMADD_UNKNOWN_OPTION in case of invalid array");

	curl_formfree(formpost);
}

/**
 * @brief Positive test case of curl_formadd()
 */
static void utc_curl_formadd_p(void)
{
	int ret = -1;
	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
	char namebuffer[] = "name buffer";
	long namelength = strlen(namebuffer);
	char buffer[] = "test buffer";

	ret = curl_formadd(&formpost, &lastptr,
				 CURLFORM_COPYNAME, "name",
				 CURLFORM_COPYCONTENTS, "content",
				 CURLFORM_END);

	dts_check_eq("curl_formadd", ret, CURL_FORMADD_OK,
		"Must return CURL_FORMADD_OK in case of valid formatadd");

	ret = curl_formadd(&formpost, &lastptr,
				 CURLFORM_PTRNAME, namebuffer,
				 CURLFORM_PTRCONTENTS, buffer,
				 CURLFORM_NAMELENGTH, namelength,
				 CURLFORM_END);

	dts_check_eq("curl_formadd", ret, CURL_FORMADD_OK,
		"Must return CURL_FORMADD_OK in case of valid formatadd");

	curl_formfree(formpost);
}

/**
 * @brief Positive test case of curl_global_init()
 */
static void utc_curl_global_init_p(void)
{
	int ret = -1;
	int flags = CURL_GLOBAL_SSL;

	ret = curl_global_init(flags);
	dts_check_eq("curl_global_init", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid flag");

	curl_global_cleanup();
}

#if 0 // Not checked data parameter by curl - Ignore it
/**
 * @brief Negative test case of curl_slist_append()
 */
static void utc_curl_slist_append_n(void)
{
	int ret = -1;
	struct curl_slist *list = NULL;
	char *buf = NULL;

    list = curl_slist_append(list, buf);
	dts_check_eq("curl_slist_append", list, NULL,
		"Must return NULL in case of valid data");
}
#endif

/**
 * @brief Positive test case of curl_slist_append()
 */
static void utc_curl_slist_append_p(void)
{
	int ret = -1;
	struct curl_slist *list = NULL;
	const char *buf = "pragma:";

    list = curl_slist_append(list, buf);
	dts_check_ne("curl_slist_append", list, NULL,
		"Must not return NULL in case of valid data");

	curl_slist_free_all(list);
}

/**
 * @brief Positive test case of  curl_share_init()
 */
static void utc_curl_share_init_p(void)
{
	CURLSH *share = NULL;

    share = curl_share_init();
	dts_check_ne("curl_share_init", share, NULL,
		"Must not return NULL in case of new CURLSH handle allocation");

	curl_share_cleanup(share);
}

/**
 * @brief Negative test case of curl_share_cleanup()
 */
static void utc_curl_share_cleanup_n(void)
{
	int ret = -1;
	CURLSH *share = NULL;

	ret = curl_share_cleanup(share);
	dts_check_eq("curl_share_cleanup", ret, CURLSHE_INVALID,
		"Must return CURLSHE_INVALID in case of invalid share handle");
}

/**
 * @brief Positive test case of curl_share_cleanup()
 */
static void utc_curl_share_cleanup_p(void)
{
	int ret = -1;
	CURLSH *share = NULL;

    share = curl_share_init();
	ret = curl_share_cleanup(share);
	dts_check_eq("curl_share_cleanup", ret, CURLSHE_OK,
		"Must return CURLSHE_OK in case of valid share handle");
}

/**
 * @brief Negative 1st test case of curl_share_setopt()
 */
static void utc_curl_share_setopt_n1(void)
{
	int ret = -1;
	CURLSH *share = NULL;
	CURLoption option = -1;

    share = curl_share_init();
	ret = curl_share_setopt(share, option, 1L);
	dts_check_eq("curl_share_setopt", ret, CURLSHE_BAD_OPTION,
		"Must return CURLSHE_BAD_OPTION in case of invalid option");

	curl_share_cleanup(share);
}

#if 0 // Not checked share handle by curl - Ignore it
/**
 * @brief Negative 2st test case of curl_share_setopt()
 */
static void utc_curl_share_setopt_n2(void)
{
	int ret = -1;
	CURLSH *share = NULL;

	ret = curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
	dts_check_eq("curl_share_setopt", ret, CURLSHE_INVALID,
		"Must return CURLSHE_INVALID in case of invalid handle");

	curl_share_cleanup(share);
}
#endif

/**
 * @brief Positive test case of curl_share_setopt()
 */
static void utc_curl_share_setopt_p(void)
{
	int ret = -1;
	CURLSH *share = NULL;

    share = curl_share_init();
	ret = curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
	dts_check_eq("curl_share_setopt", ret, CURLE_OK,
		"Must return CURLE_OK in case of valid option");

	curl_share_cleanup(share);
}

/**
 * @brief Negative test case of curl_multi_init()
 */
static void utc_curl_multi_init_p(void)
{
        CURLM *multi = NULL;

        curl_global_init(CURL_GLOBAL_ALL);

        //  allocation test of new CURLM handle
        multi = curl_multi_init();
        dts_check_ne("curl_multi_init", multi, NULL, "Must not return NULL in case of new CURLM handle allocation"); 

        curl_multi_cleanup(multi);
        curl_global_cleanup();
}

/**
 * @brief Negative test case 2 of curl_multi_add_handle()
 */
static void utc_curl_multi_add_handle_n1(void)
{
        CURLM *multi = NULL;
        CURL *easy = NULL;
        int ret = 0;

        curl_global_init(CURL_GLOBAL_ALL);

        easy = curl_easy_init();

        //  Invalid parameter test
        //  CURLMcode curl_multi_add_handle(CURLM *multi_handle, CURL *easy_handle);
        ret = curl_multi_add_handle(multi, easy);
        dts_check_eq("curl_multi_add_handle", ret, CURLM_BAD_HANDLE, "Must return CURLM_BAD_HANDLE in case of invalid CURLM parameter"); 

        curl_easy_cleanup(easy);
        curl_global_cleanup();
}

/**
 * @brief Negative test case 3 of curl_multi_add_handle()
 */
static void utc_curl_multi_add_handle_n2(void)
{
        CURLM *multi = NULL;
        CURL *easy = NULL;
        int ret = 0;

        curl_global_init(CURL_GLOBAL_ALL);

        multi = curl_multi_init();

        //  Invalid parameter test
        //  CURLMcode curl_multi_add_handle(CURLM *multi_handle, CURL *easy_handle);
        ret = curl_multi_add_handle(multi, easy);
        dts_check_eq("curl_multi_add_handle", ret, CURLM_BAD_EASY_HANDLE, "Must return CURLM_BAD_EASY_HANDLE in case of invalid CURL parameter"); 

        curl_multi_cleanup(multi);
        curl_global_cleanup();
}

/**
 * @brief Positive test case of curl_multi_add_handle()
 */
static void utc_curl_multi_add_handle_p(void)
{
  CURL *easy = NULL;
  CURLM *multi = NULL;
  int ret = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  easy = curl_easy_init();
  multi = curl_multi_init();

  //  Normal operation test
  //  CURLMcode curl_multi_add_handle(CURLM *multi_handle, CURL *easy_handle);
  ret = curl_multi_add_handle(multi, easy);
  dts_check_eq("curl_multi_add_handle", ret, CURLM_OK, "Succeeded to add multi handle");

  curl_multi_cleanup(multi);
  curl_easy_cleanup(easy);
  curl_global_cleanup();
}

#if 0 // Not checked by curl - So ignore it.
/**
 * @brief Negative test case of curl_multi_socket_action()
 */
static void utc_curl_multi_socket_action_n(void)
{
  CURLM *multi = NULL;
  curl_socket_t sockfd = CURL_SOCKET_BAD;
  int action;
  int hdl_num;
  int ret = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  sockfd = socket(PF_INET, SOCK_DGRAM, 0);
  action = CURL_POLL_IN;
  hdl_num = 0;

  // Invalid parameter test
  // CURLMcode curl_multi_socket_action(CURLM * multi_handle,   curl_socket_t sockfd, int ev_bitmask,   int *running_handles);
  // Not checked by curl - multi, sockfd, ev_bitmask, running_handles
  // Got segfault if multi parameter is NULL
  ret = curl_multi_socket_action(multi, sockfd, action, &hdl_num);
  dts_check_eq("curl_multi_socket_action", ret, CURLM_BAD_HANDLE, "Must return CURLM_BAD_HANDLE in case of invalid parameter");

  close(sockfd);
  curl_global_cleanup();
}
#endif

/**
 * @brief Positive test case of curl_multi_socket_action()
 */
static void utc_curl_multi_socket_action_p(void)
{
  CURLM *multi = NULL;
  curl_socket_t sockfd = CURL_SOCKET_BAD;
  int action;
  int hdl_num;
  int ret = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  multi = curl_multi_init();
  sockfd = socket(PF_INET, SOCK_DGRAM, 0);
  action = CURL_POLL_IN;
  hdl_num = 0;

  // Invalid parameter test
  // CURLMcode curl_multi_socket_action(CURLM * multi_handle,   curl_socket_t sockfd, int ev_bitmask,   int *running_handles);
  // Not checked by curl - sockfd, ev_bitmask, running_handles
  ret = curl_multi_socket_action(multi, sockfd, action, &hdl_num);
  dts_check_eq("curl_multi_socket_action", ret, CURLM_OK, "Must return CURLM_OK in case of valid parameter");

  close(sockfd);
  curl_multi_cleanup(multi);
  curl_global_cleanup();
}

#if 0
/**
 * @brief Negative test case of utc_curl_multi_assign_n1()
 */
static void utc_curl_multi_assign_n1(void)
{
  CURLM *multi = NULL;
  curl_socket_t sockfd = CURL_SOCKET_BAD;
  void *sockptr = NULL;
  int action;
  int hdl_num;
  int ret = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  sockfd = socket(PF_INET, SOCK_DGRAM, 0);
  action = CURL_POLL_IN;
  hdl_num = 0;
  curl_multi_socket_action(multi, sockfd, action, &hdl_num);

  sockptr = calloc(sizeof(SockInfo), 1); 

  //  Invalid parameter test
  //  CURLMcode curl_multi_assign(CURLM *multi_handle, curl_socket_t sockfd,   void *sockptr);
  //  Not checked by curl - multi_handle, sockptr
  //  Got segfault if multi parameter is NULL
  ret = curl_multi_assign(multi, sockfd, sockptr);
  dts_check_eq("curl_multi_assign", ret, CURLM_BAD_HANDLE, "Must return CURLM_BAD_HANDLE in case of invalid multi_handle parameter"); 

  free(sockptr);
  close(sockfd);
  curl_global_cleanup();
}
#endif

/**
 * @brief Negative test case of utc_curl_multi_assign_n2()
 */
static void utc_curl_multi_assign_n(void)
{
  CURLM *multi = NULL;
  curl_socket_t sockfd = CURL_SOCKET_BAD;
  void *sockptr = NULL;
  int ret = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  multi = curl_multi_init();
  sockptr = calloc(sizeof(SockInfo), 1); 

  //  Invalid parameter test
  //  CURLMcode curl_multi_assign(CURLM *multi_handle, curl_socket_t sockfd,   void *sockptr);
  //  Not checked by curl - multi_handle, sockptr
  //  Got segfault if multi parameter is NULL
  ret = curl_multi_assign(multi, sockfd, sockptr);
  dts_check_eq("curl_multi_assign", ret, CURLM_BAD_SOCKET, "Must return CURLM_BAD_SOCKET in case of invalid sockfd parameter"); 

  free(sockptr);
  curl_multi_cleanup(multi);
  curl_global_cleanup();
}

#if 0
/**
 * @brief Positive test case of curl_multi_assign()
 */
static void utc_curl_multi_assign_p(void)
{
  CURLM *multi = NULL;
  curl_socket_t sockfd = CURL_SOCKET_BAD;
  void *sockptr = NULL;
  int action;
  int hdl_num;
  int ret = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  multi = curl_multi_init();
  sockfd = socket(PF_INET, SOCK_DGRAM, 0);
  action = CURL_POLL_IN;
  hdl_num = 0;
  curl_multi_socket_action(multi, sockfd, action, &hdl_num);
  sockptr = calloc(sizeof(SockInfo), 1); 

  // XXX FIXME XXX - it's required real multi based routines.
  //  CURLMcode curl_multi_assign(CURLM *multi_handle, curl_socket_t sockfd,   void *sockptr);
  ret = curl_multi_assign(multi, sockfd, sockptr);
  dts_check_eq("curl_multi_assign", ret, CURLM_OK, "Must return CURLM_OK in case of valid parameter"); 

  free(sockptr);
  close(sockfd);
  curl_multi_cleanup(multi);
  curl_global_cleanup();
}
#endif

/**
 * @brief Negative test case of curl_multi_cleanup()
 */
static void utc_curl_multi_cleanup_n(void)
{
  CURLM *multi = NULL;
  int ret = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  //  Invalid parameter test 
  //  CURLMcode curl_multi_cleanup( CURLM *multi_handle );
  ret = curl_multi_cleanup(multi);
  dts_check_eq("curl_multi_cleanup", ret, CURLM_BAD_HANDLE, "Must return CURLM_BAD_HANDLE in case of invalid multi_handle parameter"); 

  curl_global_cleanup();
}

/**
 * @brief Positive test case of curl_multi_cleanup()
 */
static void utc_curl_multi_cleanup_p(void)
{
  CURLM *multi = NULL;
  int ret = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  multi = curl_multi_init();

  //  Invalid parameter test 
  //  CURLMcode curl_multi_cleanup( CURLM *multi_handle );
  ret = curl_multi_cleanup(multi);
  dts_check_eq("curl_multi_cleanup", ret, CURLM_OK, "Must return CURLM_OK in case of valid multi_handle parameter"); 

  curl_global_cleanup();
}

/**
 * @brief Negative test case of curl_multi_fdset()
 */
static void utc_curl_multi_fdset_n(void)
{
  CURLM *multi = NULL;
  int ret = 0;
  fd_set rd, wr, exc;
  int max_fd;

  curl_global_init(CURL_GLOBAL_ALL);

  FD_ZERO(&rd);
  FD_ZERO(&wr);
  FD_ZERO(&exc);
  max_fd = 0;

  //  Invalid parameter test
  //  CURLMcode curl_multi_fdset(CURLM *multi_handle,   fd_set *read_fd_set,   fd_set *write_fd_set,   fd_set *exc_fd_set,   int *max_fd);
  //  not checked by curl - read_fd_set, write_fd_set, exc_fd_set, max_fd 
  ret = curl_multi_fdset(multi, &rd, &wr, &exc, &max_fd);
  dts_check_eq("curl_multi_fdset", ret, CURLM_BAD_HANDLE, "Must return CURLM_BAD_HANDLE in case of invalid multi_handle parameter");

  curl_global_cleanup();
}

/**
 * @brief Positive test case of curl_multi_fdset()
 */
static void utc_curl_multi_fdset_p(void)
{
  CURLM *multi = NULL;
  int ret = 0;
  fd_set rd, wr, exc;
  int max_fd;

  curl_global_init(CURL_GLOBAL_ALL);

  multi = curl_multi_init();

  FD_ZERO(&rd);
  FD_ZERO(&wr);
  FD_ZERO(&exc);
  max_fd = 0;

  //  Invalid parameter test
  //  CURLMcode curl_multi_fdset(CURLM *multi_handle,   fd_set *read_fd_set,   fd_set *write_fd_set,   fd_set *exc_fd_set,   int *max_fd);
  //  not checked by curl - read_fd_set, write_fd_set, exc_fd_set, max_fd 
  ret = curl_multi_fdset(multi, &rd, &wr, &exc, &max_fd);
  dts_check_eq("curl_multi_fdset", ret, CURLM_OK, "Must return CURLM_OK in case of valid parameter");

  curl_multi_cleanup(multi);
  curl_global_cleanup();
}

/**
 * @brief Negative test case of curl_multi_info_read()
 */
static void utc_curl_multi_info_read_n(void)
{
  CURLM *multi = NULL;
  int ret = 0;
  int msg_left = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  //  Invalid parameter test    
  //  CURLMsg *curl_multi_info_read( CURLM *multi_handle,   int *msgs_in_queue);
  //  Not checked by curl - msgs_in_queue
  ret = curl_multi_info_read(multi, &msg_left);
  dts_check_eq("curl_multi_info_read", ret, NULL, "Must return NULL in case of invalid parameter"); 
  
  curl_global_cleanup();
}

/**
 * @brief Positive test case of curl_multi_info_read()
 */
static void utc_curl_multi_info_read_p(void)
{
  CURLM *multi = NULL;
  CURL *easy = NULL;
  int ret = 0;
  int running;
  int msg_left = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  multi = curl_multi_init();
  easy = curl_easy_init();
  curl_multi_add_handle(multi, easy);
  curl_multi_perform(multi, &running);

  //  Invalid parameter test    
  //  CURLMsg *curl_multi_info_read( CURLM *multi_handle,   int *msgs_in_queue);
  //  Not checked by curl - msgs_in_queue
  ret = curl_multi_info_read(multi, &msg_left);
  dts_check_ne("curl_multi_info_read", ret, NULL, "Must not return NULL in case of valid parameter"); 
  
  curl_multi_cleanup(multi);
  curl_easy_cleanup(easy);
  curl_global_cleanup();
}

/**
 * @brief Negative test case of curl_multi_perform()
 */
static void utc_curl_multi_perform_n(void)
{
  CURLM *multi = NULL;
  int ret = 0;
  int hdl_num;

  curl_global_init(CURL_GLOBAL_ALL);

  //  Invalid parameter test
  //  CURLMcode curl_multi_perform(CURLM *multi_handle, int *running_handles);
  //  Not checked by curl - running_handles
  ret = curl_multi_perform(multi, &hdl_num);
  dts_check_eq("curl_multi_perform", ret, CURLM_BAD_HANDLE, "Must return CURLM_BAD_HANDLE in case of invalid parameter");

  curl_global_cleanup();
}

/**
 * @brief Positive test case of curl_multi_perform()
 */
static void utc_curl_multi_perform_p1(void)
{
  CURLM *multi = NULL;
  int ret = 0;
  int hdl_num;

  curl_global_init(CURL_GLOBAL_ALL);

  multi = curl_multi_init();

  //  Invalid parameter test
  //  CURLMcode curl_multi_perform(CURLM *multi_handle, int *running_handles);
  //  Not checked by curl - running_handles
  ret = curl_multi_perform(multi, &hdl_num);
  dts_check_eq("curl_multi_perform", ret, CURLM_OK, "Must return CURLM_OK in case of valid parameter");

  curl_multi_cleanup(multi);
  curl_global_cleanup();
}

/**
 * @brief Positive test case of curl_multi_perform()
 */
static void utc_curl_multi_perform_p2(void)
{
	CURL *easy;
	CURLM *multi = NULL;
	int ret = 0;
	int hdl_num;

	curl_global_init(CURL_GLOBAL_ALL);

	easy = curl_easy_init();
	curl_easy_setopt(easy, CURLOPT_URL, UTC_REMOTE_HTTP_URL);

	multi = curl_multi_init();
	curl_multi_setopt(multi, CURLMOPT_SOCKETFUNCTION, __curlmopt_socket_cb);

	curl_multi_add_handle(multi, easy);

	//  Invalid parameter test
	//  CURLMcode curl_multi_perform(CURLM *multi_handle, int *running_handles);
	//  Not checked by curl - running_handles
	do {
		ret = curl_multi_perform(multi, &hdl_num);
	} while(ret == CURLM_CALL_MULTI_PERFORM);

	dts_check_eq("curl_multi_perform", ret, CURLM_OK, "Must return CURLM_OK in case of valid parameter");

	curl_multi_cleanup(multi);
	curl_easy_cleanup(easy);
	curl_global_cleanup();
}

/**
 * @brief Positive test case of curl_multi_perform()
 */
static void utc_curl_multi_perform_p3(void)
{
	CURL *easy;
	CURLM *multi = NULL;
	int ret = 0;
	int hdl_num;
	void *sockptr = NULL;

	curl_global_init(CURL_GLOBAL_ALL);

	easy = curl_easy_init();
	curl_easy_setopt(easy, CURLOPT_URL, UTC_REMOTE_HTTP_URL);

	multi = curl_multi_init();
	sockptr = calloc(sizeof(SockInfo), 1);
	curl_multi_setopt(multi, CURLMOPT_SOCKETDATA, sockptr);

	curl_multi_add_handle(multi, easy);

	//  Invalid parameter test
	//  CURLMcode curl_multi_perform(CURLM *multi_handle, int *running_handles);
	//  Not checked by curl - running_handles
	do {
		ret = curl_multi_perform(multi, &hdl_num);
	} while(ret == CURLM_CALL_MULTI_PERFORM);

	dts_check_eq("curl_multi_perform", ret, CURLM_OK, "Must return CURLM_OK in case of valid parameter");

	free(sockptr);
	curl_multi_cleanup(multi);
	curl_easy_cleanup(easy);
	curl_global_cleanup();
}

/**
 * @brief Positive test case of curl_multi_perform()
 */
static void utc_curl_multi_perform_p4(void)
{
	CURL *easy;
	CURLM *multi = NULL;
	int ret = 0;
	int hdl_num;

	curl_global_init(CURL_GLOBAL_ALL);

	easy = curl_easy_init();
	curl_easy_setopt(easy, CURLOPT_URL, UTC_REMOTE_HTTP_URL);

	multi = curl_multi_init();
	curl_multi_setopt(multi, CURLMOPT_PIPELINING, 1L);

	curl_multi_add_handle(multi, easy);

	//  Invalid parameter test
	//  CURLMcode curl_multi_perform(CURLM *multi_handle, int *running_handles);
	//  Not checked by curl - running_handles
	do {
		ret = curl_multi_perform(multi, &hdl_num);
	} while(ret == CURLM_CALL_MULTI_PERFORM);

	dts_check_eq("curl_multi_perform", ret, CURLM_OK, "Must return CURLM_OK in case of valid parameter");

	curl_multi_cleanup(multi);
	curl_easy_cleanup(easy);
	curl_global_cleanup();
}

/**
 * @brief Positive test case of curl_multi_perform()
 */
static void utc_curl_multi_perform_p5(void)
{
	CURL *easy;
	CURLM *multi = NULL;
	int ret = 0;
	int hdl_num;

	curl_global_init(CURL_GLOBAL_ALL);

	easy = curl_easy_init();
	curl_easy_setopt(easy, CURLOPT_URL, UTC_REMOTE_HTTP_URL);

	multi = curl_multi_init();
	curl_multi_setopt(multi, CURLMOPT_TIMERFUNCTION, __curlmopt_multi_timer_cb);

	curl_multi_add_handle(multi, easy);

	//  Invalid parameter test
	//  CURLMcode curl_multi_perform(CURLM *multi_handle, int *running_handles);
	//  Not checked by curl - running_handles
	do {
		ret = curl_multi_perform(multi, &hdl_num);
	} while(ret == CURLM_CALL_MULTI_PERFORM);

	dts_check_eq("curl_multi_perform", ret, CURLM_OK, "Must return CURLM_OK in case of valid parameter");

	curl_multi_cleanup(multi);
	curl_easy_cleanup(easy);
	curl_global_cleanup();
}

/**
 * @brief Positive test case of curl_multi_perform()
 */
static void utc_curl_multi_perform_p6(void)
{
	CURL *easy;
	CURLM *multi = NULL;
	int ret = 0;
	int hdl_num;
	void *sockptr = NULL;

	curl_global_init(CURL_GLOBAL_ALL);

	easy = curl_easy_init();
	curl_easy_setopt(easy, CURLOPT_URL, UTC_REMOTE_HTTP_URL);

	multi = curl_multi_init();
	sockptr = calloc(sizeof(SockInfo), 1);
	curl_multi_setopt(multi, CURLMOPT_TIMERDATA, sockptr);

	curl_multi_add_handle(multi, easy);

	//  Invalid parameter test
	//  CURLMcode curl_multi_perform(CURLM *multi_handle, int *running_handles);
	//  Not checked by curl - running_handles
	do {
		ret = curl_multi_perform(multi, &hdl_num);
	} while(ret == CURLM_CALL_MULTI_PERFORM);

	dts_check_eq("curl_multi_perform", ret, CURLM_OK, "Must return CURLM_OK in case of valid parameter");

	free(sockptr);
	curl_multi_cleanup(multi);
	curl_easy_cleanup(easy);
	curl_global_cleanup();
}

/**
 * @brief Positive test case of curl_multi_perform()
 */
static void utc_curl_multi_perform_p7(void)
{
	CURL *easy;
	CURLM *multi = NULL;
	int ret = 0;
	int hdl_num;

	curl_global_init(CURL_GLOBAL_ALL);

	easy = curl_easy_init();
	curl_easy_setopt(easy, CURLOPT_URL, UTC_REMOTE_HTTP_URL);

	multi = curl_multi_init();
	curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, 100L);

	curl_multi_add_handle(multi, easy);

	//  Invalid parameter test
	//  CURLMcode curl_multi_perform(CURLM *multi_handle, int *running_handles);
	//  Not checked by curl - running_handles
	do {
		ret = curl_multi_perform(multi, &hdl_num);
	} while(ret == CURLM_CALL_MULTI_PERFORM);

	dts_check_eq("curl_multi_perform", ret, CURLM_OK, "Must return CURLM_OK in case of valid parameter");

	curl_multi_cleanup(multi);
	curl_easy_cleanup(easy);
	curl_global_cleanup();
}

/**
 * @brief Negative test case of curl_multi_remove_handle()
 */
static void utc_curl_multi_remove_handle_n1(void)
{
  CURLM *multi = NULL;
  CURL *easy = NULL;
  int ret = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  multi = curl_multi_init();
  easy = curl_easy_init();
  curl_multi_add_handle(multi, easy);

  //  Invalid parameter test    
  //  CURLMcode curl_multi_remove_handle(CURLM *multi_handle, CURL *easy_handle);
  ret = curl_multi_remove_handle(NULL, easy);
  dts_check_eq("curl_multi_remove_handle", ret, CURLM_BAD_HANDLE, "Must return CURLM_BAD_HANDLE in case of invalid parameter"); 

  curl_easy_cleanup(easy);
  curl_global_cleanup();
}

/**
 * @brief Negative test case of curl_multi_remove_handle()
 */
static void utc_curl_multi_remove_handle_n2(void)
{
  CURLM *multi = NULL;
  CURL *easy = NULL;
  int ret = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  multi = curl_multi_init();
  easy = curl_easy_init();
  curl_multi_add_handle(multi, easy);

  //  Invalid parameter test    
  //  CURLMcode curl_multi_remove_handle(CURLM *multi_handle, CURL *easy_handle);
  ret = curl_multi_remove_handle(multi, NULL);
  dts_check_eq("curl_multi_remove_handle", ret, CURLM_BAD_EASY_HANDLE, "Must return CURLM_BAD_EASY_HANDLE in case of invalid parameter"); 

  curl_multi_cleanup(multi);
  curl_global_cleanup();
}

/**
 * @brief Positive test case of curl_multi_remove_handle()
 */
static void utc_curl_multi_remove_handle_p(void)
{
  CURLM *multi = NULL;
  CURL *easy = NULL;
  int ret = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  multi = curl_multi_init();
  easy = curl_easy_init();
  curl_multi_add_handle(multi, easy);

  //  Invalid parameter test    
  //  CURLMcode curl_multi_remove_handle(CURLM *multi_handle, CURL *easy_handle);
  ret = curl_multi_remove_handle(multi, easy);
  dts_check_eq("curl_multi_remove_handle", ret, CURLM_OK, "Must return CURLM_OK in case of valid parameter"); 

  curl_multi_cleanup(multi);
  curl_easy_cleanup(easy);
  curl_global_cleanup();
}

/**
 * @brief Negative test case of curl_multi_setopt()
 */
static void utc_curl_multi_setopt_n1(void)
{
  CURLM *multi = NULL;
  CURLMoption option = -1;
  int ret = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  option = CURLMOPT_PIPELINING;

  // Invalid parameter test
  // CURLMcode curl_multi_setopt(CURLM * multi_handle, CURLMoption option, param);
  // Not checked by curl - param
  ret = curl_multi_setopt(multi, option, 1L);
  dts_check_eq("curl_multi_setopt", ret, CURLM_BAD_HANDLE, "Must return CURLM_BAD_HANDLE in case of invalid parameter"); 

  curl_global_cleanup();
}

/**
 * @brief Negative test case of curl_multi_setopt()
 */
static void utc_curl_multi_setopt_n2(void)
{
  CURLM *multi = NULL;
  CURLMoption option = -1;
  int ret = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  multi = curl_multi_init();

  // Invalid parameter test
  // CURLMcode curl_multi_setopt(CURLM * multi_handle, CURLMoption option, param);
  // Not checked by curl - param
  ret = curl_multi_setopt(multi, option, 1L);
  dts_check_eq("curl_multi_setopt", ret, CURLM_UNKNOWN_OPTION, "Must return CURLM_UNKNOWN_OPTION in case of invalid parameter"); 

  curl_multi_cleanup(multi);
  curl_global_cleanup();
}

/**
 * @brief Positive test case of curl_multi_setopt()
 */
static void utc_curl_multi_setopt_p1(void)
{
  CURLM *multi = NULL;
  CURLMoption option = -1;
  int ret = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  multi = curl_multi_init();
  option = CURLMOPT_SOCKETFUNCTION;

  // Invalid parameter test
  // CURLMcode curl_multi_setopt(CURLM * multi_handle, CURLMoption option, param);
  // Not checked by curl - param
  // FIXME - need to add socket
  ret = curl_multi_setopt(multi, option, __curlmopt_socket_cb);
  dts_check_eq("curl_multi_setopt", ret, CURLM_OK, "Must return CURLM_OK in case of valid parameter"); 

  curl_multi_cleanup(multi);
  curl_global_cleanup();
}

/**
 * @brief Positive test case of curl_multi_setopt()
 */
static void utc_curl_multi_setopt_p2(void)
{
  CURLM *multi = NULL;
  CURLMoption option = -1;
  int ret = 0;
  void *sockptr = NULL;

  curl_global_init(CURL_GLOBAL_ALL);

  multi = curl_multi_init();
  option = CURLMOPT_SOCKETDATA;
  sockptr = calloc(sizeof(SockInfo), 1); 

  // Invalid parameter test
  // CURLMcode curl_multi_setopt(CURLM * multi_handle, CURLMoption option, param);
  // Not checked by curl - param
  // FIXME - need to add socket
  ret = curl_multi_setopt(multi, option, sockptr);
  dts_check_eq("curl_multi_setopt", ret, CURLM_OK, "Must return CURLM_OK in case of valid parameter"); 

  free(sockptr);
  curl_multi_cleanup(multi);
  curl_global_cleanup();
}

/**
 * @brief Positive test case of curl_multi_setopt()
 */
static void utc_curl_multi_setopt_p3(void)
{
  CURLM *multi = NULL;
  CURLMoption option = -1;
  int ret = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  multi = curl_multi_init();
  option = CURLMOPT_PIPELINING;

  // Invalid parameter test
  // CURLMcode curl_multi_setopt(CURLM * multi_handle, CURLMoption option, param);
  // Not checked by curl - param
  // FIXME - need to add socket
  ret = curl_multi_setopt(multi, option, 1L);
  dts_check_eq("curl_multi_setopt", ret, CURLM_OK, "Must return CURLM_OK in case of valid parameter"); 

  curl_multi_cleanup(multi);
  curl_global_cleanup();
}

/**
 * @brief Positive test case of curl_multi_setopt()
 */
static void utc_curl_multi_setopt_p4(void)
{
  CURLM *multi = NULL;
  CURLMoption option = -1;
  int ret = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  multi = curl_multi_init();
  option = CURLMOPT_TIMERFUNCTION;

  // Invalid parameter test
  // CURLMcode curl_multi_setopt(CURLM * multi_handle, CURLMoption option, param);
  // Not checked by curl - param
  // FIXME - need to add socket
  ret = curl_multi_setopt(multi, option, __curlmopt_multi_timer_cb);
  dts_check_eq("curl_multi_setopt", ret, CURLM_OK, "Must return CURLM_OK in case of valid parameter"); 

  curl_multi_cleanup(multi);
  curl_global_cleanup();
}

/**
 * @brief Positive test case of curl_multi_setopt()
 */
static void utc_curl_multi_setopt_p5(void)
{
  CURLM *multi = NULL;
  CURLMoption option = -1;
  int ret = 0;
  void *sockptr = NULL;

  curl_global_init(CURL_GLOBAL_ALL);

  multi = curl_multi_init();
  option = CURLMOPT_TIMERDATA;
  sockptr = calloc(sizeof(SockInfo), 1); 

  // Invalid parameter test
  // CURLMcode curl_multi_setopt(CURLM * multi_handle, CURLMoption option, param);
  // Not checked by curl - param
  // FIXME - need to add socket
  ret = curl_multi_setopt(multi, option, sockptr);
  dts_check_eq("curl_multi_setopt", ret, CURLM_OK, "Must return CURLM_OK in case of valid parameter"); 

  free(sockptr);
  curl_multi_cleanup(multi);
  curl_global_cleanup();
}

/**
 * @brief Positive test case of curl_multi_setopt()
 */
static void utc_curl_multi_setopt_p6(void)
{
  CURLM *multi = NULL;
  CURLMoption option = -1;
  int ret = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  multi = curl_multi_init();
  option = CURLMOPT_MAXCONNECTS;

  // Invalid parameter test
  // CURLMcode curl_multi_setopt(CURLM * multi_handle, CURLMoption option, param);
  // Not checked by curl - param
  // FIXME - need to add socket
  ret = curl_multi_setopt(multi, option, 100L);
  dts_check_eq("curl_multi_setopt", ret, CURLM_OK, "Must return CURLM_OK in case of valid parameter"); 

  curl_multi_cleanup(multi);
  curl_global_cleanup();
}

/**
 * @brief Negative test case of curl_multi_strerror()
 */
static void utc_curl_multi_strerror_n(void)
{
  int err = -2;
  int ret = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  // Invalid parameter test
  // const char *curl_multi_strerror(CURLMcode errornum );
  ret = curl_multi_strerror(err);
  dts_check_str_eq("curl_multi_strerror", ret, "Unknown error", "Must return \"Unknown error\" in case of invalid parameter");

  curl_global_cleanup();
}

/**
 * @brief Negative test case of curl_multi_strerror()
 */
static void utc_curl_multi_strerror_p(void)
{
  int err = -2;
  int ret = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  err = CURLM_OK;

  // Invalid parameter test
  // const char *curl_multi_strerror(CURLMcode errornum );
  ret = curl_multi_strerror(err);
  dts_check_str_eq("curl_multi_strerror", ret, "No error", "Must return \"No error\" in case of valid parameter");

  curl_global_cleanup();
}

/**
 * @brief Negative test case of curl_multi_timeout()
 */
static void utc_curl_multi_timeout_n(void)
{
  CURLM *multi = NULL;
  long timeout;
  int ret = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  // Invalid parameter test
  // CURLMcode curl_multi_timeout(CURLM *multi_handle, long *timeout);
  // Not checked by curl - timeout
  ret = curl_multi_timeout(multi, &timeout);
  dts_check_eq("curl_multi_timeout", ret, CURLM_BAD_HANDLE, "Must return CURLM_BAD_HANDLE in case of invalid parameter");

  curl_global_cleanup();
}

/**
 * @brief Negative test case of curl_multi_timeout()
 */
static void utc_curl_multi_timeout_p(void)
{
  CURLM *multi = NULL;
  long timeout;
  int ret = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  multi = curl_multi_init();

  // Invalid parameter test
  // CURLMcode curl_multi_timeout(CURLM *multi_handle, long *timeout);
  // Not checked by curl - timeout
  ret = curl_multi_timeout(multi, &timeout);
  dts_check_eq("curl_multi_timeout", ret, CURLM_OK, "Must return CURLM_OK in case of invalid parameter");

  curl_multi_cleanup(multi);
  curl_global_cleanup();
}

