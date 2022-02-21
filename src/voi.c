#include "common.h"
#include <sys/ioctl.h>
  
#define REC_CMD  "arecord -d3 -c1 -r16000 -traw -fS16_LE cmd.pcm"
#define PCM_FILE "./cmd.pcm"

int said=0;

void catch(int sig)
{
	if(sig == SIGPIPE)
	{
		printf("killed by SIGPIPE\n");
		exit(0);
	}
}

int init_asr(char* argv)
{
	int sockfd = init_sock(argv); 
	return sockfd;
}

int run_asr(int sockfd)
{
	int id_num=0;

	printf("please to start REC in 3s...\n");
	system(REC_CMD);
	send_pcm(sockfd, PCM_FILE);
	xmlChar *id = wait4id(sockfd);
	if(id == NULL)
	{
		printf("arecord fail\n");
		return -1;
	}
	id_num=atoi((char *)id);
	return id_num;
	
}

void uinit_asr(int sockfd)
{
	close(sockfd);
}