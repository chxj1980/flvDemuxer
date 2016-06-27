#include<stdio.h>
#include"flvclass.h"
#include"assert.h"
#include<stdlib.h>

//* some helper 
uint8_t uintHelpArray2[2];
uint8_t uintHelpArray3[3];
uint8_t uintHelpArray4[4];
uint8_t uintHelpArray8[8];
uint32_t uintHelpArray4ToInt()
{
  return (uintHelpArray4[0]<<24)+(uintHelpArray4[1]<<16)+(uintHelpArray4[2]<<8)+uintHelpArray4[3];
}
uint32_t uintHelpArray3ToInt()
{
  //  printf("uintHelpArray3[0]<<16=%d\n", uintHelpArray3[0]<<16);
  //  printf("uintHelpArray3[1]<<8=%d\n", uintHelpArray3[1]<<8);
  return (uintHelpArray3[0]<<16)+(uintHelpArray3[1]<<8)+uintHelpArray3[2];
}
uint32_t uintHelpArray2ToInt()
{
  return (uintHelpArray2[0]<<8)+uintHelpArray2[1];
}
int readUintHelpArrayToInt(FILE* pFile, int size)
{
  switch(size){
  case 2:
    fread(uintHelpArray2, 1, 2, pFile);
    break;
  case 3:
    fread(uintHelpArray3, 1, 3, pFile);
    break;
  case 4:
    fread(uintHelpArray4, 1, 4, pFile);
    break;
  case 8:
    fread(uintHelpArray8, 1, 8, pFile);
    break;
  default:
    assert(0);
    break;
}
  return 0;
}
int showUintHelpArrayToInt(int size)
{
  
  int i;
  i=size;
  switch(i){
  case 2:{
    printf("uintHelpArray%d[]\n",i);
    for(i=0;i<2;i++)	
      {
	printf("%02x ", uintHelpArray2[i]);
      }
    printf("\n");    
  }
    break;
  case 4:{
    printf("uintHelpArray%d[]\n",i);
    for(i=0;i<4;i++)	
      {
	printf("%02x ", uintHelpArray4[i]);
      }
    printf("\n");    
  }
    break;
  case 8:{
    printf("uintHelpArray%d[]\n",i);
    for(i=0;i<8;i++)	
      {
	printf("%02x ", uintHelpArray8[i]);
      }
    printf("\n");    
  }
    break;
  default:
    assert(0);
    break;
  }
  return 0;
}
int showData(uint8_t* data,int size)
{
  int i;
  for(i=0;i<size;i++)
    {
      printf("%x ", *(data+i));
    }
  printf("\n");
  
  return 0;
}
int skipPreviousTagSize(FLVContext* pflvContext)
{
  fseek(pflvContext->fileContext.inputFile, 4,SEEK_CUR);
  return 0;
}

int moveToNextTag(FLVContext* pflvContext,int i)
{
  long endPos;
  endPos = pflvContext->tag[i].tagHeader.startPos + pflvContext->tag[i].tagHeader.dataSize+TagHeaderNormalSize;
  seekFile(&pflvContext->fileContext,endPos);
  skipPreviousTagSize(pflvContext);
  return 0;
}
//* FlvHeader
//** readflvheader
int readFlvHeader(FLVContext * pflvContext)
{
  uint8_t* signature;
  signature = pflvContext->flvHeader.signature;

  fread(pflvContext->flvHeader.signature,1,3,pflvContext->fileContext.inputFile);
  if(signature[0]=='F'&&'L'==signature[1]&&'V'==signature[2])
    {
      fread(&pflvContext->flvHeader.version, 1,1,pflvContext->fileContext.inputFile);
      fread(&pflvContext->flvHeader.typeFlags, 1,1,pflvContext->fileContext.inputFile);
  
      fread(uintHelpArray4, 1,4,pflvContext->fileContext.inputFile);
      pflvContext->flvHeader.dataOffset = uintHelpArray4ToInt();

    }
  return 0;
}


