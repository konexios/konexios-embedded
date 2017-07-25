/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <stdio.h>
#include <curl/curl.h>
#include <ssl/md5sum.h>
#include <debug.h>
#include <arrow/mem.h>
#include <arrow/utf8.h>

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

#define pagefilename "update.file"

int arrow_gateway_software_update(const char *url) {
  CURL *curl;
  FILE *pagefile;

  curl_global_init(CURL_GLOBAL_ALL);
  /* init the curl session */
  curl = curl_easy_init();
  /* set URL to get here */
  curl_easy_setopt(curl, CURLOPT_URL, url);
  /* Switch on full protocol/debug output while testing */
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
  /* disable progress meter, set to 0L to enable and disable debug output */
  curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
  /* send all data to this function  */
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  /* open the file */
  pagefile = fopen(pagefilename, "wb");
  if(pagefile) {
    /* write the page body to this file handle */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, pagefile);
    /* get it! */
    curl_easy_perform(curl);
    /* close the header file */
    fclose(pagefile);
  }
  /* cleanup curl stuff */
  curl_easy_cleanup(curl);
  return 0;
}

int arrow_software_update(const char *url,
                          const char *checksum,
                          const char *from,
                          const char *to) {

  if ( arrow_gateway_software_update(url) < 0 ) return -1;
  FILE *fp = fopen(pagefilename, "rb");
  if ( !fp ) return -1;
  int n = 0;
  char buffer[1];
  md5_chunk_init();
  while ( (n = fread(buffer, 1, 1, fp)) ){
    md5_chunk(buffer, 1);
  }
  char md5hash[40];
  char md5hash_str[40];
  md5_chunk_hash(md5hash);
  hex_encode(md5hash_str, md5hash, 16);
  DBG("md5 sum %s", md5hash_str);
  if ( strcmp(md5hash_str, checksum) != 0 ) return -1;
  DBG("%s -> %s", from, to);
  return 0;
}

static FILE *test = NULL;
static int file_size = 0;
// this function will be executed when http client get a chunk of payload
int arrow_release_download_payload(property_t *buf, const char *payload, int size) {
  SSP_PARAMETER_NOT_USED(buf);
  if ( !test ) {
    test = fopen(pagefilename,"wb");
    if (!test) {
      DBG("Unable to open file!");
      return -1;
    }
  }
  file_size += size;
  DBG("filesze = %d", file_size);
  fwrite(payload, 1, size, test);
  fflush(test);
  return 0;
}

// this function will be executed when firmware file download complete
int arrow_release_download_complete(property_t *buf) {
  SSP_PARAMETER_NOT_USED(buf);
  DBG("file size = %d", file_size);
  file_size = 0;
  fclose(test);
  return 0;
}
