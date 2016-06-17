#include"fileclass.h"
#include<stdio.h>

int initFile(FileContext* pfileContext,char*inputFileName,char*outputFileName)
{
  pfileContext->inputFile = fopen(inputFileName,"rb");
  pfileContext->outputFile_h264 = fopen(outputFileName,"wb");
  return 0;
}

int closeFile(FileContext*pfileContext)
{
  fclose(pfileContext->inputFile);
  fclose(pfileContext->outputFile_h264);
  return 0;
}
  
long getCurrentFilePos(FileContext* pfileContext)
{
  return ftell(pfileContext->inputFile);
}

int seekFile(FileContext* pfileContext,long size)
{
  fseek(pfileContext->inputFile,size,SEEK_SET );
  return 0;
}

long getFileSize(FileContext* pfileContext)
{
  long currentPos,fileSize;
  currentPos = fseek(pfileContext->inputFile, 0, SEEK_CUR);
  fseek(pfileContext->inputFile, 0, SEEK_END);
  fileSize = ftell(pfileContext->inputFile);
  fseek(pfileContext->inputFile, currentPos, SEEK_SET);
  return fileSize;
}