//** showflvheader
int showflvheader(FLVContext* pflvContext)
{
  printf("signature %x %x %x\n",pflvContext->flvHeader.signature[0],pflvContext->flvHeader.signature[1],pflvContext->flvHeader.signature[2]);
  printf("version=%d\n", pflvContext->flvHeader.version);
  printf("dataOffset=%d\n",pflvContext->flvHeader.dataOffset);

  return 0;
}


//* readFlvTag
//** readFlvTagHeader
int showPreTagID(FLVContext* pflvContext,int i)
{
  Tag* pTag;
  pTag = &pflvContext->tag[i];
  switch(pTag->tagHeader.tagType)
    {
    case 8://audio
      printf("pre audio id = %d\n", pflvContext->audioSize-1);
      break;
    case 9://video
      printf("pre video id = %d\n", pflvContext->videoSize-1);
      break;
    case 18://scrip
      printf("pre scrip id = %d\n", pflvContext->scripSize-1);
      break;
    default:
      break;
    }
  return 0;
}
int showFlvTagHeader(FLVContext* pflvContext,int i);
int readFlvTagHeader(FLVContext*pflvContext,int i)
{
  assert(&pflvContext->fileContext);
  
  if(i>0)
  showPreTagID(pflvContext, i-1);
  

  pflvContext->tag[i].tagHeader.startPos = getCurrentFilePos(&pflvContext->fileContext);
 
  fread(&pflvContext->tag[i].tagHeader.tagType, 1,1,pflvContext->fileContext.inputFile);
  
  fread(uintHelpArray3, 1,3,pflvContext->fileContext.inputFile);
  //  printf("uintHelpArray3[0..3]=%x,%x,%x\n", uintHelpArray3[0],uintHelpArray3[1],uintHelpArray3[2]);
  pflvContext->tag[i].tagHeader.dataSize = uintHelpArray3ToInt();

  fread(uintHelpArray3, 1,3,pflvContext->fileContext.inputFile);
  pflvContext->tag[i].tagHeader.timeStamp = uintHelpArray3ToInt();

  fread(&pflvContext->tag[i].tagHeader.timeStampExtended, 1,1,pflvContext->fileContext.inputFile);

  fread(uintHelpArray3, 1,3,pflvContext->fileContext.inputFile);
  pflvContext->tag[3].tagHeader.streamId = uintHelpArray3ToInt();
  // wait to delete
  if(i==5001)
    {
      printf("5001 first show flv header\n");
      showFlvTagHeader(pflvContext,i);
    }
  return 0;
}
int showFlvTagHeader(FLVContext* pflvContext,int i)
{
  printf("tagType=%d\n", pflvContext->tag[i].tagHeader.tagType);
  printf("dataSize=%d\n",pflvContext->tag[i].tagHeader.dataSize);
  printf("timeStamp=%d\n",pflvContext->tag[i].tagHeader.timeStamp);
  printf("timeStampExtended=%d\n",pflvContext->tag[i].tagHeader.timeStampExtended);
  printf("streamId=%d\n",pflvContext->tag[i].tagHeader.streamId);
  return 0;
}
//** readFlvTagBody
//*** scripTag
int readScriptDataValue(FILE * pFile,ScripDataValue* pScripDataValue);
int readScripString(FILE*pFile,ScriptString* pScriptString)
{
  fread(uintHelpArray2, 1, 2, pFile);
  printf("uintHelpArray2[0..1]=%u,%u\n", uintHelpArray2[0],uintHelpArray2[1]);
  pScriptString->stringLen = uintHelpArray2ToInt();
  assert(pScriptString->stringLen< maxScriptStringLength);

  fread(pScriptString->stringMeta, 1, pScriptString->stringLen,pFile);
  pScriptString->stringMeta[pScriptString->stringLen]='\0';
  return 0;
}
int readScriptDataVariable(FILE*pFile,ScriptDataVariable* pScriptDataVariable)
{
  readScripString(pFile, &pScriptDataVariable->variableNameString);
  printf("variableNameString=%s\n", pScriptDataVariable->variableNameString.stringMeta);
  ScripDataValue * newScripDataValue;
  newScripDataValue =(ScripDataValue*) malloc(sizeof(ScripDataValue));
  assert(newScripDataValue);

  pScriptDataVariable->variableDataString = newScripDataValue;

  readScriptDataValue(pFile, pScriptDataVariable->variableDataString);
  return 0;
}
int readECMAArray(FILE*pFile,ECMAArray* pECMAArray)
{
  fread(uintHelpArray4, 1, 4, pFile);
  pECMAArray->length = uintHelpArray4ToInt();
  assert(pECMAArray->length<ECMAArrayMaxLength);

  int i;
  for(i=0;i<pECMAArray->length;i++)
    {
      readScriptDataVariable(pFile, pECMAArray->scriptDataVariableArray+i);
    }
  return 0;
}
uint8_t uint8_8[8];
int readDouble(FILE * pFile,ScripDataValue* pScripDataValue)
{
  /*
    fread(uint8_8, 1, 8, pFile);
    for(int i=0;i<8;i++)
    {
    printf("uint8_8=[0...7]=%x ",uint8_8[i]);
    }
    printf("\n");
  */

  /*
    fread(&pScripDataValue->scriptDataValueContent.dValue, 1, 8, pFile);
    for(int i=0;i<8;i++)
    {
    printf("[%d]=%x ",i,*((uint8_t*)&pScripDataValue->scriptDataValueContent.dValue+i));
    }
    printf("\n");
  */


  fread((uint8_t*)(&pScripDataValue->scriptDataValueContent.dValue)+7, 1, 1, pFile);
  fread((uint8_t*)(&pScripDataValue->scriptDataValueContent.dValue)+6, 1, 1, pFile);
  fread((uint8_t*)(&pScripDataValue->scriptDataValueContent.dValue)+5, 1, 1, pFile);
  fread((uint8_t*)(&pScripDataValue->scriptDataValueContent.dValue)+4, 1, 1, pFile);
  fread((uint8_t*)(&pScripDataValue->scriptDataValueContent.dValue)+3, 1, 1, pFile);
  fread((uint8_t*)(&pScripDataValue->scriptDataValueContent.dValue)+2, 1, 1, pFile);
  fread((uint8_t*)(&pScripDataValue->scriptDataValueContent.dValue)+1, 1, 1, pFile);
  fread((uint8_t*)(&pScripDataValue->scriptDataValueContent.dValue)+0, 1, 1, pFile);
  /*
    for(int i=0;i<8;i++)
    {
    printf("[%d]=%x ",i,*((uint8_t*)&pScripDataValue->scriptDataValueContent.dValue+i));
    }
  */
  printf("double=%lf\n", pScripDataValue->scriptDataValueContent.dValue);
  return 0;
}
int readScriptDataValue(FILE * pFile,ScripDataValue* pScripDataValue)
{
  fread(&pScripDataValue->type, 1, 1, pFile);
  switch(pScripDataValue->type){
  case 0:
    readDouble(pFile, pScripDataValue);
    break;
  case 1:
    fread(&pScripDataValue->scriptDataValueContent.boolValue,1,1,pFile);
    break;
    //string
  case 2:{
    ScriptString* pScriptString;
    pScriptString = &pScripDataValue->scriptDataValueContent.scriptString;
    readScripString(pFile, pScriptString);
  }
    break;
  case 7:
    break;
    //ECMAArray
  case 8:{
    ECMAArray * pECMAArray;
    pECMAArray = &pScripDataValue->scriptDataValueContent.eCMAArray;
    readECMAArray(pFile,pECMAArray);
  }
    break;
  default:
    break;
  }
  return 0;
}


