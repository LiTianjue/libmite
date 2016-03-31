#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <openssl/x509.h>
int getCertSN(const char *der_data,int der_len);

/*
int    main()
{
	X509                    *x;
	FILE                     *fp;
	unsigned char buf[5000],*p;
	int                         len,ret;
	BIO                      *b;

	fp=fopen("test_cert.cer","rb");
	if(!fp) return -1;
	len=fread(buf,1,5000,fp);
	fclose(fp);
	p=buf;
	x=X509_new();
	d2i_X509(&x,(const unsigned char **)&p,len);
	b=BIO_new(BIO_s_file());
	BIO_set_fp(b,stdout,BIO_NOCLOSE);
    ret=X509_print(b,x);
	ASN1_INTEGER *Serial = NULL;
    Serial = X509_get_serialNumber(x);

    int i = 0;
    for (i = 0; i < Serial->length; i++){
        printf("%02x", Serial->data[i]);
    }
    printf("\n");
    printf("SN_len = %d\n",Serial->length);

	BIO_free(b);
	X509_free(x);
	return 0;
}
*/
int    main()
{
    FILE                     *fp;
    unsigned char buf[5000],*p;
    int                         len,ret;

    fp=fopen("test_cert.cer","rb");
    if(!fp) return -1;
    len=fread(buf,1,5000,fp);
    fclose(fp);

    p=buf;
    ret = getCertSN(buf,len);
    printf("ret = %d\n",ret);

}
int getCertSN(const char *der_data,int der_len)
{
    X509                    *x;
    unsigned char buf[5000],*p;

    //不知道会不会改变值
    memcpy(buf,der_data,der_len);
    p = buf;
    x=X509_new();
    d2i_X509(&x,(const unsigned char **)&p,der_len);

    ASN1_INTEGER *Serial = NULL;
    Serial = X509_get_serialNumber(x);

    int i = 0;
    char sn[64] = {0};
    memset(sn,'\0',64);

    for(i = 0 ;i < Serial->length;i++)
    {
        sprintf(sn+2*i,"%02x",Serial->data[i]);
    }
    printf("sn:%s\n",sn);
    printf("SN_len = %d\n",Serial->length);

    X509_free(x);
    return 0;
}
