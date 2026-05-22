#include "RequiredFunctionsHORS.c"
#include "math.h"
 
int main(int argc, char *argv[]) {
    if (argc != 7) {
        printf("Usage: %s <Message.txt> <sign.txt> <PK.txt> <message-length> <t> <k>\n", argv[0]);
        return 1;
    }

    int messageLen = atoi(argv[4]);
    unsigned char *message = Read_File(argv[1], &messageLen);

    int k = atoi(argv[6]);
    unsigned char **sign = malloc(k * sizeof(unsigned char *));
    char **signHex = malloc(k * sizeof(char *));
    for (int i = 0; i < k; i++) {
        signHex[i] = malloc(65 * sizeof(char));
    }
    Read_Multiple_Lines_from_File(argv[2], signHex);
    for (int i = 0; i < k; i++) {
        sign[i] = malloc(32 * sizeof(unsigned char));
        Convert_To_uchar(signHex[i], sign[i], 32);
    }

    int t = atoi(argv[5]);
    unsigned char **PK = malloc(t * sizeof(unsigned char *));
    char **PKHex = malloc(t * sizeof(char *));
    for (int i = 0; i < t; i++) {
        PKHex[i] = malloc(65 * sizeof(char));
    }
    Read_Multiple_Lines_from_File(argv[3], PKHex);
    for (int i = 0; i < t; i++) {
        PK[i] = malloc(32 * sizeof(unsigned char));
        Convert_To_uchar(PKHex[i], PK[i], 32);
    }

    unsigned char *hash = Hash_SHA256(message, messageLen);

    int binaryHash[256];
    charToBinary(hash, 32, binaryHash);

    int log2t = (int)log2(t);
    int *signIndexes = malloc(k * sizeof(int));
    for (int i = 0; i < k; i++) {
        int startIndex = i * log2t;
        signIndexes[i] = binaryToInt(binaryHash, startIndex, log2t);
    }

    int verificationResult = 1;
    for (int i = 0; i < k; i++) {
        unsigned char *computedPK = Hash_SHA256(sign[i], 32);
        if (memcmp(computedPK, PK[signIndexes[i]], 32) != 0) {
            verificationResult = 0;
            break;
        }
    }

    if (verificationResult) {
        Write_int_to_File("Verification.txt", signIndexes, k);
    } else {
        Write_File("Verification.txt", "Verification Failed");
    }

    free(sign);
    free(signHex);
    free(PK);
    free(PKHex);
    free(signIndexes);

    return 0;
}