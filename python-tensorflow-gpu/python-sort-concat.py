import tensorflow as tf
import random
import time
import sys
import numpy as np

assets = 1000000

# unit64 exists, but is not supported in required TF operations
# 4 bytes signed = 2 billion
# int64 (8 bytes), high 4 bytes = index, low 4 bytes = score
# on target load metadata lookup table lang, content type, parent index,
# foo = np.int64(1).tobytes()
# print(foo)
# contentMetadataBuf = 0xffff0000
# print(hex(contentMetadataBuf))

# prebuild metadata numpy arrays and tensors with indexes shifted into upper bytes
contentMetadataBuf = 0x12340000
# metadata = tf.constant([contentMetadataBuf], dtype=tf.int64)
# print(metadata)

metadata = tf.constant([contentMetadataBuf for i in range(assets)], dtype=tf.int64)
scores = tf.constant([random.randint(0, assets-1) for i in range(assets)], dtype=tf.int64)
# indexes = tf.constant([random.randint(0, assets-1) for i in range(assets)])

def printTfHex(name, tensor):
    print(name)
    for i in range(tensor.shape[0]):
        print(i, hex(tensor.numpy()[i]))
    
# printTfHex('metadata', metadata)
# print('scores', scores)
# print('indexes', indexes)

print('starting...')

for i in range(10):
    startMs = int(round(time.time() * 1000))

    indexes = tf.argsort(scores,axis=-1,direction='ASCENDING',stable=False,name=None)
#     print('indexes', indexes)

    sortedMetadata = tf.gather(metadata, indexes)

    sortedScores = tf.gather(scores, indexes)

    elems = (sortedMetadata, sortedScores)
    combined = tf.add(sortedMetadata, sortedScores)
#     printTfHex('combined', combined)


    endMs = int(round(time.time() * 1000))
    print(endMs - startMs)