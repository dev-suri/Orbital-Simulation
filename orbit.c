#include < stdio.h > #include < math.h >
    //
    // https://physics.nist.gov/cgi-bin/cuu/Value?bg
    //
    #define G 6.674e-11 // (m^3)(kg^-1)(s^-2)
//
// https://ssd.jpl.nasa.gov/?planet_phys_par
//
# define M 5.972e+24 // kg
# define R 6.371e+6 // m
//
// parameters
//
# define DT 300 // s
//
int main() {
    //
    // time intervals - duration is 90 minutes
    //
    int n = (int)(0.5 + (150 * 60 * 60) / DT);
    //
    //////////////////////////////////////////////////
    //
    double t[n];
    double x[n];
    double y[n];
    double vx[n];
    double vy[n];
    //
    double vmoonx[n];
    double vmoony[n];
    double moonx[n];
    double moony[n];
    //////////////////////////////////////////////////
    //
    FILE * fout;
    //
    int j;
    //
    double r, a;
    //
    double m = 28801.0; // kg - Apollo 11
    //double m = 419725.0 ; // kg - ISS
    //
    //////////////////////////////////////////////////
    //
    // International Space Station
    //
    // https://www.nasa.gov/sites/default/files/atoms/filedos/np-2015-05-022-jsc-iss-guide-2015-update-111015-508c.pdf
    //
    // Page 54 - altitude : 370 km to 460 km 
    // Page 54 - speed    : 28,000 km per hour
    //

    //another way to free return:
    //speed = 1257.048;
    //angle = 62.0
    //previous way to free return:
    //speed 1527.048;
    //angle = 24.0
    //--- another------
    //speed 1679.976
    //angle 20.0
    double speed = 1375.0;
    double angle = 27.95;
    double dist = 202751774.4;
    t[0] = 0.0;
    x[0] = dist * cos((angle * 3.1416) / 180.0);
    y[0] = dist * sin((angle * 3.1416) / 180.0);
    vx[0] = speed * cos((angle * 3.1416) / 180.0); // change for more elliptical - and change duration
    vy[0] = speed * sin((angle * 3.1416) / 180.0);
    //
    double massmoon = 7.35e+22;

    vmoonx[0] = 0;
    vmoony[0] = sqrt(G * (M + massmoon) / (2 * dist));
    moonx[0] = 2 * dist;
    moony[0] = 0;
    double moonr;
    //////////////////////////////////////////////////
    //
    for (j = 1; j < n; j++) {
        t[j] = t[j - 1] + DT;
        x[j] = x[j - 1] + DT * vx[j - 1];
        y[j] = y[j - 1] + DT * vy[j - 1];
        //
        r = sqrt(x[j - 1] * x[j - 1] + y[j - 1] * y[j - 1]);
        //
        //	a = G * M / ( r * r ) ;
        //

        moonx[j] = moonx[j - 1] + DT * vmoonx[j - 1];
        moony[j] = moony[j - 1] + DT * vmoony[j - 1];
        //
        moonr = sqrt(moonx[j - 1] * moonx[j - 1] + moony[j - 1] * moony[j - 1]);
        //
        a = G * (massmoon + M) / (moonr * moonr);
        //
        vmoonx[j] = vmoonx[j - 1] - a * (moonx[j - 1] / moonr) * DT;
        vmoony[j] = vmoony[j - 1] - a * (moony[j - 1] / moonr) * DT;

        double forceearth;
        double forcemoon;

        forceearth = (M * G * m) / ((x[j] * x[j]) + (y[j] * y[j]));
        forcemoon = (massmoon * G * m) / (((x[j] - moonx[j]) * (x[j] - moonx[j])) +
            ((y[j] - moony[j]) * (y[j] - moony[j])));
        double angleearth;
        double anglemoon;
        angleearth = atan2(y[j], x[j]);
        anglemoon = atan2(y[j] - moony[j], x[j] - moonx[j]);

        double force1x = cos(angleearth) * forceearth;
        double force1y = sin(angleearth) * forceearth;
        double force2x = cos(anglemoon) * forcemoon;
        double force2y = sin(anglemoon) * forcemoon;
        double forcex = force1x + force2x;
        double forcey = force1y + force2y;
        double ax = forcex / m;
        double ay = forcey / m;

        vx[j] = vx[j - 1] - ax * (x[j - 1] / r) * DT;
        vy[j] = vy[j - 1] - ay * (y[j - 1] / r) * DT;
    }
    //
    //////////////////////////////////////////////////
    //
    fout = fopen("orbit.txt", "w");
    //
    for (j = 0; j < n; j++) {
        fprintf(fout, "%d %0.16f %0.16f %0.16f %0.16f %0.16f %0.16f %0.16f\n",
            j, t[j], x[j], y[j], vx[j], vy[j], sqrt(vx[j] * vx[j] + vy[j] * vy[j]), sqrt(x[j] * x[j] + y[j] * y[j]));
    }
    //
    fclose(fout);
    //

    fout = fopen("moon.txt", "w");
    for (j = 0; j < n; j++) {
        fprintf(fout, "%d %0.16f %0.16f %0.16f %0.16f %0.16f %0.16f %0.16f\n",
            j, t[j], moonx[j], moony[j], vmoonx[j], vmoony[j], sqrt(vmoonx[j] * vmoonx[j] + vmoony[j] * vmoony[j]), sqrt(moonx[j] * moonx[j] + moony[j] * moony[j]));

    }
    fclose(fout);

    //////////////////////////////////////////////////
    //
    fout = fopen("earth.txt", "w");
    //
    for (j = 0; j <= 360; j++) {
        fprintf(fout, "%0.16f %0.16f\n", R * cos(j * M_PI / 180.0), R * sin(j * M_PI / 180.0));
    }
    //
    fclose(fout);
    //
    // done
    //
    return 0;
}
//
// end of file
//
