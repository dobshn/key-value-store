#include "../header/crypto_util.h"
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/base64.h>

using namespace std;

/**
 * �׽�Ʈ ������ ������ key�� iv�� �����Ͽ���.
 * TODO: ������ key�� ������ iv �Է¹ޱ�
 */
CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH] = { 0 };
CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE] = { 0 };

/**
 * ���� �Է¹޾� ���� key�� iv�� ����� ��ȣȭ�� ���ڿ��� ��ȯ�Ѵ�.
 */
string encrypt(const string& plain) {
    // 1) AES-CBC�� ��ȣȭ
    string cipher;
    CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption enc;
    enc.SetKeyWithIV(key, sizeof(key), iv);
    CryptoPP::StringSource ss1(plain, true, new CryptoPP::StreamTransformationFilter(enc, new CryptoPP::StringSink(cipher)));

    // 2) ���̳ʸ��� Base64�� ��ȯ
    string cipher_b64;
    CryptoPP::StringSource ss2(cipher, true, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(cipher_b64), false));

    return cipher_b64;
}

/**
 * ��ȣ���� �Է¹޾� ���� key�� iv�� ����� ��ȣȭ�� ���ڿ��� ��ȯ�Ѵ�.
 */
string decrypt(const string& cipher_b64) {
    // 1) Base64�� ���̳ʸ��� ��ȯ
    string cipher;
    CryptoPP::StringSource ss1(cipher_b64, true, new CryptoPP::Base64Decoder(new CryptoPP::StringSink(cipher)));

    // 2) AES-CBC�� ��ȣȭ
    string plain;
    CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption dec;
    dec.SetKeyWithIV(key, sizeof(key), iv);
    CryptoPP::StringSource ss2(cipher, true, new CryptoPP::StreamTransformationFilter(dec, new CryptoPP::StringSink(plain)));

    return plain;
}
