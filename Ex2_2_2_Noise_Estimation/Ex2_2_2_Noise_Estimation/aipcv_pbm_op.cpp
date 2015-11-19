#include "aipcv_pbm_op.h"

#if defined (MAX)
int32_t PBM_SE_ORIGIN_COL = 0;
int32_t PBM_SE_ORIGIN_ROW = 0;
char  **args;
int32_t maxargs;
#else
extern int32_t PBM_SE_ORIGIN_COL;
extern int32_t PBM_SE_ORIGIN_ROW;
#endif

static void aipcv_pbm_param(char *s) {
  int32_t i, j;
  char    key[24];

  for (i = 0; i < 23; i++) {
    j = i;
    if (s[i + 1] == ' ' || s[i + 1] == '\n')  break;
    key[i] = s[i + 1];
  }

  key[j] = '\0';

  for (i = 0; i < j; i++) {
    if ((key[i] >= 'A') && (key[i] <= 'Z')) {
      key[i] = (char)((int32_t)key[i] - (int32_t)('A') + (int32_t)('a'));
    }
  }

  if (strcmp(key, "origin") == 0) {
    sscanf(&(s[j + 1]), "%d %d", &PBM_SE_ORIGIN_COL, &PBM_SE_ORIGIN_ROW);
    return;
  }
}

static void aipcv_pbm_getln(FILE *f, char *b) {
  int32_t i = 0;
  char    c = 0;

  do {
    do {
      fscanf(f, "%c", &c);
      b[i++] = c;
      if (c == '\n')  b[i] = '\0';
    } while (c != '\n');

    if (b[0] == '#') aipcv_pbm_param(b);
  } while (b[0] == '\n' || b[0] == '#');
}

static void aipcv_pbm_getNum(FILE *f, char *b, int32_t *bi, int32_t *res) {
  int32_t i;
  char    str[80];

  while (b[*bi] == ' ' || b[*bi] == '\t' || b[*bi] == '\n') {
    if (b[*bi] == '\n') {
      aipcv_pbm_getln(f, b);
      *bi = 0;
    }
    else *bi += 1;
  }

  i = 0;
  while (b[*bi] >= '0' && b[*bi] <= '9') {
    str[i++] = b[(*bi)++];
  }
    
  str[i] = '\0';
  sscanf(str, "%d", res);
}

aipcv_image_t_ptr aipcv_input_pbm(char *fn) {
  int32_t   i, j, k, n, m, bi, b;
  uint8_t   ucval;
  int32_t   val;
  uint32_t  here;
  char      buf1[256];

  FILE *f;
  aipcv_image_t_ptr im;

  strcpy_s(buf1, fn);
  f = fopen(buf1, "r");

  if (f == NULL) {
    printf("Cannot open PBM file '%s'\n", buf1);
    return NULL;
  }

  aipcv_pbm_getln(f, buf1);

  if (buf1[0] == 'P') {
    if ((buf1[1] >= '1') && (buf1[1] <= '6')) {
      k = buf1[1] - '0';
    }
    else {
      printf("Not a PBM/PGM/PPM file!\n");
      return 0;
    }
  }

  bi = 2;

  aipcv_pbm_getNum(f, buf1, &bi, &m);   /*!> Get number of colomns */
  aipcv_pbm_getNum(f, buf1, &bi, &n);   /*!> Get number of rows */

  if (k != 1 && k != 4) aipcv_pbm_getNum(f, buf1, &bi, &b);  /*!> Get maximum value */  
  
  printf("PBM file class %d size with %d columns and %d rows, Max = %d", k, m, n, b);

  if (k > 3) {
    here = ftell(f);
    fclose(f);
    f = fopen(fn, "rb");
    here++;
    if (fseek(f, here, 0) != 0) {
      printf("Input PBM: Sync Error!, file '%s'. Use ASCII PGM.\n", fn);
      exit(3);
    }
  }

  if (k == 3 | k == 6) {
    printf("This is a color image!\n");
  }
  else {
    im = (aipcv_image_t_ptr)aipcv_new_image(n, m);
    im->info->oi = PBM_SE_ORIGIN_ROW;
    im->info->oj = PBM_SE_ORIGIN_COL;
    PBM_SE_ORIGIN_ROW = 0;
    PBM_SE_ORIGIN_COL = 0;

    for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
        if (k < 3) {
          fscanf(f, "%d", &val);
          im->data[i][j] = (uint8_t)(val);
        }
        else {
          fscanf(f, "%c", &ucval);
          im->data[i][j] = ucval;
        }
      }
    }
  }

  return im;
}