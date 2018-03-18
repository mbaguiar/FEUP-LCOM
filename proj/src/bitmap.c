#include "bitmap.h"


Bitmap* loadBitmap(const char* filename) {
    // allocating necessary size
    Bitmap* bmp = (Bitmap*) malloc(sizeof(Bitmap));

    // open filename in read binary mode
    FILE *filePtr;
    filePtr = fopen(filename, "rb");
    if (filePtr == NULL)
        return NULL;

    // read the bitmap file header
    BitmapFileHeader bitmapFileHeader;
    fread(&bitmapFileHeader, 2, 1, filePtr);

    // verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.type != 0x4D42) {
        fclose(filePtr);
        return NULL;
    }

    int rd;
    do {
        if ((rd = fread(&bitmapFileHeader.size, 4, 1, filePtr)) != 1)
            break;
        if ((rd = fread(&bitmapFileHeader.reserved, 4, 1, filePtr)) != 1)
            break;
        if ((rd = fread(&bitmapFileHeader.offset, 4, 1, filePtr)) != 1)
            break;
    } while (0);

    if (rd != 1) {
        fprintf(stderr, "Error reading file\n");
        exit(-1);
    }

    // read the bitmap info header
    BitmapInfoHeader bitmapInfoHeader;
    fread(&bitmapInfoHeader, sizeof(BitmapInfoHeader), 1, filePtr);

    // move file pointer to the begining of bitmap data
    fseek(filePtr, bitmapFileHeader.offset, SEEK_SET);

    // allocate enough memory for the bitmap image data
    short* bitmapImage = (short*) malloc(
            bitmapInfoHeader.imageSize);

    // verify memory allocation
    if (!bitmapImage) {
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }

    // read in the bitmap image data
    fread(bitmapImage, bitmapInfoHeader.imageSize, 1, filePtr);

    // make sure bitmap image data was read
    if (bitmapImage == NULL) {
        fclose(filePtr);
        return NULL;
    }

    // close file and return bitmap image data
    fclose(filePtr);

    bmp->bitmapData = bitmapImage;
    bmp->bitmapInfoHeader = bitmapInfoHeader;

    return bmp;
}

void drawBitmap(Bitmap* bmp, int x, int y, Alignment alignment, int transparency) {
    if (bmp == NULL){
        printf("BMP is NULL\n");
        return;
    }
    int width = bmp->bitmapInfoHeader.width;
    int drawWidth = width;
    int height = bmp->bitmapInfoHeader.height;

    if (alignment == ALIGN_CENTER){
        x -= width / 2;
        y -= height/2;
    }
    else if (alignment == ALIGN_RIGHT)
        x -= width;

    if (x + width < 0 || x > getHRES() || y + height < 0
            || y > getVRES())
        return;


    int xCorrection = 0;
    if (x < 0) {
        xCorrection = -x;
        drawWidth -= xCorrection;
        x = 0;

        if (drawWidth > getHRES())
            drawWidth = getHRES();
    } else if (x + drawWidth >= getHRES()) {
        drawWidth = getHRES() - x;
    }

    short* bufferStartPos;
    short* imgStartPos;

    int i;
    for (i = 0; i < height; i++) {
        int pos = y + height - 1 - i;
        if (pos < 0 || pos >= getVRES())
            continue;

        bufferStartPos = getDoubleBuffer();
        bufferStartPos += x + pos * getHRES();

        imgStartPos = bmp->bitmapData + xCorrection + i * width;

        int j;

    		for (j = 0; j < drawWidth; j++) {
          if (transparency) {
            if (imgStartPos[j] != 0xFFFFFA1F) {
              bufferStartPos[j] = imgStartPos[j];
            }
          } else {
            bufferStartPos[j] = imgStartPos[j];
          }
    		}
    }
}

void deleteBitmap(Bitmap* bmp) {
    if (bmp == NULL)
        return;

    free(bmp->bitmapData);
    free(bmp);
}

void updateFrameBuffer() {
	memcpy(getVideoMem(), getDoubleBuffer(), getVramSize());
}
