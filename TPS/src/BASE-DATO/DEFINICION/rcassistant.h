#pragma pack (push,8) 
// RCAssistant.h: interface for the CRCAssistant class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCASSISTANT_H__863872C1_E108_11D2_88AB_006008AE92D1__INCLUDED_)
#define AFX_RCASSISTANT_H__863872C1_E108_11D2_88AB_006008AE92D1__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class     CRC
{
  public:
    short Compute16( unsigned short Crc, char *cpBuffer, unsigned short uiLength );
    CRC();
    virtual ~CRC();
  protected:
    unsigned short *crc16tab;
};

#endif // !defined(AFX_RCASSISTANT_H__863872C1_E108_11D2_88AB_006008AE92D1__INCLUDED_)
#pragma pack (pop) 
