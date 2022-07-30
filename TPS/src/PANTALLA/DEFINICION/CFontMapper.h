#pragma pack (push,8) 
#ifndef __CFONTMAPPER_H
#define __CFONTMAPPER_H

#include "CObject.h"

#include <vector>
#include <string>
using namespace std;

class CFontEnumerator : public wxFontEnumerator
{
  public:
    bool GotAny() const
    {
        return !m_facenames.IsEmpty();
    }
    const wxArrayString & GetFacenames() const
    {
        return m_facenames;
    }
    CFontEnumerator();
    wxString GetFacename( const wxString &name );
  protected:
    virtual bool OnFacename( const wxString &facename )
    {
        m_facenames.Add( facename );
        return TRUE;
    }
  private:
    wxArrayString m_facenames;
};

extern CFontEnumerator fontEnumerator;
#endif

#pragma pack (pop) 
