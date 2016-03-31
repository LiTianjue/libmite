TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    read_sn.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    openssl/aes.h \
    openssl/asn1.h \
    openssl/asn1_mac.h \
    openssl/asn1t.h \
    openssl/bio.h \
    openssl/blowfish.h \
    openssl/bn.h \
    openssl/buffer.h \
    openssl/camellia.h \
    openssl/cast.h \
    openssl/cmac.h \
    openssl/cms.h \
    openssl/comp.h \
    openssl/conf.h \
    openssl/conf_api.h \
    openssl/crypto.h \
    openssl/des.h \
    openssl/des_old.h \
    openssl/dh.h \
    openssl/dsa.h \
    openssl/dso.h \
    openssl/dtls1.h \
    openssl/e_os2.h \
    openssl/ebcdic.h \
    openssl/ec.h \
    openssl/ecdh.h \
    openssl/ecdsa.h \
    openssl/engine.h \
    openssl/err.h \
    openssl/evp.h \
    openssl/hmac.h \
    openssl/krb5_asn.h \
    openssl/kssl.h \
    openssl/lhash.h \
    openssl/md4.h \
    openssl/md5.h \
    openssl/modes.h \
    openssl/obj_mac.h \
    openssl/objects.h \
    openssl/ocsp.h \
    openssl/opensslconf.h \
    openssl/opensslv.h \
    openssl/ossl_typ.h \
    openssl/pem.h \
    openssl/pem2.h \
    openssl/pkcs12.h \
    openssl/pkcs7.h \
    openssl/pqueue.h \
    openssl/rand.h \
    openssl/rc2.h \
    openssl/rc4.h \
    openssl/ripemd.h \
    openssl/rsa.h \
    openssl/safestack.h \
    openssl/seed.h \
    openssl/sha.h \
    openssl/srp.h \
    openssl/srtp.h \
    openssl/ssl.h \
    openssl/ssl2.h \
    openssl/ssl23.h \
    openssl/ssl3.h \
    openssl/stack.h \
    openssl/symhacks.h \
    openssl/tls1.h \
    openssl/ts.h \
    openssl/txt_db.h \
    openssl/ui.h \
    openssl/ui_compat.h \
    openssl/whrlpool.h \
    openssl/x509.h \
    openssl/x509_vfy.h \
    openssl/x509v3.h
