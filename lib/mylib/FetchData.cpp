#include "FetchData.h"
#include <iostream>

extern bool STOP_ALL;

FetchData::FetchData(int num, std::vector<std::string> &url, std::vector<std::string> &path){
	/* Must initialize libcurl before any threads are started */
	curl_global_init(CURL_GLOBAL_ALL);

	TOTAL_NUM = num;

	/*initialize*/
	shared_node.clear();
	fetch_thread.clear();

	for(int i = 0; i < TOTAL_NUM; i++){
		DownloadNode node;
		pthread_t thread;
		bool buffer;
		shared_node.push_back(node);
		fetch_thread.push_back(thread);

		/*shared node initial*/
		shared_node[i].id = i;
		shared_node[i].url = url[i];
		shared_node[i].path = path[i];
		shared_node[i].download_file_length = get_download_file_length(url[i]);
		shared_node[i].begin = false;
		shared_node[i].buffer_is_new = false;

		/*download complete judging*/
		if(access(path[i].c_str(), 0) == 0){
			/*download complete*/
			shared_node[i].local_file_length = get_local_file_length(path[i]);
			shared_node[i].done = true;
		}
		else{
			std::string tmp_path = path[i] + ".yytmp";
			shared_node[i].local_file_length = get_local_file_length(tmp_path);
			shared_node[i].done = false;
		}
	}
}

int FetchData::download_num(int val){
	int ret = 0;
	for(int i = 0; i < val; i++){
		if(!shared_node[i].done){
			ret++;
		}
	}
	return ret;
}

void FetchData::start(){
	/*change work_num*/
	WORK_NUM = WORK_NUM < TOTAL_NUM ? WORK_NUM : TOTAL_NUM;

	/*thread start*/
	for(int i = 0; i < WORK_NUM; i++){
		pthread_create(&fetch_thread[i], NULL, yycurl, (void *)&shared_node[i]);
	}

	for(int i = WORK_NUM; i < TOTAL_NUM; i++){
		while(download_num(i) == WORK_NUM && !STOP_ALL)
			sleep(1);
		pthread_create(&fetch_thread[i], NULL, yycurl, (void *)&shared_node[i]);
	}
}

void FetchData::join(){
	for(int i = 0; i < TOTAL_NUM; i++){
		/*wait for the end*/
		pthread_join(fetch_thread[i], NULL);
	}
}

void *FetchData::yycurl(void *ptr){
	DownloadNode *node = static_cast<DownloadNode*>(ptr);
	/*if download complete*/
	if(node->done){
		pthread_exit(NULL);
	}

	/*download start*/
	node->begin = true;

	/*curl handle initialize and fetch begin*/
	CURL *curl = curl_easy_init();
	CURLcode res;
	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, node->url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_function);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, ptr);
		curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, node->local_file_length);
		/*allow 301 and 302*/
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		/*not allow signals*/
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
		/*print log*/
//		FILE *fp = fopen("/tmp/yycurl.log", "a+");
//		curl_easy_setopt(curl, CURLOPT_STDERR, fp);
//		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		res = curl_easy_perform(curl);

		if(res != CURLE_OK){
			error_output(res);
		}
		curl_easy_cleanup(curl);
//		fclose(fp);
	}

	/*rename download file*/
	if(node->local_file_length >= node->download_file_length){
		std::string tmp_path = node->path + ".yytmp";
		if(rename(tmp_path.c_str(), node->path.c_str()) < 0){
			std::ofstream fout;
			fout.open("/tmp/yycurl.error", std::ios::app);
			fout << "rename " << tmp_path.c_str() << " failed. " << std::endl;
			fout.close();
		}
		else{
			node->done = true;
		}
	}

}

void FetchData::error_output(CURLcode res){
	std::ofstream fout;
	fout.open("/tmp/yycurl.error", std::ios::app);
	fout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
	fout.close();
}

size_t FetchData::write_function(char *data, size_t size, size_t nmemb, void *ptr){
	if(STOP_ALL){
		pthread_exit(NULL);
	}
	DownloadNode *node = static_cast<DownloadNode*>(ptr);
	std::ofstream fout;
	std::string tmp_path = node->path + ".yytmp";
	fout.open(tmp_path.c_str(), std::ios::app);
	fout.write(data, nmemb * size);
	fout.close();

	FetchData fetch_data;
	fetch_data.notify(ptr);

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
