#pragma pack (push,8) 
#ifndef __PANEL_INPUT_H
#define __PANEL_INPUT_H

#include "CPanel.h"
#include "CLabel.h"
#include "CInput.h"

class PanelInput : public CPanel
{
  public:
    CLabel *LabelTitulo;
    vector<CInput*> inputs;
    string detailInput[CANTIDAD_INPUTS][CANTIDAD_CARACTERISTICAS];

    PanelInput( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

    void init();
    void createLabels();
    void create();
    void Draw()
    {
    };
    void Draw( wxDC *dc )
    {
    };
    CInput * getFocus()
    {
        return inputs[focus];
    };
    void setLabels( int type, const string &s );
    void addInput( const string &s );
    void addInput( const string &s, int x, int y, int w, int h, const string &mascara, int w1);
    void addInput( const string &s, const string &mascara );
    void addLabel( const string &s );
    void changeFocus( int type );
    void deleteInputs();
    int getHeight();
    int getWidth();
    void adjust();
    void setInputText( const string &t );
    void setPropInput( const string &f, int type, int ind1 );
    string getPropInput( int ind1, int ind2 );
    void positionInput( int input, int varios );        
    void inicializarVariables( void );
    void __setPosition( const string &f );
  private:
    int input_size;
    int space_size;
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
    int panel_width_final;
    int panel_width_default;
    vector<CLabel*> labels;
    int visible_inputs;
    int visible_labels;
    int focus;
};
#endif

#pragma pack (pop) 
