/*******************************************************************************
Description:
   Functions from CRC32 calculation
Arguments:
Return Value:
Remarks:
   * Taken from u-boot distribution ./tools/crc32.c
   * This file is derived from crc32.c from the zlib-1.1.3 distribution
   * by Jean-loup Gailly and Mark Adler.
    crc32.c -- compute the CRC-32 of a data stream
                              Copyright (c) 2009
                            Lantiq Deutschland GmbH
                     Am Campeon 3; 85579 Neubiberg, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/* ============================= */
/* Includes                      */
/* ============================= */
#include "ifx_crc.h"

/* ============================= */
/* Local variable definition     */
/* ============================= */
 /* Table of CRC-32's of all single-byte values (made by make_crc_table) */
#ifdef DYNAMIC_CRC
static   IFX_boolean_t     bCrc16TableInit = IFX_FALSE;
static   IFX_boolean_t     bCrc32TableInit = IFX_FALSE;
static   IFX_uint32_t   crc32_table[256];
static   IFX_uint16_t   crc16_table[256];
#else
static const IFX_uint_t   crc32_table[256] = {
   0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
   0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
   0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
   0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
   0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
   0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
   0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
   0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
   0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
   0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
   0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
   0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
   0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
   0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
   0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
   0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
   0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
   0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
   0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
   0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
   0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
   0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
   0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
   0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
   0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
   0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
   0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
   0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
   0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
   0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
   0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
   0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
   0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
   0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
   0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
   0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
   0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
   0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
   0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
   0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
   0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
   0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
   0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
   0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
   0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
   0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
   0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
   0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
   0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
   0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
   0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
   0x2d02ef8dL
};

static const   IFX_uint16_t   crc16_table[256] =
{
   0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
   0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
   0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
   0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
   0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
   0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
   0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
   0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
   0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
   0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
   0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
   0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
   0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
   0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
   0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
   0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
   0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
   0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
   0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
   0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
   0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
   0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
   0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
   0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
   0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
   0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
   0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
   0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
   0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
   0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
   0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
   0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};
#endif /* DYNAMIC_CRC */

/* ============================= */
/* Local function definition     */
/* ============================= */
#ifdef DYNAMIC_CRC
static IFX_uint16_t DoCrc16 (IFX_uint16_t data, IFX_uint16_t genpoly, IFX_uint16_t accum);
static IFX_void_t   BuildCrc16Table(IFX_void_t);
static IFX_void_t   BuildCrc32Table(IFX_void_t);
#endif

/*******************************************************************************
Description:
   Calculate the checksum over a buffer. The buffer has to be IFX_uint_t aligned.
Arguments:
   nBufferAddress - the address of the buffer
   nCount - the number of BYTES's in the buffer !!!!
Return Value:
   uiChecksum - the checksum
*******************************************************************************/
IFX_uint16_t IFX_CalcChecksum(IFX_uint8_t* nBufferAddress, IFX_uint_t nCount)
{
   IFX_uint16_t uiChecksum = 0;

   while(nCount--)
   {

      uiChecksum += *nBufferAddress++;
   }

   return uiChecksum;
}

#ifdef DYNAMIC_CRC
/*******************************************************************************
Description:
   Function CalcChecksumCRC16 to calculate a CRC16 as defined by CCITT
Arguments:
   src - the address of the buffer
   length - the number of BYTES's in the buffer !!!!
Return Value:
   crc16 - the crrc16 value
*******************************************************************************/
static IFX_uint16_t DoCrc16(IFX_uint16_t data, IFX_uint16_t genpoly, IFX_uint16_t accum)
{
    IFX_int_t j;

    data <<= 1;
    for (j=8; j>0; j--)
    {
        data >>= 1;
        if ( (data ^ accum) & 0x0001)
            accum = (accum >> 1) ^ genpoly;
        else
            accum >>= 1;
    }
    return accum;
}
#endif /* DYNAMIC_CRC */

