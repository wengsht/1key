DEBUG = y
obj-m := ok_crypto.o
ok_crypto-objs := ok_main.o ok_file.o ok_mem.o ok_test.o ok_api.o ok_crypto_api.o

TOMMATH_OBJECTS = libtommath/bncore.o libtommath/bn_mp_init.o libtommath/bn_mp_clear.o libtommath/bn_mp_exch.o libtommath/bn_mp_grow.o libtommath/bn_mp_shrink.o \
	libtommath/bn_mp_clamp.o libtommath/bn_mp_zero.o  libtommath/bn_mp_set.o libtommath/bn_mp_set_int.o libtommath/bn_mp_init_size.o libtommath/bn_mp_copy.o \
	libtommath/bn_mp_init_copy.o libtommath/bn_mp_abs.o libtommath/bn_mp_neg.o libtommath/bn_mp_cmp_mag.o libtommath/bn_mp_cmp.o libtommath/bn_mp_cmp_d.o \
	libtommath/bn_mp_rshd.o libtommath/bn_mp_lshd.o libtommath/bn_mp_mod_2d.o libtommath/bn_mp_div_2d.o libtommath/bn_mp_mul_2d.o libtommath/bn_mp_div_2.o \
	libtommath/bn_mp_mul_2.o libtommath/bn_s_mp_add.o libtommath/bn_s_mp_sub.o libtommath/bn_fast_s_mp_mul_digs.o libtommath/bn_s_mp_mul_digs.o \
	libtommath/bn_fast_s_mp_mul_high_digs.o libtommath/bn_s_mp_mul_high_digs.o libtommath/bn_fast_s_mp_sqr.o libtommath/bn_s_mp_sqr.o \
	libtommath/bn_mp_add.o libtommath/bn_mp_sub.o libtommath/bn_mp_karatsuba_mul.o libtommath/bn_mp_mul.o libtommath/bn_mp_karatsuba_sqr.o \
	libtommath/bn_mp_sqr.o libtommath/bn_mp_div.o libtommath/bn_mp_mod.o libtommath/bn_mp_add_d.o libtommath/bn_mp_sub_d.o libtommath/bn_mp_mul_d.o \
	libtommath/bn_mp_div_d.o libtommath/bn_mp_mod_d.o libtommath/bn_mp_expt_d.o libtommath/bn_mp_addmod.o libtommath/bn_mp_submod.o \
	libtommath/bn_mp_mulmod.o libtommath/bn_mp_sqrmod.o libtommath/bn_mp_gcd.o libtommath/bn_mp_lcm.o libtommath/bn_fast_mp_invmod.o libtommath/bn_mp_invmod.o \
	libtommath/bn_mp_reduce.o libtommath/bn_mp_montgomery_setup.o libtommath/bn_fast_mp_montgomery_reduce.o libtommath/bn_mp_montgomery_reduce.o \
	libtommath/bn_mp_exptmod_fast.o libtommath/bn_mp_exptmod.o libtommath/bn_mp_2expt.o libtommath/bn_reverse.o \
	libtommath/bn_mp_count_bits.o libtommath/bn_mp_read_unsigned_bin.o libtommath/bn_mp_read_signed_bin.o libtommath/bn_mp_to_unsigned_bin.o \
	libtommath/bn_mp_to_signed_bin.o libtommath/bn_mp_unsigned_bin_size.o libtommath/bn_mp_signed_bin_size.o  \
	libtommath/bn_mp_rand.o libtommath/bn_mp_montgomery_calc_normalization.o \
	libtommath/bn_mp_prime_is_divisible.o libtommath/bn_prime_tab.o libtommath/bn_mp_prime_miller_rabin.o \
	libtommath/bn_mp_prime_is_prime.o libtommath/bn_mp_prime_next_prime.o libtommath/bn_mp_dr_reduce.o \
	libtommath/bn_mp_dr_is_modulus.o libtommath/bn_mp_dr_setup.o libtommath/bn_mp_reduce_setup.o \
	libtommath/bn_mp_toom_mul.o libtommath/bn_mp_toom_sqr.o libtommath/bn_mp_div_3.o libtommath/bn_s_mp_exptmod.o \
	libtommath/bn_mp_reduce_2k.o libtommath/bn_mp_reduce_is_2k.o libtommath/bn_mp_reduce_2k_setup.o \
	libtommath/bn_mp_reduce_2k_l.o libtommath/bn_mp_reduce_is_2k_l.o libtommath/bn_mp_reduce_2k_setup_l.o \
	libtommath/bn_mp_cnt_lsb.o libtommath/bn_error.o libtommath/bn_mp_init_multi.o libtommath/bn_mp_clear_multi.o \
	libtommath/bn_mp_prime_random_ex.o libtommath/bn_mp_get_int.o libtommath/bn_mp_init_set.o \
	libtommath/bn_mp_init_set_int.o libtommath/bn_mp_invmod_slow.o libtommath/bn_mp_prime_rabin_miller_trials.o \
	libtommath/bn_mp_to_signed_bin_n.o libtommath/bn_mp_to_unsigned_bin_n.o

