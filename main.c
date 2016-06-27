#include<stdlib.h>
#include"fileclass.h"
#include"flvclass.h"


//* init some global variable
FLVContext flvContext;

//* main_init
void main_init()
{
  initFile(&flvContext.fileContext,INPUTFILENAME,OUTPUTFILENAME);
}
//* main_close
void main_close()
{
  //  free(flvContext.tag[1].tagBody.videoData.avcVideoPacket.avcVideoPacketContent.avcDecoderConfigurationRecord.sequenceParameterSetNALUnit);
  //  free(flvContext.tag[1].tagBody.videoData.avcVideoPacket.avcVideoPacketContent.avcDecoderConfigurationRecord.pictureParameterSetNALUnit);
  closeFile(&flvContext.fileContext);
}
//* main
int main()
{
  main_init();
  DEMUX_run(&flvContext);
  //  releaseSource();

  printf("begin write h.264\n");
  DEMUX_writeH264(&flvContext);
  
  main_close();
  return 0;
}
