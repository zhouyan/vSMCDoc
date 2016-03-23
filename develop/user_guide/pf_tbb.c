#include <vsmc/vsmc.h>
#include <math.h>
#include <stdio.h>

static const int PosX = 0;
static const int PosY = 1;
static const int VelX = 2;
static const int VelY = 3;

// Storage for data
static int data_size = 0;
static int pf_obs_x_size = 0;
static int pf_obs_y_size = 0;
static double *pf_obs_x = NULL;
static double *pf_obs_y = NULL;

// Temporaries used by pf_init and pf_move
static int pf_pos_x_size = 0;
static int pf_pos_y_size = 0;
static int pf_vel_x_size = 0;
static int pf_vel_y_size = 0;
static int pf_weight_size = 0;
static double *pf_pos_x = NULL;
static double *pf_pos_y = NULL;
static double *pf_vel_x = NULL;
static double *pf_vel_y = NULL;
static double *pf_weight = NULL;

static void pf_resize(double **ptr, int *size, int new_size)
{
    if (*ptr && *size != new_size)
        vsmc_free(*ptr);
    if (!*ptr)
        *ptr = vsmc_malloc(sizeof(double) * new_size, 32);
    *size = new_size;
}

static double pf_log_likelihood(int t, const vsmc_single_particle *sp)
{
    double llh_x = 10 * (sp->state[PosX] - pf_obs_x[t]);
    double llh_y = 10 * (sp->state[PosY] - pf_obs_y[t]);
    llh_x = log(1 + llh_x * llh_x / 10);
    llh_y = log(1 + llh_y * llh_y / 10);

    return -0.5 * (10 + 1) * (llh_x + llh_y);
}

static void pf_read_data(const char *param)
{
    if (!param)
        return;

    FILE *data = fopen(param, "r");
    int n = 0;
    while (1) {
        double x;
        double y;
        int nx = fscanf(data, "%lg", &x);
        int ny = fscanf(data, "%lg", &y);
        if (nx == 1 && ny == 1)
            ++n;
        else
            break;
    }
    pf_resize(&pf_obs_x, &pf_obs_x_size, n);
    pf_resize(&pf_obs_y, &pf_obs_y_size, n);
    data_size = n;
    fseek(data, 0, SEEK_SET);
    for (int i = 0; i < data_size; ++i) {
        fscanf(data, "%lg", pf_obs_x + i);
        fscanf(data, "%lg", pf_obs_y + i);
    }
    fclose(data);
}

void pf_init_param(vsmc_particle particle, void *param)
{
    pf_read_data((const char *) param);
}

void pf_init_pre(vsmc_particle particle)
{
    vsmc_rng rng = vsmc_particle_rng(particle, 0);
    const int size = vsmc_particle_size(particle);
    const double sd_pos = 2;
    const double sd_vel = 1;
    pf_resize(&pf_pos_x, &pf_pos_x_size, size);
    pf_resize(&pf_pos_y, &pf_pos_y_size, size);
    pf_resize(&pf_vel_x, &pf_vel_x_size, size);
    pf_resize(&pf_vel_y, &pf_vel_y_size, size);
    pf_resize(&pf_weight, &pf_weight_size, size);
    vsmc_normal_distribution(rng, size, pf_pos_x, 0, sd_pos);
    vsmc_normal_distribution(rng, size, pf_pos_y, 0, sd_pos);
    vsmc_normal_distribution(rng, size, pf_vel_x, 0, sd_vel);
    vsmc_normal_distribution(rng, size, pf_vel_y, 0, sd_vel);
}

int pf_init_sp(vsmc_single_particle sp)
{
    sp.state[PosX] = pf_pos_x[sp.id];
    sp.state[PosY] = pf_pos_y[sp.id];
    sp.state[VelX] = pf_vel_x[sp.id];
    sp.state[VelY] = pf_vel_y[sp.id];
    pf_weight[sp.id] = pf_log_likelihood(0, &sp);

    return 0;
}

void pf_init_post(vsmc_particle particle)
{
    vsmc_weight_set_log(vsmc_particle_weight(particle), pf_weight, 1);
}

void pf_move_pre(int t, vsmc_particle particle)
{
    vsmc_rng rng = vsmc_particle_rng(particle, 0);
    const int size = vsmc_particle_size(particle);
    const double sd_pos = sqrt(0.02);
    const double sd_vel = sqrt(0.001);
    pf_resize(&pf_pos_x, &pf_pos_x_size, size);
    pf_resize(&pf_pos_y, &pf_pos_y_size, size);
    pf_resize(&pf_vel_x, &pf_vel_x_size, size);
    pf_resize(&pf_vel_y, &pf_vel_y_size, size);
    pf_resize(&pf_weight, &pf_weight_size, size);
    vsmc_normal_distribution(rng, size, pf_pos_x, 0, sd_pos);
    vsmc_normal_distribution(rng, size, pf_pos_y, 0, sd_pos);
    vsmc_normal_distribution(rng, size, pf_vel_x, 0, sd_vel);
    vsmc_normal_distribution(rng, size, pf_vel_y, 0, sd_vel);
}

int pf_move_sp(int t, vsmc_single_particle sp)
{
    sp.state[PosX] += pf_pos_x[sp.id] + 0.1 * sp.state[VelX];
    sp.state[PosY] += pf_pos_y[sp.id] + 0.1 * sp.state[VelY];
    sp.state[VelX] += pf_vel_x[sp.id];
    sp.state[VelY] += pf_vel_y[sp.id];
    pf_weight[sp.id] = pf_log_likelihood(t, &sp);

    return 0;
}

void pf_move_post(int t, vsmc_particle particle)
{
    vsmc_weight_add_log(vsmc_particle_weight(particle), pf_weight, 1);
}

void pf_eval_sp(int t, int dim, vsmc_single_particle sp, double *r)
{
    r[0] = sp.state[PosX];
    r[1] = sp.state[PosY];
}

int main(int argc, char **argv)
{
    int N = 10000;
    if (argc > 1)
        N = atoi(argv[1]);

    vsmc_sampler sampler;
    vsmc_sampler_malloc(&sampler, N, 4, vSMCMultinomial, 0.5);
    vsmc_sampler_init_tbb(
        sampler, pf_init_sp, pf_init_param, pf_init_pre, pf_init_post);
    vsmc_sampler_move_tbb(sampler, pf_move_sp, pf_move_pre, pf_move_post, 0);
    vsmc_sampler_set_monitor_tbb(
        sampler, "pos", 2, pf_eval_sp, NULL, NULL, 0, vSMCMonitorMCMC);

    vsmc_stop_watch watch;
    vsmc_stop_watch_malloc(&watch);
    vsmc_stop_watch_start(watch);
    vsmc_sampler_initialize(sampler, (void *) "pf.data");
    vsmc_sampler_iterate(sampler, data_size - 1);
    vsmc_stop_watch_stop(watch);
    printf("Time (ms): %lg\n", vsmc_stop_watch_milliseconds(watch));
    vsmc_stop_watch_free(&watch);

    vsmc_sampler_save_f(sampler, "pf.out");

    vsmc_sampler_free(&sampler);
    vsmc_free(pf_obs_x);
    vsmc_free(pf_obs_y);
    vsmc_free(pf_pos_x);
    vsmc_free(pf_pos_y);
    vsmc_free(pf_vel_x);
    vsmc_free(pf_vel_y);
    vsmc_free(pf_weight);

    return 0;
}
