#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>


int main(void)
{
        curl_global_init(CURL_GLOBAL_ALL);
        CURLcode success = CURLE_FAILED_INIT;
        CURL *handle = curl_easy_init();
        if (handle) {
                curl_easy_setopt(handle, CURLOPT_URL, "http://httpbin.org/get");
                success = curl_easy_perform(handle);
        }
        curl_global_cleanup();
        return success;
}