TOMCRYPT_OBJECTS = libtomcrypt/hashes/cryptodev_cipher.o \
libtomcrypt/misc/zeromem.o libtomcrypt/misc/crypt/crypt_argchk.o \
	libtomcrypt/pk/asn1/der/bit/der_decode_bit_string.o libtomcrypt/pk/asn1/der/bit/der_encode_bit_string.o \
	libtomcrypt/pk/asn1/der/bit/der_length_bit_string.o libtomcrypt/pk/asn1/der/boolean/der_decode_boolean.o \
	libtomcrypt/pk/asn1/der/boolean/der_encode_boolean.o libtomcrypt/pk/asn1/der/boolean/der_length_boolean.o \
	libtomcrypt/pk/asn1/der/choice/der_decode_choice.o libtomcrypt/pk/asn1/der/ia5/der_decode_ia5_string.o \
	libtomcrypt/pk/asn1/der/ia5/der_encode_ia5_string.o libtomcrypt/pk/asn1/der/ia5/der_length_ia5_string.o \
	libtomcrypt/pk/asn1/der/integer/der_decode_integer.o libtomcrypt/pk/asn1/der/integer/der_encode_integer.o \
	libtomcrypt/pk/asn1/der/integer/der_length_integer.o libtomcrypt/pk/asn1/der/object_identifier/der_decode_object_identifier.o \
	libtomcrypt/pk/asn1/der/object_identifier/der_encode_object_identifier.o libtomcrypt/pk/asn1/der/object_identifier/der_length_object_identifier.o \
	libtomcrypt/pk/asn1/der/octet/der_decode_octet_string.o libtomcrypt/pk/asn1/der/octet/der_encode_octet_string.o \
	libtomcrypt/pk/asn1/der/octet/der_length_octet_string.o libtomcrypt/pk/asn1/der/printable_string/der_decode_printable_string.o \
	libtomcrypt/pk/asn1/der/printable_string/der_encode_printable_string.o libtomcrypt/pk/asn1/der/printable_string/der_length_printable_string.o \
	libtomcrypt/pk/asn1/der/sequence/der_decode_sequence_ex.o libtomcrypt/pk/asn1/der/sequence/der_decode_sequence_flexi.o \
	libtomcrypt/pk/asn1/der/sequence/der_decode_sequence_multi.o libtomcrypt/pk/asn1/der/sequence/der_encode_sequence_ex.o \
	libtomcrypt/pk/asn1/der/sequence/der_encode_sequence_multi.o libtomcrypt/pk/asn1/der/sequence/der_length_sequence.o \
	libtomcrypt/pk/asn1/der/sequence/der_sequence_free.o libtomcrypt/pk/asn1/der/short_integer/der_decode_short_integer.o \
	libtomcrypt/pk/asn1/der/short_integer/der_encode_short_integer.o libtomcrypt/pk/asn1/der/short_integer/der_length_short_integer.o \
	libtomcrypt/pk/asn1/der/utctime/der_decode_utctime.o libtomcrypt/pk/asn1/der/utctime/der_encode_utctime.o \
	libtomcrypt/pk/asn1/der/utctime/der_length_utctime.o libtomcrypt/pk/asn1/der/utf8/der_decode_utf8_string.o \
	libtomcrypt/pk/asn1/der/utf8/der_encode_utf8_string.o libtomcrypt/pk/asn1/der/utf8/der_length_utf8_string.o \
	libtomcrypt/pk/asn1/der/set/der_encode_set.o  libtomcrypt/pk/asn1/der/set/der_encode_setof.o \
	libtomcrypt/pk/asn1/der/x509/der_decode_subject_public_key_info.o \
	libtomcrypt/math/rand_prime.o libtomcrypt/hashes/hash_get_oid.o \
	libtomcrypt/hashes/crypt_hash_is_valid.o libtomcrypt/hashes/hash_memory.o libtomcrypt/hashes/hash_memory_multi.o \
	libtomcrypt/pk/rsa/rsa_decrypt_key.o libtomcrypt/pk/rsa/rsa_encrypt_key.o \
	libtomcrypt/pk/rsa/rsa_exptmod.o libtomcrypt/pk/rsa/rsa_free.o  \
	libtomcrypt/pk/rsa/rsa_make_key.o libtomcrypt/pk/rsa/rsa_sign_hash.o libtomcrypt/pk/rsa/rsa_verify_hash.o \
	libtomcrypt/pk/pkcs1/pkcs_1_i2osp.o libtomcrypt/pk/pkcs1/pkcs_1_mgf1.o 	libtomcrypt/pk/pkcs1/pkcs_1_oaep_decode.o \
	libtomcrypt/pk/pkcs1/pkcs_1_oaep_encode.o libtomcrypt/pk/pkcs1/pkcs_1_os2ip.o libtomcrypt/pk/pkcs1/pkcs_1_pss_decode.o \
	libtomcrypt/pk/pkcs1/pkcs_1_pss_encode.o libtomcrypt/pk/pkcs1/pkcs_1_v1_5_decode.o libtomcrypt/pk/pkcs1/pkcs_1_v1_5_encode.o \
	libtomcrypt/misc/pk_get_oid.o libtomcrypt/pk/asn1/der/x509/der_encode_subject_public_key_info.o \
	libtomcrypt/pk/asn1/der/x509/der_decode_subject_public_key_info.o 



