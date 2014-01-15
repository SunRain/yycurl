#include "FetchData.h"
#include <iostream>

extern bool STOP_ALL;

FetchData::FetchData(int num, std::vector<std::string> url, std::vector<std::string> path){
	// Must initialize libcurl before any threads are started
	curl_global_init(CURL_GLOBAL_ALL);

	//initialize
	TOTAL_NUM = num;
	shared_node.clear();
	pthread_mutex_init(&my_queue.mutex, NULL);

	for(int i = 0; i < TOTAL_NUM; i++){
		DownloadNode node;
		shared_node.push_back(node);

		//shared node initialize
		shared_node[i].url = url[i];
		shared_node[i].path = path[i];
		shared_node[i].begin = false;
		shared_node[i].done = false;
		shared_node[i].buffer_is_new = false;
		shared_node[i].local_file_length = 0;
		shared_node[i].download_file_length = 0;
		shared_node[i].retry_time = 0;
	}
}

void FetchData::start(){
	//change work_num
	WORK_NUM = WORK_NUM < TOTAL_NUM ? WORK_NUM : TOTAL_NUM;

	//initialize worker_thread and task queue
	worker_thread.clear();
	for(int i = 0; i < WORK_NUM; i++){
		pthread_t thread;
		worker_thread.push_back(thread);
	}
	my_queue.task.clear();
	for(int i = 0; i < TOTAL_NUM; i++){
		my_queue.task.push_back(&shared_node[i]);
	}

	//worker threads start
	for(int i = 0; i < WORK_NUM; i++){
		pthread_create(&worker_thread[i], NULL, yycurl, (void *)&my_queue);
	}
}

void FetchData::join(){
	for(int i = 0; i < WORK_NUM; i++){
		pthread_join(worker_thread[i], NULL);
	}
}

void *FetchData::yycurl(void *ptr){
	MyQueue *que = static_cast<MyQueue *>(ptr);

	FetchData tmp;
	//if task queue is empty, thread exit
	while(!que->task.empty() && !STOP_ALL){
		pthread_mutex_lock(&que->mutex);
		DownloadNode *node = que->task.front();
		que->task.pop_front();
		pthread_mutex_unlock(&que->mutex);

		//download start
		node->begin = true;

		//get total length of download file
		node->download_file_length = tmp.get_download_file_length(node->url);

		//download complete judging, if file exits(return 0), download complete
		if(access(node->path.c_str(), 0) == 0){
			node->local_file_length = tmp.get_local_file_length(node->path);
			node->done = true;
		}
		else{
			std::string tmp_path = node->path + ".yytmp";
			node->local_file_length = tmp.get_local_file_length(tmp_path);
			node->done = false;
		}

		//jump over completed tasks
		if(node->done)
			continue;

		//curl handle initialize and fetch begin
		CURL *curl = curl_easy_init();
		CURLcode res;

		if(curl){
			curl_easy_setopt(curl, CURLOPT_URL, node->url.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_function);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)node);
			curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, node->local_file_length);
			//allow 301 and 302
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			//not allow signals
			curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
			//if perform error, retry!
			if((res = curl_easy_perform(curl)) != CURLE_OK){
				error_output(node, res);
				node->retry_time++;
				pthread_mutex_lock(&que->mutex);
				que->task.push_front(node);
				pthread_mutex_unlock(&que->mutex);
			}
			curl_easy_cleanup(curl);
		}

		//rename download file
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
}

void FetchData::error_output(DownloadNode *node, CURLcode res){
	std::ofstream fout;
	fout.open("/tmp/yycurl.error", std::ios::app);
	fout << "[" << node->url << "] (" << node->retry_time << ") local_length: " << node->local_file_length
			<<"\ncurl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
	fout.close();
}

size_t FetchData::write_function(char *data, size_t size, size_t nmemb, void *ptr){
	//if ctrl+z, stop the thread
	if(STOP_ALL){
		pthread_exit(NULL);
	}
	//write data into the file
	DownloadNode *node = static_cast<DownloadNode*>(ptr);
	std::ofstream fout;
	std::string tmp_path = node->path + ".yytmp";
	fout.open(tmp_path.c_str(), std::ios::app);
	fout.write(data, nmemb * size);
	fout.close();
	//notify
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
	curl_easy_cleanup(curl);
	return downloadfilelength;
}
