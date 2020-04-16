import tensorflow as tf
import random
import time
import sys
import numpy as np

assets = 1000000

# unit64 exists, but is not supported in required TF operations
# 4 bytes signed = 2 billion
# int64 (8 bytes), high 4 bytes = index, low 4 bytes = score
# on content-search load metadata lookup table lang, content type, parent index,

# foo = np.int64(1).tobytes()
# print(foo)
# contentMetadataBuf = 0xffff0000
# print(hex(contentMetadataBuf))
# print(hex(contentMetadataBuf+1))
# prebuild metadata numpy arrays and tensors with indexes shifted into upper bytes
contentMetadataBuf = 0x12340000
# metadata = tf.constant([contentMetadataBuf], dtype=tf.int64)
# print(metadata)
      

def random_sample(size=None, dtype=np.float64):
    if type(dtype) == str:
        dtype = np.dtype(dtype).type

    type_max = 1 << np.finfo(dtype).nmant
    sample = np.empty(size, dtype=dtype)
    sample[...] = np.random.randint(0, type_max, size=size) / dtype(type_max)
    return sample

metadata = tf.constant([contentMetadataBuf for i in range(assets)], dtype=tf.int64)
# scores = tf.constant([random.randint(0, assets-1) for i in range(assets)], dtype=tf.int64)
scores = tf.keras.backend.constant(np.random.random_sample((assets)))
# scores = tf.constant([random.randint(0, assets-1) for i in range(assets)], dtype=tf.float32)
# print(scores)

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
    scores = tf.constant([random.randint(0, assets-1) for i in range(assets)], dtype=tf.float32)

    startMs = int(round(time.time() * 1000))
    
    scores = tf.multiply(scores, 1073741824) # scale into int32 space
    scores = tf.cast(scores, dtype=tf.int64) # cast from from float32 to int64
#     printTfHex('scores', scores)

    indexes = tf.argsort(scores,axis=-1,direction='ASCENDING',stable=False,name=None)
#     print('indexes', indexes)
    sortedMetadata = tf.gather(metadata, indexes)
    sortedScores = tf.gather(scores, indexes)
#     printTfHex('sortedMetadata', sortedMetadata)
    combined = tf.add(sortedMetadata, sortedScores)
#     print(tf.print(tf.slice(combined, [0], [2])))
#     printTfHex('combined', combined)

    b = bytearray(assets*8)
    foo = combined.numpy()
    a = 0
    for i in range(assets):
        b[i*8:i*8+8] = foo[0].item().to_bytes(8, 'big')
    endMs = int(round(time.time() * 1000))
    print(endMs - startMs)