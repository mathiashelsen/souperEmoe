#include "keystream.hpp"

void keystream::setKey(char key)
{
  _mutex.lock();

  _keytable.insert_or_assign(key, true);

  _mutex.unlock();
}

void keystream::clearKey(char key)
{
  _mutex.lock();

  _keytable.erase(key);

  _mutex.unlock();
}

char* keystream::getAllPressed(int& nKeys)
{
  _mutex.lock();

  nKeys = _keytable.size();
  char* retVal;

  if(nKeys)
  {
    retVal = new char[nKeys];
    int i  = 0;
    for(auto it = _keytable.begin(); it != _keytable.end(); ++it)
    {
      retVal[i] = it->first;
      i++;
    }
  }
  else
  {
    retVal = NULL;
  }

  _mutex.unlock();

  return retVal;
}
