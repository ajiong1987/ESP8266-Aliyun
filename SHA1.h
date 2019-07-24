/*
	100% free public domain implementation of the SHA-1 algorithm
	by Dominik Reichl <dominik.reichl@t-online.de>
	Web: http://www.dominik-reichl.de/

	Version 1.6 - 2005-02-07 (thanks to Howard Kapustein for patches)
	- You can set the endianness in your files, no need to modify the
	  header file of the CSHA1 class any more
	- Aligned data support
	- Made support/compilation of the utility functions (ReportHash
	  and HashFile) optional (useful, if bytes count, for example in
	  embedded environments)

	Version 1.5 - 2005-01-01
	- 64-bit compiler compatibility added
	- Made variable wiping optional (define SHA1_WIPE_VARIABLES)
	- Removed unnecessary variable initializations
	- ROL32 improvement for the Microsoft compiler (using _rotl)

	======== Test Vectors (from FIPS PUB 180-1) ========

	SHA1("abc") =
		A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D

	SHA1("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq") =
		84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1

	SHA1(A million repetitions of "a") =
		34AA973C D4C4DAA4 F61EEB2B DBAD2731 6534016F
*/

#ifndef ___SHA1_HDR___
#define ___SHA1_HDR___

#if !defined(SHA1_UTILITY_FUNCTIONS) && !defined(SHA1_NO_UTILITY_FUNCTIONS)
#define SHA1_UTILITY_FUNCTIONS
#endif

//#include <memory.h> // Needed for memset and memcpy

#ifdef SHA1_UTILITY_FUNCTIONS
#include <stdio.h>  // Needed for file access and sprintf
#include <string.h> // Needed for strcat and strcpy
#endif

#ifdef _MSC_VER
#include <stdlib.h>
#endif


#if !defined(SHA1_LITTLE_ENDIAN) && !defined(SHA1_BIG_ENDIAN)
#define SHA1_LITTLE_ENDIAN
#endif

#if !defined(SHA1_WIPE_VARIABLES) && !defined(SHA1_NO_WIPE_VARIABLES)
#define SHA1_WIPE_VARIABLES
#endif

#ifndef uint32_t

#ifdef _MSC_VER

#define uint8_t  unsigned __int8
#define uint32_t unsigned __int32

#else

#define uint8_t unsigned char

#if (ULONG_MAX == 0xFFFFFFFF)
#define uint32_t unsigned long
#else
#define uint32_t unsigned int
#endif

#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// Declare SHA1 workspace

#include <stdio.h>
#include <stdlib.h>

typedef union
{
	uint8_t  c[64];
	uint32_t l[16];
} SHA1_WORKSPACE_BLOCK;

class CSHA1
{
public:
#ifdef SHA1_UTILITY_FUNCTIONS
	// Two different formats for ReportHash(...)
	enum
	{
		REPORT_HEX = 0,
		REPORT_DIGIT = 1
	};
#endif

	// Constructor and Destructor
	CSHA1();
	~CSHA1();

	uint32_t m_state[5];
	uint32_t m_count[2];
	uint32_t __reserved1[1];
	uint8_t  m_buffer[64];
	uint8_t  m_digest[20];
	uint32_t __reserved2[3];

	void Reset();

	// Update the hash value
	void Update(uint8_t *data, uint32_t len);
#ifdef SHA1_UTILITY_FUNCTIONS
	bool HashFile(char *szFileName);
#endif

	// Finalize hash and report
	void Final();

	// Report functions: as pre-formatted and raw data
#ifdef SHA1_UTILITY_FUNCTIONS
	void ReportHash(char *szReport, unsigned char uReportType = REPORT_HEX);
#endif
	void GetHash(uint8_t *puDest);

private:
	// Private SHA-1 transformation
	void Transform(uint32_t *state, uint8_t *buffer);

	// Member variables
	uint8_t m_workspace[64];
	SHA1_WORKSPACE_BLOCK *m_block; // SHA1 pointer to the byte array above
};

#endif
