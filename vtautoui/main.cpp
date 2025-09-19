#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main(array<System::String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    vtautoui::MainForm^ form = gcnew vtautoui::MainForm();
    Application::Run(form);

    return 0;
}
