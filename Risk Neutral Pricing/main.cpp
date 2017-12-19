/*
 * main.c
 *
 *  Created on: December 19, 2017
 *      Author: Harvard Tseng
 */

/* Includes ------------------------------------------------------------------*/
#include <iostream>
#include <cstdlib>
#include <cmath>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
using namespace std;

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
double *forward_Price(float S0, float u, float d, int vDate);
double backward_Payoff(double *price, float p, float r, float K, int vDate);

/* Private variables ---------------------------------------------------------*/

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
    double *last_S = forward_Price(S0, u, d, vDate);

    cout << "=====Backward V=====" << endl;
    float V0 = backward_Payoff(last_S, p, r, K, vDate);
    cout << "V0=" << V0 << endl;

    free(last_S);
    return 0;
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Calculate the price at valid date.
  * @param  S0: Starting price.
  * @param  u: Up pricing rate.
  * @param  d: Down pricing rate.
  * @param  vDate: Valid date.
  * @retval Pointer to the last S.
  */
double *forward_Price(float S0, float u, float d, int vDate){
    // Apply a vDate+1 spaces array for last date.
    double *last_S = (double*)malloc(sizeof(double) * (vDate + 1));

    for(int i = 0; i < vDate + 1; i++){
        last_S[i] = S0 * pow(d, i) * pow(u, vDate - i);
    }

    return last_S;
}

/**
  * @brief  Calculate the payoff of the option.
  * @param  price: Pointer to price on valid date.
  * @param  p: Up's probability.
  * @param  r: Interest rate.
  * @param  K: Expected price.
  * @param  vDate: Valid date.
  * @retval V0.
  */
double backward_Payoff(double *price, float p, float r, float K, int vDate){
    float q = 1.0 - p;  // Down's probability
    // Apply a vDate+1 spaces array for last date.
    double *cur_V = (double*)malloc(sizeof(double) * (vDate + 1));

    for(int i = 0; i < vDate + 1; i++){
        float Vn = price[i] - K;  // Vn = (Sn - K).
        if(Vn < 0) Vn = 0;
        cur_V[i] = Vn;
    }

    for(int pre_d = vDate; pre_d >= 1; pre_d--){
        for(int i = 0; i < pre_d; i++){
            // Calculate previous V.
            float temp = (p * cur_V[i] + q * cur_V[i + 1]) / (1 + r);
            if(temp < 0) temp = 0;
            cur_V[i] = temp;
        }
    }

    return cur_V[0];    // return V0.
}
