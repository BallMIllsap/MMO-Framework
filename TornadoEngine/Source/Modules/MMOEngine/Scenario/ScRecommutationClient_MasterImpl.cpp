/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ScRecommutationClient_MasterImpl.h"
#include "ContextScRecommutationClient.h"
#include "ManagerSession.h"

using namespace nsMMOEngine;
using namespace nsRecommutationClientStruct;

TScRecommutationClient_MasterImpl::TScRecommutationClient_MasterImpl(IScenario* pSc):
TBaseScRecommutationClient(pSc)
{

}
//--------------------------------------------------------------------------------------
void TScRecommutationClient_MasterImpl::Work(unsigned int time_ms)
{

}
//--------------------------------------------------------------------------------------
void TScRecommutationClient_MasterImpl::RecvInherit(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->from)
  {
    case eSlaveDonor:
      RecvFromSlaveDonor(pDesc);
      break;
    case eSlaveRecipient:
      RecvFromSlaveRecipient(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------------------------------
void TScRecommutationClient_MasterImpl::RecvFromSlaveDonor(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->subType)
  {
    case eCheckBeginDonor:
      CheckBeginDonor(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------------------------------
void TScRecommutationClient_MasterImpl::RecvFromSlaveRecipient(TDescRecvSession* pDesc)
{
  THeader* pHeader = (THeader*)pDesc->data;
  switch(pHeader->subType)
  {
    case eCheckBeginRecipient:
      CheckBeginRecipient(pDesc);
      break;
    case eClientConnect:
      ClientConnect(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//--------------------------------------------------------------------------------------
void TScRecommutationClient_MasterImpl::Start(unsigned int id_session_recipient,
                                              unsigned int id_client)
{
  Context()->SetClientKey(id_client);
	Context()->SetSessionRecipient(id_session_recipient);
	if(Begin()==false)
	{
		return;
	}
	SendFirstPacket();
}
//--------------------------------------------------------------
void TScRecommutationClient_MasterImpl::DelayBegin()
{
	SendFirstPacket();
}
//--------------------------------------------------------------
void TScRecommutationClient_MasterImpl::SendFirstPacket()
{
	// откуда уходим?
  NeedSessionDonorByClientKey(mScenario);
  // сценарий активен
  EventActivate();
  // проверка на совпадение	Донора с Реципиентом
  if(Context()->GetSessionDonor()==Context()->GetSessionRecipient())
  {
    End();
    return;
  }
  
  TBreakPacket bp;
  THeaderBeginDonor h;
  h.id_client = Context()->GetClientKey();
  bp.PushFront((char*)&h, sizeof(h));
  Context()->GetMS()->Send(Context()->GetSessionDonor(), bp);
}
//--------------------------------------------------------------
void TScRecommutationClient_MasterImpl::CheckBeginDonor(TDescRecvSession* pDesc)
{
  THeaderCheckBeginDonor* pHeader = (THeaderCheckBeginDonor*)pDesc->data;
  NeedContextByClientKey(pHeader->id_client);
  if(Context()==NULL)
  {
    return;
  }
  // запомнить число, которое знаю только участники перекоммутации
  TDescRequestConnectForRecipient privateNum;
  privateNum.Generate();
  Context()->SetRandomNum(privateNum.random_num);

  TBreakPacket bp;
  // поместить контекст Донора в пакет
  bp.PushFront(pDesc->data + sizeof(THeaderCheckBeginDonor), 
               pDesc->sizeData - sizeof(THeaderCheckBeginDonor));

  THeaderBeginRecipient h;
  h.id_client   = Context()->GetClientKey();
  h.random_num  = privateNum.random_num;
  bp.PushFront((char*)&h, sizeof(h));

  Context()->GetMS()->Send(Context()->GetSessionRecipient(), bp);
}
//--------------------------------------------------------------
void TScRecommutationClient_MasterImpl::CheckBeginRecipient(TDescRecvSession* pDesc)
{
  THeaderCheckBeginRecipient* pHeader = (THeaderCheckBeginRecipient*)pDesc->data;
  NeedContextByClientKey(pHeader->id_client);
  if(Context()==NULL)
  {
    return;
  }
  //==============================================
  // узнать IP и порт Реципиента
  TIP_Port ip_port_recipient;
  bool resIP_recipient = 
    Context()->GetMS()->GetInfo(Context()->GetSessionRecipient(), ip_port_recipient);
  if(resIP_recipient==false)
  {
    End();
    BL_FIX_BUG();
    return;
  }
  //==============================================
  TBreakPacket bp;
  THeaderInfoRecipientToDonor h;
  h.id_client         = Context()->GetClientKey();
  h.random_num        = Context()->GetRandomNum();
  h.ip_port_recipient = ip_port_recipient;
  bp.PushFront((char*)&h, sizeof(h));

  Context()->GetMS()->Send(Context()->GetSessionDonor(), bp);
}
//--------------------------------------------------------------
void TScRecommutationClient_MasterImpl::ClientConnect(TDescRecvSession* pDesc)
{
  THeaderClientConnect* pHeader = (THeaderClientConnect*)pDesc->data;
  NeedContextByClientKey(pHeader->id_client);
  if(Context()==NULL)
    return;

  End();
}
//--------------------------------------------------------------
void TScRecommutationClient_MasterImpl::DisconnectClient()
{
  // уведомить Реципиента о потери связи с Клиентом
  TBreakPacket bp;
  THeaderDisconnectClient h;
  h.id_client = Context()->GetClientKey();
  bp.PushFront((char*)&h,  sizeof(h));
  Context()->GetMS()->Send(Context()->GetSessionRecipient(), bp);
  //Нельзя вызывать End();
}
//--------------------------------------------------------------

