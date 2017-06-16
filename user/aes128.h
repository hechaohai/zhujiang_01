#ifndef __AES128_H__
#define __AES128_H__

/*
 * 解密p[0]~p[15]的数据,会直接覆盖解密前的数据
 */
void InvCipher(INT8U *p);

#endif /* __AES128_H__ */
