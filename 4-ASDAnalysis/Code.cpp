/**
 * Function: extract the accumulated shear displacement (ASD) as described in the manuscript, for each small time interval of the simulation

 * Input:  a single index <id> for output file.
 * Output: text files data_<id>.txt and processed_<id>.csv.

 * This code has been re-edited for better readability
 */


#include <cstdio>
#include <cstring>
#include <cctype>
#include <cmath>
#include <algorithm>

// ----------------------------------------------------------------------
//  Constants
// ----------------------------------------------------------------------
constexpr int NUM_ATOMS = 922560;
constexpr double BOX_X = 688.738268;
constexpr double BOX_Y = 1122.368922;
constexpr double BOX_Z = 19.840867;
constexpr int TOP_K = 150;                // T
constexpr int DEFAULT_LAYERS = 50;        // LAYER
constexpr int TIME_STEP = 30000;

// ----------------------------------------------------------------------
//  Global data
// ----------------------------------------------------------------------
struct IndexDist {
    int index;
    double dist_sq;
};

std::pair<double, double> prev_pos[NUM_ATOMS + 10];
std::pair<double, double> next_pos[NUM_ATOMS + 10];
IndexDist dist_sorted[NUM_ATOMS + 10];

FILE* g_fin = stdin;          // reads the identifier
FILE* g_fout = nullptr;       // data_<id>.txt
FILE* g_fcsv = nullptr;       // processed_<id>.csv

// ----------------------------------------------------------------------
//  Periodic adjustment: bring x close to reference_x by adding/subtracting BOX_X
// ----------------------------------------------------------------------
double adjust_x_periodic(double x, double reference_x) {
    while (x - reference_x > BOX_X / 10.0)   x -= BOX_X;
    while (x - reference_x < -BOX_X * 2.0/3.0) x += BOX_X;
    return x;
}

// ----------------------------------------------------------------------
//  Read one trajectory file into a position array (skips to "c_dis[4]")
// ----------------------------------------------------------------------
void read_trajectory(const char* filename, std::pair<double, double>* pos) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return;   // original silently continues

    char buf[100];
    while (fscanf(fp, "%s", buf), strcmp(buf, "c_dis[4]") != 0);

    // format: id mol x y z vx vy fx fy fz c_dis[4] ...
    for (int i = 1; i <= NUM_ATOMS; ++i) {
        double x, y;
        fscanf(fp, "%*d%*d%lf%lf%*lf%*lf%*lf%*lf%*lf%*lf%*lf%*lf", &x, &y);
        pos[i] = {x, y};
    }
    fclose(fp);
}

// ----------------------------------------------------------------------
//  For a given y threshold, sort atoms by squared distance to (BOX_X/2, y_th)
// ----------------------------------------------------------------------
void sort_atoms_by_distance_to_point(double y_th, IndexDist* out) {
    const double centre_x = BOX_X / 2.0;
    for (int i = 1; i <= NUM_ATOMS; ++i) {
        double dx = prev_pos[i].first - centre_x;
        double dy = prev_pos[i].second - y_th;
        out[i] = {i, dx*dx + dy*dy};
    }
    std::sort(out + 1, out + NUM_ATOMS + 1,
              [](const IndexDist& a, const IndexDist& b) { return a.dist_sq < b.dist_sq; });
}

// ----------------------------------------------------------------------
//  Displacement vector (next - prev) for a given atom index
// ----------------------------------------------------------------------
std::pair<double, double> displacement(int atom_idx) {
    return { next_pos[atom_idx].first - prev_pos[atom_idx].first,
             next_pos[atom_idx].second - prev_pos[atom_idx].second };
}

