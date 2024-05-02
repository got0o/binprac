import java.io.IOException;
import java.io.FileInputStream;

enum JnlHeaderInfo
{
  YEAR(0, 4),
  MONTH(4, 2),
  DAY(6, 2),

  HOUR(8, 2),
  MINUTE(10, 2),
  SECOND(12, 2),
  MSECOND(14, 3),

  KIND(17, 1),
  DATA(18, 8);

  private final int offset;
  private final int len;

  private JnlHeaderInfo(int offset, int len)
  {
    this.offset = offset;
    this.len = len;
  }

  public int offset()
  {
    return offset;
  }

  public int len()
  {
    return len;
  }
}

class JnlHeader
{
  final static int SIZE = 26;

  final private byte[] h;

  JnlHeader(final byte[] h)
  {
    this.h = h;
  }

  JnlHeader(FileInputStream in) throws IOException
  {
    byte[] buffer = new byte[SIZE];
    int readByte = in.read(buffer);

    if(SIZE != readByte)
    {
      throw new IOException("読込み長が足りませんでした。");
    }

    h = buffer;
  }

  public String year()
  {
    String str = new String(h, JnlHeaderInfo.YEAR.offset(), JnlHeaderInfo.YEAR.len());
    return str;
  }

  public String month()
  {
    String str = new String(h, JnlHeaderInfo.MONTH.offset(), JnlHeaderInfo.MONTH.len());
    return str;
  }

  public String day()
  {
    String str = new String(h, JnlHeaderInfo.DAY.offset(), JnlHeaderInfo.DAY.len());
    return str;
  }

  public String hour()
  {
    String str = new String(h, JnlHeaderInfo.HOUR.offset(), JnlHeaderInfo.HOUR.len());
    return str;
  }

  public String minute()
  {
    String str = new String(h, JnlHeaderInfo.MINUTE.offset(), JnlHeaderInfo.MINUTE.len());
    return str;
  }

  public String second()
  {
    String str = new String(h, JnlHeaderInfo.SECOND.offset(), JnlHeaderInfo.SECOND.len());
    return str;
  }

  public String msecond()
  {
    String str = new String(h, JnlHeaderInfo.MSECOND.offset(), JnlHeaderInfo.MSECOND.len());
    return str;
  }

  public String kind()
  {
    String str = new String(h, JnlHeaderInfo.KIND.offset(), JnlHeaderInfo.KIND.len());
    return str;
  }

  public String data()
  {
    String str = new String(h, JnlHeaderInfo.DATA.offset(), JnlHeaderInfo.DATA.len());
    return str;
  }

  public String date()
  {
    return year() + "/" + month() + "/" + day();
  }

  public String time()
  {
    return hour() + ":" + minute() + ":" + second() + "." + msecond();
  }

  public int dataLen()
  {
    return Integer.parseInt(data());
  }

  void print()
  {
    String str = new String(h, 0, SIZE);
    System.out.printf("%s\n", str);
  }
}

class JnlRecord
{
  JnlHeader h;
  byte[] data;

  public JnlRecord(FileInputStream in) throws IOException
  {
    h = new JnlHeader(in);
    data = new byte[h.dataLen()];

    int readByte = in.read(data);
    if(h.dataLen() != readByte)
    {
      throw new IOException("読込み長が足りませんでした。");
    }
  }
}

class Main
{
  public static void main(String[] args)
  {

    final int buffer_size = 20 * 1024 * 1024;
    byte[] buffer = new byte[buffer_size];

    try(FileInputStream in = new FileInputStream(args[0]))
    {
      System.out.println("ファイル(" + args[0] + ")を開きました。");

      JnlRecord jnlRecord = new JnlRecord(in);
      System.out.printf("date : %s, time : %s, dataLen : %d\n",
          jnlRecord.h.date(), jnlRecord.h.time(), jnlRecord.h.dataLen());
    }
    catch(IOException e)
    {
      e.printStackTrace();
    }
  }
}
