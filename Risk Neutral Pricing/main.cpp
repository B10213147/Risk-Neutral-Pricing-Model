#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

float *forward_Price(float S0, float u, float d, int vDate);
float backward_Payoff(float *price, float p, float r, float K, int vDate);

int main()
{
    float p = 0.5; // Up's Probability
    float S0 = 4;   // Starting price
    float u = 2, d = 0.5; // Up/Down pricing rate
    float r = 0.25;    // Interest Rate
    float K = 3;    // Expected price
    int vDate = 3;  // Valid date

    cout << "=====This is Risk Neutral Pricing Model=====" << endl;
    cout << "p:" << p << endl;
    cout << "q:" << 1.0 - p << endl;
    cout << "S0:" << S0 << endl;
    cout << "u:" << u << endl;
    cout << "d:" << d << endl;
    cout << "r:" << r << endl;
    cout << "K:" << K << endl;
    cout << "Valid date:" << vDate << endl;
    /*
    cout << "Please enter the following parameter:" << endl;
    cout << "p:"; cin >> p;
    q = 1.0 - p;
    cout << "q:" << q << endl;
    cout << "S0:"; cin >> S0;
    cout << "u:"; cin >> u;
    cout << "d:"; cin >> d;
    cout << "r:"; cin >> r;
    cout << "K:"; cin >> K;
    cout << "Valid date:"; cin >> vDate;
    */
    cout << "=====Forward S=====" << endl;
    float *last_S = forward_Price(S0, u, d, vDate); // last_S carries an array.

    cout << "=====Backward V=====" << endl;
    float V0 = backward_Payoff(last_S, p, r, K, vDate);
    cout << "V0=" << V0 << endl;

    free(last_S);   // Free memory space.
    return 0;
}

/**
  * @brief  Calculate the price at valid date.
  * @param  S0: Starting price.
  * @param  u: Up pricing rate.
  * @param  d: Down pricing rate.
  * @param  vDate: Valid date.
  * @retval Pointer(array) of the last S.
  */
float *forward_Price(float S0, float u, float d, int vDate){
    // Apply a one space array for initial date.
    float *cur_S = (float*)malloc(sizeof(float));
    cur_S[0] = S0;

    for(int next_d = 1; next_d <= vDate; next_d++){
        // Apply a 2^next_d spaces array for next date.
        float *next_S = (float*)malloc(sizeof(float) * pow(2, next_d));

        for(int i = 0; i < pow(2, next_d - 1); i++){
            // Next date will split into two states.
            next_S[2 * i] = u * cur_S[i];
            next_S[2 * i + 1] = d * cur_S[i];
        }

        free(cur_S);    // Free memory space.
        cur_S = next_S; // Point to the next date.
    }

    return cur_S;
}

/**
  * @brief  Calculate the payoff of the option.
  * @param  price: Pointer(array) to price on valid date.
  * @param  p: Up's probability.
  * @param  r: Interest rate.
  * @param  K: Expected price.
  * @param  vDate: Valid date.
  * @retval V0.
  */
float backward_Payoff(float *price, float p, float r, float K, int vDate){
    float q = 1.0 - p;  // Down's probability
    // Apply a 2^vDate spaces array for last date.
    float *cur_V = (float*)malloc(sizeof(float) * pow(2, vDate));

    for(int i = 0; i < pow(2, vDate); i++){
        float Vn = price[i] - K;  // Vn = (Sn - K).
        if(Vn < 0) Vn = 0;
        cur_V[i] = Vn;
    }

    for(int pre_d = vDate - 1; pre_d >= 0; pre_d--){
        // Apply a 2^pre_d spaces array for previous date.
        float *pre_V = (float*)malloc(sizeof(float) * pow(2, pre_d));

        for(int i = 0; i < pow(2, pre_d); i++){
            // Calculate previous V.
            float temp = (p * cur_V[2 * i] + q * cur_V[2 * i + 1]) / (1 + r);
            if(temp < 0) temp = 0;
            pre_V[i] = temp;
        }

        free(cur_V);    // Free memory space.
        cur_V = pre_V;  // Point to the previous date.
    }

    float V0 = cur_V[0];
    free(cur_V);    // Free memory space.
    return V0;
}
