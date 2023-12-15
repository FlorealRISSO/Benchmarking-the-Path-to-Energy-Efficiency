#define INLINE __attribute__((always_inline))

#define STRINGIZE(x) #x

#define LINE 10000
#define COL 10000
#define COLORS 3
#define SIZE (LINE * COL * COLORS)
#define FSIZE (SIZE + 10)
#define AVXD 4

#define PPM_HEAD "P6\n" STRINGIZE(LINE) STRINGIZE(COL) "\n255\n"
#define PPM_SIZE ((sizeof PPM_HEAD) - 1)

#define XMIN -1.0
#define XMAX 1.0
#define YMIN -1.0
#define YMAX 1.0
#define iterationmax 1000
#define A -0.8
#define B 0.156

// Precalculate constants outside the loop
const double step_x = (XMAX - XMIN) / LINE;
const double step_y = (YMAX - YMIN) / COL;

