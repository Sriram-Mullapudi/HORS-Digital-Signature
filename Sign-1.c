#include "RequiredFunctionsHORS.c"
#include "math.h"

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <Message.txt> <SK.txt> <t> <k>\n", argv[0]);
        return 1;
    }

    int messageLen;
    unsigned char *message = Read_File(argv[1], &messageLen);

    int t = atoi(argv[3]);
    unsigned char **SK = malloc(t * sizeof(unsigned char *));
    char **SKHex = malloc(t * sizeof(char *));
    for (int i = 0; i < t; i++) {
        SKHex[i] = malloc(65 * sizeof(char));
    }
    Read_Multiple_Lines_from_File(argv[2], SKHex);
    for (int i = 0; i < t; i++) {
        SK[i] = malloc(32 * sizeof(unsigned char));
        Convert_To_uchar(SKHex[i], SK[i], 32);
    }

    int k = atoi(argv[4]);

    unsigned char *hash = Hash_SHA256(message, messageLen);

    int binaryHash[256];
    charToBinary(hash, 32, binaryHash);

    int log2t = (int)log2(t);
    int *signIndexes = malloc(k * sizeof(int));
    for (int i = 0; i < k; i++) {
        int startIndex = i * log2t;
        signIndexes[i] = binaryToInt(binaryHash, startIndex, log2t);
    }

    char **signHex = malloc(k * sizeof(char *));
    for (int i = 0; i < k; i++) {
        signHex[i] = malloc(65 * sizeof(char));
        Convert_to_Hex(signHex[i], SK[signIndexes[i]], 32);
    }
    Write_Multiple_Lines_to_File("Signature.txt", signHex, k);

    free(SK);
    free(SKHex);
    free(signIndexes);
    free(signHex);

    return 0;
}