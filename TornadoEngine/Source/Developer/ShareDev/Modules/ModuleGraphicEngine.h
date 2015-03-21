/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef ModuleGraphicEngineH
#define ModuleGraphicEngineH

#include <boost/smart_ptr/scoped_ptr.hpp>

#include "ModuleDev.h"

class TGraphicEngine_OGRE_MyGUI;

class DllExport TModuleGraphicEngine : public TModuleDev
{
	boost::scoped_ptr<TGraphicEngine_OGRE_MyGUI> mGE;
public:
  TModuleGraphicEngine();
  virtual ~TModuleGraphicEngine();

  virtual void StartEvent();
  virtual bool Work();
  virtual void StopEvent();

  TGraphicEngine_OGRE_MyGUI* GetGE();
protected:
};

#endif