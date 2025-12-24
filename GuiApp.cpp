#include "GuiApp.h"
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

enum { ID_ADD_TX = 1, ID_MINE = 2 };

wxBEGIN_EVENT_TABLE(BlockchainFrame, wxFrame)
    EVT_BUTTON(ID_ADD_TX, BlockchainFrame::OnAddTransaction)
        EVT_BUTTON(ID_MINE, BlockchainFrame::OnMine) wxEND_EVENT_TABLE()

            BlockchainFrame::BlockchainFrame(Blockchain &bc)
    : wxFrame(NULL, wxID_ANY, "ToyChain Modern Dashboard", wxDefaultPosition,
              wxSize(800, 600)),
      blockchain(bc) {

  wxPanel *mainPanel = new wxPanel(this);
  mainPanel->SetBackgroundColour(wxColour(30, 30, 30)); // Dark Mode

  wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

  // Title
  wxStaticText *title =
      new wxStaticText(mainPanel, wxID_ANY, "TOYCHAIN BLOCKCHAIN VISUALIZER");
  title->SetForegroundColour(*wxWHITE);
  wxFont font = title->GetFont();
  font.SetPointSize(18);
  font.SetWeight(wxFONTWEIGHT_BOLD);
  title->SetFont(font);
  mainSizer->Add(title, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 20);

  // List Controls
  wxBoxSizer *listSizer = new wxBoxSizer(wxHORIZONTAL);

  blockList = new wxListView(mainPanel, wxID_ANY, wxDefaultPosition,
                             wxDefaultSize, wxLC_REPORT);
  blockList->AppendColumn("Index", wxLIST_FORMAT_LEFT, 50);
  blockList->AppendColumn("Hash", wxLIST_FORMAT_LEFT, 200);
  blockList->AppendColumn("Status", wxLIST_FORMAT_LEFT, 100);
  blockList->SetBackgroundColour(wxColour(45, 45, 45));
  blockList->SetForegroundColour(*wxWHITE);

  txList = new wxListView(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                          wxLC_REPORT);
  txList->AppendColumn("From", wxLIST_FORMAT_LEFT, 100);
  txList->AppendColumn("To", wxLIST_FORMAT_LEFT, 100);
  txList->AppendColumn("Amount", wxLIST_FORMAT_LEFT, 100);
  txList->SetBackgroundColour(wxColour(45, 45, 45));
  txList->SetForegroundColour(*wxWHITE);

  listSizer->Add(blockList, 1, wxEXPAND | wxALL, 10);
  listSizer->Add(txList, 1, wxEXPAND | wxALL, 10);
  mainSizer->Add(listSizer, 1, wxEXPAND);

  // Status Area
  statusText = new wxStaticText(mainPanel, wxID_ANY, "Chain Status: VALID");
  statusText->SetForegroundColour(wxColour(0, 255, 0));
  mainSizer->Add(statusText, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);

  // Buttons
  wxBoxSizer *btnSizer = new wxBoxSizer(wxHORIZONTAL);
  wxButton *addBtn =
      new wxButton(mainPanel, ID_ADD_TX, "Add Random Transaction");
  wxButton *mineBtn = new wxButton(mainPanel, ID_MINE, "Mine Block");

  btnSizer->Add(addBtn, 0, wxALL, 5);
  btnSizer->Add(mineBtn, 0, wxALL, 5);
  mainSizer->Add(btnSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 20);

  mainPanel->SetSizer(mainSizer);
  UpdateDisplay();
}

void BlockchainFrame::UpdateDisplay() {
  blockList->DeleteAllItems();
  const auto &chain = blockchain.getChain();
  for (const auto &block : chain) {
    long index = blockList->InsertItem(blockList->GetItemCount(),
                                       wxString::Format("%d", block.index));
    blockList->SetItem(index, 1, block.hash);
    blockList->SetItem(index, 2, "Confirmed");
  }

  if (!blockchain.isChainValid()) {
    statusText->SetLabel("Chain Status: TAMPERED!");
    statusText->SetForegroundColour(*wxRED);
  } else {
    statusText->SetLabel("Chain Status: VALID");
    statusText->SetForegroundColour(wxColour(0, 255, 0));
  }
}

void BlockchainFrame::OnAddTransaction(wxCommandEvent &event) {
  blockchain.addTransaction(Transaction("User", "Recipient", 10.0, 0.5));
  wxMessageBox("Transaction added to mempool!", "Success",
               wxOK | wxICON_INFORMATION);
}

void BlockchainFrame::OnMine(wxCommandEvent &event) {
  blockchain.minePendingTransactions("GUI_Miner");
  UpdateDisplay();
  wxMessageBox("New block mined successfully!", "Mining Complete",
               wxOK | wxICON_INFORMATION);
}

wxIMPLEMENT_APP_NO_MAIN(GuiApp);

bool GuiApp::OnInit() {
  // We pass the blockchain in from the real main
  return true;
}
