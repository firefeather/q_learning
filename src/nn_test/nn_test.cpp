#include "nn_test.h"


CNNTest::CNNTest()
{
    test_result.error_average = 0.0;
    test_result.error_max = 0.0;
    test_result.error_min = 0.0;
}

CNNTest::~CNNTest()
{

}

void CNNTest::process()
{
    u32 i, j;

    char result_path_0[] = "results/mcculloch_pitts_linear_neuron_1_layer/";
    char result_path_1[] = "results/mcculloch_pitts_linear_neuron_2_layers/";
    char result_path_2[] = "results/mcculloch_pitts_tanh_neuron_1_layer/";
    char result_path_3[] = "results/mcculloch_pitts_tanh_neuron_2_layers/";


    char result_path_4[] = "results/testing_neuron_1_layer/";
    char result_path_5[] = "results/testing_neuron_2_layers/";

    char result_path_6[] = "results/kohonen_layer/";
    char result_path_7[] = "results/kohonen_layer_mcculloch_pitts_tanh_neuron_1_layer/";
    char result_path_8[] = "results/kohonen_layer_testing_neuron_1_layer/";

    char log_file_name[1024];

    for (j = 0; j < 4; j++)
    {
        struct sNNInitStruct nn_init;

        nn_init.neuron_type = NN_LAYER_NEURON_TYPE_LINEAR;

        switch (j)
        {
            case 1:
                    nn_init.neuron_type = NN_LAYER_NEURON_TYPE_TANH;
                    nn_init.init_vector.push_back(2 + 1);
                    nn_init.init_vector.push_back(32);
                    nn_init.init_vector.push_back(1);

                    break;

            case 2:
                    nn_init.neuron_type = NN_LAYER_NEURON_TYPE_TANH;
                    nn_init.init_vector.push_back(2 + 1);
                    nn_init.init_vector.push_back(32);
                    nn_init.init_vector.push_back(32);
                    nn_init.init_vector.push_back(1);

                    break;

            case 3:
                    nn_init.neuron_type = NN_LAYER_NEURON_TYPE_TANH;
                    nn_init.init_vector.push_back(2 + 1);
                    nn_init.init_vector.push_back(32);
                    nn_init.init_vector.push_back(32);
                    nn_init.init_vector.push_back(32);
                    nn_init.init_vector.push_back(1);

                    break;

            default:
                    nn_init.neuron_type = NN_LAYER_NEURON_TYPE_LINEAR;
                    nn_init.init_vector.push_back(2 + 1);
                    nn_init.init_vector.push_back(32);
                    nn_init.init_vector.push_back(1);

                    break;
        }


        nn_init.weight_range = 4.0;
        nn_init.init_weight_range = 0.3*nn_init.weight_range;
        nn_init.learning_constant = 1.0/1000.0;
        nn_init.output_limit = 4.0;
        nn_init.order = 5;

        CNN *nn_req;
        nn_req = new CNN(nn_init);
        nn_required.push_back(nn_req);
    }

    for (j = 0; j < 9; j++)
    {
        char *result_path;
        switch (j)
        {
            case 0: result_path = result_path_0;  break;
            case 1: result_path = result_path_1;  break;
            case 2: result_path = result_path_2;  break;
            case 3: result_path = result_path_3;  break;
            case 4: result_path = result_path_4;  break;
            case 5: result_path = result_path_5;  break;
            case 6: result_path = result_path_6;  break;
            case 7: result_path = result_path_7;  break;
            case 8: result_path = result_path_8;  break;
            default : result_path = NULL;
        }

        sprintf(log_file_name, "%ssummary_result_log.txt", result_path);
        class CLog *summary_result_log;
        summary_result_log = new CLog(log_file_name, 3);

        experiment_result.error_average = 0.0;
        experiment_result.error_max = 0.0;
        experiment_result.error_min = 0.0;

        for (i = 0; i < (8 + 4); i++)
        {
            printf("***********************************************************\n");
            printf("processing experiment %u %u : \n", j, i);
            printf("results saved into %s\n", result_path);

            process_test(result_path, i, j);

            printf("  result : Eav = %f Emax = %f Emin = %f\n", experiment_result.error_average,  experiment_result.error_max,  experiment_result.error_min);
            printf("***********************************************************\n\n\n");

            summary_result_log->add(0, experiment_result.error_average);
            summary_result_log->add(1, experiment_result.error_max);
            summary_result_log->add(2, experiment_result.error_min);
        }


        //printf("save to >%s\n", );
        summary_result_log->save();
        delete summary_result_log;
    }
}


