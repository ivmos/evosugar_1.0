#include <openssl/md5.h>

// gcc -lssl -lcrypto ejemplomd5.c
void print_md5_sum(unsigned char* md) {
    int i;
    for(i=0; i <MD5_DIGEST_LENGTH; i++) {
            printf("%02x",md[i]);
    }
}



int main() {
	unsigned char * hash;
	hash = MD5("hola",4,NULL);
	char buffer[20];
	unsigned char *finalbuffer;	
	int i;
	for (i=0; i <MD5_DIGEST_LENGTH; i++) {
		sprintf(buffer,"%02x",hash[i]);
		if (i <= 0) {
			sprintf(finalbuffer,"%s",buffer);
		 } else {
			sprintf(finalbuffer,"%s%s",finalbuffer,buffer);	
		}
		
	}
	printf("%s\n",finalbuffer);
	return 0;
}
