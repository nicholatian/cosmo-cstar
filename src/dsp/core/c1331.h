#ifndef COSMOPOLITAN_DSP_CORE_C1331_H_
#define COSMOPOLITAN_DSP_CORE_C1331_H_
#if !(__ASSEMBLER__ + __LINKER__ + 0)

/**
 * Byte sized kernel for resampling memory in half.
 *
 * @define (1*𝑎 + 3*𝑏 + 3*𝑐 + 1*𝑑) / (1 + 3 + 3 + 1)
 * @see C161() afterward for superior sin(𝑥)/𝑥
 * @limit [0,255] → [0..2,044] → [0..255]
 */
forceinline pureconst artificial unsigned char C1331(unsigned char al,
                                                     unsigned char bl,
                                                     unsigned char cl,
                                                     unsigned char dl) {
  short ax, bx;
  bx = bl;
  bx += cl;
  bx *= 3;
  ax = al;
  ax += dl;
  ax += bx;
  ax += 4;
  ax >>= 3;
  al = ax;
  return al;
}

#endif /* !(__ASSEMBLER__ + __LINKER__ + 0) */
#endif /* COSMOPOLITAN_DSP_CORE_C1331_H_ */
