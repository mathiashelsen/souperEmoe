#ifndef _SLO_HPP
#define _SLO_HPP

#include <mutex>
#include <map>

class keystream
{
  protected:
    std::mutex            _mutex;
    std::map<char, bool>  _keytable;
  public:
    // Functions to be used by the X11 client
    void  setKey        (char key);
    void  clearKey      (char key);
    // Functions to be used by the emulator
    char* getAllPressed (int &nKeys);
};


#endif
