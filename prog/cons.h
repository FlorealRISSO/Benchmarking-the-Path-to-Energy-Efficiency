#define INLINE __attribute__((always_inline))

#define _STRINGIZE(x) #x
#define STRINGIZE(x) _STRINGIZE(x)

#define LINE 400
#define COL 400
#define PPM_HEAD "P6\n" STRINGIZE(LINE) " " STRINGIZE(COL) "\n255\n"
#define COLORS 3
#define SIZE (LINE * COL * COLORS)
#define FSIZE (SIZE + 10)
#define AVXD 4

#define PPM_SIZE ((sizeof PPM_HEAD) - 1)

#define XMIN -1.0
#define XMAX 1.0
#define YMIN -1.0
#define YMAX 1.0
#define MAXITER 100
#define A (-0.8)
#define B (0.156)

#define NAN (0.0 / 0.0)

// Precalculate constants outside the loop
#define XSTEP ((XMAX - XMIN) / LINE)
#define YSTEP ((YMAX - YMIN) / COL)

