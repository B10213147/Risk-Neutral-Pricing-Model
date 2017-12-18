#include <iostream>
#include <cstdlib>

using namespace std;

float *forward_Price(float S0, float u, float d, int vDate);
float backward_Payoff(float *price, float p, float q, float r, float K, int vDate);

int main()
{
    float p = 0.5, q = 1 - p; // Up/Down's Probability
    float S0 = 4;   // Starting price
    float u = 2, d = 0.5; // Up/Down pricing rate
    float r = 0.25;    // Interest Rate
    float K = 3;    // Expected price
    int vDate = 3;  // Valid date

    cout << "=====This is Risk Neutral Pricing Model=====" << endl;
    cout << "p:" << p << endl;
    cout << "q:" << q << endl;
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
    float *last_price_list = forward_Price(S0, u, d, vDate);
    cout << "=====Backward V=====" << endl;
    float V0 = backward_Payoff(last_price_list,
                               p, q, r, K, vDate);

    //cout << "V0=" << V0 << endl;
    free(last_price_list);
    return 0;
}

float *forward_Price(float S0, float u, float d, int vDate){
    float *cur_array = (float*)malloc(sizeof(float));
    cur_array[0] = S0;
    cout << "S0> " << S0 << endl;
    int cur_d = 1;
    while(cur_d <= vDate){
        float *next_array = (float*)malloc(sizeof(float) * (0x1 << cur_d));
        for(int i = 0; i < (0x1 << (cur_d - 1)); i++){
            next_array[2*i] = u * cur_array[i];
            next_array[2*i + 1] = d * cur_array[i];
        }
        free(cur_array);
        cur_array = next_array;
        cout << 'S' << cur_d << "> ";
        for(int i = 0; i < 0x1 << cur_d; i++){
            cout << cur_array[i] << ' ';
        }
        cout << endl;
        cur_d++;
    }
    return cur_array;
}

float backward_Payoff(float *price, float p, float q, float r, float K, int vDate){
    float *cur_V = (float*)malloc(sizeof(float) * (0x1 << vDate));
    cout << 'V' << vDate << "> ";
    for(int i = 0; i < 0x1 << vDate; i++){
        float temp = price[i] - K;
        if(temp < 0) temp = 0;
        cur_V[i] = temp;
        cout << cur_V[i] << ' ';
    }
    cout << endl;
    vDate--;
    while(vDate >= 0){
        float *next_V = (float*)malloc(sizeof(float) * (0x1 << vDate));
        cout << 'V' << vDate << "> ";
        for(int i = 0; i < (0x1 << vDate); i++){
            float temp = (p * cur_V[2*i] + q * cur_V[2*i + 1]) / (1 + r);
            if(temp < 0) temp = 0;
            next_V[i] = temp;
            cout << next_V[i] << ' ';
        }
        cout << endl;
        free(cur_V);
        cur_V = next_V;
        vDate--;
    }

    float V0 = cur_V[0];
    free(cur_V);
    return V0;
}
