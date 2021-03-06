/*
 * $Id: TDXVADecoder.h 1179 2009-07-22 15:01:57Z casimir666 $
 *
 * (C) 2006-2007 see AUTHORS
 *
 * This file is part of mplayerc.
 *
 * Mplayerc is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mplayerc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _TDXVADECODER_H_
#define _TDXVADECODER_H_
#include "stdafx.h"
#include <atlbase.h>
#include <dxva.h>
#include <dxva2api.h>
#include <videoacc.h>
#include <streams.h>
#include <dvdmedia.h>
#include <d3dx9.h>
#include <evr.h>
#include <mfapi.h>
#include <Mferror.h>
#include "IffdshowBase.h"
#include "IffdshowDec.h"
#include "IffdshowDecVideo.h"
#include "TffDecoderVideo.h"
#include "dsutil.h"
#include "strmif.h"

#define CHECK_HR(x) hr = ##x; if (FAILED (hr)) { DPRINTF(_l("Error : 0x%08x\n"), hr); ASSERT (hr==VFW_E_NOT_COMMITTED); return hr; }

typedef enum {
    ENGINE_DXVA1,
    ENGINE_DXVA2
} DXVA_ENGINE;

typedef enum {
    H264_VLD,
    VC1_VLD
} DXVAMode;

typedef enum {
    PICT_TOP_FIELD     = 1,
    PICT_BOTTOM_FIELD  = 2,
    PICT_FRAME         = 3
} FF_FIELD_TYPE;

typedef enum {
    I_TYPE  = 1, ///< Intra
    P_TYPE  = 2, ///< Predicted
    B_TYPE  = 3, ///< Bi-dir predicted
    S_TYPE  = 4, ///< S(GMC)-VOP MPEG4
    SI_TYPE = 5, ///< Switching Intra
    SP_TYPE = 6, ///< Switching Predicted
    BI_TYPE = 7
} FF_SLICE_TYPE;

typedef struct {
    bool                        bRefPicture;       // True if reference picture
    int                         bInUse;            // Slot in use
    bool                        bDisplayed;        // True if picture have been presented
    CComPtr<IMediaSample>       pSample;           // Only for DXVA2 !
    CComPtr<IDirect3DSurface9>  pDecoderRenderTarget;// Only for DXVA2 !
    REFERENCE_TIME              rtStart;
    REFERENCE_TIME              rtStop;
    FF_FIELD_TYPE               n1FieldType;       // Top or bottom for the 1st field
    FF_SLICE_TYPE               nSliceType;
    int                         nCodecSpecific;
    DWORD                       dwDisplayCount;
} PICTURE_STORE;


#define MAX_COM_BUFFER    6     // Max uncompressed buffer for an Execute command (DXVA1)
#define COMP_BUFFER_COUNT 18

class TvideoCodecLibavcodecDxva;

class TDXVADecoder
{
public :
    // === Public functions
    virtual                 ~TDXVADecoder();
    DXVAMode                GetMode() {
        return m_nMode;
    };
    DXVA_ENGINE             GetEngine() {
        return m_nEngine;
    };
    void                    AllocExecuteParams (int nSize);
    void                    SetDirectXVideoDec (IDirectXVideoDecoder* pDirectXVideoDec)  {
        m_pDirectXVideoDec = pDirectXVideoDec;
    };

    virtual HRESULT         DecodeFrame  (BYTE* pDataIn, UINT nSize, REFERENCE_TIME rtStart, REFERENCE_TIME rtStop) = NULL;
    virtual void            SetExtraData (BYTE* pDataIn, UINT nSize);
    virtual void            CopyBitstream(BYTE* pDXVABuffer, BYTE* pBuffer, UINT& nSize);
    virtual void            Flush();
    virtual HRESULT         ConfigureDXVA1(void);

    static TDXVADecoder*    CreateDecoder (IffdshowDecVideo *IdeciV, IAMVideoAccelerator*  pAMVideoAccelerator, const GUID* guidDecoder, int nPicEntryNumber);
    static TDXVADecoder*    CreateDecoder (IffdshowDecVideo *IdeciV, IDirectXVideoDecoder* pDirectXVideoDec, const GUID* guidDecoder, int nPicEntryNumber, DXVA2_ConfigPictureDecode* pDXVA2Config);


protected :
    TDXVADecoder (IffdshowDecVideo *IdeciV, IAMVideoAccelerator*  pAMVideoAccelerator, DXVAMode nMode, int nPicEntryNumber);
    TDXVADecoder (IffdshowDecVideo *IdeciV, IDirectXVideoDecoder* pDirectXVideoDec, DXVAMode nMode, int nPicEntryNumber, DXVA2_ConfigPictureDecode* pDXVA2Config);

    IffdshowDecVideo               *deciV;
    TvideoCodecLibavcodecDxva*     m_pCodec;
    bool                           m_bFlushed;
    int                            m_nMaxWaiting;

    PICTURE_STORE*                 m_pPictureStore;           // Store reference picture, and delayed B-frames
    int                            m_nPicEntryNumber;         // Total number of picture in store
    int                            m_nWaitingPics;            // Number of picture not yet displayed

    // === DXVA functions
    HRESULT                        AddExecuteBuffer (DWORD CompressedBufferType, UINT nSize, void* pBuffer, UINT* pRealSize = NULL);
    HRESULT                        GetDeliveryBuffer(REFERENCE_TIME rtStart, REFERENCE_TIME rtStop, IMediaSample** ppSampleToDeliver);
    HRESULT                        Execute();
    DWORD                          GetDXVA1CompressedType (DWORD dwDXVA2CompressedType);
    HRESULT                        FindFreeDXVA1Buffer(DWORD dwTypeIndex, DWORD& dwBufferIndex);
    HRESULT                        BeginFrame(int nSurfaceIndex, IMediaSample* pSampleToDeliver);
    HRESULT                        EndFrame(int nSurfaceIndex);
    HRESULT                        PostProcessFrame(int dwBufferIndex, CComPtr<IDirect3DSurface9> pDecoderRenderTarget);
    HRESULT                        QueryStatus(PVOID LPDXVAStatus, UINT nSize);
    BYTE                           GetConfigIntraResidUnsigned();
    BYTE                           GetConfigResidDiffAccelerator();
    DXVA_ConfigPictureDecode*      GetDXVA1Config() {
        return &m_DXVA1Config;
    };

    // === Picture store functions
    bool                           AddToStore (int nSurfaceIndex, IMediaSample* pSample, bool bRefPicture, REFERENCE_TIME rtStart, REFERENCE_TIME rtStop, bool bIsField, FF_FIELD_TYPE nFieldType, FF_SLICE_TYPE nSliceType, int nCodecSpecific);
    void                           UpdateStore (int nSurfaceIndex, REFERENCE_TIME rtStart, REFERENCE_TIME rtStop);
    void                           RemoveRefFrame (int nSurfaceIndex);
    HRESULT                        DisplayNextFrame();
    HRESULT                        GetFreeSurfaceIndex(int& nSurfaceIndex, IMediaSample** ppSampleToDeliver, REFERENCE_TIME rtStart, REFERENCE_TIME rtStop);
    virtual int                    FindOldestFrame();

private :
    DXVAMode                       m_nMode;
    DXVA_ENGINE                    m_nEngine;

    CComPtr<IMediaSample>          m_pFieldSample;
    int                            m_nFieldSurface;

    // === DXVA1 variables
    CComQIPtr<IAMVideoAccelerator> m_pAMVideoAccelerator;
    AMVABUFFERINFO                 m_DXVA1BufferInfo[MAX_COM_BUFFER];
    DXVA_BufferDescription         m_DXVA1BufferDesc[MAX_COM_BUFFER];
    DWORD                          m_dwNumBuffersInfo;
    DXVA_ConfigPictureDecode       m_DXVA1Config;
    AMVACompBufferInfo             m_ComBufferInfo[COMP_BUFFER_COUNT];
    DWORD                          m_dwBufferIndex;
    DWORD                          m_dwDisplayCount;

    // === DXVA2 variables
    CComPtr<IDirectXVideoDecoder>  m_pDirectXVideoDec;
    CComPtr<IDirect3DSurface9>     m_pDecoderRenderTarget;
    DXVA2_ConfigPictureDecode      m_DXVA2Config;
    DXVA2_DecodeExecuteParams      m_ExecuteParams;

    void Init(IffdshowDecVideo *IdeciV, DXVAMode nMode, int nPicEntryNumber);
    void FreePictureSlot (int nSurfaceIndex);
    void SetTypeSpecificFlags(PICTURE_STORE* pPicture, IMediaSample* pMS);
};

#endif
