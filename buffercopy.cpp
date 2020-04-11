#include <stdio.h>
#include <iostream>
#include <cstring>
#include <sys/time.h>

using namespace std;

struct timeval tp;

const uint8_t contentByteCnt = 10;
const uint8_t scoreByteCnt = 4;
const uint8_t contentAndScoreByteCnt = contentByteCnt + scoreByteCnt;

uint8_t contentByteArray[contentByteCnt] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
uint8_t scoreByteArray[scoreByteCnt] = {0x0b,0x0c,0x0d,0x0e};

const uint32_t ASSET_NUM = 1000000; //200000; //1000000;

uint8_t contentListByteArrays [ ASSET_NUM * contentByteCnt ];
uint8_t contentScoreListByteArrays [ ASSET_NUM * scoreByteCnt ];
uint32_t randomIndexes [ ASSET_NUM ];

uint8_t *responseByteArray = 0;

long int startUs = 0;
long int endUs = 0;

int main() {

  //Fill the content list arrays with content byte arrays and score byte arrays
  for (uint32_t i = 0; i < ASSET_NUM; i++) {
    memmove(contentListByteArrays + i * contentByteCnt, contentByteArray, contentByteCnt);
    memmove(contentScoreListByteArrays + i * scoreByteCnt, scoreByteArray, scoreByteCnt);
  }

  //Allocate memory for a new response buffer
  responseByteArray = (uint8_t *) malloc(ASSET_NUM * contentAndScoreByteCnt);
  if (responseByteArray == NULL) {
    fprintf(stderr, "malloc failed\n");
    return -1;
  }


  //Generate random indexes
  srand(time(NULL)); // randomize the randomizer
  for (int i = 0; i < ASSET_NUM; i++) {
    int rndIdx = rand()%(ASSET_NUM + 1);
    randomIndexes[i] = rndIdx;
  }

/*
  // ****************************************************************************************************
  // populate buffer from static buffers, no index lookup - 200K ~1ms, 1M 6ms
  gettimeofday(&tp, NULL);
  startUs = tp.tv_sec * 1000 + tp.tv_usec;

  //Allocate memory for a new response buffer
  responseByteArray = (uint8_t *) malloc(ASSET_NUM * contentAndScoreByteCnt);
  if (responseByteArray == NULL) {
    fprintf(stderr, "malloc failed\n");
    return -1;
  }

  //Fill the response buffer from content list byte arrays - from static buffers
  for (uint32_t i = 0; i < ASSET_NUM; i++) {
    memcpy(responseByteArray + i * contentAndScoreByteCnt, contentByteArray, contentByteCnt);
    memcpy(responseByteArray + i * contentAndScoreByteCnt + contentByteCnt, scoreByteArray, scoreByteCnt);
  }

  gettimeofday(&tp, NULL);
  endUs = tp.tv_sec * 1000 + tp.tv_usec;
  cout << endUs - startUs << " us" << endl;
  // ****************************************************************************************************
*/
  
/*
  // ****************************************************************************************************
  // populate buffer from sequential index lookup, pipelining/cache efficient - 200K ~1.5ms, 1M ~8ms
  gettimeofday(&tp, NULL);
  startUs = tp.tv_sec * 1000 + tp.tv_usec;

  //Allocate memory for a new response buffer
  responseByteArray = (uint8_t *) malloc(ASSET_NUM * contentAndScoreByteCnt);
  if (responseByteArray == NULL) {
    fprintf(stderr, "malloc failed\n");
    return -1;
  }

  //Fill the response buffer from content list byte arrays - from static buffers
  for (int i = 0; i < ASSET_NUM; i++) {
    memcpy(responseByteArray + i * contentAndScoreByteCnt, contentListByteArrays + i*contentAndScoreByteCnt, contentByteCnt);
    memcpy(responseByteArray + i * contentAndScoreByteCnt + contentByteCnt, contentScoreListByteArrays + i*scoreByteCnt, scoreByteCnt);
  }

  gettimeofday(&tp, NULL);
  endUs = tp.tv_sec * 1000 + tp.tv_usec;
  cout << endUs - startUs << " us" << endl;
  // ****************************************************************************************************
*/


  // ****************************************************************************************************
  // populate buffer from random index lookup, NOT pipelining/cache efficient - 200K 2ms, 1M 33ms
  gettimeofday(&tp, NULL);
  startUs = tp.tv_sec * 1000 + tp.tv_usec;

  //Allocate memory for a new response buffer
  responseByteArray = (uint8_t *) malloc(ASSET_NUM * contentAndScoreByteCnt);
  if (responseByteArray == NULL) {
    fprintf(stderr, "malloc failed\n");
    return -1;
  }

  //Fill the response buffer from content list byte arrays - from static buffers
  for (int i = 0; i < ASSET_NUM; i++) {
    memcpy(responseByteArray + i * contentAndScoreByteCnt, contentListByteArrays + randomIndexes[i]*contentAndScoreByteCnt, contentByteCnt);
    memcpy(responseByteArray + i * contentAndScoreByteCnt + contentByteCnt, contentScoreListByteArrays + randomIndexes[i]*scoreByteCnt, scoreByteCnt);
  }

  gettimeofday(&tp, NULL);
  endUs = tp.tv_sec * 1000 + tp.tv_usec;
  cout << endUs - startUs << " us" << endl;
  // ****************************************************************************************************


/*
  for (uint32_t i = 0; i < ASSET_NUM*contentAndScoreByteCnt; i++) {
    if (i > 0 && i % contentAndScoreByteCnt == 0) {
      cout << endl;
    }
    cout << hex << int(responseByteArray[i]) << ",";
  }
  cout << endl;
*/

/*
  if (ASSET_NUM == 1) {
    for (auto val : responseByteArray) {
      printf("\\x%.2x", val);
    }
  }

  //printf("\\x%.2x", contentListByteArrays[79]);
  //cout << endl;
*/

  return 0;
}