int readFlvTagBody_Scrip(FLVContext *pflvContext,int i)
{
  ScripDataValue *pamf1;
  pamf1 = &pflvContext->tag[i].tagBody.scripData.amf1;
  readScriptDataValue(pflvContext->fileContext.inputFile, pamf1);
  printf("metaData = %s\n", pamf1->scriptDataValueContent.scriptString.stringMeta);

  ScripDataValue *pamf2;
  pamf2 = &pflvContext->tag[i].tagBody.scripData.amf2;
  readScriptDataValue(pflvContext->fileContext.inputFile, pamf2);
  

  return 0;
}
//*** video tag
int readAVCDecoderConfigurationRecord(FILE* pFile,AVCDecoderConfigurationRecord*pAVCDecoderConfigurationRecord)
{
  fread(&pAVCDecoderConfigurationRecord->configurationVersion, 1, 1, pFile);
  fread(&pAVCDecoderConfigurationRecord->AVCProfileIndication, 1, 1, pFile);
  fread(&pAVCDecoderConfigurationRecord->profile_compatibility, 1, 1, pFile);
  fread(&pAVCDecoderConfigurationRecord->AVCLevelIndication, 1, 1, pFile);
  fread(&pAVCDecoderConfigurationRecord->reservedAndLengthSizeMinusOne, 1, 1, pFile);

  //read sps
  fread(&pAVCDecoderConfigurationRecord->numOfSequenceParameterSets, 1, 1, pFile);
  pAVCDecoderConfigurationRecord->numOfSequenceParameterSets = pAVCDecoderConfigurationRecord->numOfSequenceParameterSets & 0x1f;
    printf("numOfSequenceParameterSets=%x\n",pAVCDecoderConfigurationRecord->numOfSequenceParameterSets);
  assert(pAVCDecoderConfigurationRecord->numOfSequenceParameterSets == 1);
  readUintHelpArrayToInt(pFile, 2);
  int spsLength;
  spsLength=pAVCDecoderConfigurationRecord->sequenceParameterSetLength=uintHelpArray2ToInt();
  assert(pAVCDecoderConfigurationRecord->sequenceParameterSetNALUnit = (uint8_t*)malloc(sizeof(uint8_t)*(spsLength)));
  fread(pAVCDecoderConfigurationRecord->sequenceParameterSetNALUnit, 1, spsLength, pFile);
  showData(pAVCDecoderConfigurationRecord->sequenceParameterSetNALUnit,spsLength);
  //read pps
  fread(&pAVCDecoderConfigurationRecord->numOfPictureParameterSets, 1, 1, pFile);
  printf("numOfPictureParameterSets=%x\n",pAVCDecoderConfigurationRecord->numOfPictureParameterSets);
  assert(pAVCDecoderConfigurationRecord->numOfPictureParameterSets == 1);
  int ppsLength;
  readUintHelpArrayToInt(pFile, 2);
  ppsLength=pAVCDecoderConfigurationRecord->pictureParameterSetLength=uintHelpArray2ToInt();
  assert(pAVCDecoderConfigurationRecord->pictureParameterSetNALUnit = (uint8_t*)malloc(sizeof(uint8_t)*(ppsLength)));
  fread(pAVCDecoderConfigurationRecord->pictureParameterSetNALUnit, 1, ppsLength, pFile);
  showData(pAVCDecoderConfigurationRecord->pictureParameterSetNALUnit,ppsLength);
  //free(pAVCDecoderConfigurationRecord->pictureParameterSetNALUnit);
  //free(pAVCDecoderConfigurationRecord->sequenceParameterSetNALUnit);
  return 0;
}
int readAvcData(FILE*pFile,AvcEsDataPos* pAvcEsDataPos,int avcEsDataPosSize)
{
  pAvcEsDataPos->startPos = ftell(pFile);
  pAvcEsDataPos->size = avcEsDataPosSize;
  return 0;
}
#define TempDataSize 60000
uint8_t tempData[TempDataSize];
int showAvcData(AvcEsDataPos* pAvcEsDataPos)
{
  FILE* tempFile;
  tempFile = fopen(INPUTFILENAME, "rb");
  fseek(tempFile, pAvcEsDataPos->startPos, SEEK_SET);

  assert(pAvcEsDataPos->size<TempDataSize);
  fread(tempData, 1, pAvcEsDataPos->size, tempFile);
  printf("line=%d,pAvcEsDataPos->startPos=%ld,pAvcEsDataPos->size=%ld\n",__LINE__,pAvcEsDataPos->startPos, pAvcEsDataPos->size);
  int i;
    for(i=0;i<pAvcEsDataPos->size;i++)
  //  for(i=0;i<10;i++)
    {
      printf("%x ", tempData[i]);
    }
  printf("\n");
  fclose(tempFile);
  return 0;
}
int readAvcVideoPacket(FILE* pFile,AvcVideoPacket*pAvcVideoPacket,int avcEsDataPosSize)
{
  fread(&pAvcVideoPacket->AVCPacketType, 1, 1, pFile);
  readUintHelpArrayToInt(pFile, 3);
  pAvcVideoPacket->CompositionTime = uintHelpArray3ToInt();

  switch(pAvcVideoPacket->AVCPacketType)
    {
    case 0:{
      AVCDecoderConfigurationRecord * pAvcDecoderConfigurationRecord;
      pAvcDecoderConfigurationRecord = &pAvcVideoPacket->avcVideoPacketContent.avcDecoderConfigurationRecord;
      readAVCDecoderConfigurationRecord(pFile,pAvcDecoderConfigurationRecord);
    }
      break;
    case 1:{
      AvcEsDataPos* pAvcEsDataPos;
      pAvcEsDataPos = &pAvcVideoPacket->avcVideoPacketContent.avcEsDataPos;
      readAvcData(pFile,pAvcEsDataPos,avcEsDataPosSize);
//      showAvcData(pAvcEsDataPos);
    }
      break;
    case 2:
      {
	printf("AVCPacketType = 2\n");
      }
      break;
    default:{
      assert(0);
    }
      break;
    }
  return 0;
}
int readFlvTagBody_Video(FLVContext *pflvContext,int i)
{
  uint8_t tUint8;
  VideoData *vIdeoData;
  vIdeoData = &pflvContext->tag[i].tagBody.videoData;
  fread(&tUint8, 1, 1, pflvContext->fileContext.inputFile);
  vIdeoData->frameType = tUint8>>4;
  vIdeoData->codecId = tUint8 & 0x0f;
  printf("frameType=%d,codecId=%d\n", vIdeoData->frameType,vIdeoData->codecId);

  switch(vIdeoData->codecId){

  case 7:{
    AvcVideoPacket* pAvcVideoPacket;
    pAvcVideoPacket = &vIdeoData->avcVideoPacket;
    int avcEsDataSize = pflvContext->tag[i].tagHeader.dataSize-5;
    printf("line=%d,pAvcEsDataPos->size=%d\n",__LINE__, avcEsDataSize);
    readAvcVideoPacket(pflvContext->fileContext.inputFile, pAvcVideoPacket,avcEsDataSize);
  }
    break;
    
  default:
    assert(0);
    break;
  }
  
  
  return 0;
}

