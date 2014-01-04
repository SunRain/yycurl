#include "FetchData.h"

extern std::vector<DownloadNode> shared_node;
extern struct timeval time_start;

FetchData::FetchData(std::vector<std::string> url, std::vector<std::string> path){
	shared_node.clear();
	/*get start time*/
	gettimeofday(&time_start, NULL);

	/*shard_node initialize*/
	for(int i = 0; i < url.size(); i++){
		DownloadNode node;
		pthread_t thread;
		node.url = url[i];
		node.path = path[i];
		node.old_local_file_length = get_local_file_length(path[i]);
		node.download_file_length = get_download_file_length(url[i]);
		shared_node.push_back(node);
		pthread_create(&thread, NULL, yycurl, (void *)&shared_node[i]);
		fetch_thread.push_back(thread);
	}

	/*wait for the end*/
	for(int i = 0; i < (int)fetch_thread.size(); i++){
		pthread_join(fetch_thread[i], NULL);
	}
}

void *FetchData::yycurl(void *ptr){
	CURL *curl = curl_easy_init();
	CURLcode res;
	DownloadNode *node = static_cast<DownloadNode*>(ptr);

	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, node->url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_function);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, ptr);
		curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, node->old_local_file_length);

		res = curl_easy_perform(curl);
		if(res != CURLE_OK){
			error_output(res);
		}

		/* always cleanup */
		curl_easy_cleanup(curl);
	}
}

void FetchData::error_output(CURLcode res){
	std::ofstream fout;
	fout.open("/tmp/yycurl.error", std::ios::app);
	fout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
	fout.close();
}

size_t FetchData::write_function(char *data, size_t size, size_t nmemb, void *ptr){
	DownloadNode *node = static_cast<DownloadNode*>(ptr);
	std::ofstream fout;
	fout.open(node->path.c_str(), std::ios::app);
	fout.write(data, nmemb * size);
	fout.close();
	FetchData *now = new FetchData();
	now->notify();
	return nmemb * size;
}

int FetchData::get_download_file_length(std::string url){
    double downloadfilelength = 0;
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HEADER, 0);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
    if(curl_easy_perform(curl) == CURLE_OK){
    	curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &downloadfilelength);
    }
    else{
		downloadfilelength = -1;
    }
    return downloadfilelength;
}
