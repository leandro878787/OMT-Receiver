#pragma once
#include <cstdint>

// Resolume Source holding frame: complete Plugins Lab mark (test card + R+).
// The FFGL SDK does not expose a portable per-source icon setter, so this
// artwork is used as the source preview/holding frame until live video arrives.
static const int kHoldingW = 256;
static const int kHoldingH = 144;

static const uint8_t* MakeHoldingImage()
{
    static uint8_t pixels[kHoldingW * kHoldingH * 4] = {};
    static bool initialized = false;
    if (initialized) return pixels;
    initialized = true;

    const int left = 56, top = 0, size = 144;
    for (int y = 0; y < kHoldingH; ++y) {
        for (int x = 0; x < kHoldingW; ++x) {
            const int i = (y * kHoldingW + x) * 4;
            pixels[i+0] = 5; pixels[i+1] = 5; pixels[i+2] = 5; pixels[i+3] = 255;
            if (x >= left && x < left+size && y >= top && y < top+size) {
                const int sx = x-left, sy = y-top;
                // Test-card base: colored horizontal/vertical bars and fine grid.
                uint8_t r=10, g=10, b=10;
                if ((sx % 32) < 2) { r=20; g=255; b=40; }
                if ((sy % 32) < 2) { r=30; g=30; b=255; }
                if ((sx % 64) < 2 && (sy % 64) >= 2) { r=255; g=25; b=25; }
                if (sx == sy || sx == size-1-sy || sx == size/2 || sy == size/2) {
                    r=235; g=235; b=235;
                }
                // Four large calibration circles.
                const int cx[4]={36,108,36,108}, cy[4]={36,36,108,108};
                for(int c=0;c<4;++c) {
                    const int dx=sx-cx[c], dy=sy-cy[c];
                    const int d=dx*dx+dy*dy;
                    if(d>=31*31 && d<=33*33) { r=245; g=245; b=245; }
                }
                pixels[i+0]=b; pixels[i+1]=g; pixels[i+2]=r;
            }
        }
    }

    // Bold white 5x7 R and plus, scaled for visibility over the test card.
    const uint8_t R[7]={30,17,17,30,20,18,17};
    const uint8_t P[7]={0,4,4,31,4,4,0};
    const int scale=8, startX=104, startY=43;
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
                const int px= startX+5*scale+5+x*5+xx, py=startY+8+y*5+yy;
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