// ----------------------------------------------------------------------
//  Process one pair of consecutive trajectory files
// ----------------------------------------------------------------------
void process_pair(int baseline_time, int next_time) {
    char file_prev[128], file_next[128];
    snprintf(file_prev, sizeof(file_prev), "data_b.%d.lammpstrj", baseline_time);
    snprintf(file_next, sizeof(file_next), "data_b.%d.lammpstrj", next_time);

    read_trajectory(file_prev, prev_pos);
    read_trajectory(file_next, next_pos);

    double layer_avg_x[DEFAULT_LAYERS + 2] = {0.0};

    // *** This q is preserved across layers***
    double q = 0.0;

    for (int layer = 1; layer <= DEFAULT_LAYERS; ++layer) {
        double y_th = BOX_Y * layer / (DEFAULT_LAYERS + 1.0);

        sort_atoms_by_distance_to_point(y_th, dist_sorted);

        double sum_x = 0.0;

        for (int k = 1; k <= TOP_K; ++k) {
            int idx = dist_sorted[k].index;
            double dx = displacement(idx).first;
            dx = adjust_x_periodic(dx, q);   // uses current q (unchanged during this layer)
            sum_x += dx;
        }

        // *** Update q ONCE after the layer, using the first (nearest) atom ***
        q = adjust_x_periodic(displacement(dist_sorted[1].index).first, q);

        layer_avg_x[layer] = sum_x / TOP_K;
        printf("done layer %d\n", layer);
    }

    // enforce continuity between consecutive layers (wrap around BOX_X)
    for (int ly = 2; ly <= DEFAULT_LAYERS; ++ly) {
        while (layer_avg_x[ly] - layer_avg_x[ly-1] > BOX_X * 10.0) layer_avg_x[ly] -= BOX_X;
        while (layer_avg_x[ly] - layer_avg_x[ly-1] < -BOX_X * 2.0/3.0) layer_avg_x[ly] += BOX_X;
        printf("debug w %d = %lf\n", ly, layer_avg_x[ly]);
    }

    double total_disp = 0.0;
    double disl_selected = 0.0;

    for (int ly = 2; ly <= DEFAULT_LAYERS; ++ly) {
        double delta = layer_avg_x[ly-1] - layer_avg_x[ly];   // original ¦¤ = -w[ly] + w[ly-1]
        printf("¡÷%d = %.5lf\n", ly-1, delta);
        fprintf(g_fout, "%.5lf%s", delta, (ly == DEFAULT_LAYERS) ? ",\n" : ", ");

        total_disp += delta;
        // special layer ranges: 10:14 and 36:40 (inclusive)
        if ((10 <= ly-1 && ly-1 <= 14) || (36 <= ly-1 && ly-1 <= 40))
            disl_selected += delta;
    }

    fprintf(g_fcsv, "%.5lf, %.5lf, %.5lf,\n", disl_selected, total_disp - disl_selected, total_disp);
}

// ----------------------------------------------------------------------
//  Main driver ¨C matches original I/O exactly
// ----------------------------------------------------------------------
void solve() {
    printf("id: ");               // original prompt
    char id[128];
    fscanf(g_fin, "%s", id);

    char fname_data[128], fname_csv[128];
    snprintf(fname_data, sizeof(fname_data), "data_%s.txt", id);
    snprintf(fname_csv, sizeof(fname_csv), "processed_%s.csv", id);
    g_fout = fopen(fname_data, "w");
    g_fcsv = fopen(fname_csv, "w");

    // find first existing file (steps of 30000)
    int baseline = 0;
    char test_name[128];
    do {
        baseline += TIME_STEP;
        snprintf(test_name, sizeof(test_name), "data_b.%d.lammpstrj", baseline);
    } while (fopen(test_name, "r") == NULL);

    // process all consecutive pairs
    for (int next = baseline + TIME_STEP; ; next += TIME_STEP) {
        snprintf(test_name, sizeof(test_name), "data_b.%d.lammpstrj", next);
        FILE* check = fopen(test_name, "r");
        if (!check) break;
        fclose(check);
        process_pair(baseline, next);
        baseline = next;
    }

    fclose(g_fout);
    fclose(g_fcsv);
}

int main() {
    solve();
    return 0;
}
