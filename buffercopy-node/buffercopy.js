const assets = 1000000 //200000

const contentMetadataBuf = Buffer.allocUnsafe(10);
for (let i = 0; i < 10; i++)  contentMetadataBuf[i] = i+1

const scoreBuf = Buffer.allocUnsafe(4);
for (let i = 0; i < 4; i++) scoreBuf[i] = i+11

const contentMetadataLen = contentMetadataBuf.length
const scoreBufLen = scoreBuf.length
const contentRespBufSize = contentMetadataLen + scoreBufLen

const contentBufList = []
const scoreBufList = []
const indexes = []

for (let i = 0; i < assets; i++) {
    contentBufList[i] = contentMetadataBuf
    scoreBufList[i] = scoreBuf
    indexes[i] = Math.floor(Math.random() * Math.floor(assets))
}

for (let i = 0; i < 1000; i++) {
  const startMs = Date.now()

  const respBuf = Buffer.allocUnsafe(assets*contentRespBufSize)

  for (let i = 0; i < assets; i++) {
    contentBufList[indexes[i]].copy(respBuf, i*contentRespBufSize)
    scoreBufList[indexes[i]].copy(respBuf, i*contentRespBufSize+contentMetadataLen)
  }

  const endMs = Date.now()
  console.log(endMs - startMs)
}