#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

typedef struct _Records{
    int integer;
    char string[1024];
}Records;

#define NRECORDS 100

int main(void)
{
    Records record, *mapped;
    int i, f;
    FILE *file;

    file = fopen("records.dat", "wb+");
    for(i = 0; i < NRECORDS; i++){
        record.integer = i;
        sprintf(record.string, "RECORD-%d", i);
        fwrite(&record, sizeof(record), 1, file);
    }

    fclose(file);
    file = fopen("records.dat", "rb+");
    // 设置偏移量.
    fseek(file, 43 * sizeof(record), record.integer);
    // 读取文件.
    fread(&record, sizeof(record), 1, file);

    record.integer = 143;
    sprintf(record.string, "RECORDS-%d", record.integer);
    // 设置偏移量.
    fseek(file,43 * sizeof(record), record.integer);
    // 写入文件.
    fwrite(&record, sizeof(record), 1, file);

    f = open("records.dat", O_RDWR);
    mapped = (Records *)mmap(0, NRECORDS * sizeof(record), PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);
    mapped[43].integer = 243;
    sprintf(mapped[43].string, "RECORD-%d", mapped[43].integer);

    msync((void *)mapped, NRECORDS * sizeof(record), MS_ASYNC);
    munmap((void *)mapped, NRECORDS * sizeof(record));
    close(f);

    return 0;
}