#TOMCRYPT_OBJECTS = libtomcrypt-kernel/pk/pkcs1/pkcs_1_i2osp.o \
#				   libtomcrypt-kernel/misc/zeromem.o \
#				   libtomcrypt-kernel/misc/crypt/crypt_prng_descriptor.o \
				   libtomcrypt-kernel/misc/crypt/crypt_prng_is_valid.o \
				   libtomcrypt-kernel/misc/crypt/crypt_argchk.o \
libtomcrypt-kernel/pk/pkcs1/pkcs_1_v1_5_decode.o \
libtomcrypt-kernel/pk/pkcs1/pkcs_1_os2ip.o \
libtomcrypt-kernel/pk/pkcs1/pkcs_1_v1_5_encode.o
#libtomcrypt-kernel/pk/pkcs1/pkcs_1_mgf1.o 
#libtomcrypt-kernel/pk/pkcs1/pkcs_1_oaep_decode.o \
libtomcrypt-kernel/pk/pkcs1/pkcs_1_oaep_encode.o \
libtomcrypt-kernel/pk/pkcs1/pkcs_1_pss_decode.o \
libtomcrypt-kernel/pk/pkcs1/pkcs_1_pss_encode.o \
libtomcrypt-kernel/pk/pkcs1/pkcs_1_v1_5_decode.o \
libtomcrypt-kernel/pk/pkcs1/pkcs_1_v1_5_encode.o

ok_crypto-y += $(TOMMATH_OBJECTS) $(TOMCRYPT_OBJECTS)

EXTRA_CFLAGS+=-I$(PWD)/include  -I$(PWD)/libtomcrypt/headers -I$(PWD)/libtommath

ifeq ($(DEBUG), y)
	DEBFLAGS = -O -g -DOK_DEBUG 
else
	DEBFLAGS = -O2
endif
EXTRA_CFLAGS+=$(DEBFLAGS)

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
MAKE := make

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

test:
	make 
	sudo ./ok_init exit
	sudo ./ok_init
	echo 1123123 > /dev/ok_crypto0
	cat /dev/ok_crypto0
clean:
	rm *.o *.ko *.symvers *.order *.mod.c
	rm libtommath/*.o
	rm libtomcrypt/*/*.o
	
