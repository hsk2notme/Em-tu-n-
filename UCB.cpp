#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <limits>
#include <numeric>  
#include <algorithm> 
#include <utility>   

using namespace std;
double random_normal(double mi, double sigma) {
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
    BanditTestbed(const vector<double>& real_value) {
        true_values = real_value;
        k = true_values.size();
    }

    double get_reward(int action) {
        return random_normal(true_values[action], 1.0);
    }
};
pair<vector<double>, vector<double> > run_ucb_experiment(const vector<double>& real_value, int num_steps, int num_runs, double c) {
    

    int optimal_action = distance(real_value.begin(), max_element(real_value.begin(), real_value.end()));
    
    vector<double> average_rewards(num_steps, 0.0);
    vector<double> optimal_action_percentage(num_steps, 0.0);
    int k = real_value.size();

    for (int run = 0; run < num_runs; ++run) {
        BanditTestbed testbed(real_value);
        vector<double> Q(k, 0.0);
        vector<int> N(k, 0);   

        for (int step = 0; step < num_steps; ++step) {
            int action_to_take = -1;


            for (int i = 0; i < k; ++i) {
                if (N[i] == 0) {
                    action_to_take = i;
                    break;
                }
            }


            if (action_to_take == -1) {
                double max_ucb = -numeric_limits<double>::max();
                int best_action_ucb = -1;
                for (int i = 0; i < k; ++i) {
                    double bonus = c * sqrt(log(step + 1) / N[i]);
                    double ucb_value = Q[i] + bonus;
                    if (ucb_value > max_ucb) {
                        max_ucb = ucb_value;
                        best_action_ucb = i;
                    }
                }
                action_to_take = best_action_ucb;
            }

            double reward = testbed.get_reward(action_to_take);

            N[action_to_take]++;
            Q[action_to_take] += (1.0 / N[action_to_take]) * (reward - Q[action_to_take]);

            average_rewards[step] += reward;
            if (action_to_take == optimal_action) {
                optimal_action_percentage[step]++;
            }
        }
    }

    for (int i = 0; i < num_steps; ++i) {
        average_rewards[i] /= num_runs;
        optimal_action_percentage[i] = (optimal_action_percentage[i] / num_runs) * 100.0;
    }

    return {average_rewards, optimal_action_percentage};
}

int main() {
    srand(time(0));

    const vector<double> real_value = {0.2, -0.8, 1.5, 0.5, 1.1, -1.2, 0.8, -0.1, 0.0, 0.9};
    int k = real_value.size();
    int optimal_action = distance(real_value.begin(), max_element(real_value.begin(), real_value.end()));
	const int NUM_STEPS = 1000;
    const int NUM_RUNS = 2000;

    while (true) {
        double c;
        cout << "c="; cin >> c;
        if (c < 0) {
            cout << "Thoat chuong trinh." << endl;
            break;
        }
        pair<vector<double>, vector<double>> results = run_ucb_experiment(real_value, NUM_STEPS, NUM_RUNS, c);

        vector<double> avg_rewards = results.first;
        vector<double> opt_percentages = results.second;

        cout << "Step,AverageReward,OptimalActionPercentage\n";
        for (int i = 0; i < NUM_STEPS; ++i) {
            cout << i + 1 << "," << avg_rewards[i] << "," << opt_percentages[i] << "\n";
        }
        cout << endl;
    }

    return 0;
}
