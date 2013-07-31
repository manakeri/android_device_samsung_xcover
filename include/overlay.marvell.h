/*******************************************************************************
//(C) Copyright [2009] Marvell International Ltd.
//All Rights Reserved
*******************************************************************************/

#ifndef ANDROID_OVERLAY_MARVELL_H
#define ANDROID_OVERLAY_MARVELL_H

struct overlay_buffer_header_t {
    void    *paddr; // physical address
    void    *vaddr; // virtual address
    int       len;
    int       flag;   // 1: FB buffer, 2: codec buffer, 3: flush buffer
    void    *user;  // pointer to APP's private data passed via callback
    void (*free)(void *user);
    void    *reserved;
    int       x_off;
    int       y_off;
    int       stride_x;
    int       stride_y;
    int 	deinterlace;
    int 	step0; //Y step
    int 	step1; //U step of UV step [420p: U step] [420sp: UV step]
    int 	step2; //V step
};

enum {
    OVERLAY_FORMAT_YCbCr_420_P = OVERLAY_FORMAT_DEFAULT + 1,
    OVERLAY_FORMAT_IPP_PICTURE = OVERLAY_FORMAT_DEFAULT + 2,
};

#endif  // ANDROID_OVERLAY_MARVELL_H
