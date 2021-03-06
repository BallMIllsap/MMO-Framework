/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef EditorModelH
#define EditorModelH

#include "BaseGUI.h"
#include <MyGUI.h>
#include <Ogre.h>

#include <boost/smart_ptr/scoped_ptr.hpp>

class TEditorModel : public TBaseGUI
{
public:
  TEditorModel();
  virtual ~TEditorModel();

protected:
  virtual void Activate();
  virtual void* GetParent();
  virtual const char* GetNameLayout();
  virtual void SetupTabChild();

  virtual void KeyEvent(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char);
protected:
};

#endif 
