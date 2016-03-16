TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/base64/mite_base64.c \
    test/base64_test.c \
    test/md5_test.c \
    src/md5/mite_md5.c \
    src/json/test/test.c \
    src/json/test/test_utils.c \
    src/json/cJSON.c \
    src/json/cJSON_Utils.c \
    test/json_test.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    inc/base64/mite_base64.h \
    inc/openssl/aes.h \
    inc/openssl/asn1.h \
    inc/openssl/asn1_mac.h \
    inc/openssl/asn1t.h \
    inc/openssl/bio.h \
    inc/openssl/blowfish.h \
    inc/openssl/bn.h \
    inc/openssl/buffer.h \
    inc/openssl/camellia.h \
    inc/openssl/cast.h \
    inc/openssl/cmac.h \
    inc/openssl/cms.h \
    inc/openssl/comp.h \
    inc/openssl/conf.h \
    inc/openssl/conf_api.h \
    inc/openssl/crypto.h \
    inc/openssl/des.h \
    inc/openssl/des_old.h \
    inc/openssl/dh.h \
    inc/openssl/dsa.h \
    inc/openssl/dso.h \
    inc/openssl/dtls1.h \
    inc/openssl/e_os2.h \
    inc/openssl/ebcdic.h \
    inc/openssl/ec.h \
    inc/openssl/ecdh.h \
    inc/openssl/ecdsa.h \
    inc/openssl/engine.h \
    inc/openssl/err.h \
    inc/openssl/evp.h \
    inc/openssl/hmac.h \
    inc/openssl/krb5_asn.h \
    inc/openssl/kssl.h \
    inc/openssl/lhash.h \
    inc/openssl/md4.h \
    inc/openssl/md5.h \
    inc/openssl/modes.h \
    inc/openssl/obj_mac.h \
    inc/openssl/objects.h \
    inc/openssl/ocsp.h \
    inc/openssl/opensslconf.h \
    inc/openssl/opensslv.h \
    inc/openssl/ossl_typ.h \
    inc/openssl/pem.h \
    inc/openssl/pem2.h \
    inc/openssl/pkcs12.h \
    inc/openssl/pkcs7.h \
    inc/openssl/pqueue.h \
    inc/openssl/rand.h \
    inc/openssl/rc2.h \
    inc/openssl/rc4.h \
    inc/openssl/ripemd.h \
    inc/openssl/rsa.h \
    inc/openssl/safestack.h \
    inc/openssl/seed.h \
    inc/openssl/sha.h \
    inc/openssl/srp.h \
    inc/openssl/srtp.h \
    inc/openssl/ssl.h \
    inc/openssl/ssl2.h \
    inc/openssl/ssl23.h \
    inc/openssl/ssl3.h \
    inc/openssl/stack.h \
    inc/openssl/symhacks.h \
    inc/openssl/tls1.h \
    inc/openssl/ts.h \
    inc/openssl/txt_db.h \
    inc/openssl/ui.h \
    inc/openssl/ui_compat.h \
    inc/openssl/whrlpool.h \
    inc/openssl/x509.h \
    inc/openssl/x509_vfy.h \
    inc/openssl/x509v3.h \
    inc/md5/mite_md5.h \
    inc/json/cJSON.h \
    inc/json/cJSON_Utils.h