//** readFlvTag
int readFlvTagBody(FLVContext*pflvContext,int i)
{
  TagHeader* ptagHeader;
  ptagHeader = &pflvContext->tag[i].tagHeader;
  
  switch(ptagHeader->tagType){
    //audio data
  case 8:{
    printf("line=%d,data=%d\n", __LINE__, ptagHeader->dataSize);
    //(pflvContext->audioSize)++;
    printf("line=%d,data=%d\n", __LINE__, ptagHeader->dataSize);
  }
    break;
    //video data
  case 9:{
    readFlvTagBody_Video(pflvContext, i);
    pflvContext->videoSize++;
  }
    break;
    //scrip data
  case 18:{
    //    printf("tagType=%d\n", ptagHeader->tagType);
    //readFlvTagBody_Scrip(pflvContext,i);
    pflvContext->scripSize++;
  }
    break;
  default:
    assert(0);
    break;
  }
  moveToNextTag(pflvContext, i);
  return 0;
}
int readFlvTag(FLVContext* pflvContext,int i)
{
  readFlvTagHeader(pflvContext,i);
  showFlvTagHeader(pflvContext,i);

  readFlvTagBody(pflvContext,i);
  return 0;
}

//* DEMUX_run
int DEMUX_run(FLVContext *pflvContext)
{

  pflvContext->fileBeginPos = ftell(pflvContext->fileContext.inputFile);
  pflvContext->fileEndPos = getFileSize(&pflvContext->fileContext);
  pflvContext->currentFilePos = pflvContext->fileBeginPos;

  readFlvHeader(pflvContext);
  showflvheader(pflvContext);
  skipPreviousTagSize(pflvContext);
 
  pflvContext->currentFilePos = ftell(pflvContext->fileContext.inputFile);
  pflvContext->tagSize = 0;

  int i=0;
   while(pflvContext->currentFilePos < pflvContext->fileEndPos - 4)
     // while(i<6002)
     //   while(i<4)
    {
      printf("packet i = %d\n", i);
        readFlvTag(pflvContext,i++);
	pflvContext->tagSize++;
	pflvContext->currentFilePos = ftell(pflvContext->fileContext.inputFile);
//	assert(pflvContext->currentFilePos < pflvContext->fileEndPos);
	  
	printf("\n");
    }
 

  //confirm curpos at video 0,after confirm must comment it,or will have error
  /*
  readUintHelpArrayToInt(pflvContext, 8);
  showUintHelpArrayToInt(8);
  */
  return 0;
}
//* write h.264
uint8_t data[4]={0x00,0x00,0x00,0x01};
int writeAvcData(FLVContext* pflvContext, Tag* tag)
{
  AvcEsDataPos* pAvcEsDataPos;
  pAvcEsDataPos= &tag->tagBody.videoData.avcVideoPacket.avcVideoPacketContent.avcEsDataPos;
  FILE* tempFile;
  tempFile = fopen(INPUTFILENAME, "rb");
  fseek(tempFile, pAvcEsDataPos->startPos, SEEK_SET);
  printf("pAvcEsDataPos->size=%ld\n", pAvcEsDataPos->size);
  assert(pAvcEsDataPos->size<TempDataSize);
  fread(tempData,  pAvcEsDataPos->size,1, tempFile);
  //showData(tempData,pAvcEsDataPos->size);

  fwrite(data, 4, 1, pflvContext->fileContext.outputFile_h264);
  fwrite(tempData+4, pAvcEsDataPos->size-4, 1, pflvContext->fileContext.outputFile_h264);//attention:this +4 and -4
  fclose(tempFile);
  return 0;
}
int writeSpsPpsData(FLVContext* pflvContext, Tag* tag)
{
  FILE* pFile;
  pFile= pflvContext->fileContext.outputFile_h264;

  AVCDecoderConfigurationRecord* pAVCDecoderConfigurationRecord;
  pAVCDecoderConfigurationRecord = &tag->tagBody.videoData.avcVideoPacket.avcVideoPacketContent.avcDecoderConfigurationRecord;
  fwrite(data, 4, 1, pFile);
  fwrite(pAVCDecoderConfigurationRecord->sequenceParameterSetNALUnit,pAVCDecoderConfigurationRecord->sequenceParameterSetLength , 1,pFile);
  fwrite(data, 4, 1, pFile);
  fwrite(pAVCDecoderConfigurationRecord->pictureParameterSetNALUnit,pAVCDecoderConfigurationRecord->pictureParameterSetLength , 1,pFile);
    fflush(pFile);
  return 0;
}
int DEMUX_writeH264(FLVContext* pFLVContext)
{
  int i;
  Tag* pTag;
  VideoData* pVideoData;
  
  for(i=0;i<pFLVContext->tagSize;i++)
    {
      printf("write i=%d\n", i);
      pTag = &pFLVContext->tag[i];
      pVideoData = &pTag->tagBody.videoData;
      switch(pTag->tagHeader.tagType)
	{
	case 9:
	  {
	    // if(pVideoData->frameType == 1)
	    //{
		if(pVideoData->avcVideoPacket.AVCPacketType == 0)
		  {//head
		    writeSpsPpsData(pFLVContext, pTag);		    
		  }
		else
		  {//body
		    writeAvcData(pFLVContext, pTag);
		  }
		//}else
		//	      {
		//		assert(0);
		//	      }

	  }
	  break;
	default:
	  break;
	}
    }
  return 0;
}
