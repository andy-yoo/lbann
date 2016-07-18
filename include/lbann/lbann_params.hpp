////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2014-2016, Lawrence Livermore National Security, LLC. 
// Produced at the Lawrence Livermore National Laboratory. 
// Written by the LBANN Research Team (B. Van Essen, et al.) listed in
// the CONTRIBUTORS file. <lbann-dev@llnl.gov>
//
// LLNL-CODE-697807.
// All rights reserved.
//
// This file is part of LBANN: Livermore Big Artificial Neural Network
// Toolkit. For details, see http://software.llnl.gov/LBANN or
// https://github.com/LLNL/LBANN. 
//
// Licensed under the Apache License, Version 2.0 (the "Licensee"); you
// may not use this file except in compliance with the License.  You may
// obtain a copy of the License at:
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
// implied. See the License for the specific language governing
// permissions and limitations under the license.
////////////////////////////////////////////////////////////////////////////////

#ifndef LBANN_PARAMS_HPP_INCLUDED
#define LBANN_PARAMS_HPP_INCLUDED

#include "lbann.hpp"
#include <string>

namespace lbann
{

  class TrainingParams
  {
  public:
    /// Constructor
    TrainingParams(void);
    /// Parse command-line arguments
    void parse_params();

  public:
    /// Enable profiling
    bool EnableProfiling;

    /// Random seed
    int RandomSeed;
    /// Shuffle training data
    /** 0 - Pick first N training samples, 1 - Select N random training samples */
    int ShuffleTrainingData;

    /// Maximum number of samples for training
    int MaxTrainingSamples;
    /// Maximum number of samples for validation
    int MaxValidationSamples;
    /// Maximum number of samples for testing
    int MaxTestSamples;
    /// Test with training data
    int TestWithTrainData;

    // Number of training epochs
    int EpochCount;
    /// Size of the mini-batch to be trained
    int MBSize;
    /// Maximum number of mini-batches to use for training
    int MaxMBCount;

    /// Learning rate
    float LearnRate;
    /// Learning method
    /** 1 - Adagrad, 2 - RMSprop */
    int LearnRateMethod;
    /// How much does the learning rate decay
    float LrDecayRate;
    /// How often does the learning rate decay
    int LrDecayCycles;
    /// Activation function
    /** 1 - Sigmoid, 2 - Tanh, 3 - reLU */
    activation_type ActivationType;
    /// Dropout probability
    /** Probability of dropping a neuron/input in
        dropout_layer. Default is -1 (no dropout). */
    float DropOut;
    /// Lambda for L2 regularization
    double Lambda;

    /// Location of training and test data
    std::string DatasetRootDir;
    /// Location to save output images
    std::string SaveImageDir;
    /// Location to save model parameters
    std::string ParameterDir;
    /// Save model file
    bool SaveModel;
    /// Load a saved model file
    bool LoadModel;
    /// Number of epochs between checkpoints
    int Checkpoint;
    /// Train data file name
    std::string TrainFile;
    /// Test data file name
    std::string TestFile;

  };

  /// Performance parameters
  class PerformanceParams {

  public:
    /// Constructor
    PerformanceParams(void);
    /// Parse command-line arguments
    void parse_params();

  public:
    /// libElemental block size
    int BlockSize;
    /// Maximum parallel I/O size (0 - unlimited)
    int MaxParIOSize;
  };

  /// Network parameters
  class NetworkParams {

  public:
    /// Constructor
    NetworkParams(void);
    /// Parse command-line arguments
    void parse_params(void);

  private:
    /// Parse network string
    void parse_network_string(void);

  public:
    /// String specifying the hidden layers of the topology
    std::string NetworkStr;
    /// The hidden layers of the topology
    std::vector<int> Network;
  };

  /// System parameters
  class SystemParams{
  public:
    /// Constructor
    SystemParams(void);
    /// Parse command-line arguments
    void parse_params(void);

  public:
    /// HPC hostname
    std::string HostName;
    /// Total allocation size
    int NumNodes;
    /// Number of cores per node
    int NumCores;
    /// MPI tasks allowed per node
    int TasksPerNode;
  };
}

#endif // LBANN_PARAMS_HPP_INCLUDED
