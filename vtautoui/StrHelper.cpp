#include "StrHelper.h"
#include <msclr/marshal_cppstd.h>

std::string VTAutoStrHelper::ToStdStr(System::String^ managedStr)
{
    return msclr::interop::marshal_as<std::string>(managedStr);
}

System::String^ VTAutoStrHelper::ToSysStr(const std::string& nativeStr)
{
    return msclr::interop::marshal_as<System::String^>(nativeStr);
}

System::String^ VTAutoStrHelper::ToSysStr(const char* cStr)
{
    return msclr::interop::marshal_as<System::String^>(cStr);
}