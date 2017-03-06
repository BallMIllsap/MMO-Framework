/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef PatternContext_SkyplaneH
#define PatternContext_SkyplaneH

#include "TypeDef.h"
#include "BehaviourPatternContext.h"

class DllExport TPatternContext_Skyplane : public TBehaviourPatternContext
{
public:
	TPatternContext_Skyplane(TBehaviourPatternModel* pModel);
	virtual ~TPatternContext_Skyplane();
};

#endif
