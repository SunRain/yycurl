#include "FetchData.h"

extern bool STOP_ALL;

FetchData::FetchData(int num, std::vector<std::string> &url, std::vector<std::string> &path){
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
		shared_node[i].download_file_length = get_download_file_length(url[i]);
		shared_node[i].begin = false;
		shared_node[i].buffer_is_new = false;

		//download complete judging, if file exits(return 0), download complete
		if(access(path[i].c_str(), 0) == 0){
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

void FetchData::start(){
	//change work_num
	WORK_NUM = WORK_NUM < TOTAL_NUM ? WORK_NUM : TOTAL_NUM;

	//initialize worker_thread and task queue
	worker_thread.clear();
	for(int i = 0; i < WORK_NUM; i++){
		pthread_t thread;
		worker_thread.push_back(thread);
	}
	while(!my_queue.task.empty()) my_queue.task.pop();
	for(int i = 0; i < TOTAL_NUM; i++){
		my_queue.task.push(&shared_node[i]);
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

	while(1){
		pthread_mutex_lock(&que->mutex);
		//if task queue is empty, thread exit
		if(que->task.empty())
			break;
		DownloadNode *node = que->task.front();
		que->task.pop();
		pthread_mutex_unlock(&que->mutex);

		if(node->done)
			continue;
		//download start
		node->begin = true;
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

			res = curl_easy_perform(curl);

			if(res != CURLE_OK){
				error_output(res);
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

void FetchData::error_output(CURLcode res){
	std::ofstream fout;
	fout.open("/tmp/yycurl.error", std::ios::app);
	fout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
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
	return downloadfilelength;
}
