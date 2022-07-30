#include "CFontMapper.h"

CFontEnumerator fontEnumerator;

CFontEnumerator::CFontEnumerator()
{
    fontEnumerator.EnumerateFacenames();
}


wxString CFontEnumerator::GetFacename( const wxString &name )
{
    for( unsigned int i = 0;i < m_facenames.GetCount();i++ ) {
        if( m_facenames[i] == name ) {
            return m_facenames[i];
        }
    }
    return "";
}