void CNNTest::process_test(char *result_path, u32 id, u32 type)
{
    u32 k;
    u32 tests_count = 16;

    char log_file_name_tmp[1024];
    char error_log_file_name[1024];

    sprintf(log_file_name_tmp, "%sexperiment_%u/", result_path, id);
    sprintf(error_log_file_name, "%serror_log.txt", log_file_name_tmp );

    class CLog *error_log;

    error_log = new CLog(error_log_file_name, 3);


    experiment_result.error_average = 0.0;
    experiment_result.error_max = 0.0;
    experiment_result.error_min = 9999999999999999999999.9;

    for (k = 0; k < tests_count; k++)
    {
        process_single_test(id, false, log_file_name_tmp, type);

        error_log->add(0, test_result.error_average);
        error_log->add(1, test_result.error_max);
        error_log->add(2, test_result.error_min);

        experiment_result.error_average+= test_result.error_average;

        if (experiment_result.error_max < test_result.error_max)
            experiment_result.error_max = test_result.error_max;

        if (experiment_result.error_min > test_result.error_min)
            experiment_result.error_min = test_result.error_min;
    }

    experiment_result.error_average = experiment_result.error_average/tests_count;

    error_log->save();

    process_single_test(id, true, log_file_name_tmp, type);
}



void CNNTest::process_single_test(u32 id, bool log_enabled, char *log_path, u32 type)
{
    srand(time(NULL));

    std::vector<float> input;
    std::vector<float> required_output, nn_output;

    input = get_input(rnd(), rnd());
    required_output = get_required_output(input, id);
    nn_output =  get_required_output(input, id);

    struct KNNLayerInitStructure knn_init;
    struct sNNInitStruct nn_init;

    class CKohonenLayer *knn = NULL;
    class CNN *nn = NULL;


    // u32 input_size = input.size();
    u32 hidden_neurons_count = 32;




    std::vector<u32> init_vector_;

    nn_init.init_vector = init_vector_;
    nn_init.neuron_type = 0;
    nn_init.weight_range = 4.0;
    nn_init.init_weight_range = nn_init.weight_range*0.25;
    nn_init.learning_constant = 1.0/1000.0;
    nn_init.output_limit = 4.0;
    nn_init.order = 5;



    knn_init.neurons_count = 8;
    knn_init.weight_range = 1.0;
    knn_init.learning_constant = 1.0/100.0;
    knn_init.output_limit = 1.0;
    knn_init.inputs_count = 0;
    knn_init.outputs_count = 0;


    switch (type&255)
    {
        case 0:
                    nn_init.neuron_type = NN_LAYER_NEURON_TYPE_LINEAR;

                    nn_init.init_vector.push_back(input.size());
                    nn_init.init_vector.push_back(hidden_neurons_count);
                    nn_init.init_vector.push_back(required_output.size());

                    nn = new CNN(nn_init);
                    break;

        case 1:
                    nn_init.neuron_type = NN_LAYER_NEURON_TYPE_LINEAR;

                    nn_init.init_vector.push_back(input.size());
                    nn_init.init_vector.push_back(hidden_neurons_count);
                    nn_init.init_vector.push_back(hidden_neurons_count);
                    nn_init.init_vector.push_back(required_output.size());

                    nn = new CNN(nn_init);
                    break;

        case 2:
                    nn_init.neuron_type = NN_LAYER_NEURON_TYPE_TANH;

                    nn_init.init_vector.push_back(input.size());
                    nn_init.init_vector.push_back(hidden_neurons_count);
                    nn_init.init_vector.push_back(required_output.size());

                    nn = new CNN(nn_init);
                    break;
        case 3:
                    nn_init.neuron_type = NN_LAYER_NEURON_TYPE_TANH;

                    nn_init.init_vector.push_back(input.size());
                    nn_init.init_vector.push_back(hidden_neurons_count);
                    nn_init.init_vector.push_back(hidden_neurons_count);
                    nn_init.init_vector.push_back(required_output.size());

                    nn = new CNN(nn_init);
                    break;


        case 4:
                    nn_init.neuron_type = NN_LAYER_NEURON_TYPE_INTERSYNAPTICS;

                    nn_init.init_vector.push_back(input.size());
                    nn_init.init_vector.push_back(hidden_neurons_count);
                    nn_init.init_vector.push_back(required_output.size());

                    nn = new CNN(nn_init);
                    break;

        case 5:
                    nn_init.neuron_type = NN_LAYER_NEURON_TYPE_INTERSYNAPTICS;

                    nn_init.init_vector.push_back(input.size());
                    nn_init.init_vector.push_back(hidden_neurons_count);
                    nn_init.init_vector.push_back(hidden_neurons_count);
                    nn_init.init_vector.push_back(required_output.size());

                    nn = new CNN(nn_init);
                    break;

        case 6:
                    knn_init.outputs_count = required_output.size();
                    knn_init.inputs_count = input.size();

                    knn = new CKohonenLayer(knn_init);
                    break;

        case 7:
                    knn_init.outputs_count = required_output.size();
                    knn_init.inputs_count = input.size();



                    nn_init.neuron_type = NN_LAYER_NEURON_TYPE_TANH;

                    nn_init.init_vector.push_back(knn_init.neurons_count + 1);
                    nn_init.init_vector.push_back(hidden_neurons_count);
                    nn_init.init_vector.push_back(required_output.size());

                    nn = new CNN(nn_init);
                    knn = new CKohonenLayer(knn_init);
                    break;

        case 8:
                    knn_init.outputs_count = required_output.size();
                    knn_init.inputs_count = input.size();

                    nn_init.neuron_type = NN_LAYER_NEURON_TYPE_INTERSYNAPTICS;

                    nn_init.init_vector.push_back(knn_init.neurons_count + 1);
                    nn_init.init_vector.push_back(hidden_neurons_count);
                    nn_init.init_vector.push_back(required_output.size());

                    nn = new CNN(nn_init);
                    knn = new CKohonenLayer(knn_init);
                    break;
    }



    u32 iterations = 100000;

    u32 k;

    if (log_enabled)
    {
        printf("    FNN parameters : \n");

        printf("    network configuration : ");
        for (k = 0; k < nn_init.init_vector.size(); k++)
            printf("%u ", nn_init.init_vector[k]);
        printf("\n");

        printf("    neuron_type %u\n", nn_init.neuron_type);
        printf("    weight_range %f\n", nn_init.weight_range);
        printf("    init_weight_range %f\n", nn_init.init_weight_range);
        printf("    learning_constant %f\n", nn_init.learning_constant);
        printf("    output_limit %f\n", nn_init.output_limit);
        printf("    iterations %u\n", iterations);

        printf("\n");
        printf("    KNN parameters : \n");


        printf("    neurons_count %u\n", knn_init.neurons_count);
        printf("    inputs_count %u\n", knn_init.inputs_count);
        printf("    outputs_count %u\n", knn_init.outputs_count);
        printf("    weight_range %f\n", knn_init.weight_range);
        printf("    learning_constant %f\n", knn_init.learning_constant);
        printf("    output_limit %f\n", knn_init.output_limit);
        printf("    iterations %u\n", iterations);
    }




    std::vector<float> nn_input;

  	for (k = 0; k < iterations; k++)
  	{
  		input = get_input(rnd(), rnd());
  		required_output = get_required_output(input, id);

      if (knn != NULL)
      {
        knn->process(input, true);
        knn->learn(&required_output);

        nn_input =  knn->get_output();
        nn_output = (*knn->get_nn_output())[knn->get_output_winning_neuron_idx()];
      }
      else
        nn_input = input;

      if (nn != NULL)
      {
        if (knn != NULL)
          nn_input.push_back(1.0);

        nn->process(nn_input);
        nn_output = nn->get();
        nn->learn(required_output);
      }
  	}

    float dt = 1.0/32.0;

    class CLog *result_log = NULL;


    if (log_enabled)
    {
        char result_log_file_name[1024];
        sprintf(result_log_file_name, "%sresult_log.txt", log_path);
        result_log = new CLog((char*)result_log_file_name, input.size() + required_output.size() + nn_output.size() + 1);
    }

    float error = 0.0;
    float error_average = 0.0;
    float error_min = 9999999999999999.0;
    float error_max = 0.0;
    float error_average_cnt = 0.0;

    float x, y;

    for (y = -1.0; y < 1.0; y+= dt)
    for (x = -1.0; x < 1.0; x+= dt)
	  {
      input = get_input(rnd(), rnd());
      required_output = get_required_output(input, id);

      if (knn != NULL)
      {
        knn->process(input, true);

        nn_input =  knn->get_output();
        nn_output = (*knn->get_nn_output())[knn->get_output_winning_neuron_idx()];
      }
      else
        nn_input = input;

      if (nn != NULL)
      {
        if (knn != NULL)
          nn_input.push_back(1.0);

        nn->process(nn_input);
        nn_output = nn->get();
      }



  		u32 i;
  		error = 0.0;
  		for (i = 0; i < required_output.size(); i++)
  			error+= abs_(required_output[i] - nn_output[i]);

      if (error > error_max)
        error_max = error;

      if (error < error_min)
        error_min = error;

      error_average+= abs_(error);
      error_average_cnt+= 1.0;

      if (result_log != NULL)
      {
        u32 ptr = 0;

        for (i = 0; i < input.size(); i++)
        {
        	result_log->add(ptr++, input[i]);
        }

        for (i = 0; i < required_output.size(); i++)
        {
        	result_log->add(ptr++, required_output[i]);
        }

        for (i = 0; i < nn_output.size(); i++)
        {
          result_log->add(ptr++, nn_output[i]);
        }

        result_log->add(ptr++, error);
      }
	}


	error_average = error_average/error_average_cnt;

    /*
    printf("maximal error %f\n", error_max);
    printf("minimal error %f\n", error_min);
	printf("average error %f\n", error_average);
    printf("\n");
    */

    if (result_log != NULL)
    {
      result_log->save();
      delete result_log;
    }

    if (nn != NULL)
      delete nn;

    if (knn != NULL)
      delete knn;

    test_result.error_average = error_average;
    test_result.error_max = error_max;
    test_result.error_min = error_min;
}

