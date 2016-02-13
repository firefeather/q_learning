#include "q_func_knn.h"



CQFuncKNN::CQFuncKNN(u32 state_size, u32 action_size, float state_density,
              float action_density, float alpha)
{
    this->alpha = alpha;

    struct KNNLayerInitStructure knn_init;

    knn_init.inputs_count = state_size;
    knn_init.neurons_count = 8;
    knn_init.weight_range = 1.0;
    knn_init.learning_constant = 0.01;
    knn_init.output_limit = 1.0;

    knn = new CKohonenLayer(knn_init);


    u32 hidden_neurons_count = 32;

    std::vector<u32> init_vector;
    init_vector.push_back(knn_init.neurons_count + action_size + 1);
    init_vector.push_back(hidden_neurons_count);
    init_vector.push_back(1);

    struct sNNInitStruct nn_init;

    nn_init.init_vector = init_vector;

    nn_init.order = 5;
    nn_init.neuron_type = NN_LAYER_NEURON_TYPE_TANH;
    nn_init.weight_range = 4.0;
    nn_init.init_weight_range = nn_init.weight_range*0.25;
    nn_init.learning_constant = 1.0/1000.0;
    nn_init.output_limit = 4.0;


    nn = new CNN(nn_init);
}

CQFuncKNN::~CQFuncKNN()
{
    delete nn;
    delete knn;
}

float CQFuncKNN::get(std::vector<float> state, std::vector<float> action)
{
    u32 ptr = 0, i;

    //map state into kohonen network
    knn->process(state);

    u32 knn_output_size = knn->get_output().size();
    //kohonen output as input into fnn
    for (i = 0; i < knn_output_size; i++)
        nn_input[ptr++] = knn->get_output()[i];

    for (i = 0; i < action.size(); i++)
        nn_input[ptr++] = action[i];

    //bias
    nn_input[ptr++] = 1.0;

    nn->process(nn_input);

    return nn->get()[0];
}

void CQFuncKNN::learn(std::vector<float> state, std::vector<float> action, float required_value)
{
    std::vector<float> required_value_;

    required_value_.push_back( tanh(1.3*required_value));
    //required_value_.push_back(tanh(required_value));


    u32 ptr = 0, i;

    //knn->process_without_learn(state);
    knn->process(state);
    knn->learn();


    u32 knn_output_size = knn->get_output().size();
    //kohonen output as input into fnn
    for (i = 0; i < knn_output_size; i++)
        nn_input[ptr++] = knn->get_output()[i];

    for (i = 0; i < action.size(); i++)
        nn_input[ptr++] = action[i];

    nn_input[ptr++] = 1.0; //add bias

    nn->process(nn_input);
    nn->learn(required_value_);
}

i32 CQFuncKNN::save(char *file_name)
{
    return -1;
}

i32 CQFuncKNN::load(char *file_name)
{
    return -1;
}

void CQFuncKNN::merge(class CQFuncKNN *q_func)
{
  //TODO
}