#include<stdio.h>

int main(int argc, char *argv[]){
    FILE *fp;
    fp=fopen(argv[1],"r");
    char ch[128];
    fgets(ch,sizeof(ch),fp);
    fclose(fp);
}