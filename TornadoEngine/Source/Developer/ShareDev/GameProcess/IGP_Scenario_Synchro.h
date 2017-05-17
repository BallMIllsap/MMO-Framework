/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef IGP_Scenario_SynchroH
#define IGP_Scenario_SynchroH

#include "TypeDef.h"
#include "MapItem.h"

class TGameObject;

class DllExport IGP_Scenario_Synchro
{
public:
  IGP_Scenario_Synchro();
  virtual ~IGP_Scenario_Synchro();

	// atomic operation, результат сразу после вызова методов
	virtual TGameObject* AddGameObject(TMapItem::TObject* pObject) = 0;
	// любая сущность сцены может быть выражена с помощью ресурсов (частный случай - игровой итэм)
	virtual void UpdateResourcesByGameObject(int id) = 0;
	virtual void UpdateGameObjectByResources(int id) = 0;

	virtual bool DeleteGameObject(int id) = 0;

};

#endif