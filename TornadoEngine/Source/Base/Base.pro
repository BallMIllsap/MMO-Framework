TEMPLATE = lib
LANGUAGE = C++

CONFIG += dll qt warn_on

TARGET = /usr/lib/Base_d

INCLUDEPATH = \
. \
./Headers \
./Sources \

DEFINES += _DEBUG
DEFINES += _USRDLL
DEFINES += _CRT_SECURE_NO_WARNINGS
DEFINES += MARKUP_STL
DEFINES += USE_MATH_TOOLS

COMPILER_FLAGS = -fPIC

OBJECTS_DIR = ../../Temp/Debug/Share

LIBS += -lssl
LIBS += -lcrypto
LIBS += -ldl
LIBS += -lboost_filesystem
LIBS += -lboost_system
LIBS += -lboost_thread
LIBS += -lboost_chrono
LIBS += -lboost_locale


SOURCES = \
./Sources/ArrContainer.cpp \
./Sources/BL_Debug.cpp \
./Sources/BreakPacket.cpp \
./Sources/CmdParam.cpp \
./Sources/ConverterLocale.cpp \
./Sources/ContainerRise.cpp \
./Sources/CRC8.cpp \
./Sources/CryptoAES_Impl.cpp \
./Sources/CryptoRSA_Impl.cpp \
./Sources/DescCallBack.cpp \
./Sources/DstEvent.cpp \
./Sources/FileOperation.cpp \
./Sources/HiTimer.cpp \
./Sources/InfoFile.cpp \
./Sources/IXML.cpp \
./Sources/LoaderDLL_Unix.cpp \
./Sources/LoaderDLL_Win32.cpp \
./Sources/LoadFromFile.cpp \
./Sources/Logger.cpp \
./Sources/MakerLoaderDLL.cpp \
./Sources/MakerXML.cpp \
./Sources/ManagerForm.cpp \
./Sources/ManagerStateMachine.cpp \
./Sources/ManagerTime.cpp \
./Sources/MapCallBack.cpp \
./Sources/MapXML_Field.cpp \
./Sources/Markup.cpp \
./Sources/MathTools.cpp \
./Sources/MD5.cpp \
./Sources/Mutex.cpp \
./Sources/Only_N_Object.cpp \
./Sources/ResolverSelf_IP_v4.cpp \
./Sources/SaveToFile.cpp \
./Sources/SetOrderElement.cpp \
./Sources/SHA256.cpp \
./Sources/ShareLibWin.cpp \
./Sources/ShareMisc.cpp \
./Sources/SrcEvent.cpp \
./Sources/StorePathResources.cpp \
./Sources/ThreadBoost.cpp \
./Sources/XML_Markup.cpp


HEADERS = \
./Headers/ArrContainer.h\
./Headers/BL_Debug.h \
./Headers/BreakPacket.h \
./Headers/CallBackRegistrator.h \
./Headers/CmdParam.h \
./Headers/ConverterLocale.h \
./Headers/ContainerArrObj.h \
./Headers/ContainerPtrArrObj.h \
./Headers/ContainerRise.h \
./Headers/ContainerTypes.h \
./Headers/CRC8.h \
./Headers/CryptoAES_Impl.h \
./Headers/CryptoRSA_Impl.h \
./Headers/DataExchange2Thread.h \
./Headers/DescCallBack.h \
./Headers/DescEvent.h \
./Headers/DstEvent.h \
./Headers/FileOperation.h \
./Headers/HiTimer.h \
./Headers/IContainer.h \
./Headers/ILoaderDLL.h \
./Headers/InfoFile.h \
./Headers/IXML.h \
./Headers/License.h \
./Headers/ListPtr.h \
./Headers/LoaderDLL_Unix.h \
./Headers/LoaderDLL_Win32.h \
./Headers/LoadFromFile.h \
./Headers/LockFreeDef.h \
./Headers/Logger.h \
./Headers/MacroMaker.h \
./Headers/MacroMaker_Prepare.h \
./Headers/MakerLoaderDLL.h \
./Headers/MakerXML.h \
./Headers/ManagerForm.h \
./Headers/ManagerStateMachine.h \
./Headers/ManagerTime.h \
./Headers/MapCallBack.h \
./Headers/MapXML_Field.h \
./Headers/Markup.h \
./Headers/MathTools.h \
./Headers/MD5.h \
./Headers/Mutex.h \
./Headers/Only_N_Object.h \
./Headers/ResolverSelf_IP_v4.h \
./Headers/SaveToFile.h \
./Headers/SetOrderElement.h \
./Headers/SHA256.h \
./Headers/ShareMisc.h \
./Headers/SrcEvent_ex.h \
./Headers/SrcEvent.h \
./Headers/StateMachine.h \
./Headers/StatisticValue.h \
./Headers/StorePathResources.h \
./Headers/TypeDef.h \
./Headers/ThreadBoost.h \
./Headers/XML_Markup.h
