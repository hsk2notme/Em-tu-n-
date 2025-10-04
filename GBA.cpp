#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
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

    BanditTestbed(const vector<double>& true_q_values) {
        true_values = true_q_values;
        k = true_values.size();
    }

    double get_reward(int action) {
        return random_normal(true_values[action], 1.0);
    }
};

pair<vector<double>, vector<double>> run_gradient_bandit_experiment(
    const vector<double>& true_q_values, int num_steps, int num_runs, double alpha) {
    

    int optimal_action = distance(true_q_values.begin(), max_element(true_q_values.begin(), true_q_values.end()));
    
    vector<double> average_rewards(num_steps, 0.0);
    vector<double> optimal_action_percentage(num_steps, 0.0);
    int k = true_q_values.size();

    for (int run = 0; run < num_runs; ++run) {
        BanditTestbed testbed(true_q_values);
        vector<double> H(k, 0.0); 
        double baseline = 0.0;   

        for (int step = 0; step < num_steps; ++step) {
      
            vector<double> pi(k);
            double sum_exp_H = 0.0;
            for (int i = 0; i < k; ++i) {
                sum_exp_H += exp(H[i]);
            }
            for (int i = 0; i < k; ++i) {
                pi[i] = exp(H[i]) / sum_exp_H;
            }

            int action_to_take;
            double rand_num = (double)rand() / RAND_MAX;
            double cumulative_prob = 0.0;
            for (int i = 0; i < k; ++i) {
                cumulative_prob += pi[i];
                if (rand_num < cumulative_prob) {
                    action_to_take = i;
                    break;
                }
            }

            double reward = testbed.get_reward(action_to_take);

            average_rewards[step] += reward;
            
            if (action_to_take == optimal_action) {
                optimal_action_percentage[step]++;
            }

            baseline += (1.0 / (step + 1)) * (reward - baseline);


            for (int i = 0; i < k; ++i) {
                if (i == action_to_take) {
                    H[i] += alpha * (reward - baseline) * (1 - pi[i]);
                } else {
                    H[i] -= alpha * (reward - baseline) * pi[i];
                }
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
    const vector<double> TRUE_Q_VALUES = {0.2, -0.8, 1.5, 0.5, 1.1, -1.2, 0.8, -0.1, 0.0, 0.9};
    int k = TRUE_Q_VALUES.size();
    int optimal_action = distance(TRUE_Q_VALUES.begin(), max_element(TRUE_Q_VALUES.begin(), TRUE_Q_VALUES.end()));

    const int NUM_STEPS = 1000;
    const int NUM_RUNS = 2000;

    while (true) {
        double alpha;
		cout << "alpha="; cin >> alpha;

        if (alpha < 0) {
            cout << "Thoat chuong trinh." << endl;
            break;
        }
        pair<vector<double>, vector<double>> results = run_gradient_bandit_experiment(TRUE_Q_VALUES, NUM_STEPS, NUM_RUNS, alpha);
        
        vector<double> avg_rewards = results.first;
        vector<double> opt_percentages = results.second;
        
        cout << "Step,AverageReward,OptimalActionPercentage\n";
        for (int i = 0; i < NUM_STEPS; ++i) {
            cout << i + 1 << "," << avg_rewards[i] << "," << opt_percentages[i] << "\n";
        }
    }

    return 0;
}
