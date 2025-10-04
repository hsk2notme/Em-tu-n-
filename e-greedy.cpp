#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm> 
#include <utility>   
#include <string>    

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


pair<vector<double>, vector<double>> run_epsilon_greedy_experiment(
    const vector<double>& true_q_values, int num_steps, int num_runs, double epsilon) {
    
    int k = true_q_values.size();
    int optimal_action = distance(true_q_values.begin(), max_element(true_q_values.begin(), true_q_values.end()));
    
    vector<double> average_rewards(num_steps, 0.0);
    vector<double> optimal_action_percentage(num_steps, 0.0);

    for (int i = 0; i < num_runs; ++i) {
        BanditTestbed testbed(true_q_values);
        vector<double> Q(k, 0.0);
        vector<int> N(k, 0); 

        for (int j = 0; j < num_steps; ++j) {
            int action_to_take;
            if (((double)rand() / RAND_MAX) < epsilon) {
                action_to_take = rand() % k;
            } else {
                action_to_take = distance(Q.begin(), max_element(Q.begin(), Q.end()));
            }
            
            double reward = testbed.get_reward(action_to_take);
            
            N[action_to_take]++;
            Q[action_to_take] += (1.0 / N[action_to_take]) * (reward - Q[action_to_take]);
            average_rewards[j] += reward;

            if (action_to_take == optimal_action) {
                optimal_action_percentage[j]++;
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
    const int NUM_STEPS = 1000;
    const int NUM_RUNS = 2000;

    while (true) {
        double epsilon;
        cout << "epsilon: ";
        cin >> epsilon;
        if (epsilon < 0) {
            cout << "Thoat chuong trinh." << endl;
            break;
        }
        if (epsilon > 1) {
            cout << "Epsilon khong hop le (phai trong doan [0, 1]). Vui long nhap lai.\n";
            continue;
        }
        pair<vector<double>, vector<double>> results = run_epsilon_greedy_experiment(TRUE_Q_VALUES, NUM_STEPS, NUM_RUNS, epsilon);
        vector<double> avg_rewards = results.first;
        vector<double> opt_percentages = results.second;
        cout << "Step,AverageReward,OptimalActionPercentage\n";
        for (int i = 0; i < NUM_STEPS; ++i) {
            cout << i + 1 << "," << avg_rewards[i] << "," << opt_percentages[i] << "\n";
        }
    }

    return 0;
}
