#include <curl/curl.h>

#include <iostream>

int main() {
  CURLcode init_code = curl_global_init(CURL_GLOBAL_DEFAULT);
  if (init_code != CURLE_OK) {
    std::cerr << "curl_global_init failed: "
              << curl_easy_strerror(init_code) << "\n";
    return 1;
  }

  CURL* handle = curl_easy_init();
  if (!handle) {
    std::cerr << "curl_easy_init failed\n";
    curl_global_cleanup();
    return 1;
  }

  curl_slist* headers = nullptr;
  headers = curl_slist_append(headers, "User-Agent: cur/1.0");

  curl_easy_setopt(handle, CURLOPT_URL, "https://example.com");
  curl_easy_setopt(handle, CURLOPT_NOBODY, 1L);
  curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);

  CURLcode result = curl_easy_perform(handle);
  if (result != CURLE_OK) {
    std::cerr << "curl_easy_perform failed: " << curl_easy_strerror(result)
              << "\n";
  }

  curl_slist_free_all(headers);
  curl_easy_cleanup(handle);
  curl_global_cleanup();

  return result == CURLE_OK ? 0 : 1;
}
