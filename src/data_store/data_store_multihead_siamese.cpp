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
////////////////////////////////////////////////////////////////////////////////

#include "lbann/data_store/data_store_multihead_siamese.hpp"
#include "lbann/data_readers/data_reader_multihead_siamese.hpp"
#include "lbann/utils/exception.hpp"
#include "lbann/utils/options.hpp"
#include "lbann/utils/timer.hpp"

namespace lbann {

std::vector<std::string> data_store_multihead_siamese::get_sample(size_t idx) const {
  const data_reader_multihead_siamese *reader = dynamic_cast<data_reader_multihead_siamese*>(m_reader);
  data_reader_multihead_siamese::sample_t sample = reader->get_sample(idx);
  return sample.first;
}

void data_store_multihead_siamese::setup() {
  double tm1 = get_time();
  if (m_rank == 0) {
    std::cerr << "starting data_store_multihead_siamese::setup() for data reader with role: " << m_reader->get_role() << std::endl;
  }

  set_name("data_store_multihead_siamese");

  //sanity check
  data_reader_multihead_siamese *reader = dynamic_cast<data_reader_multihead_siamese*>(m_reader);
  if (reader == nullptr) {
    std::stringstream err;
    err << __FILE__ << " " << __LINE__ << " :: "
        << "dynamic_cast<data_reader_multihead_siamese*>(m_reader) failed";
    throw lbann_exception(err.str());
  }

  data_store_multi_images::setup();

  if (m_rank == 0) {
    std::cerr << "TIME for data_store_multihead_siamese setup: " << get_time() - tm1 << std::endl;
  }
}

}  // namespace lbann
