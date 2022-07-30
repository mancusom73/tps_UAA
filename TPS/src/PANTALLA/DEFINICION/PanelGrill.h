#pragma pack (push,8) 
#ifndef __PANEL_GRILL_H
#define __PANEL_GRILL_H

#include "CPanel.h"
#include "CLabel.h"
#include "CGrid.h"
#include "CInput.h"
#include "CScreencfg.h"

class PanelGrill : public CPanel
{
  public:
    PanelGrill( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

    CLabel *LabelTitulo;
    CGrid *Grilla;
    CInput *input_de_grilla;

    CScreencfg *screenXml;
    CInput *getFocus();
    void init();
    void createTitle();
    void create();
    void Draw()
    {
    };
    void setLabelTitulo( const string &s );
    void Draw( wxDC *dc )
    {
    };
    int getHeight();
    int getWidth();
    void adjust();
    void positionInput( int input, int varios );
    void addRow( const string &s );
    void scrollGrill( const string &lines );
    void vaciar();

  private:
    //int cantRow;
    int start_offset;
    int end_offset;
    int start_point_label;
    int start_point_input;
    int width_input;
    string label_font;
    string label_colour;
    string label_align;
    string titulo_font;
    string titulo_colour;
    string titulo_align;
    int titulo_pos_x;
    int titulo_pos_y;
    int panel_width;
    int panel_height;
};
#endif

#pragma pack (pop) 
