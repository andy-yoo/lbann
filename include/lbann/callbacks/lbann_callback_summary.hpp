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
//
// lbann_callback_summary .hpp .cpp - Callback hooks to summarize to Tensorboard
////////////////////////////////////////////////////////////////////////////////

#ifndef LBANN_CALLBACKS_CALLBACK_SUMMARY_HPP_INCLUDED
#define LBANN_CALLBACKS_CALLBACK_SUMMARY_HPP_INCLUDED

#include "lbann/callbacks/lbann_callback.hpp"
#include "lbann/utils/lbann_summary.hpp"

namespace lbann {

/**
 * Summarize information to Tensorboard using LBANN's summary interface.
 */
class lbann_callback_summary : public lbann_callback {
public:
  lbann_callback_summary(lbann_summary* summarizer, int _batch_interval = 1);
  void on_batch_end(Model* m);
  void on_epoch_end(Model* m);
  void on_test_end(Model* m);
};

}  // namespace lbann

#endif  // LBANN_CALLBACKS_CALLBACK_SUMMARY_HPP_INCLUDED
