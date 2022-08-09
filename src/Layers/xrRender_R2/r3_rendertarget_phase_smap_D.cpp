#include "stdafx.h"

void CRenderTarget::phase_smap_direct(light* L, u32 sub_phase)
{
    // Targets
    u_setrt(nullptr, nullptr, nullptr, rt_smap_depth);

    //	Don't have rect clear for DX11
    //	TODO: DX9:	Full clear must be faster for the near phase for SLI
    //	inobody clears this buffer _this_ frame.
    // Clear
    // if (SE_SUN_NEAR==sub_phase)			{
    // optimized clear
    //	D3DRECT		R;
    //	R.x1		= L->X.D.minX;
    //	R.x2		= L->X.D.maxX;
    //	R.y1		= L->X.D.minY;
    //	R.y2		= L->X.D.maxY;
    //	CHK_DX							(HW.pDevice->Clear( 1L, &R,	  D3DCLEAR_ZBUFFER,	0xFFFFFFFF, 1.0f, 0L));
    //} else
    {
        // full-clear
        RCache.ClearZB(rt_smap_depth, 1.0f);
    }

    //	Prepare viewport for shadow map rendering
    if (sub_phase != SE_SUN_RAIN_SMAP)
        RImplementation.rmNormal();
    else
    {
        RCache.SetViewport({ (float)L->X.D.minX, (float)L->X.D.minY,
            (float)(L->X.D.maxX - L->X.D.minX), (float)(L->X.D.maxY - L->X.D.minY),
            0.f, 1.f });
    }

    // Stencil	- disable
    RCache.set_Stencil(FALSE);

    //	TODO: DX11:	Implement culling reverse for DX11
    // Misc		- draw only front/back-faces
    /*
    if (SE_SUN_NEAR==sub_phase)			RCache.set_CullMode			( CULL_CCW	);	// near
    else								{
        if (RImplementation.o.HW_smap)	RCache.set_CullMode			( CULL_CW	);	// far, reversed
        else							RCache.set_CullMode			( CULL_CCW	);	// far, front-faces
    }
    if (RImplementation.o.HW_smap)		RCache.set_ColorWriteEnable	( FALSE		);
    else								RCache.set_ColorWriteEnable	( );
    */
}

void CRenderTarget::phase_smap_direct_tsh(light* L, u32 sub_phase)
{
    VERIFY(RImplementation.o.Tshadows);
    RCache.set_ColorWriteEnable();
    //	Prepare viewport for shadow map rendering
    RImplementation.rmNormal();
    RCache.ClearRT(RCache.get_RT(), { 1.0f, 1.0f, 1.0f, 1.0f }); // color_rgba(127, 127, 12, 12);
}
