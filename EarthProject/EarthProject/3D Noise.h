#ifndef NOISE3D
#define NOISE3D
class Noise3D
{
public:
	
	double CompleteNoise(double x, double y, double z, int octaves, double pers, double i);
	
	double grad(int  hash, double x, double y, double z);
	double noise(double x, double y, double z);
	double fade(double t){ return t * t * t * (t * (t * 6 - 15) + 10); }
    double lerp(double t, double a, double b) { return a + t * (b - a); }
	
	Noise3D();

private:
	
	
};
#endif
Noise3D & Instance();