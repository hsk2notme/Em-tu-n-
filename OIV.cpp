#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <utility>
#include <algorithm>
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

    BanditTestbed(const vector<double>& predefined_values) {
        k = predefined_values.size();
        true_values = predefined_values;
    }

    double get_reward(int action) {
        return random(true_values[action], 1.0);
    }

    int get_optimal_action() {

        return distance(true_values.begin(), max_element(true_values.begin(), true_values.end()));
    }
};

pair<vector<double>, vector<double>> OIV_test(BanditTestbed& testbed, int num_steps, int num_runs, double fake_q, double alpha) {
    vector<double> average_rewards(num_steps, 0.0);
    vector<double> optimal_action_percent(num_steps, 0.0);
    int k = testbed.k;
    int optimal_action = testbed.get_optimal_action();

    for (int run = 0; run < num_runs; ++run) {
        vector<double> Q(k, fake_q);

        for (int step = 0; step < num_steps; ++step) {
            int action_to_take = distance(Q.begin(), max_element(Q.begin(), Q.end()));

            double reward = testbed.get_reward(action_to_take);

            Q[action_to_take] += alpha * (reward - Q[action_to_take]);

            average_rewards[step] += reward;
            if (action_to_take == optimal_action) {
                optimal_action_percent[step]++;
            }
        }
    }

    for (int i = 0; i < num_steps; ++i) {
        average_rewards[i] /= num_runs;
        optimal_action_percent[i] = (optimal_action_percent[i] / num_runs) * 100.0;
    }

    return {average_rewards, optimal_action_percent};
}


int main() {
    srand(time(0));
    vector<double> my_true_values = {0.2, -0.8, 1.5, 0.4, 1.1, -1.5, 2.0, -0.5, 0.9, 0.6};
    BanditTestbed custom_testbed(my_true_values);
    int k = custom_testbed.k;
    const int NUM_STEPS = 1000;
    const int NUM_RUNS = 2000;

    while (true) {
        double fake_q;
        double alpha;
        cout << "Q1="; cin >> fake_q; cout << ",alpha="; cin >> alpha;

        if (fake_q < 0) {
            cout << "Thoat chuong trinh." << endl;
            break;
        }
        auto results_pair = OIV_test(custom_testbed, NUM_STEPS, NUM_RUNS, fake_q, alpha);
        vector<double> oiv_rewards = results_pair.first;
        vector<double> oiv_optimal = results_pair.second;
        for (int i = 0; i < NUM_STEPS; ++i) {
            cout << i + 1 << "," << oiv_rewards[i] << "," << oiv_optimal[i] << "\n";
        }
    }

    return 0;
}
