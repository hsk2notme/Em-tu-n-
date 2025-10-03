#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

double random(double mi, double sigma) {
    static bool call = false;
    static double spare;

    if (call) {
        call = false;
        return mi + sigma * spare;
    }

    call = true;
    double x, y, A;
    do {
        x = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        y = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        A = x * x + y * y;
    } while (A >= 1.0 || A == 0.0);

    A = sqrt(-2.0 * log(A) / A);
    spare = y * A;
    return mi + sigma * x * A;
}

class BanditTestbed {
public:
    int k;
    vector<double> true_values;

    BanditTestbed(int num_actions) {
        k = num_actions;
        true_values.resize(k);
        for (int i = 0; i < k; ++i) {
            true_values[i] = random(0.0, 1.0);
        }
    }

    double get_reward(int action) {
        return random(true_values[action], 1.0);
    }
};

vector<double> OIV_test(int k, int num_steps, int num_runs, double fake_q, double alpha) {
    vector<double> average_rewards(num_steps, 0.0);

    for (int run = 0; run < num_runs; ++run) {
        BanditTestbed testbed(k);
        vector<double> Q(k, fake_q);
        for (int step = 0; step < num_steps; ++step) {
            int action_to_take = 0;
            for (int i = 1; i < k; ++i) {
                if (Q[i] > Q[action_to_take]) {
                    action_to_take = i;
                }
            }

            double reward = testbed.get_reward(action_to_take);
            Q[action_to_take] += alpha * (reward - Q[action_to_take]);
            average_rewards[step] += reward;
        }
    }

    for (int i = 0; i < num_steps; ++i) {
        average_rewards[i] /= num_runs;
    }

    return average_rewards;
}


int main() {
    srand(time(0));
    const int k = 10;
    const int NUM_STEPS = 1000;
    const int NUM_RUNS = 2000;
    while (true) {
        double fake_q;
        double alpha;
        vector<double> oiv_results = OIV_test(k, NUM_STEPS, NUM_RUNS, fake_q, alpha);

        cout << "Step,AvgReward,Q1="; cin >> fake_q; cout << ",alpha="; cin >> alpha;
        for (int i = 0; i < NUM_STEPS; ++i) {
            cout << i + 1 << "," << oiv_results[i] << "\n";
        }
    }

    return 0;
}
