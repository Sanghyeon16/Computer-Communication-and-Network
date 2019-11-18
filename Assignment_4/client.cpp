#include "utility.h"

using namespace std;

int main(int argc, char *argv[])
{
	/*regular connection sequence*/
	int error;
	if(argc!=4)									
	{
		fprintf(stderr,"[Client Error] : Should type < Server IP > < Port number > < URL >\n");
		return 1;
	}
	struct addrinfo hints;
	struct addrinfo *servinfo, *p;				

	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if( (error = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0){	
		fprintf(stderr, "[Client Error] : Getaddrinfo error: %s\n", gai_strerror(error));
		exit(1);
	}

	int sockfd;
	for(p=servinfo; p!=NULL; p=p->ai_next)
	{
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))== -1){	
			perror("[Client Error] : Socket Failed");
			continue;
		}
		break;
	}
	
	if(p==NULL)
	{
		fprintf(stderr, "[Client Error] : Connect failed\n");
		return 2;
	}

	int size = 2048, bytes_num;
	char *buffer = (char *)malloc((size+1)*sizeof(char));
	unsigned short int length;
	
	if( (error = connect(sockfd,p->ai_addr,p->ai_addrlen)) == -1)			
	{
		perror("[Client Error] : Connect Failed");
	}
	freeaddrinfo(servinfo);
	
	char *query = http(argv[3]);

    length = strlen(query);
    cout << "[Client Log] Request - " << argv[3] << endl;
	if( (error = send(sockfd,query,length,0)) == -1)			
	{
		perror("[Client Error] : Send Failed");
	}
	
	ofstream testFile;
	char *filename = get_filename(argv[3]);
	stringstream ss;
	ss << filename;

	testFile.open(ss.str().c_str(),ios::out | ios::binary);
	if(!testFile.is_open())
	{
		cout << "[Client Error] : Unable to open file in filesystem" << endl;
		exit(0);
	}
	
	bool header = true;
	bool first=true;
	
	while(1)
	{
		int i = 0;
		bytes_num = recv(sockfd,buffer,2048,0);			
		if(first)
		{
			stringstream tempstream;
			tempstream << buffer;
			string line;
			getline(tempstream,line);
			if(line.find("404")!=string::npos)
				cout << "[Client Error] : Unable to  find the page " << line << endl;
			first = false;
		}

		if(header)
		{
			if(bytes_num>=4)
			{
				for(i=3; i<bytes_num; i++)
				{
					if(buffer[i]=='\n' && buffer[i-1]=='\r')
					{
						if(buffer[i-2]=='\n' && buffer[i-3]=='\r')
						{
							header=false;				
							i++;
							break;
						}
					}
				}	
			}
		}
		
		if(!header)
			testFile.write(buffer+i,bytes_num-i);			 
		
		if(bytes_num == -1)
		{
			perror("[Client Error] : Recv Failed ");
		}
		else if(bytes_num == 0)
		{						
			close(sockfd);
			cout << "[Client Log] File name : " << filename << endl;
			testFile.flush();
			testFile.close();
			break;							
		}
		testFile.flush();				
	}
	testFile.close();					
	return 0;
}