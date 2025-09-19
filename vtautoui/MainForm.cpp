#include "MainForm.h"
#include <Windows.h>
#include "Npc.h"
#include <vector>
#include <unordered_map>
#include <msclr/marshal_cppstd.h> // For string conversion
#include <chrono>
#include "StrHelper.h"
#include "../vtautodll/vtautodll.h"
#include <Psapi.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "psapi.lib")

using namespace std;
using namespace std::chrono;
using namespace msclr::interop;

System::Void vtautoui::MainForm::MainForm_Load(System::Object^ sender, System::EventArgs^ e)
{
	this->logger = new VTAutoLogger();
	this->wndManager = new GameWndManager(this->logger);
	this->myTimer = gcnew System::Threading::Timer(gcnew TimerCallback(this, &MainForm::OnTimerTick), nullptr, 0, 500); // 0 initial delay, 500ms interval
	
	this->logger->writeLog("************************** VTAuto App started **************************");
	Console::WriteLine("[VTAuto] ************************** VTAuto App started **************************");
}

System::Void vtautoui::MainForm::OnTimerTick(Object^ state) {
	this->wndManager->RefreshGameData();
	this->updatePlayerListView();
	this->wndManager->notifyDeath();
	
	// Log game data refresh every 10 seconds
	static int tickCount = 0;
	tickCount++;
	if (tickCount % 10 == 0) {
		int gameCount = this->wndManager->gameWnds.size();
		std::string logMessage = "INFO: Refreshed game data - Found " + std::to_string(gameCount) + " game windows";
		this->logger->writeLog(logMessage);
		Console::WriteLine("[VTAuto] INFO: Refreshed game data - Found " + gameCount + " game windows");
	}
}

System::Void vtautoui::MainForm::updatePlayerListView() {
	if (lvPlayers->InvokeRequired) {
		lvPlayers->Invoke(gcnew Action(this, &MainForm::updatePlayerListView));
		return;
	}

	lvPlayers->BeginUpdate();
	for (int i = lvPlayers->Items->Count - 1; i >= 0; i--) {
		auto item = lvPlayers->Items[i];
		String^ pIdStr = item->Name;

		if (this->wndManager->getGameWnd(pIdStr) == NULL) {
			lvPlayers->Items->RemoveAt(i);
		}
	}
	lvPlayers->EndUpdate();

	for (auto it : this->wndManager->gameWnds) {
		GameWnd* gw = it.second;
		String^ itemText = "VLTK";
		String^ processId = Convert::ToString(static_cast<int>(gw->wndMetadata.processId));

		int idx = lvPlayers->Items->IndexOfKey(processId);
		String^ pName = VTAutoStrHelper::ToSysStr(gw->myChar->name);
		String^ pHP = Convert::ToString(static_cast<int>(gw->myChar->curHp));
		String^ pMP = Convert::ToString(static_cast<int>(gw->myChar->curMp));

		if (idx == -1) {
			ListViewItem^ item = gcnew ListViewItem();
			item->Name = processId;
			item->Text = pName;
			item->SubItems->Add(pHP);
			item->SubItems->Add(pMP);

			lvPlayers->Items->Add(item);
		}
		else {
			auto curItem = lvPlayers->Items[idx];
			curItem->Text = pName;
			curItem->SubItems[1]->Text = pHP;
			curItem->SubItems[2]->Text = pMP;
		}
	}
}

System::Void vtautoui::MainForm::applyConfigState(ConfigState* cfgState)
{
	if (!cfgState) {
		return;
	}

	this->chboxNotifyDeath->Checked = cfgState->notifyDeath;
}

GameWnd* vtautoui::MainForm::selectedWnd()
{
	if (lvPlayers->SelectedItems->Count != 1) {
		return NULL;
	}
	auto item = lvPlayers->SelectedItems[0];
	auto pid = item->Name;

	return this->wndManager->getGameWnd(pid);
}

System::Void vtautoui::MainForm::chboxNotifyDeath_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
{
	GameWnd* gw = this->selectedWnd();
	if (!gw) return;

	gw->cfgState->notifyDeath = this->chboxNotifyDeath->Checked;
}

System::Void vtautoui::MainForm::timer1_Tick(System::Object^ sender, System::EventArgs^ e)
{
}

System::Void vtautoui::MainForm::timer2_Tick(System::Object^ sender, System::EventArgs^ e)
{

}

