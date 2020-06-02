#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdlib.h>
#include "common.h"

const char* optstring = "s:p:h";
//参数结构体,包含敏感词和监控路径
typedef struct arg {
    char string[1024];
    char path[1024];
};

struct arg Arg;

//解析参数
void resolve_argv(int argc, char *argv[]) {
    int ch;

    while((ch = getopt(argc,argv,optstring))!= -1)
    {
        switch(ch)
        {
	case 'h':
	    printf("-s specify string\n" \
		   "-p specify path\n");
	    exit(0);
	    break;
    	case 's':
    	    strcpy(Arg.string, optarg);
    	    break;
    	case 'p':
	    realpath(optarg, Arg.path);
    	    //strcpy(Arg.path, optarg);
    	    break;
    	default:
	    printf("invalid arguments\n");
	    exit(1);
    	    break;
        }
    }
}

int main(int argc, char *argv[]) {
   //解析参数
    resolve_argv(argc, argv);
   //打开监控配置文件
    FILE *fp = fopen(MONITOR_CONF, "w");
    if (!fp) return 0;
    char file_string[2048];
    char file_path[2048];
   //格式化并写入配置文件
    sprintf(file_string, "string=%s\n", Arg.string);
    sprintf(file_path, "path=%s\n", Arg.path);
    fwrite(file_string, strlen(file_string), 1, fp);
    fwrite(file_path, strlen(file_path), 1, fp);
    fflush(fp);
    fclose(fp);
    return 0;
}
