/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "Logger.h"
#include <boost/foreach.hpp>

using namespace std;

class TLoggerOneObject : public TLogger
{
};
//-----------------------------------------------------------------------
TLoggerOneObject g_Logger;
TLogger* GetLogger()
{
  return (TLogger*)&g_Logger;
}
//-----------------------------------------------------------------------
TSaveToFile* GetLogger(const char* nameLog)
{
  return g_Logger.Get(nameLog);
}
//-----------------------------------------------------------------------
void TLogger::InitLogger(TSaveToFile* saver, const char* sName, const char* extension)
{
  if(saver->IsOpen()) return;
  char nameLogFile[260];
  sprintf(nameLogFile,".\\%s%s.%s",sName,sPrefix.data(),extension);
  saver->ReOpen(nameLogFile);
}
//-----------------------------------------------------------------------
bool TLogger::Register(const char* nameLogger, const char* extension)
{
  if(mMapNamePtr.find(nameLogger)!=mMapNamePtr.end())
    return false;

  TDescFile* pDF = new TDescFile;
  pDF->stf.SetPrintf(flgPrintf);
  pDF->stf.SetEnable(flgEnable);
  pDF->stf.SetBufferization(flgBuffer);
  pDF->sExtension = extension;

  mMapNamePtr.insert(TMapStrPtr::value_type(nameLogger,pDF));
	mVecPtr.push_back(&pDF->stf);
  if(sPrefix.length())
  {
    InitLogger(&pDF->stf, nameLogger, pDF->sExtension.data());
  }
  return true;
}
//-----------------------------------------------------------------------
void TLogger::Init(char* prefix)
{
  sPrefix = prefix;
  
  BOOST_FOREACH( TMapStrPtr::value_type& bit, mMapNamePtr )
    InitLogger(&bit.second->stf, bit.first.data(), bit.second->sExtension.data());
}
//-----------------------------------------------------------------------
TSaveToFile* TLogger::Get(const char* nameLog)
{
  TMapStrPtr::iterator fit = mMapNamePtr.find(nameLog);
  if(mMapNamePtr.end()!=fit)
    return &fit->second->stf;

  return NULL;
}
//-----------------------------------------------------------------------
void TLogger::Done()
{
  BOOST_FOREACH(TMapStrPtr::value_type& bit,mMapNamePtr)
    delete bit.second;
  mMapNamePtr.clear();
	mVecPtr.clear();
}
//-----------------------------------------------------------------------
TLogger::~TLogger()
{
  Done();
}
//-----------------------------------------------------------------------
void TLogger::SetPrintf(bool val)
{
  if(flgPrintf==val)
    return;
  
  flgPrintf = val;
  BOOST_FOREACH(TMapStrPtr::value_type &bit,mMapNamePtr)
    bit.second->stf.SetPrintf(flgPrintf);
}
//-----------------------------------------------------------------------
bool TLogger::GetPrintf()
{
  return flgPrintf;
}
//-----------------------------------------------------------------------
void TLogger::SetEnable(bool val)
{
  if(flgEnable==val)
    return;

  flgEnable = val;
  BOOST_FOREACH(TMapStrPtr::value_type &bit,mMapNamePtr)
    bit.second->stf.SetEnable(flgEnable);
}
//-----------------------------------------------------------------------
bool TLogger::GetEnable()
{
  return flgEnable;
}
//-----------------------------------------------------------------------
void TLogger::SetBufferization(bool val)
{
  if(flgBuffer==val)
    return;
  
  flgBuffer = val;
  BOOST_FOREACH(TMapStrPtr::value_type &bit,mMapNamePtr)
    bit.second->stf.SetBufferization(flgBuffer);
}
//-----------------------------------------------------------------------
bool TLogger::GetBufferization()
{
  return flgBuffer;
}
//-----------------------------------------------------------------------
int TLogger::GetCount()
{
	return mVecPtr.size();
}
//-----------------------------------------------------------------------
TSaveToFile* TLogger::GetByIndex( int index )
{
	if((index >= GetCount()) ||
		 (index < 0          ))
		return NULL;
	return mVecPtr[index];
}
//-----------------------------------------------------------------------
