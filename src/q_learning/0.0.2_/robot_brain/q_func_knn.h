#ifndef _Q_FUNC_NNK_H_
#define _Q_FUNC_NNK_H_

#include "../common.h"
#include "neural_network.h"
#include "kohonen_neural_network.h"

#define Q_FUNC_KNN_MAGIC    (u32)0xABCD0002


class CQFuncKNN
{
    private:
        class CNeuralNetwork *nn;
        class CKohonenNeuralNetwork *knn;

        float alpha;

        std::vector<float> nn_input;

    public:
        CQFuncKNN( u32 state_size, u32 action_size,
                float density, float action_density,
                float alpha = 0.9);
        ~CQFuncKNN();

        float get(std::vector<float> state, std::vector<float> action);
        void learn(std::vector<float> state, std::vector<float> action, float required_value);

        i32 save(char *file_name);
        i32 load(char *file_name);
};

#endif
