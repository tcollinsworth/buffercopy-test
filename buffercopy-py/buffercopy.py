import random
import time

assets = 1000000 # 200000

contentMetadataBuf = b'123456789a'
scoreBuf = b'bcde'

contentMetadataLen = len(contentMetadataBuf)
scoreBufLen = len(scoreBuf)
contentRespBufSize = contentMetadataLen + scoreBufLen

contentBufList = [contentMetadataBuf for i in range(assets)]
scoreBufList = [scoreBuf for i in range(assets)]

indexes = [random.randint(0, assets-1) for i in range(assets)]

for i in range(1000):
  startMs = int(round(time.time() * 1000))

  respBuf = bytearray(assets*contentRespBufSize)

  for i in range(assets):
    respBuf[i*contentRespBufSize:i*contentRespBufSize+contentMetadataLen] = contentBufList[indexes[i]]
    respBuf[i*contentRespBufSize+contentMetadataLen:i*contentRespBufSize+contentMetadataLen+scoreBufLen] = scoreBufList[indexes[i]]

  endMs = int(round(time.time() * 1000))
  print(endMs - startMs)