/*******************************************************************************
Description:
   Function CalcChecksumCRC16 to calculate a CRC16 as defined by CCITT
   If crc table has not been initialized, function BuildCrcTable() will
   initialized crc table.
Arguments:
   src - the address of the buffer
   length - the number of BYTES's in the buffer !!!!
Return Value:
   crc16 - the crc16 value
*******************************************************************************/
IFX_uint16_t IFX_CalcChecksumCRC16(const IFX_char_t* buf, IFX_uint_t length)
{
   IFX_uint16_t crc16;
   IFX_uint32_t i;
   /* CRC polynomial using a static hash table */
   #define CRC_16_CCITT_M(c,b)    (c >> 8) ^ crc16_table[(c ^ b) & 0x00FF]
   /* CRC polynomial without an hash table */
   /* #define CRC_16_CCITT_M(c,b)    (c >> 8) ^ Do_crc(((c ^ (IFX_uint16_t)b) & 0x00FF), 0x8408, 0); */

   crc16 = 0xFFFF;

   #ifdef DYNAMIC_CRC
   if(!bCrc16TableInit)
      BuildCrc16Table();
   #endif

   /* Computation of CRC via look up table */
   for (i = 0; i < length; i++)
   {
      crc16 = CRC_16_CCITT_M(crc16,*(buf+i));
   }

   return (crc16);
}


#ifdef DYNAMIC_CRC
/*******************************************************************************
Description:
   Function BuildCrc16Table calculates the CRC16 look up table
Arguments:
Return Value:
Remarks:
*******************************************************************************/
static IFX_void_t BuildCrc16Table()
{
   IFX_int_t i;
   /* Calculate the CRC table */
   for (i=0; i<256; i++)
   {
      crc16_table[i] = DoCrc16(i, 0x8408, 0);
   }
   bCrc16TableInit = IFX_TRUE;
}


/*******************************************************************************
Description:
   Function BuildCrc32Table calculates the CRC32 look up table
Arguments:
Return Value:
Remarks:
*******************************************************************************/
static IFX_void_t BuildCrc32Table()
{
  IFX_uint32_t c;
  IFX_int_t n, k;
  IFX_uint32_t poly;            /* polynomial exclusive-or pattern */

  /* terms of polynomial defining this crc (except x^32): */
  static const IFX_uint8_t p[] = {0,1,2,4,5,7,8,10,11,12,16,22,23,26};

  /* make exclusive-or pattern from polynomial (0xedb88320L) */
  poly = 0L;
  for (n = 0; n < sizeof(p)/sizeof(IFX_uint8_t); n++)
    poly |= 1L << (31 - p[n]);

  for (n = 0; n < 256; n++)
  {
    c = (IFX_uint32_t)n;
    for (k = 0; k < 8; k++)
      c = c & 1 ? poly ^ (c >> 1) : c >> 1;
    crc32_table[n] = c;
  }
  bCrc32TableInit = IFX_TRUE;
}
#endif /* DYNAMIC_CRC */

#define DO1(buf) crc = crc32_table[((IFX_int_t)crc ^ (*buf++)) & 0xff] ^ (crc >> 8);
#define DO2(buf)  DO1(buf); DO1(buf);
#define DO4(buf)  DO2(buf); DO2(buf);
#define DO8(buf)  DO4(buf); DO4(buf);

/*******************************************************************************
Description:
   Functions from CRC32 calculation
Arguments:
Return Value:
Remarks:
*******************************************************************************/
IFX_uint32_t IFX_CalcChecksumCRC32 (IFX_uint_t crc, const IFX_char_t* buf, IFX_uint_t len)
{

#ifdef DYNAMIC_CRC
    if (!bCrc32TableInit)
      BuildCrc32Table();
#endif
    crc = crc ^ 0xffffffffL;
    while (len >= 8)
    {
      DO8(buf);
      len -= 8;
    }
    if (len) do {
      DO1(buf);
    } while (--len);
    return crc ^ 0xffffffffL;
}


