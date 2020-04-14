import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class BufferCopy {

	int metadataLength = 10;
	int scoreLength = 4;
	int metadataAndScoreLength = metadataLength + scoreLength;

	int assets = 200000; // 10; // 1000000;
	// 200K 32 ms
	// 1M 390 ms

	byte[] contentMetadataBuffer = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a };
	byte[] scoreBuffer = { 0x0b, 0x0c, 0x0d, 0x0e };

	List<byte[]> contentMetadataList = new ArrayList<>(assets);
	List<byte[]> scoreList = new ArrayList<>(assets);

	int[] indexes = new int[assets];

	public static void main(String[] args) {
		BufferCopy bc = new BufferCopy();
		bc.thisMain();
	}

	private void thisMain() {
		Random rnd = new Random();

		for (int i = 0; i < assets; i++) {
			contentMetadataList.add(i, contentMetadataBuffer.clone());
			scoreList.add(i, scoreBuffer.clone());
			indexes[i] = rnd.nextInt(assets);
		}

		for (int x = 0; x < 1000; x++) {
			long startNs = System.nanoTime();

			byte[] responeList = new byte[assets * metadataAndScoreLength];
			for (int i = 0; i < assets; i++) {
				System.arraycopy(contentMetadataList.get(indexes[i]), 0, responeList, i * metadataAndScoreLength,
						metadataLength);
				System.arraycopy(scoreList.get(indexes[i]), 0, responeList,
						i * metadataAndScoreLength + metadataLength, scoreLength);
				// System.out.println(byteArrayToHex(responeList));
			}

			long endNs = System.nanoTime();
			System.out.println(String.format("%d %d us", x, (endNs - startNs) / 1000));
		}
	}

	public static String byteArrayToHex(byte[] ba) {
		StringBuilder sb = new StringBuilder(ba.length * 2);
		for (byte b : ba) {
			sb.append(String.format("%02x", b));
		}
		return sb.toString();
	}
}
