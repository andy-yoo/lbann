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
// lbann_summary .hpp .cpp - Write summary statistics to Tensorboard
////////////////////////////////////////////////////////////////////////////////

#ifndef LBANN_SUMMARY_HPP_INCLUDED
#define LBANN_SUMMARY_HPP_INCLUDED

#include <string>
#include <vector>
#include "lbann/lbann_base.hpp"
#include "lbann/lbann_comm.hpp"

#ifdef __HAVE_TBINF
#include "TBinf.hpp"
#endif

namespace lbann {

#ifdef __HAVE_TBINF

/**
 * Interface for computing summary statistics within and among models and
 * outputting them to Tensorboard.
 * All methods take a step parameter that gives the global step to use when
 * reporting the output.
 * All methods buffer data locally. The flush method must be called to write
 * summaries out, and must be called by every LBANN process.
 * Be sure to add summaries in the same order on every process, or confusion
 * will result.
 * Distributed matrices should be distributed by model.
 * This class automatically prepends "modelN/" to each tag. The tag is only
 * relevant at the world master process.
 * 
 * @note WHEN YOU UPDATE THE PUBLIC API HERE, REMEMBER TO UPDATE THE KLUDGE FOR
 * NON-TENSORBOARD BUILDS BELOW!
 */
class lbann_summary {
public:

  /**
   * Create a new summary manager.
   * @param logdir The directory to output events to.
   * @param comm Communicator to use.
   */
  lbann_summary(std::string logdir, lbann_comm* comm);
  ~lbann_summary();

  /** Report the mean of mat. */
  void reduce_mean(std::string tag, DistMat& mat, int64_t step);
  /** Report the minimum value of mat. */
  void reduce_min(std::string tag, DistMat& mat, int64_t step);
  /** Report the maximum value of mat. */
  void reduce_max(std::string tag, DistMat& mat, int64_t step);
  /** Report the standard deviation of mat. */
  void reduce_stdev(std::string tag, DistMat& mat, int64_t step);
  /** Report a scalar from each model (only meaningful on model masters). */
  void reduce_scalar(std::string tag, DataType s, int64_t step);
  /** Do a model_reduce (sum) on a scalar, then report that sum. */
  void sum_reduce_scalar(std::string tag, DataType s, int64_t step);
  /** Report a histogram of the values in mat. */
  void reduce_histogram(std::string tag, DistMat& mat, int64_t step);

  /**
   * Write all summaries out.
   * @todo This can be made faster by combining collective operations element-
   * wise for each type of summary.
   */
  void flush();

private:
  lbann_comm* comm;
  TBinf::SummaryWriter* sw;

  /** Represent a pending summary operation. */
  struct pending_op {
    pending_op(std::string tag, int64_t step, DataType local,
               DataType local2 = 0.0f, int num = 0) :
      tag(tag), step(step), local(local), local2(local2), num(num) {}
    /** Associated tag. */
    std::string tag;
    /** Global step. */
    int64_t step;
    /** Locally-computed data. */
    DataType local;
    /** More locally-computed data (for stdev). */
    DataType local2;
    /** Size of matrix (needed for mean/stdev). */
    int num;
  };

  /** Currently-pending reduce_means. */
  std::vector<pending_op> pending_means;
  /** Currently-pending reduce_mins. */
  std::vector<pending_op> pending_mins;
  /** Currently-pending reduce_maxes. */
  std::vector<pending_op> pending_maxes;
  /** Currently-pending reduce_stdevs. */
  std::vector<pending_op> pending_stdevs;
  /** Currently-pending reduce_scalars. */
  std::vector<pending_op> pending_scalars;
  /** Currently-pending sum_reduce_scalars. */
  std::vector<pending_op> pending_sum_scalars;

  /** Execute all pending mean operations. */
  void flush_means();
  /** Execute all pending min operations. */
  void flush_mins();
  /** Execute all pending max operations. */
  void flush_maxes();
  /** Execute all pending stdev operations. */
  void flush_stdevs();
  /** Execute all pending scalar operations. */
  void flush_scalars();
  /** Execute all pending sum-scalar operations. */
  void flush_sum_scalars();

  /** Compute the sum of local elements in mat. */
  DataType local_sum(DistMat& mat);
  /** Prepend "model<model>/" to tag. */
  std::string prepend_model(std::string tag, int model);
  /** Gather and write out a scalar summary for each model. */
  void gather_scalar_summary(std::string tag, DataType s, int64_t step);
};

#else

/** Dummy class when TBinf is not present. */
class lbann_summary {
public:
  lbann_summary(std::string logdir, lbann_comm* comm) {}

  void reduce_mean(std::string tag, DistMat& mat, int64_t step) {}
  void reduce_min(std::string tag, DistMat& mat, int64_t step) {}
  void reduce_max(std::string tag, DistMat& mat, int64_t step) {}
  void reduce_stdev(std::string tag, DistMat& mat, int64_t step) {}
  void reduce_scalar(std::string tag, DataType s, int64_t step) {}
  void sum_reduce_scalar(std::string tag, DataType s, int64_t step) {}
  void reduce_histogram(std::string tag, DistMat& mat, int64_t step) {}
  void flush() {}
};

#endif  // __HAVE_TBINF

}  // namespace lbann

#endif  // LBANN_SUMMARY_HPP_INCLUDED
