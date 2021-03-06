#ifndef _AGENT_H_
#define _AGENT_H_

#include "q_learning.h"



struct sAgentInit
{
  u32 id;
  u32 type;

  float dt;
  float gamma, alpha, k;
  u32 function_type;

  u32 inputs_count;
  std::vector<std::vector<float>> actions;
};

class CAgent
{
  private:
    class CQlearning *q_learning;
    struct sAgentInit agent_init;
    struct sQlearningRes q_res;

  public:
    CAgent(struct sAgentInit ag_init_struct);
    ~CAgent();

    void process(std::vector<float> input, float reward, u32 learn, u32 force_learn = 0);

    void reset();
    struct sQlearningRes get();

    void save_q(std::vector<float> subspace, char *file_name);
    void save();
};



#endif
