#ifndef FLVCLASS
#define FLVCLASS

#include<stdint.h>

#include"fileclass.h"
//* some helper variable
extern uint8_t uintHelpArray2[2];
extern uint8_t uintHelpArray3[3];
extern uint8_t uintHelpArray4[4];
extern uint8_t uintHelpArray8[8];
//* flv header
typedef struct FlvHeader
{
  uint8_t signature[3];
  uint8_t version;
  uint8_t typeFlags;
  uint32_t dataOffset;
}FlvHeader;
//* flv body Tag
//** tag header
#define TagHeaderNormalSize 11
typedef struct TagHeader
{
  uint8_t tagType;
  uint32_t dataSize;
  uint32_t timeStamp;
  uint8_t timeStampExtended;
  uint32_t streamId;

  long startPos;
}TagHeader;
//** tag body
//*** scrip data
//**** sub element
//***** scripString
#define maxScriptStringLength 100
typedef struct ScriptString{
  uint16_t stringLen;
  char stringMeta[maxScriptStringLength];
}ScriptString;
//***** ECMAArray
typedef struct ScriptDataVariable{
  ScriptString variableNameString;
  //ScriptDataValue variableDataString;
  void*  variableDataString;
}ScriptDataVariable;

#define ECMAArrayMaxLength 60
typedef struct ECMAArray{
  int length;
  ScriptDataVariable scriptDataVariableArray[ECMAArrayMaxLength];
}ECMAArray;
//**** top struct
typedef union ScriptDataValueContent{
  double dValue;
  int boolValue;
  uint8_t uint8Value;
  uint16_t uint16Value;
  ScriptString scriptString;
  ECMAArray eCMAArray;
}ScriptDataValueContent;
typedef struct ScripDataValue{
  int type;
  ScriptDataValueContent scriptDataValueContent;
}ScripDataValue;

//**** scrip body main
typedef struct ScripData{
  ScripDataValue amf1;
  ScripDataValue amf2;
}ScripData;

//*** video data
typedef struct AVCDecoderConfigurationRecord{
  uint8_t configurationVersion;
  uint8_t AVCProfileIndication;
  uint8_t profile_compatibility;
  uint8_t AVCLevelIndication;
  uint8_t reservedAndLengthSizeMinusOne;
  
  uint8_t numOfSequenceParameterSets;
  uint16_t sequenceParameterSetLength;
  uint8_t* sequenceParameterSetNALUnit;

  uint8_t numOfPictureParameterSets;
  uint16_t pictureParameterSetLength;
  uint8_t* pictureParameterSetNALUnit;
}AVCDecoderConfigurationRecord;

typedef struct AvcEsDataPos{
  long startPos;
  long size;
}AvcEsDataPos;

typedef union AvcVideoPacketContent{
  AVCDecoderConfigurationRecord avcDecoderConfigurationRecord;
  AvcEsDataPos avcEsDataPos;
}AvcVideoPacketContent;
typedef struct  AvcVideoPacket{
  uint8_t AVCPacketType;
  uint32_t CompositionTime;
  AvcVideoPacketContent avcVideoPacketContent;  
}AvcVideoPacket;

typedef struct VideoData{
  uint8_t frameType;
  uint8_t codecId;
  AvcVideoPacket avcVideoPacket;
}VideoData;
//** tag body Tag
typedef union TagBody{
  ScripData scripData;
  VideoData videoData;
}TagBody;
typedef struct Tag{
  TagHeader tagHeader;
  TagBody tagBody;
}Tag;
//* application struct
typedef enum MediaCodecID {
  /* video codecs */
  V_CODEC_ID_NONE,
  V_CODEC_ID_H264,
  V_CODEC_ID_H263,
  V_CODEC_ID_MPEG4,

  /* audio codecs */
  A_CODEC_ID_NONE,
  A_CODEC_ID_PCM,
  A_CODEC_ID_MP2,
  A_CODEC_ID_MP3,
  A_CODEC_ID_AAC,
}MediaCodecID;

typedef enum MEDIA_TYPE{
  TYPE_VIDEO,
  TYPE_AUDIO,
}MEDIA_TYPE;
typedef struct
{
  char SPSData[128];
  int  SPSDataSize;
  char PPSData[128];
  int  PPSDataSize;

}SPS_PPS_t;

typedef struct MediaInfo{
  MediaCodecID video_codec_id;
  int video_width;
  int video_height;
  float  fps;
  int64_t video_bit_rate;                  //bps
  uint64_t time;                           //ms

   MediaCodecID audio_codec_id;
  int audio_channel;
  int audio_sample_rate;
  int audio_sample_size;                   //8 or 16
  int audio_bit_rate;                      //bps

  SPS_PPS_t    SPS_PPS;
  char         AacHeader[7];
}MediaInfo;

typedef struct DataPacket
{
  MEDIA_TYPE stream_type;     //0：video 1:audio
  int64_t pts;
  int64_t dts;
  int key_frame;              //-1:unknow 0:not keyframe 1:keyframe
  int data_size;
  char *data;

}DataPacket;

#define TagLength 10000
typedef struct FLVContext
{
  FileContext fileContext;
  long fileBeginPos,fileEndPos,currentFilePos;
  MediaInfo mediaInfo;

  FlvHeader flvHeader;
  Tag tag[TagLength];
  int tagSize;
  int audioSize;
  int videoSize;
  int scripSize;
}FLVContext;

//* some function
int readFlvHeader(FLVContext* pflvContext);
int showflvheader(FLVContext* pflvContext);
int DEMUX_run(FLVContext *pflvContext);
int DEMUX_writeH264(FLVContext* pFLVContext);
#endif