System::Void vtautoui::MainForm::buttonTrySomething_Click(System::Object^ sender, System::EventArgs^ e)
{
	GameWnd* gw = this->selectedWnd();
	if (!gw || gw->isHooked == false) {
		this->logger->writeLog("ERROR: Lost connection to game window - Send Message 1 failed");
		Console::WriteLine("[VTAuto] ERROR: Lost connection to game window - Send Message 1 failed");
		return;
	}

	LRESULT result = SendMessage(gw->wndMetadata.hwnd, WH_VTAUTO, NULL, 1);
	if (result == 0) {
		std::string logMessage = "SUCCESS: Send Message 1 sent successfully to PID " + std::to_string(static_cast<DWORD>(gw->wndMetadata.processId));
		this->logger->writeLog(logMessage);
		Console::WriteLine("[VTAuto] SUCCESS: Send Message 1 sent successfully to PID " + static_cast<int>(gw->wndMetadata.processId));
	} else {
		std::string logMessage = "ERROR: Send Message 1 failed with result " + std::to_string(static_cast<LRESULT>(result));
		this->logger->writeLog(logMessage);
		Console::WriteLine("[VTAuto] ERROR: Send Message 1 failed with result " + static_cast<long>(result));
	}
}

System::Void vtautoui::MainForm::button2_Click(System::Object^ sender, System::EventArgs^ e)
{
	GameWnd* gw = this->selectedWnd();
	if (!gw || gw->isHooked == false) {
		this->logger->writeLog("ERROR: Lost connection to game window - Send Message 2 failed");
		Console::WriteLine("[VTAuto] ERROR: Lost connection to game window - Send Message 2 failed");
		return;
	}

	LRESULT result = SendMessage(gw->wndMetadata.hwnd, WH_VTAUTO, NULL, 2);
	if (result == 0) {
		std::string logMessage = "SUCCESS: Send Message 2 sent successfully to PID " + std::to_string(static_cast<DWORD>(gw->wndMetadata.processId));
		this->logger->writeLog(logMessage);
		Console::WriteLine("[VTAuto] SUCCESS: Send Message 2 sent successfully to PID " + static_cast<int>(gw->wndMetadata.processId));
	} else {
		std::string logMessage = "ERROR: Send Message 2 failed with result " + std::to_string(static_cast<LRESULT>(result));
		this->logger->writeLog(logMessage);
		Console::WriteLine("[VTAuto] ERROR: Send Message 2 failed with result " + static_cast<long>(result));
	}
}

System::Void vtautoui::MainForm::lvPlayers_ItemChecked(System::Object^ sender, System::Windows::Forms::ItemCheckedEventArgs^ e)
{
	auto item = e->Item;
	auto pid = item->Name;

	GameWnd* gw = this->wndManager->getGameWnd(pid);
	if (!gw) {
		std::string logMessage = "ERROR: GameWnd not found for PID " + marshal_as<std::string>(pid);
		this->logger->writeLog(logMessage);
		Console::WriteLine("[VTAuto] ERROR: GameWnd not found for PID " + pid);
		return;
	}

	if (item->Checked && gw->isHooked == false) {
		std::string logMessage = "INFO: Attempting to inject DLL into PID " + marshal_as<std::string>(pid);
		this->logger->writeLog(logMessage);
		Console::WriteLine("[VTAuto] INFO: Attempting to inject DLL into PID " + pid);
		
		InjectDll(gw->wndMetadata.hwnd, gw->wndMetadata.threadId);
		logMessage = "INFO: DLL injection attempted for PID " + marshal_as<std::string>(pid);
		this->logger->writeLog(logMessage);
		Console::WriteLine("[VTAuto] INFO: DLL injection attempted for PID " + pid);
		MessageBeep(MB_OK);
		gw->isHooked = true;
	}

	if (item->Checked == false && gw->isHooked == true) {
		std::string logMessage = "INFO: Attempting to unmap DLL from PID " + marshal_as<std::string>(pid);
		this->logger->writeLog(logMessage);
		Console::WriteLine("[VTAuto] INFO: Attempting to unmap DLL from PID " + pid);
		
		UnmapDll(gw->wndMetadata.hwnd);
		logMessage = "INFO: DLL unmapping attempted for PID " + marshal_as<std::string>(pid);
		this->logger->writeLog(logMessage);
		Console::WriteLine("[VTAuto] INFO: DLL unmapping attempted for PID " + pid);
		MessageBeep(MB_OKCANCEL);
		gw->isHooked = false;
	}
}

System::Void vtautoui::MainForm::lvPlayers_ItemSelectionChanged(System::Object^ sender, System::Windows::Forms::ListViewItemSelectionChangedEventArgs^ e)
{
	auto item = e->Item;
	if (!item->Selected) return;

	GameWnd* gw = this->selectedWnd();
	if (!gw) return;

	this->applyConfigState(gw->cfgState);
}