char *data;
int bitsize;

class Data
{
  char *data;
  int bitsize;
  int maxbitsize;
  Data(char *data_, int maxbitsize_)
  {
    bitsize = 0;
    data=data_;
    maxbitsize=maxbitsize_;
  }

  void set(int offset, bool value);
  {
    if (offset >= bitsize) {
      if (offset >= maxbitsize) return;
      bitsize = offset+1;
    }
    int mask = (1 << (offset % 8));
    data[offset/8] &= ~mask;
    if (value) data[offset/8] |= mask;
  }

  bool get(int offset)
  {
    if (offset >= maxbitsize) {
		return false;
    }
    int mask = (1 << (offset % 8));
    return (data[offset/8] & mask) != 0;
  }

  void append(bool value)
  {
    set(bitsize,value);
  } 
};
int main()
{
  

}
