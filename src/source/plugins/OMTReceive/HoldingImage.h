#pragma once
#include <cstdint>

// Full-width Resolume Source holding frame: test card + centered R+ mark.
// This is the source preview/holding frame until live OMT video arrives.
static const int kHoldingW = 256;
static const int kHoldingH = 144;

static const uint8_t* MakeHoldingImage()
{
    static uint8_t pixels[kHoldingW * kHoldingH * 4] = {};
    static bool initialized = false;
    if (initialized) return pixels;
    initialized = true;

    // Fill the entire 16:9 frame; no narrow centered square or side gutters.
    for (int y = 0; y < kHoldingH; ++y) {
        for (int x = 0; x < kHoldingW; ++x) {
            const int i = (y * kHoldingW + x) * 4;
            uint8_t r=5, g=5, b=5;

            // Wide calibration-card pattern across the complete image.
            if ((x % 32) < 2) { r=20; g=255; b=40; }
            if ((y % 24) < 2) { r=30; g=30; b=255; }
            if ((x % 64) < 2 && (y % 48) >= 2) { r=255; g=25; b=25; }
            if (x == y * 256 / 144 || x == 255 - y * 256 / 144 ||
                x == kHoldingW/2 || y == kHoldingH/2) {
                r=235; g=235; b=235;
            }

            // Four calibration circles spread across the full width.
            const int cx[4]={48,208,48,208}, cy[4]={36,36,108,108};
            for(int c=0;c<4;++c) {
                const int dx=x-cx[c], dy=y-cy[c];
                const int d=dx*dx+dy*dy;
                if(d>=23*23 && d<=25*25) { r=245; g=245; b=245; }
            }

            // BGRA pixel order.
            pixels[i+0]=b; pixels[i+1]=g; pixels[i+2]=r; pixels[i+3]=255;
        }
    }

    // Bold white R+ mark, centered horizontally over the full-width card.
    const uint8_t R[7]={30,17,17,30,20,18,17};
    const uint8_t P[7]={0,4,4,31,4,4,0};
    const int scale=8, startX=92, startY=43;
    for(int y=0;y<7;++y) for(int x=0;x<5;++x) {
        if(R[y] & (1u << (4-x))) {
            for(int yy=0;yy<scale;++yy) for(int xx=0;xx<scale;++xx) {
                const int px=startX+x*scale+xx, py=startY+y*scale+yy;
                if(px>=0&&px<kHoldingW&&py>=0&&py<kHoldingH) {
                    const int i=(py*kHoldingW+px)*4;
                    pixels[i]=255; pixels[i+1]=255; pixels[i+2]=255;
                }
            }
        }
        if(P[y] & (1u << (4-x))) {
            for(int yy=0;yy<5;++yy) for(int xx=0;xx<5;++xx) {
                const int px=startX+5*scale+5+x*5+xx, py=startY+8+y*5+yy;
                if(px>=0&&px<kHoldingW&&py>=0&&py<kHoldingH) {
                    const int i=(py*kHoldingW+px)*4;
                    pixels[i]=255; pixels[i+1]=255; pixels[i+2]=255;
                }
            }
        }
    }
    return pixels;
}

#define kHoldingData (MakeHoldingImage())
