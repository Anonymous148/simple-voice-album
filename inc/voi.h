#ifndef __VOI_H
#define __VOI_H
#define REC_CMD  "arecord -d3 -c1 -r16000 -traw -fS16_LE cmd.pcm"
#define PCM_FILE "./cmd.pcm"
void catch(int sig);
int init_asr(char* argv);
int run_asr(int sockfd);
void uinit_asr(int sockfd);
#endif