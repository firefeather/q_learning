#include "agent.h"

CAgent::CAgent(struct sAgentInit ag_init_struct)
{
  this->agent_init = ag_init_struct;

  struct sQlearningInit q_learning_init;

  q_learning_init.state_vector_size = agent_init.inputs_count;
  q_learning_init.actions_count = agent_init.actions.size();
  q_learning_init.action_vector_size = agent_init.actions[0].size();
  q_learning_init.function_type = agent_init.function_type;
  q_learning_init.gamma = agent_init.gamma;
  q_learning_init.alpha = agent_init.alpha;
  q_learning_init.k = agent_init.k;
  q_learning_init.density = agent_init.dt;

  #ifdef DEBUG_MODE
  printf("NEW agent : \n");
  printf(" inputs count %u \n", agent_init.inputs_count);
  printf(" %6.3f %6.3f %6.3f %u\n", agent_init.gamma, agent_init.alpha, agent_init.k, agent_init.function_type);
  printf(" actions count %u \n", q_learning_init.actions_count);
  printf(" action vector size %u \n", q_learning_init.action_vector_size);
  #endif

  q_learning = new CQlearning(q_learning_init);

  agent_res.q_res = q_learning->get();
  agent_res.action =  agent_init.actions[agent_res.q_res.selected_action];
}

CAgent::~CAgent()
{
  delete q_learning;
}

void CAgent::process(std::vector<float> input, float reward, u32 learn)
{
  q_learning->process(input, agent_init.actions, reward, learn);

  agent_res.q_res = q_learning->get();
  agent_res.action =  agent_res.q_res.action; //agent_init.actions[agent_res.q_res.selected_action];
  agent_res.best_action =  agent_res.q_res.action_best; //agent_init.actions[agent_res.q_res.best_action];
}


void CAgent::reset()
{
  q_learning->reset();
}

struct sAgentRes CAgent::get()
{
  return agent_res;
}

void CAgent::save_best_q_plot(std::vector<float> subspace, char *file_name)
{
  float x, y;
  CLog log(file_name, 4);

  for (y = -1.0; y < 1.0; y+= agent_init.dt)
  {
    for (x = -1.0; x < 1.0; x+= agent_init.dt)
    {
      subspace[0] = x;
      subspace[1] = y;

      float q = q_learning->get_max_q(subspace, agent_init.actions);
      u32 action_id = q_learning->get_max_q_action_id(subspace, agent_init.actions);

      log.add(0, subspace[0]);
      log.add(1, subspace[1]);
      log.add(2, q);
      log.add(3, action_id);
    }
  }

  log.save();
}