#ifndef SHZ__Encript_H_
#define SHZ__Encript_H_

#ifdef  __cplusplus
extern "C" {
#endif

namespace o_o
{
// ����
bool Decrypt(unsigned char* buff, unsigned int size, const char* key="");
// ����
bool Encrypt(unsigned char* buff, unsigned int size, const char* key="");

// �򵥼���
bool EncryptA(unsigned char* buf, unsigned int size, int offset = 0);
// �򵥽���
bool DecryptA(unsigned char* buf, unsigned int size, int offset = 0);

};

#ifdef  __cplusplus
}
#endif

#endif // _Encript_H_