//for wmadec under Android, under android, no rand(), sincos(), ...
#ifdef ANDROID
extern long lrand48(void);

extern double sin(double x);
extern float sinf(float x);
extern double cos(double x);
extern float cosf(float x);

int rand(void)
{
        return (int)lrand48();
}

void sincos(double x, double* ptr_sin, double* ptr_cos)
{
	if(ptr_sin != (double *)0) {
		*ptr_sin = sin(x);
	}
	if(ptr_cos != (double *)0) {
		*ptr_cos = cos(x);
	}
	return;
}

void sincosf(float x, float* ptr_sin, float* ptr_cos)
{
	if(ptr_sin != (float *)0) {
		*ptr_sin = sinf(x);
	}
	if(ptr_cos != (float *)0) {
		*ptr_cos = cosf(x);
	}
	return;
}


#endif

