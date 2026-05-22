#include "RequiredFunctionsHORS.c"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <Seed.txt> <t>\n", argv[0]);
        return 1;
    }

    int seedLen;
    unsigned char *seed = Read_File(argv[1], &seedLen);

    int t = atoi(argv[2]);

    unsigned char **SK = malloc(t * sizeof(unsigned char *));
    unsigned char **PK = malloc(t * sizeof(unsigned char *));

    for (int i = 0; i < t; i++) {
        unsigned char counter[2];
        counter[0] = (i >> 8) & 0xFF;counter[1] = i & 0xFF;

        unsigned char seedWithCounter[seedLen + 2];
        memcpy(seedWithCounter, seed, seedLen);
        memcpy(seedWithCounter + seedLen, counter, 2);

        SK[i] = PRNG(seedWithCounter, seedLen + 2, 32);
        PK[i] = Hash_SHA256(SK[i], 32);
    }

    char **SHHex = malloc(t * sizeof(char *));
    for (int i = 0; i < t; i++) {
        SHHex[i] = malloc(65 * sizeof(char));
        Convert_to_Hex(SHHex[i], SK[i], 32);
    }
    Write_Multiple_Lines_to_File("SK.txt", SHHex, t);

    char **PKHex = malloc(t * sizeof(char *));
    for (int i = 0; i < t; i++) {
        PKHex[i] = malloc(65 * sizeof(char));
        Convert_to_Hex(PKHex[i], PK[i], 32);
    }
    Write_Multiple_Lines_to_File("PK.txt", PKHex, t);

    free(SK);
    free(PK);
    free(SHHex);
    free(PKHex);

    return 0;
}