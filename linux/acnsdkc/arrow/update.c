/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <stdio.h>
#include <curl/curl.h>
//#include <curl/easy.h>

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

int arrow_gateway_software_update(const char *url) {
  CURL *curl;
  static const char *pagefilename = "update.file";
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
