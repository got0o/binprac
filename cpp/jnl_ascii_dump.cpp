#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cstring>

void asciiDump(char *pc, int len);

const int JNL_YEAR_LEN = 4;
const int JNL_MONTH_LEN = 2;
const int JNL_DAY_LEN = 2;

const int JNL_HOUR_LEN = 2;
const int JNL_MINUTE_LEN = 2;
const int JNL_SECOND_LEN = 2;
const int JNL_MSECOND_LEN = 3;

const int JNL_KIND_LEN = 1;
const int JNL_DATA_LEN = 8;

struct jnl_header
{
  char year[JNL_YEAR_LEN];
  char month[JNL_MONTH_LEN];
  char day[JNL_DAY_LEN];

  char hour[JNL_HOUR_LEN];
  char minute[JNL_MINUTE_LEN];
  char second[JNL_SECOND_LEN];
  char msecond[JNL_MSECOND_LEN];

  char kind[JNL_KIND_LEN];
  char dataLen[JNL_DATA_LEN];

  void print();

} __attribute__((packed));

void jnl_header::print()
{
  std::printf("%*.*s/%*.*s/%*.*s,%*.*s:%*.*s:%*.*s.%*.*s,%*.*s,%*.*s",
      JNL_YEAR_LEN, JNL_YEAR_LEN, year,
      JNL_MONTH_LEN, JNL_MONTH_LEN, month,
      JNL_DAY_LEN, JNL_DAY_LEN, day,
      JNL_HOUR_LEN, JNL_HOUR_LEN, hour,
      JNL_MINUTE_LEN, JNL_MINUTE_LEN, minute,
      JNL_SECOND_LEN, JNL_SECOND_LEN, second,
      JNL_MSECOND_LEN, JNL_MSECOND_LEN, msecond,
      JNL_KIND_LEN, JNL_KIND_LEN, kind,
      JNL_DATA_LEN, JNL_DATA_LEN, dataLen);
}

const int dataBufferSize = 20 * 1024 * 1024;
char *dataBuffer;

void dumpStream(FILE *fp)
{
  while(1)
  {
    struct jnl_header jnlh;
    size_t retFread = std::fread(&jnlh, sizeof(jnlh), 1, fp);
    if(1 != retFread)
    {
      if(std::feof(fp))
      {
        break;
      }

      std::perror("ジャーナルヘッダfread異常");
      std::exit(1);
    }

    std::printf("HEADER:");
    jnlh.print();
    std::putchar('\n');

    char dataLenBuf[JNL_DATA_LEN + 1];
    std::memcpy(dataLenBuf, jnlh.dataLen, JNL_DATA_LEN);
    dataLenBuf[JNL_DATA_LEN] = '\0';

    size_t dataLen = std::atoi(dataLenBuf);

    retFread = std::fread(dataBuffer, sizeof(char), dataLen, fp);
    if(retFread < dataLen)
    {
      std::perror("データfread異常");
      std::exit(1);
    }

    printf("DATA:");
    asciiDump(dataBuffer, dataLen);
    putchar('\n');
  }
}

void asciiDumpArgs(int argc, char *argv[])
{
  for(int i = 1; i < argc; i++)
  {
    FILE *fp = std::fopen(argv[i], "r");
    if(NULL == fp)
    {
      std::perror("fopen異常");
      std::exit(1);
    }

    dumpStream(fp);

    int retFclose = fclose(fp);
    if(0 != retFclose)
    {
      std::perror("fclose異常");
      std::exit(1);
    }
  }
}

/* 1文字毎の出力関数呼び出し */
void asciiDump(char *pc, int len)
{
  for(int i = 0; i < len; i++)
  {
    if(std::isprint(*pc))
    {
      std::putchar(*pc);
    }
    else
    {
      std::putchar('.');
    }
  }
}

int main(int argc, char *argv[])
{
  int isUseStdin = 0;
  if(argc == 1)
  {
    isUseStdin = 1;
  }
  else if(argc == 2 && 0 == strcmp("-h", argv[1]))
  {
    fprintf(stderr, "Usage : %s ファイル名\n", argv[0]);
    std::exit(1);
  }

  dataBuffer = (char *)std::malloc(dataBufferSize);
  if(NULL == dataBuffer)
  {
    std::perror("malloc異常");
    std::exit(1);
  }

  if(isUseStdin)
  {
    dumpStream(stdin);
  }
  else
  {
    asciiDumpArgs(argc, argv);
  }

  return 0;
}
