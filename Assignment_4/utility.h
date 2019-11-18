/*
*HTTP Proxy Utility Header
*/

#ifndef __HTTP_PROXY_H__
#define __HTTP_PROXY_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <string>
#include <map>
#include <time.h>

using namespace std;
int max_cache_capacity;
unsigned short int client_num;

map<int,struct condition> req_parameters; 
map<int,int> client_request;
map<int,int> get_request;
map<int,int> type;
map<string,int> block_number;
map<int,string> URL;
map<int,bool> Random_number;


struct http{
	char file_name[1024];
	char host_name[1024];
};

struct cache_block{
	string expire_date;
	string host_file;
	int lastused;
	int current;
	int expire;
};
struct cache_block cache[10];

/* Structure to represent the request parameters */
struct condition{
	int expires;
	int length;
	int cb_index;
	int index;
	
	char filename[200];
	bool boolean;
	bool conditional;
};

char * http(char *url)
{
	int length = strlen(url);
	char *out = (char *)malloc((length+50)*sizeof(char));
	int i=0, j=4;
	
	out[0]='G';out[1]='E';out[2]='T';out[3]=' ';
	
	if(length>7)
	{
		if(url[0]=='h' && url[1]=='t' && url[2]=='t' && url[3]=='p' && url[4]==':' && url[5]=='/' && url[6]=='/')
			i = 7;
		if(url[0]=='h' && url[1]=='t' && url[2]=='t' && url[3]=='p' && url[4]=='s' && url[5]==':' && url[6]=='/' && url[7]=='/')
			i = 8;
	}
	
	int temp = i;								
	
	while(i<length && url[temp]!='/')
		temp++;
	
	out[j++]='/';
	temp++;
	
	while(temp<length)
		out[j++]=url[temp++];
	
	out[j++]=' ';out[j++]='H';out[j++]='T';out[j++]='T';out[j++]='P';out[j++]='/';
	out[j++]='1';out[j++]='.';out[j++]='0';out[j++]='\r';out[j++]='\n';
	out[j++]='H';out[j++]='o';out[j++]='s';out[j++]='t';out[j++]=':';out[j++]=' ';
	
	while(i<length && url[i]!='/')
		out[j++] = url[i++];
	
	out[j++]='\r';out[j++]='\n';out[j++]='\r';out[j++]='\n';out[j]='\0';
	
	return out;
}

char *get_filename(char *file)
{
	char *file_name = (char *)malloc(512*sizeof(char));
	int j=0;
	int last = 0;
	int i=0;
	int length = strlen(file);
	
	while(i < length)
	{
		if(file[i]=='/')
			last = i;
		i++;
	}
	if(last!=0)
	{
		memcpy(file_name,&file[last+1],(length-last-1));
		file_name[length-last]='\0';
	}
	if(strlen(file_name)==0)
	{
		file_name[j++]='i';file_name[j++]='n';file_name[j++]='d';file_name[j++]='e';file_name[j++]='x';file_name[j++]='.';		
		file_name[j++]='h';file_name[j++]='t';file_name[j++]='m';file_name[j++]='l';file_name[j]='\0';
		return file_name;
	}
	return file_name;
}


// Updating Cache objects
void update(int block)
{
	if(cache[block].lastused==1)
		return;
	cache[block].lastused = 1;
	for(int i=0; i<10; i++)
	{
		if(i!=block)
		{
			cache[i].lastused++;
			if(cache[i].lastused>10)
				cache[i].lastused = 10;
		}
	}
	return;
}

// Updating LRU Cache 
int update_lru()
{
	int end = 10;
	int ans = -1;
	int i;
	while(end > 0)
	{
		for(i=0; i<10; i++)
		{
			if(cache[i].lastused==end && cache[i].current==0)
			{
				
				ans = i;
				cache[ans].current=-2;
				end = 0;
				break;
			}
		}
		end--;
	}
	return ans;
}

void show_cache_list(int nList)
{
	int i,j, num; 
	num = 10 - nList;
	printf("*******************************************************************\n");
	printf("*                       Current Cashe List                        *\n");
	printf("*******************************************************************\n");

	for( i = 0; i < 10; i++)
	{		
		j = 9 - i;
		if(cache[j].host_file.empty()==true)
			break;
		cout << i+1 << " Cache File: " << cache[j].host_file  << endl;
	}
	printf("*********************** Waiting Next Command **********************\n");
}

// Below we shall check for the stale cachee
bool calc_expire_bool(int cacheBlock_index)
{
	time_t raw_time;
	time(&raw_time);
	struct tm * utc;
	utc = gmtime(&raw_time);
	raw_time = mktime(utc);
	if(cache[cacheBlock_index].expire-raw_time > 0)
	{
		return false;
	}	
	return true;
}

// Check for the exisiting request in the cache
int checkCache(string req)
{
	if(block_number.find(req)==block_number.end())
	{
		return -1;
	}

	int cacheBlock_index = block_number[req];

	if(cache[cacheBlock_index].host_file == req)
	{
		return cacheBlock_index;
	}
	return -1;
}

// Decoding buffer into the host name and the file name 
struct http * parse_http_request(char *buf, int bytes_num)
{
	struct http* out = (struct http *)malloc(sizeof(struct http));
	char* strtmp = (char *)malloc(64);
	int temp;
	
	for(temp=0; temp<bytes_num; temp++)
	{
		if(buf[temp]==' ')
		{
			temp++;
			break;
		}
	}
	int j=0;
	out->file_name[0]='/';
	
	if(buf[temp+8]=='\r' && buf[temp+9]=='\n')
	{
		out->file_name[1]='\0';
	}
	else
	{
		while(buf[temp]!=' ')
			out->file_name[j++]=buf[temp++];
		out->file_name[j]='\0';
	}
	temp+=1;
	while(buf[temp++]!=' ');
	
	j=0;
	
	while(buf[temp]!='\r')
		out->host_name[j++]=buf[temp++];
	
	out->host_name[j]='\0';
	
	return out;
}


#endif
