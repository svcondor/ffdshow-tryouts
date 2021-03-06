/*
 *    Copyright (C) 2003-2004 Gabest
 *    http://www.gabest.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GNU Make; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#ifndef _DECSSINPUTPIN_H_
#define _DECSSINPUTPIN_H_

class CDeCSSInputPin : public CTransformInputPin, public IKsPropertySet
{
    int m_varient;
    BYTE m_Challenge[10], m_KeyCheck[5], m_Key[10];
    BYTE m_DiscKey[6], m_TitleKey[6];

protected:
    // return S_FALSE here if you don't want the base class
    // to call CTransformFilter::Receive with this sample
    virtual HRESULT Transform(IMediaSample* pSample) {
        return S_OK;
    }

    virtual HRESULT SetPropSetRate(DWORD Id, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropertyData, DWORD cbPropData) {
        return E_NOTIMPL;
    }
    virtual HRESULT GetPropSetRate(DWORD Id, LPVOID pInstanceData, DWORD InstanceLength, LPVOID pPropertyData, DWORD cbPropData, DWORD *pcbReturned) {
        return E_NOTIMPL;
    }
    virtual HRESULT SupportPropSetRate(DWORD dwPropID, DWORD *pTypeSupport) {
        return E_NOTIMPL;
    }

public:
    CDeCSSInputPin(const char_t* pObjectName, CTransformFilter* pFilter, HRESULT* phr, const wchar_t *pName);

    DECLARE_IUNKNOWN
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void** ppv);

    void StripPacket(BYTE*& p, long& len);

    // IMemInputPin
    STDMETHODIMP Receive(IMediaSample* pSample);

    // IKsPropertySet
    STDMETHODIMP Set(REFGUID PropSet, ULONG Id, LPVOID InstanceData, ULONG InstanceLength, LPVOID PropertyData, ULONG DataLength);
    STDMETHODIMP Get(REFGUID PropSet, ULONG Id, LPVOID InstanceData, ULONG InstanceLength, LPVOID PropertyData, ULONG DataLength, ULONG* pBytesReturned);
    STDMETHODIMP QuerySupported(REFGUID PropSet, ULONG Id, ULONG* pTypeSupport);
};

#endif
