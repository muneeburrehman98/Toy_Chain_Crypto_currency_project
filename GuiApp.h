#ifndef GUIAPP_H
#define GUIAPP_H

#include "Blockchain.h"
#include <wx/listctrl.h>
#include <wx/wx.h>


class BlockchainFrame : public wxFrame {
public:
  BlockchainFrame(Blockchain &bc);

private:
  void OnAddTransaction(wxCommandEvent &event);
  void OnMine(wxCommandEvent &event);
  void UpdateDisplay();

  Blockchain &blockchain;
  wxListView *blockList;
  wxListView *txList;
  wxStaticText *statusText;

  wxDECLARE_EVENT_TABLE();
};

class GuiApp : public wxApp {
public:
  virtual bool OnInit();
};

#endif // GUIAPP_H
