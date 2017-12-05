typedef struct {
  int quot, rem;
} _my_div_t;

// Our 8cc doesn't support returning a structure value.
// TODO: update 8cc.
void my_div(unsigned int a, unsigned int b, _my_div_t* o) {
  unsigned int d[24];
  unsigned int r[24];
  unsigned int i;
  r[0] = 1;
  for (i = 0;; i++) {
    d[i] = b;
    unsigned int nb = b + b;
    if (nb > a || nb < b)
      break;
    r[i+1] = r[i] + r[i];
    b = nb;
  }

  unsigned int q = 0;
  for (;; i--) {
    unsigned int v = d[i];
    if (a >= v) {
      q += r[i];
      a -= v;
    }
    if (i == 0)
      break;
  }
  o->quot = q;
  o->rem = a;
}

static unsigned int __builtin_div(unsigned int a, unsigned int b) {
  if (b == 1)
    return a;
  _my_div_t r;
  my_div(a, b, &r);
  return r.quot;
}

static unsigned int __builtin_mod(unsigned int a, unsigned int b) {
  _my_div_t r;
  my_div(a, b, &r);
  return r.rem;
}

int putchar(int);
static void print_int(int _) { }

int main() {
  int i;
  for (i = 1; i <= 100; i++) {
    int done = 0;
    if (i % 3 == 0) {
      putchar('F');
      putchar('i');
      putchar('z');
      putchar('z');
      done = 1;
    }
    if (i % 5 == 0) {
      putchar('B');
      putchar('u');
      putchar('z');
      putchar('z');
      done = 1;
    }

    if (!done)
      print_int(i);
    putchar('\n');
  }
  return 0;
}
