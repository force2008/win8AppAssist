#include "aes.h"
#include "aesTool.h"

namespace native
{

AESTool::AESTool()
{

}

AESTool::~AESTool()
{

}

int AESTool::AESMakeKey(KeyInstance *key, BYTE direction, int keyLen, char *keyMaterial) 
{
	int i;
	char *keyMat;
	uint8_t cipherKey[AES_MAXKB];
	
	if (key == NULL) {
		return BAD_KEY_INSTANCE;
	}

	if ((direction == CIPHER_ENCRYPT) || (direction == CIPHER_DECRYPT)) {
		key->direction = direction;
	} else {
		return BAD_KEY_DIR;
	}

	if ((keyLen == 128) || (keyLen == 192) || (keyLen == 256)) {
		key->keyLen = keyLen;
	} else {
		return BAD_KEY_MAT;
	}

	if (keyMaterial != NULL) {
		strncpy(key->keyMaterial, keyMaterial, keyLen/4);
	}

	/* initialize key schedule: */
	keyMat = key->keyMaterial;
 	for (i = 0; i < key->keyLen/8; i++) {
		int t, v;

		t = *keyMat++;
		if ((t >= '0') && (t <= '9')) v = (t - '0') << 4;
		else if ((t >= 'a') && (t <= 'f')) v = (t - 'a' + 10) << 4;
		else if ((t >= 'A') && (t <= 'F')) v = (t - 'A' + 10) << 4;
		else return BAD_KEY_MAT;
		
		t = *keyMat++;
		if ((t >= '0') && (t <= '9')) v ^= (t - '0');
		else if ((t >= 'a') && (t <= 'f')) v ^= (t - 'a' + 10);
		else if ((t >= 'A') && (t <= 'F')) v ^= (t - 'A' + 10);
		else return BAD_KEY_MAT;
		
		cipherKey[i] = (uint8_t)v;
	}
	if (direction == CIPHER_ENCRYPT) {
		key->Nr = AES::MakeKeyEnc(key->rk, cipherKey, keyLen);
	} else {
		key->Nr = AES::MakeKeyDec(key->rk, cipherKey, keyLen);
	}
	AES::MakeKeyEnc(key->ek, cipherKey, keyLen);
	return TRUE;
}

int AESTool::AESCipherInit(CipherInstance *cipher, BYTE mode, char *IV) 
{
	if ((mode == CIPHER_MODE_ECB) || (mode ==  CIPHER_MODE_CBC) || (mode ==  CIPHER_MODE_CFB)) {
		cipher->mode = mode;
	} else {
		return BAD_CIPHER_MODE;
	}
	if (IV != NULL) {
		int i;
 		for (i = 0; i < MAX_IV_SIZE; i++) {
			int t, j;

			t = IV[2*i];
			if ((t >= '0') && (t <= '9')) j = (t - '0') << 4;
			else if ((t >= 'a') && (t <= 'f')) j = (t - 'a' + 10) << 4;
			else if ((t >= 'A') && (t <= 'F')) j = (t - 'A' + 10) << 4;
			else return BAD_CIPHER_INSTANCE;
		
			t = IV[2*i+1];
			if ((t >= '0') && (t <= '9')) j ^= (t - '0');
			else if ((t >= 'a') && (t <= 'f')) j ^= (t - 'a' + 10);
			else if ((t >= 'A') && (t <= 'F')) j ^= (t - 'A' + 10);
			else return BAD_CIPHER_INSTANCE;
			
			cipher->IV[i] = (uint8_t)j;
		}
	} else {
		memset(cipher->IV, 0, MAX_IV_SIZE);
	}
	return TRUE;
}

void AESTool::AESEncrypt(const uint32_t rk[/*4*(Nr + 1)*/], int Nr, const uint8_t pt[16], uint8_t ct[16])
{
	AES::Encrypt(rk, Nr, pt, ct);
}

void AESTool::AESDecrypt(const uint32_t rk[/*4*(Nr + 1)*/], int Nr, const uint8_t ct[16], uint8_t pt[16])
{
	AES::Decrypt(rk, Nr, ct, pt);
}

int AESTool::AESBlockEncrypt(CipherInstance *cipher, KeyInstance *key,	BYTE *input, int inputLen, BYTE *outBuffer)
{
	int i, k, t, numBlocks;
	uint8_t block[16], *iv;

	if (cipher == NULL ||
		key == NULL ||
		key->direction == CIPHER_DECRYPT) {
		return BAD_CIPHER_STATE;
	}
	if (input == NULL || inputLen <= 0) {
		return 0; /* nothing to do */
	}

	numBlocks = inputLen/128;
	
	switch (cipher->mode) {
	case  CIPHER_MODE_ECB:
		for (i = numBlocks; i > 0; i--) {
			AES::Encrypt(key->rk, key->Nr, input, outBuffer);
			input += 16;
			outBuffer += 16;
		}
		break;
		
	case  CIPHER_MODE_CBC:
		iv = cipher->IV;
		for (i = numBlocks; i > 0; i--) {
			((uint32_t*)block)[0] = ((uint32_t*)input)[0] ^ ((uint32_t*)iv)[0];
			((uint32_t*)block)[1] = ((uint32_t*)input)[1] ^ ((uint32_t*)iv)[1];
			((uint32_t*)block)[2] = ((uint32_t*)input)[2] ^ ((uint32_t*)iv)[2];
			((uint32_t*)block)[3] = ((uint32_t*)input)[3] ^ ((uint32_t*)iv)[3];
			AES::Encrypt(key->rk, key->Nr, block, outBuffer);
			iv = outBuffer;
			input += 16;
			outBuffer += 16;
		}
		break;

    case  CIPHER_MODE_CFB:
		iv = cipher->IV;
        for (i = numBlocks; i > 0; i--) {

			memcpy(outBuffer, input, 16);
            for (k = 0; k < 128; k++) {
				AES::Encrypt(key->ek, key->Nr, iv, block);
                outBuffer[k >> 3] ^= (block[0] & 0x80U) >> (k & 7);
                for (t = 0; t < 15; t++) {
                	iv[t] = (iv[t] << 1) | (iv[t + 1] >> 7);
                }
               	iv[15] = (iv[15] << 1) | ((outBuffer[k >> 3] >> (7 - (k & 7))) & 1);
            }
            outBuffer += 16;
            input += 16;
        }
        break;

	default:
		return BAD_CIPHER_STATE;
	}
	
	return 128*numBlocks;
}

int AESTool::AESBlockDecrypt(CipherInstance *cipher, KeyInstance *key,	BYTE *input, int inputLen, BYTE *outBuffer) 
{
	int i, k, t, numBlocks;
	uint8_t block[16], *iv;

	if (cipher == NULL ||
		key == NULL ||
		cipher->mode != CIPHER_MODE_CFB && key->direction == CIPHER_ENCRYPT) {
		return BAD_CIPHER_STATE;
	}
	if (input == NULL || inputLen <= 0) {
		return 0; /* nothing to do */
	}

	numBlocks = inputLen/128;

	switch (cipher->mode) {
	case  CIPHER_MODE_ECB:
		for (i = numBlocks; i > 0; i--) {
			AES::Decrypt(key->rk, key->Nr, input, outBuffer);
			input += 16;
			outBuffer += 16;
		}
		break;
		
	case CIPHER_MODE_CBC:
		iv = cipher->IV;
		for (i = numBlocks; i > 0; i--) {
			AES::Decrypt(key->rk, key->Nr, input, block);
			((uint32_t*)block)[0] ^= ((uint32_t*)iv)[0];
			((uint32_t*)block)[1] ^= ((uint32_t*)iv)[1];
			((uint32_t*)block)[2] ^= ((uint32_t*)iv)[2];
			((uint32_t*)block)[3] ^= ((uint32_t*)iv)[3];
			memcpy(cipher->IV, input, 16);
			memcpy(outBuffer, block, 16);
			input += 16;
			outBuffer += 16;
		}
		break;

    case CIPHER_MODE_CFB:
		iv = cipher->IV;
        for (i = numBlocks; i > 0; i--) {
			memcpy(outBuffer, input, 16);

            for (k = 0; k < 128; k++) {
				AES::Encrypt(key->ek, key->Nr, iv, block);
                for (t = 0; t < 15; t++) {
                	iv[t] = (iv[t] << 1) | (iv[t + 1] >> 7);
                }
               	iv[15] = (iv[15] << 1) | ((input[k >> 3] >> (7 - (k & 7))) & 1);
                outBuffer[k >> 3] ^= (block[0] & 0x80U) >> (k & 7);
            }
            outBuffer += 16;
            input += 16;
        }
        break;

	default:
		return BAD_CIPHER_STATE;
	}
	
	return 128*numBlocks;
}

}
