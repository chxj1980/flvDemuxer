#ifndef FILECLASS
#define FILECLASS
#include<stdio.h>
typedef struct FileContext
{
  FILE *inputFile;
  FILE *outputFile_h264;
}FileContext;

int initFile(FileContext* pfileContext,char*inputFileName,char*outputFileName);
int closeFile(FileContext*pfileContext);
long getCurrentFilePos(FileContext* pfileContext);
int seekFile(FileContext* pfileContext,long size);
long getFileSize(FileContext* pfileContext);
#endif
