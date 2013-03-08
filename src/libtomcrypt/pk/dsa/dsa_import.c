/* LibTomCrypt, modular cryptographic library -- Tom St Denis
 *
 * LibTomCrypt is a library that provides various cryptographic
 * algorithms in a highly modular and flexible manner.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 *
 * Tom St Denis, tomstdenis@gmail.com, http://libtom.org
 */
#include "tomcrypt.h"
#include <ncr-int.h>

/**
   @file dsa_import.c
   DSA implementation, import a DSA key, Tom St Denis
*/

#ifdef LTC_MDSA

/**
   Import a DSA key 
   @param in       The binary packet to import from
   @param inlen    The length of the binary packet
   @param key      [out] Where to store the imported key
   @return CRYPT_OK if successful, upon error this function will free all allocated memory
*/
int dsa_import(const unsigned char *in, unsigned long inlen, dsa_key * key)
{
	int err;
	unsigned long zero = 0;
	unsigned char *tmpbuf = NULL;
	const struct algo_properties_st *algo = _ncr_algo_to_properties(NCR_ALG_DSA);

	LTC_ARGCHK(in != NULL);
	LTC_ARGCHK(key != NULL);

	if (algo == NULL) {
		return CRYPT_INVALID_ARG;
	}

	/* init key */
	if (mp_init_multi(&key->p, &key->g, &key->q, &key->x, &key->y, NULL) !=
	    CRYPT_OK) {
		return CRYPT_MEM;
	}

	/* get key type */
	if ((err = der_decode_sequence_multi(in, inlen,
					     LTC_ASN1_SHORT_INTEGER, 1UL, &zero,
					     LTC_ASN1_INTEGER, 1UL, &key->p,
					     LTC_ASN1_INTEGER, 1UL, &key->q,
					     LTC_ASN1_INTEGER, 1UL, &key->g,
					     LTC_ASN1_INTEGER, 1UL, &key->y,
					     LTC_ASN1_INTEGER, 1UL, &key->x,
					     LTC_ASN1_EOL, 0UL,
					     NULL)) == CRYPT_OK) {
		key->type = PK_PRIVATE;
	} else {		/* public */
		ltc_asn1_list params[3];
		unsigned long tmpbuf_len = MAX_RSA_SIZE * 8;

		LTC_SET_ASN1(params, 0, LTC_ASN1_INTEGER, &key->p, 1UL);
		LTC_SET_ASN1(params, 1, LTC_ASN1_INTEGER, &key->q, 1UL);
		LTC_SET_ASN1(params, 2, LTC_ASN1_INTEGER, &key->g, 1UL);

		tmpbuf = XCALLOC(1, tmpbuf_len);
		if (tmpbuf == NULL) {
			err = CRYPT_MEM;
			goto LBL_ERR;
		}

		err = der_decode_subject_public_key_info(in, inlen,
							 algo, tmpbuf,
							 &tmpbuf_len,
							 LTC_ASN1_SEQUENCE,
							 params, 3);
		if (err != CRYPT_OK) {
			goto LBL_ERR;
		}

		if ((err =
		     der_decode_integer(tmpbuf, tmpbuf_len,
					&key->y)) != CRYPT_OK) {
			goto LBL_ERR;
		}

		XFREE(tmpbuf);
		key->type = PK_PUBLIC;
	}

	key->qord = mp_unsigned_bin_size(&key->q);

	if (key->qord >= LTC_MDSA_MAX_GROUP || key->qord <= 15 ||
	    (unsigned long)key->qord >= mp_unsigned_bin_size(&key->p)
	    || (mp_unsigned_bin_size(&key->p) - key->qord) >= LTC_MDSA_DELTA) {
		err = CRYPT_INVALID_PACKET;
		goto LBL_ERR;
	}

	return CRYPT_OK;

LBL_ERR:
	XFREE(tmpbuf);
	mp_clear_multi(&key->p, &key->g, &key->q, &key->x, &key->y, NULL);
	return err;
}

#endif

/* $Source: /cvs/libtom/libtomcrypt/src/pk/dsa/dsa_import.c,v $ */
/* $Revision: 1.14 $ */
/* $Date: 2007/05/12 14:32:35 $ */