float CNNTest::f_and(float x0, float x1)
{
    if (x0 < x1)
        return x0;
    return x1;
}

float CNNTest::f_or(float x0, float x1)
{
    if (x0 > x1)
        return x0;
    return x1;
}

float CNNTest::f_not(float x0)
{
    return -x0;
}


std::vector<float> CNNTest::get_input(float x0, float x1)
{
    std::vector<float> res;
    res.push_back(x0);
    res.push_back(x1);

    res.push_back(1.0);

    return res;
}

std::vector<float> CNNTest::get_required_output(std::vector<float> input, u32 id)
{
    std::vector<float> res;

    float y = 0.0;
    switch (id)
    {
        case 0: y = f_not(f_and(input[0], input[1])); break;
        case 1: y = f_and( f_or(f_and(input[0], input[1]), input[1]), f_not(f_and(input[0], input[1])) ); break;
        case 2: y = input[0]*input[1]; break;

        case 3: y = sin(input[0] * input[1] * 5.0); break;
        case 4: y = 0.25*(cos(input[0] * input[1] + 0.731) - cos(2.3*input[0] - 4.45*input[1]) +
                    - cos(sin(3.17*input[0] - 2.787*input[1]))*sin(3.5435*input[0]) + sin(input[1] + input[0] * input[1] * 5.0)) ;

                    break;

        case 5: y = sin(3.3214*input[0])*sin(3.742*input[1]); break;

        case 6: y = sgn(input[0]*input[1]); break;
        case 7: y = (input[0] + input[1])/2.0; break;


        case 8: nn_required[0]->process(input);
                y = 0.25*nn_required[0]->get()[0];
                break;

        case 9: nn_required[1]->process(input);
                y = nn_required[1]->get()[0];
                break;

        case 10: nn_required[2]->process(input);
                y = nn_required[2]->get()[0];
                break;

        case 11: nn_required[3]->process(input);
                y = nn_required[3]->get()[0];
                break;

        default : y = 0.0; break;
    }

    res.push_back(y);
    return res;
}


float CNNTest::rnd()
{
    return ( (rand()%20000000) - 10000000 ) / 10000000.0;
}
