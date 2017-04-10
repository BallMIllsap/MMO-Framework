/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "PatternModel_Model.h"

#include <boost/foreach.hpp>
#include <OgreRoot.h>
#include <OgreEntity.h>

#include "ModuleLogic.h"
#include "ModuleGraphicEngine.h"
#include "FactoryGameItem.h"

#include "ShapeItem.h"

#include <btBulletDynamicsCommon.h>

#include "ModulePhysicEngine.h"
#include "Dynamics/btDiscreteDynamicsWorld.h"

namespace nsPatternModel_Model
{
  const char* sNameGameItem         = "NameGameItem";
  const char* sVariantPatternConfig = "VariantPatternConfig";
  const char* sMobility             = "Mobility";

  static TPatternConfigItem::TMapStrStr g_DefaultParameterMap;
}

using namespace nsPatternModel_Model;

TPatternModel_Model::TPatternModel_Model()
{
  g_DefaultParameterMap.insert(TPatternConfigItem::TMapStrStrVT(sNameGameItem,""));
  g_DefaultParameterMap.insert(TPatternConfigItem::TMapStrStrVT(sVariantPatternConfig,""));
  g_DefaultParameterMap.insert(TPatternConfigItem::TMapStrStrVT(sMobility,""));
}
//---------------------------------------------------------------------------
TPatternModel_Model::~TPatternModel_Model()
{

}
//---------------------------------------------------------------------------
TBehaviourPatternContext* TPatternModel_Model::MakeNewConext()
{ 
  return new TPatternContext_Model(this);
}
//---------------------------------------------------------------------------
const TPatternConfigItem::TMapStrStr* TPatternModel_Model::GetDefaultParameterMap()
{
  return &g_DefaultParameterMap;
}
//---------------------------------------------------------------------------
bool TPatternModel_Model::SetParameterFromPattern(TContainer c)
{
  return true;
}
//---------------------------------------------------------------------------
TContainer TPatternModel_Model::GetParameterToPattern()
{
  TContainer c;
  return c;
}
//---------------------------------------------------------------------------
void TPatternModel_Model::SetPosition(nsMathTools::TVector3& v)
{
  // подготовить задание для физики
  nsMathTools::TVector3* pV = new nsMathTools::TVector3(v);
  mPipePositionLogic2Bullet.Add(pV);
}
//---------------------------------------------------------------------------
bool TPatternModel_Model::GetPosition(nsMathTools::TVector3& v)
{
  return GetFromPipe(v, &mPipePositionLogic2Bullet);
}
//---------------------------------------------------------------------------
void TPatternModel_Model::SetOrientation(nsMathTools::TVector3& v)
{
  // подготовить задание для физики
  nsMathTools::TVector3* pV = new nsMathTools::TVector3(v);
  mPipeOrientationLogic2Bullet.Add(pV);
}
//---------------------------------------------------------------------------
bool TPatternModel_Model::GetOrientation(nsMathTools::TVector3& v)
{
  return GetFromPipe(v, &mPipeOrientationLogic2Bullet);
}
//---------------------------------------------------------------------------
bool TPatternModel_Model::GetFromPipe(nsMathTools::TVector3& v, TDataExchange2Thread<nsMathTools::TVector3>* pPipe)
{
  nsMathTools::TVector3** ppV = pPipe->GetFirst();
  if( ppV==NULL )
    return false;
  while(1)
  {
    v = *(*ppV);
    pPipe->RemoveFirst();
    ppV = pPipe->GetFirst();
    if( ppV==NULL )
      return true;
  }
  return false;
}
//---------------------------------------------------------------------------
bool TPatternModel_Model::LoadFromParameterMap()
{
  return true;
}
//---------------------------------------------------------------------------
bool TPatternModel_Model::UpdateFromGameItem(TBaseItem* pBI)
{
  return true;
}
//---------------------------------------------------------------------------
bool TPatternModel_Model::Unload()
{
  return true;
}
//---------------------------------------------------------------------------
void TPatternModel_Model::LoadByModule_Logic()
{
  TPatternContext_Model* pContextModel = (TPatternContext_Model*)mContext;
	const TPatternConfigItem::TMapStrStr* pMap = pContextModel->GetParameterMap();
	// найти имя модели
  std::string nameGameItem = pContextModel->GetNameGameItem();
  if( nameGameItem.length()==0 )
  {
    TPatternConfigItem::TMapStrStrConstIt itNameGameItem = pMap->find(sNameGameItem);
    if( itNameGameItem==pContextModel->GetParameterMap()->end() )
      return;
    nameGameItem = itNameGameItem->second;
    pContextModel->SetNameGameItem(nameGameItem);
  }
  // найти вариант настроек паттерна
  std::string variantPatternConfig = pContextModel->GetNameVariantPatternConfig();
  if( variantPatternConfig.length()==0 )
  {
    TPatternConfigItem::TMapStrStrConstIt itVariant = pMap->find(sVariantPatternConfig);
    if( itVariant!=pContextModel->GetParameterMap()->end() )// если нет настройки, ничего не делать
    {
      variantPatternConfig = itVariant->second;
      pContextModel->SetNameVariantPatternConfig(variantPatternConfig);
    }
  }
  // найти будет ли модель мобильной
  TPatternConfigItem::TMapStrStrConstIt itMobility = pMap->find(sMobility);
  if( itMobility!=pContextModel->GetParameterMap()->end() )// если нет, то ничего не делать
  {
    bool mobility = itMobility->second=="true" ? true : false;
    pContextModel->SetMobility(mobility);
  }
  TFactoryGameItem* pFGI = TModuleLogic::Get()->GetFGI();

  TModelItem* pModel = (TModelItem*)pFGI->Get(TFactoryGameItem::Model, nameGameItem);
  if( pModel==NULL )
    return;
  
  TPatternConfigItem* pPattern = (TPatternConfigItem*)pFGI->Get(TFactoryGameItem::PatternConfig, GetName());
  if( pPattern )
  {
    TPatternConfigItem::TMapStrMapIt itMapVariant = pPattern->mMapVariant.find(variantPatternConfig);
    if( itMapVariant!=pPattern->mMapVariant.end() )
      pContextModel->SetMapVariant(itMapVariant->second);// использовать в конце загрузки в каждом из потоков (графика,звук,физика)
  }

  // задача: создать модели по имени. При синхронизации менять положение и ориентацию форм или моделей
  pContextModel->SetTypeContent(pModel->mTypeCollection);
  if( pModel->mTypeCollection==TModelItem::eModel )
    LoadModelsFromThread_Logic(pModel->mMapNamePart);
  else
    LoadShapesFromThread_Logic(pModel->mMapNamePart);
}
//---------------------------------------------------------------------------
bool TPatternModel_Model::LoadByModule_Graphic(bool fast)
{
  TPatternContext_Model* pContextModel = (TPatternContext_Model*)mContext;
  int cntPart = pContextModel->GetCountPart();
  for( int iPart = 0 ; iPart < cntPart ; iPart++ )
  {
    std::string namePart = pContextModel->GetNamePart(iPart);
    int cntVariant = pContextModel->GetCountVariant(namePart);
    for( int iVariant = 0 ; iVariant < cntVariant ; iVariant++ )
    {
      std::string nameVariant = pContextModel->GetNameVariant(namePart, iVariant);
      TPatternContext_Model::TBaseDesc* pDesc = pContextModel->GetDesc(namePart, nameVariant);
      if( pDesc==NULL )
        continue;
      if( pDesc->type==TModelItem::eModel )
      {
        TPatternContext_Model::TModelDesc* pModelDesc = (TPatternContext_Model::TModelDesc*)pDesc;
        pModelDesc->pModel->LoadByModule_Graphic();
      }
      else
      {
        TPatternContext_Model::TShapeDesc* pShapeDesc = (TPatternContext_Model::TShapeDesc*)pDesc;
        LoadShapeFromThread_Ogre(pShapeDesc);
      }
    }
  }
	PostLoadFromThread_Ogre();
	return true;
}
//---------------------------------------------------------------------------
bool TPatternModel_Model::LoadByModule_Physic( bool fast )
{
	TPatternContext_Model* pContextModel = (TPatternContext_Model*)mContext;
	int cntPart = pContextModel->GetCountPart();
	for( int iPart = 0 ; iPart < cntPart ; iPart++ )
	{
		std::string namePart = pContextModel->GetNamePart(iPart);
		int cntVariant = pContextModel->GetCountVariant(namePart);
		for( int iVariant = 0 ; iVariant < cntVariant ; iVariant++ )
		{
			std::string nameVariant = pContextModel->GetNameVariant(namePart, iVariant);
			TPatternContext_Model::TBaseDesc* pDesc = pContextModel->GetDesc(namePart, nameVariant);
			if( pDesc==NULL )
				continue;
			if( pDesc->type==TModelItem::eModel )
			{
				TPatternContext_Model::TModelDesc* pModelDesc = (TPatternContext_Model::TModelDesc*)pDesc;
				pModelDesc->pModel->LoadByModule_Physic();
			}
			else
			{
				TPatternContext_Model::TShapeDesc* pShapeDesc = (TPatternContext_Model::TShapeDesc*)pDesc;
				LoadShapeFromThread_Bullet(pShapeDesc);
			}
		}
	}
	PostLoadFromThread_Bullet();
	return true;
}
//---------------------------------------------------------------------------
bool TPatternModel_Model::LoadByModule_Sound( bool fast )
{
	return true;
}
//---------------------------------------------------------------------------
void TPatternModel_Model::UnloadByModule_Logic()
{

}
//---------------------------------------------------------------------------
bool TPatternModel_Model::UnloadByModule_Graphic( bool fast )
{
	return true;
}
//---------------------------------------------------------------------------
bool TPatternModel_Model::UnloadByModule_Physic( bool fast )
{
	return true;
}
//---------------------------------------------------------------------------
bool TPatternModel_Model::UnloadByModule_Sound( bool fast )
{
	return true;
}
//---------------------------------------------------------------------------
void TPatternModel_Model::SynchroByModule_Logic()
{

}
//---------------------------------------------------------------------------
void TPatternModel_Model::SynchroByModule_Graphic()
{
	TPatternContext_Model* pContextModel = (TPatternContext_Model*)mContext;
	// синхронизируем всё!
	int cntPart = pContextModel->GetCountPart();
	for( int iPart = 0 ; iPart < cntPart ; iPart++ )
	{
		std::string namePart = pContextModel->GetNamePart(iPart);
		int cntVariant = pContextModel->GetCountVariant(namePart);
		for( int iVariant = 0 ; iVariant < cntVariant ; iVariant++ )
		{
			std::string nameVariant = pContextModel->GetNameVariant(namePart, iVariant);
			TPatternContext_Model::TBaseDesc* pDesc = pContextModel->GetDesc(namePart, nameVariant);
			if( pDesc==NULL )
				continue;
			if( pDesc->type==TModelItem::eModel )
			{
				TPatternContext_Model::TModelDesc* pDescModel = 
					(TPatternContext_Model::TModelDesc*)pDesc;
				pDescModel->pModel->SynchroByModule_Graphic();
			}
			else
			{
				TPatternContext_Model::TShapeDesc* pShapeDesc = 
					(TPatternContext_Model::TShapeDesc*)pDesc;

				btMotionState* pMS = pShapeDesc->pRigidBody->getMotionState();
				btTransform trans;
				pMS->getWorldTransform(trans);
				btVector3& posBullet = trans.getOrigin();
				
				Ogre::Entity* pEntity = pShapeDesc->pEntity;
	
				Ogre::Vector3 posOgre;
				posOgre.x = posBullet.x();
				posOgre.y = posBullet.y();
				posOgre.z = posBullet.z();
				pEntity->getParentSceneNode()->setPosition(posOgre);

				btQuaternion quat = trans.getRotation();
				Ogre::Real w = quat.w(); 
				Ogre::Real x = quat.x(); 
				Ogre::Real y = quat.y();
				Ogre::Real z = quat.z();
				pEntity->getParentSceneNode()->setOrientation( w, x, y, z);
			}
		}
	}
}
//---------------------------------------------------------------------------
void TPatternModel_Model::SynchroByModule_Physic()
{
  // проверка на изменение позиции и ориентации
	TPatternContext_Model* pContextModel = (TPatternContext_Model*)mContext;
	int cntPart = pContextModel->GetCountPart();
	for( int iPart = 0 ; iPart < cntPart ; iPart++ )
	{
		std::string namePart = pContextModel->GetNamePart(iPart);
		int cntVariant = pContextModel->GetCountVariant(namePart);
		for( int iVariant = 0 ; iVariant < cntVariant ; iVariant++ )
		{
			std::string nameVariant = pContextModel->GetNameVariant(namePart, iVariant);
			TPatternContext_Model::TBaseDesc* pDesc = pContextModel->GetDesc(namePart, nameVariant);
			if( pDesc==NULL )
				continue;
			if( pDesc->type==TModelItem::eShape )
			{
				TPatternContext_Model::TShapeDesc* pShapeDesc = 
					(TPatternContext_Model::TShapeDesc*)pDesc;

				const btVector3& velocity = pShapeDesc->pRigidBody->getLinearVelocity();
				float speed = velocity.length();
				if( speed > 15.0f )
				{
					int a = 0;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void TPatternModel_Model::SynchroByModule_Sound()
{

}
//---------------------------------------------------------------------------
void TPatternModel_Model::LoadModelsFromThread_Logic(TModelItem::TMapStrPart& mapNamePart)
{
	TPatternContext_Model* pContextModel = (TPatternContext_Model*)mContext;

  TFactoryBehaviourPatternModel* pFBPM = TModuleLogic::Get()->GetFBPM();
  TFactoryGameItem* pFGI = TModuleLogic::Get()->GetFGI();
  // создать другие модели и сохранить в контексте
  BOOST_FOREACH( TModelItem::TMapStrPartVT& vtPart, mapNamePart )
  {
    std::string namePart = vtPart.first;
    BOOST_FOREACH( TModelItem::TVariant& variant, vtPart.second.vecVariant )
    {
      TModelItem* pModel = (TModelItem*)pFGI->Get( TFactoryGameItem::Model, variant.nameItem);
      if( pModel==NULL )
        continue;
      TBehaviourPatternModel* pBPM = pFBPM->GetPatternByName(pModel->mNamePattern);
      if( pBPM==NULL )
        continue;

      TPatternContext_Model::TModelDesc* pModelDesc = new TPatternContext_Model::TModelDesc;
      pModelDesc->namePart    = namePart;
      pModelDesc->nameVariant = variant.name;
			pModelDesc->pModel = (TPatternModel_Model*)pBPM;
			TPatternContext_Model* pContext = (TPatternContext_Model*)pModelDesc->pModel->GetContext();
      //pModelDesc->pCtxModel = (TPatternContext_Model*)pBPM->MakeNewConext();
      pContext->SetNameGameItem(variant.nameItem);
      pContext->SetMobility(pContextModel->GetMobility());// мобильность наследуется
      pContextModel->AddDesc(pModelDesc);

      pBPM->LoadByModule_Logic();// дальше по итерации
    }
  }
}
//---------------------------------------------------------------------------
void TPatternModel_Model::LoadShapesFromThread_Logic(TModelItem::TMapStrPart& mapNamePart)
{
	TPatternContext_Model* pContextModel = (TPatternContext_Model*)mContext;

  TFactoryGameItem* pFGI = TModuleLogic::Get()->GetFGI();
  
  BOOST_FOREACH( TModelItem::TMapStrPartVT& vtPart, mapNamePart )
  {
    std::string namePart = vtPart.first;
    BOOST_FOREACH( TModelItem::TVariant& variant, vtPart.second.vecVariant )
    {
      // передача данных от Логики к Физике и Графике для каждого потока ( создание формы )
      TShapeItem* pShapeItem = (TShapeItem*)pFGI->Get( TFactoryGameItem::Shape, variant.nameItem);
      if( pShapeItem==NULL )
        continue;

      TPatternContext_Model::TShapeDesc* pShape = new TPatternContext_Model::TShapeDesc;
      pShape->namePart      = namePart;
      pShape->nameVariant   = variant.name;
      pShape->nameShapeItem = variant.nameItem;
      if( variant.redefinitionMaterial.length() )
        pShape->nameMaterial = variant.redefinitionMaterial;
      else
        pShape->nameMaterial = pShapeItem->mNameMaterial;
      pContextModel->AddDesc(pShape);
    }
  }
}
//---------------------------------------------------------------------------
void TPatternModel_Model::LoadShapeFromThread_Ogre(TPatternContext_Model::TShapeDesc* pShapeDesc)
{
	TPatternContext_Model* pContextModel = (TPatternContext_Model*)mContext;
  TFactoryGameItem* pFGI = TModuleLogic::Get()->GetFGI();
  TShapeItem* pShapeItem = (TShapeItem*)pFGI->Get(TFactoryGameItem::Shape,pShapeDesc->nameShapeItem);
  if( pShapeItem==NULL )
    return;

  TBuilder_Ogre* pBOgre = GetBuilderOgre();

  Ogre::Entity* pEntity = pBOgre->GetShapeMaker()->Build( pShapeItem );
	pShapeDesc->pEntity = pEntity;

	//### TODO убрать, всё позиционирование производится после загрузки всех форм (PostLoad)
	// сделано временно для визуализации (отладка)
	nsMathTools::TVector3 pos;
	pContextModel->GetPosition(pos);
	Ogre::Vector3 vPos(pos.x, pos.y, pos.z);
	pEntity->getParentSceneNode()->setPosition(vPos);
	pEntity->setCastShadows(true);

	nsMathTools::TVector4 orient;
	pContextModel->GetOrientation(orient);
	pEntity->getParentSceneNode()->setOrientation(orient.w, orient.x, orient.y, orient.z);
	//###
}
//---------------------------------------------------------------------------
void TPatternModel_Model::LoadShapeFromThread_Bullet(TPatternContext_Model::TShapeDesc* pShapeDesc)
{
	TPatternContext_Model* pContextModel = (TPatternContext_Model*)mContext;

	TFactoryGameItem* pFGI = TModuleLogic::Get()->GetFGI();
	TShapeItem* pShapeItem = (TShapeItem*)pFGI->Get(TFactoryGameItem::Shape,pShapeDesc->nameShapeItem);
	if( pShapeItem==NULL )
		return;

	int id_world = pContextModel->GetPhysicWorld();

	TBuilder_Bullet* pBBullet = GetBuilderBullet();
	btRigidBody* pRB = pBBullet->GetShapeMaker()->Build( id_world, pShapeItem );
	pShapeDesc->pRigidBody = pRB;

	//### TODO убрать, всё позиционирование производится после загрузки всех форм (PostLoad)
	// сделано временно для визуализации (отладка)
	nsMathTools::TVector3 pos;
	pContextModel->GetPosition(pos);
	btTransform& trans = pShapeDesc->pRigidBody->getWorldTransform();
	btVector3& posBullet = trans.getOrigin();
	posBullet.setX(pos.x);
	posBullet.setY(pos.y);
	posBullet.setZ(pos.z);

	nsMathTools::TVector4 orient;
	pContextModel->GetOrientation(orient);
	btQuaternion quat;
	quat.setX(orient.x);
	quat.setY(orient.y);
	quat.setZ(orient.z);
	quat.setW(orient.w);
	trans.setRotation(quat);

	pShapeDesc->pRigidBody->setWorldTransform(trans);

	pShapeDesc->pRigidBody->setLinearVelocity(btVector3(0,-10,0));
	//pShapeDesc->pRigidBody->setAngularVelocity(btVector3(0,10,0));

	//btDiscreteDynamicsWorld* pWorld = TModuleLogic::Get()->GetC()->pPhysicEngine->GetPE()->GetWorld(id_world);
	//###
}
//---------------------------------------------------------------------------
void TPatternModel_Model::PostLoadFromThread_Ogre()
{

}
//---------------------------------------------------------------------------
void TPatternModel_Model::PostLoadFromThread_Bullet()
{

}
//---------------------------------------------------------------------------
