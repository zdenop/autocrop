/*
 *  auto_crop.c
 *
 *    auto crop of input image.
 *    gcc auto_crop.c -o auto_crop -llept
 */

#include "leptonica/allheaders.h"

int main(int    argc, char **argv) {
    static char  mainName[] = "auto_crop";
    char        *filein, *outfile;
    BOX         *box1, *box2;
    BOXA        *boxa1, *boxa2;
    l_int32     i, border, x, y, w, h, xb, yb, wb, hb, ncomp;
    PIX         *fpixs, *pixs, *pixt1, *pixd;
    char  buf[BUFSIZ];
  
    border = 40;
    
    if (argc != 2)
        return ERROR_INT(" Syntax:  auto_crop filein", mainName, 1);
    filein = argv[1];
    if ((fpixs = pixRead(filein)) == NULL)
        return ERROR_INT("pix not made", mainName, 1);
    pixs = pixConvertTo1(fpixs, 130);
    
    pixt1 = pixMorphSequence(pixs, "r11 + c10.20 + o5.50 + x4", 0);
    boxa1 = pixConnComp(pixt1, NULL, 8);
    ncomp = boxaGetCount(boxa1);
    if (ncomp == 0) {
        fprintf(stderr, "Warning: no components on page %s\n", filein);
    } else {
        fprintf(stdout, "Found %d components in %s\n", ncomp, filein);
    }
    for (i = 0; i < ncomp; i++) {
        boxa2 = boxaSort(boxa1, L_SORT_BY_AREA, L_SORT_INCREASING, NULL);
        box1 = boxaGetBox(boxa2, i, L_CLONE);
        boxGetGeometry(box1, &x, &y, &w, &h);
        xb = L_MAX(0, x - border);
        yb = L_MAX(0, y - border);
        wb = w + 2 * border;
        hb = h + 2 * border;
        box2 = boxCreate(xb, yb, wb, hb);
        pixd = pixClipRectangle(fpixs, box2, NULL);
        sprintf(buf, "auto_crop-out_%d.png", i);
        pixWrite(buf, pixd, IFF_PNG);  // IFF_TIFF_G4 IFF_TIFF
    }

    pixDestroy(&pixs);
    return;
}
