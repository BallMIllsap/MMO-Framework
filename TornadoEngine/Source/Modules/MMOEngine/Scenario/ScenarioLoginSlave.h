/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef SCENARIO_LOGIN_SLAVE_H
#define SCENARIO_LOGIN_SLAVE_H

#include "IScenario.h"
#include "ContextScLoginSlave.h"
#include "MakerScenario.h"

#ifdef WIN32
#pragma pack(push, 1)
#endif

namespace nsMMOEngine
{
  class TScenarioLoginSlave : public IScenario
  {
    enum{eFromSlave,eAnswerMaster,};
    struct THeaderLoginSlave : public IScenario::TBaseHeader
    {
      THeaderLoginSlave(){type=TMakerScenario::eLoginSlave;} 
    }_PACKED;
    //-------------------------------------------------
    struct THeaderFromSlave : public THeaderLoginSlave
    {
      THeaderFromSlave(){subType=eFromSlave;}
    }_PACKED;
    //-------------------------------------------------
    struct THeaderAnswerMaster : public THeaderLoginSlave
    {
      THeaderAnswerMaster(){subType=eAnswerMaster;}
    }_PACKED;
    //-------------------------------------------------
    enum{
      eTimeoutWait_ms = 20000,
    };

  public:
    TScenarioLoginSlave();

    virtual void Recv(TDescRecvSession* pDesc);

    void ConnectToMaster( unsigned int ip, unsigned short port, 
                          void* pLogin, int sizeLogin, void* pPassword, int sizePassword,
                          unsigned char subNet);
  protected:
    virtual void Work();

    void RecvFromMaster(TDescRecvSession* pDesc);
    void RecvFromSlave(TDescRecvSession* pDesc);
  private:
    TContextScLoginSlave* Context(){return (TContextScLoginSlave*)mCurContext;}
  };
}

#ifdef WIN32
#pragma pack(pop)
#endif

#endif