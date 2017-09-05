#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/test.h"

float **test_sphereVolume(int nbFeatureSamples) {
    float **samples = malloc(nbFeatureSamples * sizeof(float *));
    fprintf(stdout, "id, x, y, z, radius, volume\n");
    for (int i = 0; i < nbFeatureSamples; ++i) {
        samples[i] = malloc(5 * sizeof(float));
        samples[i][0] = ((float)rand()/(float)(RAND_MAX)) * 10.0 - 10.0 / 2;
        samples[i][1] = ((float)rand()/(float)(RAND_MAX)) * 10.0 - 10.0 / 2;
        samples[i][2] = ((float)rand()/(float)(RAND_MAX)) * 10.0 - 10.0 / 2;
        samples[i][3] = ((float)rand()/(float)(RAND_MAX)) * 10.0;
        samples[i][4] = M_PI * 4.0 / 3.0 * samples[i][3] * samples[i][3] * samples[i][3];

        
        fprintf(stdout, "%i, %f, %f, %f, %f, %f\n", i, samples[i][0], samples[i][1], samples[i][2], samples[i][3], samples[i][4]);
        // fprintf(stdout, "SAMPLE (%f, %f, %f) r(%f) = %f\n", samples[i][0], samples[i][1], samples[i][2], samples[i][3], samples[i][4]);
    }

    return samples;
}

float **test_sumABC(int nbFeatureSamples) {
    float **samples = malloc(nbFeatureSamples * sizeof(float *));
    fprintf(stdout, "id, a, b, c, d, result\n");
    for (int i = 0; i < nbFeatureSamples; ++i) {
        samples[i] = malloc(5 * sizeof(float));
        samples[i][0] = ((float)rand()/(float)(RAND_MAX)) * 10.0 - 10.0 / 2;
        samples[i][1] = ((float)rand()/(float)(RAND_MAX)) * 10.0 - 10.0 / 2;
        samples[i][2] = ((float)rand()/(float)(RAND_MAX)) * 10.0 - 10.0 / 2;
        samples[i][3] = ((float)rand()/(float)(RAND_MAX)) * 10.0;
        samples[i][4] = samples[i][0] + samples[i][1] + samples[i][2];

        fprintf(stdout, "%i, %f, %f, %f, %f, %f\n", i, samples[i][0], samples[i][1], samples[i][2], samples[i][3], samples[i][4]);
        // fprintf(stdout, "SAMPLE (%f, %f, %f) r(%f) = %f\n", samples[i][0], samples[i][1], samples[i][2], samples[i][3], samples[i][4]);
    }

    return samples;
}

float **test_cubicXYZ(int nbFeatureSamples) {
    // 7x^3 + 4y^2 - 3z + 12
    fprintf(stdout, "id, x, y, z, w, result\n");
    float **samples = malloc(nbFeatureSamples * sizeof(float *));
    for (int i = 0; i < nbFeatureSamples; ++i) {
        samples[i] = malloc(5 * sizeof(float));
        samples[i][0] = ((float)rand()/(float)(RAND_MAX)) * 10.0 - 10.0 / 2;
        samples[i][1] = ((float)rand()/(float)(RAND_MAX)) * 10.0 - 10.0 / 2;
        samples[i][2] = ((float)rand()/(float)(RAND_MAX)) * 10.0 - 10.0 / 2;
        samples[i][3] = ((float)rand()/(float)(RAND_MAX)) * 10.0;
        samples[i][4] = 7 * samples[i][0] * samples[i][0] * samples[i][0] + 4 * samples[i][1] * samples[i][1] - 3 * samples[i][2] + 12;

        fprintf(stdout, "%i, %f, %f, %f, %f, %f\n", i, samples[i][0], samples[i][1], samples[i][2], samples[i][3], samples[i][4]);
        // fprintf(stdout, "SAMPLE (%f, %f, %f) r(%f) = %f\n", samples[i][0], samples[i][1], samples[i][2], samples[i][3], samples[i][4]);
    }

    return samples;
}