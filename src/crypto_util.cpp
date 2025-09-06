#include "../header/crypto_util.h"
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/base64.h>

using namespace std;

/**
 * 테스트 용으로 간단한 key와 iv로 설정하였다.
 * TODO: 생성된 key와 무작위 iv 입력받기
 */
CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH] = { 0 };
CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE] = { 0 };

/**
 * 평문을 입력받아 전역 key와 iv를 사용해 암호화된 문자열을 반환한다.
 */
string encrypt(const string& plain) {
    // 1) AES-CBC로 암호화
    string cipher;
    CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption enc;
    enc.SetKeyWithIV(key, sizeof(key), iv);
    CryptoPP::StringSource ss1(plain, true, new CryptoPP::StreamTransformationFilter(enc, new CryptoPP::StringSink(cipher)));

    // 2) 바이너리를 Base64로 변환
    string cipher_b64;
    CryptoPP::StringSource ss2(cipher, true, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(cipher_b64), false));

    return cipher_b64;
}

/**
 * 암호문을 입력받아 전역 key와 iv를 사용해 복호화된 문자열을 반환한다.
 */
string decrypt(const string& cipher_b64) {
    // 1) Base64를 바이너리로 변환
    string cipher;
    CryptoPP::StringSource ss1(cipher_b64, true, new CryptoPP::Base64Decoder(new CryptoPP::StringSink(cipher)));

    // 2) AES-CBC로 복호화
    string plain;
    CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption dec;
    dec.SetKeyWithIV(key, sizeof(key), iv);
    CryptoPP::StringSource ss2(cipher, true, new CryptoPP::StreamTransformationFilter(dec, new CryptoPP::StringSink(plain)));

    return plain;
}
