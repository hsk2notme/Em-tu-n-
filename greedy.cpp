#include <iostream>
#include <vector>
#include <cstdlib>   
#include <ctime>     
#include <cmath>
#include <numeric>   
#include <algorithm> 

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

class Testbed {
public:
    int k;
    vector<double> true_values; 

    Testbed(const vector<double>& predefined_values) {
        true_values = predefined_values;
        k = true_values.size();
    }

    double get_reward(int action) {
        return random_normal(true_values[action], 1.0);
    }

    int get_optimal_action() {
        return distance(true_values.begin(), max_element(true_values.begin(), true_values.end()));
    }
};


int main() {
    srand(time(0));
    const vector<double> TRUE_Q_VALUES = {0.2, -0.8, 1.5, 0.5, 1.1, -1.2, 0.8, -0.1, 0.0, 0.9};
    const int k = TRUE_Q_VALUES.size();

    const int NUM_STEPS = 1000;
    const int NUM_RUNS = 2000;

    vector<double> average_rewards(NUM_STEPS, 0.0);
    vector<double> optimal_action_percent(NUM_STEPS, 0.0);

    for (int run = 0; run < NUM_RUNS; ++run) {
        Testbed solieu(TRUE_Q_VALUES);
        int optimal_action = solieu.get_optimal_action();

        vector<double> Q(k, 0.0);
        vector<int> N(k, 0);  

        for (int step = 0; step < NUM_STEPS; ++step) {

            int action_chosen = distance(Q.begin(), max_element(Q.begin(), Q.end()));

            double reward = solieu.get_reward(action_chosen);

            N[action_chosen]++;
            Q[action_chosen] += (1.0 / N[action_chosen]) * (reward - Q[action_chosen]);

            average_rewards[step] += reward;
            if (action_chosen == optimal_action) {
                optimal_action_percent[step]++;
            }
        }
    }
    cout << "Step,AverageReward,OptimalActionPercentage\n";
    for (int i = 0; i < NUM_STEPS; ++i) {
        average_rewards[i] /= NUM_RUNS;
        optimal_action_percent[i] = (optimal_action_percent[i] / NUM_RUNS) * 100.0;
        cout << i + 1 << "," << average_rewards[i] << "," << optimal_action_percent[i] << "\n";
    }

    return 0;
}
